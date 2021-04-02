#!/usr/bin/env python3
import os
import json
import sys


def amend_status_line(status_line):
    """ Return an amended status line. The status line is represented as a list of dicts."""
    lang_code = os.popen("xset -q|grep LED| awk '{ print $10 }'").read()[0:8]
    lang = ''
    if lang_code == '00000000':
        lang = ' EN '
    elif lang_code == '00001004':
        lang = ' BG '

    layout = {
        'instance': 'layout',
        'full_text': lang
    }
    status_line.insert(5, layout)
    return status_line


def main():
    with os.popen("/usr/bin/i3status -c ~/.config/i3/i3status.conf", mode='r', buffering=2) as status:
        while True:
            sys.stdout.flush()
            line = status.readline()
            if line == "":
                break
            if not line.startswith(","):
                    print(line.strip())
                    continue
            parsed = json.loads(line[1:])
            print(",%s" % (json.dumps(amend_status_line(parsed)),))


if __name__ == "__main__":
    main()
