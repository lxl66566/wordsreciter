#coding=utf-8
import json
import os
from random import randint
from pathlib import Path

def reciter():
    filenme = os.listdir(os.path.dirname(__file__) + '\\words')
    temp1 = []
    for i in filenme:
        temp1.append(i.rstrip('.json'))
    for i in range(len(filenme)):
        print(i,temp1[i],sep=' ')
    for i in range(len(filenme)):
        print(i + len(filenme),temp1[i] + ' by recite mode',sep=' ')
    cin = 0
    while(1):
        try:
            cin = int(input('Choose a json to recite:'))
            if 0 <= cin < 2 * len(filenme):
                break
            else:
                print('write a correct number!')
        except ValueError:
            print('choose a number to recite!',end=' ')
    flag = False
    if cin >= len(filenme):
        cin -= len(filenme)
        flag = True
    with open(Path(os.path.dirname(__file__) + '\\words\\' + filenme[cin])) as f:
        data = json.load(f)
        k = list(data.keys())
        while(1):
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
            t2 = input()

if __name__ == '__main__' :
    reciter()