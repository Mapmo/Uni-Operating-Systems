#!/usr/bin/env python3

import json
import os
import requests

home = os.path.expanduser("~")
cwd = home + '/.config/i3/'
with open(cwd + '.weather_api.key') as key_fd:
    key = key_fd.read()

url = 'http://api.weatherapi.com/v1/current.json?key=' + key + '&q=Sofia&aqi=no'
response = requests.get(url)

data = response.json() or os.eixt('Request failed')

code = data['current']['condition']['code']

icon = ''
cloudy_codes = [1006, 1009, 1030, 1135, 1147]
rainy_codes = [1063, 1072, 1150, 1153, 1168, 1171, 1180, 1183, 1186, 1189, 1192, 1195, 1198, 1201, 1240, 1243, 1246]
snowy_codes = [1066, 1069, 1114, 1117, 1204, 1207, 1210, 1213, 1216, 1219, 1222, 1225, 1249, 1252, 1255, 1258, 1261, 1282]
thunder_codes = [1087, 1273, 1276, 1279, 1282]

if code == 1000:
    if data['current']['is_day']:
        icon = '🌞'
    else:
        icon = '🌛'
elif code == 1003:
    icon = '⛅'
elif code in cloudy_codes:
    icon = '🌥'
elif rainy_codes:
    icon = '🌧'
elif code in thunder_codes:
    icon = '🌩'
elif code in snowy_codes:
    icon = '⛄'
else:
    icon = 404

info = '{"temp_c":"' + str(data['current']['temp_c']) + '","icon":"' + icon + '"}'

info_json = json.loads(info)
with open(cwd + '.weather_data.json', 'w') as output:
    json.dump(info_json, output)
