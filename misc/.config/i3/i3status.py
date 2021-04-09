#!/usr/bin/env python3
import os
import json
import sys
import re


def amend_status_line(status_line):
    # Return an amended status line. The status line is represented as a list of dicts.

    # Add layout to the status bar
    lang_code = os.popen("xset -q|grep LED| awk '{ print $10 }'").read()[0:7]
    lang = ''
    if lang_code == '0000000':
        lang = ' EN '
    elif lang_code == '0000100':
        lang = ' BG '

    layout = {
        'instance': 'layout',
        'full_text': lang
    }
    status_line.insert(5, layout)

    # Add weather info
    home = os.path.expanduser("~")
    try:
        with open(home + '/.config/i3/.weather_data.json') as w_fd:
            w_json = json.load(w_fd)
            weather = {
                'instance': 'weather',
                'full_text': w_json['icon'] + ' ' + w_json['temp_c'] + '℃ '
            }
    except IOError:
        weather = {
            'instance': 'weather',
            'full_text': 'no_data',
            'color': '#FF0000'
        }

    status_line.insert(7, weather)
    return status_line


def strip_full_battery(status_line):
    # Removes the () text when the battery is full
    for field in status_line:
        if field["name"] == 'battery':
            field["full_text"] = re.sub(r'\(\)$', '', field["full_text"])
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
            strip_full_battery(parsed)
            print(",%s" % (json.dumps(amend_status_line(parsed)),))


if __name__ == "__main__":
    main()
