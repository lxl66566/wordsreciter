#coding=utf-8
import json
import os
from random import choices
from pathlib import Path
import webbrowser


try:
    with open(os.path.dirname(__file__) + os.sep + 'settings.json','r',encoding='utf-8') as f:
        settingdic = json.load(f)
    page_nums = settingdic["page_nums"]
except FileNotFoundError:
    settingdic = {"page_nums" : 8}
    with open(os.path.dirname(__file__) + os.sep + 'settings.json','w',encoding='utf-8') as f:
        json.dump(settingdic,f,indent=4)
    page_nums = 8

filenme = os.listdir(os.path.dirname(__file__) + os.sep + 'words')
temp1 = []
for i in filenme:
    temp1.append(i.rstrip('.json'))
for i in range(len(filenme)):
    print(i,temp1[i],sep=' ')
cin = 0
while(1):
    try:
        cin = int(input('Choose a dictionary to recite:'))
        if 0 <= cin < len(filenme):
            break
        else:
            print('please input correct number!')
    except ValueError:
        print('please input a number!',end=' ')
with open(Path(os.path.dirname(__file__) + '\\words\\' + filenme[cin])) as f:
    data = json.load(f)
    k = list(data.keys())
    while(1):
        s = choices(k,k= page_nums)
        for i in s:
            if filenme[cin] == '日本語.json':
                url = f'https://www.weblio.jp/content/{i}'
            else:
                url = f'https://www.oxfordlearnersdictionaries.com/definition/english/{i}'
            webbrowser.open(url,0,True)
        t = input()