import argparse
import re
from collections import namedtuple
import subprocess
from io import StringIO
import logging
from typing import List
from multiprocessing import Pool


TESTS_RE = re.compile(
    r"# (?P<name>.*)$\n"
    r"(# (?P<description>.*)\n|)var := \[(?P<vars>[^\]]*)\]:$\n"
    r"sys := \[(?P<ideal>[^\]]*)\]:",
    re.MULTILINE,
)

SYSTEM_RE = re.compile(
    r",",
    re.MULTILINE,
)

POLY_RE = re.compile(
    r"(\+|\-)",
    re.MULTILINE,
)

MONOMIAL_RE = re.compile(
    r"\*",
    re.MULTILINE,
)

VARIABLE_RE = re.compile(
    r"x(?P<idx>\d+)(\^(?P<degree>\d+)|)",
    re.MULTILINE,
)

TEST_RESULT_RE = re.compile(
    r"^(?P<test_name>[^:]*):(?P<test_time>[^:]*)$",
    re.MULTILINE,
)

Monomial = namedtuple("Monomial", ("num", "denom", "vars"))
Test = namedtuple("Test", ("name", "description", "results"))

logger = logging.getLogger("TestLauncher")
logger.setLevel(logging.INFO)

fh = logging.FileHandler("test_launcher.log")

formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)

logger.addHandler(fh)


def read_system(system: str):
    polys = SYSTEM_RE.split(system)
    res = []
    for poly in polys:
        poly_res = []
        next_sign = 1
        for monomial in POLY_RE.split(poly):
            if not monomial.strip():
                continue
            if monomial == "-":
                next_sign = -1
                continue
            if monomial == "+":
                next_sign = 1
                continue
            num = 1
            vars_list = []
            for elem in MONOMIAL_RE.split(monomial):
                find_var = VARIABLE_RE.search(elem)
                if find_var:
                    idx = find_var.group("idx")
                    degree = find_var.group("degree") or 1
                    vars_list.append((idx, degree))
                else:
                    num = int(elem.strip())
            num *= next_sign
            poly_res.append(Monomial(num, 1, vars_list))
        res.append(poly_res)
    return res


def system_to_input_format(sys):
    output = StringIO()

    print(len(sys), file=output)
    for poly in sys:
        print(len(poly), file=output)
        for mono in poly:
            print(mono.num, mono.denom, file=output)
            print(len(mono.vars), file=output)
            for var in mono.vars:
                print(var[0], var[1], file=output)

    res = output.getvalue()
    output.close()
    return res


def iterate_through_matrix(matrix):
    for row_idx, row in enumerate(matrix):
        for col_idx, elem in enumerate(row):
            yield (row_idx, col_idx, elem)


def render_tests(tests: List[Test]):
    all_timings = set()
    for test in tests:
        for time_name, time in test.results.items():
            all_timings.add(time_name)
    timing_order = list(all_timings)
    table = [["name", "description"] + timing_order]
    for test in tests:
        new_line = [test.name, test.description]
        for time_name in timing_order:
            new_line.append(test.results.get(time_name, " --- "))
        table.append(new_line)
    columns = len(table[0])
    rows = len(table)
    col_size = [0] * columns
    for row, col, item in iterate_through_matrix(table):
        col_size[col] = max(col_size[col], len(item))
    close_line = " ".join(["-" * sz for sz in col_size])
    rendered_lines = [" ".join(["{{:>{}}}".format(col_size[col_idx]).format(elem) for col_idx, elem in enumerate(row)]) for row in table]
    return "\n".join([close_line] + rendered_lines + [close_line])


def run_test(name, description, variables, ideal, exec_filename, timeout):
    descr = description
    sys = read_system(ideal)
    input_for_program = system_to_input_format(sys)
    process = subprocess.Popen(
        [exec_filename],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    logger.debug("Input for program:\n%s", input_for_program)
    current_test = Test(name, descr, dict())
    try:
        outs, errs = process.communicate(input_for_program.encode(), timeout=timeout)
        logger.info("Test %s successful", name)
    except subprocess.TimeoutExpired:
        process.kill()
        logger.info("Test %s timeout", name)
        outs, errs = process.communicate()
    outs = outs.decode().strip()
    for match in TEST_RESULT_RE.finditer(outs):
        time_name = match.group("test_name")
        time = float(match.group("test_time").strip())
        current_test.results[time_name] = "{:.8f}".format(time)
    logger.info("Stderr for test %s:\n%s", name, errs.decode())
    return current_test


def run_test_wrapper(test):
    return run_test(*test)


def main(exec_filename, test_filename, timeout, processes):
    with open(test_filename, "r") as f:
        text = f.read()
    parsed_tests_configs = []
    for test_match in TESTS_RE.finditer(text):
        name = test_match.group("name")
        description = test_match.group("description") or "no description"
        ideal = test_match.group("ideal")
        variables = test_match.group("vars")
        parsed_tests_configs.append((name, description, variables, ideal, exec_filename, timeout))

    with Pool(processes=processes) as pool:
        all_tests = pool.map(run_test_wrapper, parsed_tests_configs)
    print(render_tests(all_tests))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("exec_file", help="Tested program")
    parser.add_argument("--test_file", help="File with tests", required=True)
    parser.add_argument("--timeout", help="Timeout for one test", default=10.0, type=float)
    parser.add_argument("--processes", help="Ho much processes run", default=1, type=int)
    args = parser.parse_args()
    main(args.exec_file, args.test_file, args.timeout, args.processes)