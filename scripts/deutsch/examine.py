#!/usr/bin/python

import argparse
import json
import os
import random
import re
import sys


def add_to_retry(key: str, val: str, args: argparse.Namespace):
    retry_file_path = args.retryfile
    if args.reversed:
        val, key = key, val
    if not os.path.exists(retry_file_path):
        with open(retry_file_path, "w") as fd:
            json.dump({}, fd)
    with open(retry_file_path) as fd:
        retry_json = json.load(fd)
        if key in retry_json:
            return
        retry_json[key] = val
        with open(retry_file_path, "w") as fd:
            json.dump(retry_json, fd, indent=4)


def process_question(key: str, tests: dict, args: argparse.Namespace):
    if input(key + "\n") == re.sub(" \\(.*", "", tests[key]):
        print("Richtig")
        print("----")
    else:
        print("Falsch")
        print(f"Die Antwort ist {tests[key]}")
        if args.retryfile:
            add_to_retry(key, tests[key], args)


def run_tests(args: argparse.Namespace):
    with open(args.filename) as fd:
        tests = json.load(fd)
        if args.reversed:
            tests = {v: k for k, v in tests.items()}
        test_keys = list(tests.keys())
        random.shuffle(test_keys)
        for key in test_keys:
            process_question(key, tests, args)


def main() -> int:
    parser = argparse.ArgumentParser(prog="test")
    parser.add_argument("filename")
    parser.add_argument("--reversed", "-r", action="store_true")
    parser.add_argument("--retryfile", "-f", required=False)
    args = parser.parse_args()
    try:
        run_tests(args)
    except FileNotFoundError:
        print("File not found")
        return 1


if __name__ == "__main__":
    sys.exit(main())
