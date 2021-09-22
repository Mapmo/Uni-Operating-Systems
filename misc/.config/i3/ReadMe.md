To use the weather API properly, you need a registration at: https://www.weatherapi.com/

You will be given a key, that you need to insert in file in ~/.config/i3/.weather_api.key

After that you will need this crontab, to refresh the data for the weather
```
* * * * * /usr/bin/python3 /home/mapmo/.config/i3/get_weather.py
```

For language switching, when using XFCE as a base. Go to Setting -> Input Method (keyboard icon) and select None instead of ibus.
