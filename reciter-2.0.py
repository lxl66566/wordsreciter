#coding=utf-8
import json
import os
from random import choices,randint
from pathlib import Path
import webbrowser


try:
    with open(os.path.dirname(__file__) + os.sep + 'settings.json','r',encoding='utf-8') as f:
        settingdic = json.load(f)
except FileNotFoundError:
    settingdic = {"mode" : 1,"page_nums" : 8}
    with open(os.path.dirname(__file__) + os.sep + 'settings.json','w',encoding='utf-8') as f:
        json.dump(settingdic,f,indent=4)

page_nums = settingdic["page_nums"]
mode = settingdic["mode"]
filenme = os.listdir(os.path.dirname(__file__) + os.sep + 'words')
temp1 = []
for i in filenme:
    temp1.append(i.rstrip('.json'))
for i in range(len(filenme)):
    print(i,temp1[i],sep=' ')
if mode != 1:
    for i in range(len(filenme)):
        print(i + len(filenme),temp1[i] + ' by recite mode',sep=' ')
cin = 0
while(1):
    try:
        cin = int(input('Choose a dictionary to recite:'))
        if 0 <= cin < len(filenme) * mode:
            break
        else:
            print('please input correct number!')
    except ValueError:
        print('please input a number!',end=' ')

flag = False
if cin >= len(filenme):
    cin -= len(filenme)
    flag = True

with open(Path(os.path.dirname(__file__) + '\\words\\' + filenme[cin])) as f:
    data = json.load(f)
    k = list(data.keys())
    while(1):
        if mode == 1:
            s = choices(k,k= page_nums)
            for i in s:
                if filenme[cin] == '日本語.json':
                    url = f'https://www.weblio.jp/content/{i}'
                else:
                    url = f'https://www.oxfordlearnersdictionaries.com/definition/english/{i}'
                webbrowser.open(url,0,True)
        elif mode == 2:
            try:
                s = randint(0,len(data) - 1)
                if filenme[cin] == '日本語.json':
                    print(k[s],end='')
                else:
                    print(k[s],end='   ')
                if flag:
                    t2 = input()
                if filenme[cin] == '日本語.json':
                    print(data[k[s]])
                else:
                    print(data[k[s]]['中释'].lstrip(' '),data[k[s]]['英释'],sep='  ')
            except KeyError:
                print(list(data[k[s]].values())[0])
        t = input()