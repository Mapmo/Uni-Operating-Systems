import argparse

import pandas as pd
import requests
from bs4 import BeautifulSoup


def left_align(df):
    return df.applymap(str).apply(lambda col: col.str.ljust(col.map(len).max()), axis=0)


parser = argparse.ArgumentParser(description="Scrape buy/sell prices from a webpage.")
parser.add_argument("--type", help="URL of the webpage to scrape")
args = parser.parse_args()

url = "http://igold.bg/"
if args.type == "bar":
    url += "zlatni-kyulcheta-investitsionni"
elif args.type == "new":
    url += "moderni-investitzionni-moneti"
elif args.type == "old":
    url += "istoricheski-investitzionni-moneti"
elif args.type == "silv":
    url += "srebro"
elif args.type == "plat":
    url += "platina"
elif args.type == "pall":
    url += "paladiy"

crawl = requests.get(url)
soup = BeautifulSoup(crawl.text, "lxml")
ul = soup.find("ul", {"class": "kv__member-list"})
data = []
# Loop through each <li> inside the <ul>
for li in ul.find_all("li"):
    #buy_dt = li.find("span", class_="type-outer obnovi cat2-94")
    #sell_dt = li.find("dt", class_="kv__member-cat-right")
    dt = [span.text for span in li.find_all("span", class_="type-outer") if "лв." in span.text]
    name_h2 = li.find("dd", class_="kv__member-name").find("h2")
    if dt and name_h2:
        # Extract numeric values from the text
        buy_text = dt[0]
        sell_text = dt[1]
        name_text = name_h2.get_text(strip=True)
        # Extract numbers (you can also use regex for more complex formats)
        buy_price = float(buy_text.split()[0])
        sell_price = float(sell_text.split()[0])
        if not sell_price:
            continue
        ratio = ((sell_price - buy_price) / sell_price) * 100
        data.append(
            {
                "Name": name_text,
                "Ratio": round(ratio, 2),
                "Buy": buy_price,
                "Sell": sell_price,
            }
        )

df = pd.DataFrame(data)
if not df.size:
    print("DF is empty")
    exit(0)
df_sorted = df.sort_values("Ratio", ascending=False)
print(left_align(df_sorted).to_string(index=False))
