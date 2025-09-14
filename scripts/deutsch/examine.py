#!/usr/bin/python

import argparse
import json
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
            for key in tests.keys():
                if input(key) == tests[key]:
                    print("Richtig")
                else:
                    print("Falsch")
                    print(f"Dei Antwort ist {tests[key]}")
    except FileNotFoundError:
        print("File not found")
        return 1


if __name__ == "__main__":
    sys.exit(main())
