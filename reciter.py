#coding=utf-8
import json,os,sys,webbrowser,atexit
from random import choices,randint
from pathlib import Path
sitelib = ['https://www.merriam-webster.com/dictionary/','https://www.oxfordlearnersdictionaries.com/definition/english/']
settingdic = {
    "mode" : 1,
    "page_nums" : 8,
    "website" : 0
    }
wrongdict = set([])
DIRNAME = os.path.dirname(__file__)
FILENAME = os.listdir(DIRNAME + os.sep + 'words')
itemname = []
cin = -1
WRONGNAME = '错题本ForEnglish'
ADD_WRONG_ITEM = '向' + WRONGNAME + '加入单词'

def loadjson():
    global settingdic,wrongdict
    try:
        with open(DIRNAME + os.sep + 'settings.json','r',encoding='utf-8') as f:
            settingdic = json.load(f)
    except FileNotFoundError:
        with open(DIRNAME + os.sep + 'settings.json','w',encoding='utf-8') as f:
            json.dump(settingdic,f,indent=4)
    try:
        with open(DIRNAME + os.sep + 'words' + os.sep + WRONGNAME + '.json','r',encoding='utf-8') as f:
            temp = json.load(f)
            wrongdict = set(temp['default'])
    except FileNotFoundError:
        pass

@atexit.register
def commitwrong():
    if itemname[cin] != ADD_WRONG_ITEM:
        return
    with open(DIRNAME + os.sep + 'words' + os.sep + WRONGNAME + '.json','w',encoding='utf-8') as f:
        temp = {'default':list(wrongdict)}
        json.dump(temp,f,indent=4)

def addwrongdict():
    print(
'''这里是添加单词模式。请选择指令后回车。添加完毕后，请正常关闭程序。
添加单词：直接输入要添加的单词
撤销上一个添加的单词：输入 undo
删除单词：输入 del 与你要删除的单词，以空格键隔开'''
    )
    last = ''
    while True:
        command = input()
        if command == 'exit':
            sys.exit()
        if command == 'undo':
            if last == '':
                print('撤销失败：未找到上一个单词')
                continue
            wrongdict.discard(last)
            print('撤销成功！')
            last = ''
        elif command.find('del') != -1:
            try:
                wrongdict.remove(command.split()[-1].strip())
                print('删除成功！')
            except KeyError:
                print('删除失败：未找到该单词')
        else:
            last = command
            wrongdict.add(last)
            print('添加成功！')

def open_in_website(url:str):
    webbrowser.open(url,0,True)

def print_item_names():
    for i in range(len(itemname)):
        print(i,itemname[i],sep=' ')

if __name__ == '__main__':
    loadjson()
    page_nums = settingdic["page_nums"]
    mode = settingdic["mode"]
    website = settingdic["website"]

    for i in FILENAME:
        itemname.append(i.rstrip('.json'))
    if mode != 1:
        if itemname.find(WRONGNAME) != -1:
            itemname.remove(WRONGNAME)
        for i in range(len(itemname)):
            itemname.append(itemname[i] + ' - by recite mode')
    itemname.append(ADD_WRONG_ITEM)
    print_item_names()
    cin = 0
    while(1):
        try:
            cin = int(input('Choose an item:'))
            if 0 <= cin < len(itemname):
                break
            else:
                print('please input correct number!')
        except ValueError:
            print('please input a number!',end=' ')

    if cin == len(itemname) - 1:    #最后一个
        addwrongdict()
    flag = False
    if cin >= len(FILENAME):
        flag = True
    cin %= len(FILENAME)
    print('当前选择：' + FILENAME[cin])

    with open(Path(DIRNAME + os.sep + 'words' + os.sep + FILENAME[cin])) as f:
        data = json.load(f)
        if FILENAME[cin] == WRONGNAME + '.json':
            k = data['default']
        else:
            k = list(data.keys())
        while(1):
            if mode == 1:
                s = choices(k,k= page_nums)
                for i in s:
                    if FILENAME[cin] == '日本語.json':
                        url = f'https://www.weblio.jp/content/{i}'
                    else:
                        url = sitelib[website % len(sitelib)] + f'{i}'
                    open_in_website(url)
                print('press enter to open next pages')
            elif mode == 2:
                try:
                    s = randint(0,len(data) - 1)
                    if FILENAME[cin] == '日本語.json':
                        print(k[s],end='')
                    else:
                        print(k[s],end='   ')
                    if flag:
                        t2 = input()
                    if FILENAME[cin] == '日本語.json':
                        print(data[k[s]])
                    else:
                        print(data[k[s]]['中释'].lstrip(' '),data[k[s]]['英释'],sep='  ')
                except KeyError:
                    print(list(data[k[s]].values())[0])
            t = input()