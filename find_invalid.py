import re,json
from pathlib import Path
from contextlib import suppress

def go():
    failed = set()
    with suppress(FileNotFoundError):
        with open("failed.json",'r',encoding='utf-8') as f:
            failed = set(json.load(f))

    s = ["./forward.txt","./reverse.txt"]
    for i in s:
        te = Path(i).read_text(encoding='utf-8')
        for j in re.findall(r"\n(\w+)\t(<br>)+\n",te):
            failed.add(j[0])
        Path(i).write_text(re.sub(r"\n\w+\t(<br>)+\n",'\n',te),encoding='utf-8')

    with open("failed.json",'w') as f:
        json.dump(list(failed),f,indent=4, ensure_ascii=False)