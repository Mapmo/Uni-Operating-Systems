#!/usr/bin/python

import argparse
import json
import random
import sys


def main() -> int:
    parser = argparse.ArgumentParser(prog="test")
    parser.add_argument("filename")
    parser.add_argument("--reversed", "-r", action="store_true")
    args = parser.parse_args()
    try:
        with open(args.filename) as fd:
            tests = json.load(fd)
            if args.reversed:
                tests = {v: k for k, v in tests.items()}
            test_keys = list(tests.keys())
            random.shuffle(test_keys)
            print(test_keys)
            for key in test_keys:
                if input(key + "\n") == tests[key]:
                    print("Richtig")
                    print("----")
                else:
                    print("Falsch")
                    print(f"Die Antwort ist {tests[key]}")
    except FileNotFoundError:
        print("File not found")
        return 1


if __name__ == "__main__":
    sys.exit(main())
