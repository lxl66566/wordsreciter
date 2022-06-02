#coding=utf-8
import json,os,webbrowser,time #,atexit,sys
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
filename = os.listdir(DIRNAME + os.sep + 'words')
itemname = []
cin = -1
WRONGNAME = 'notebook'
ADD_WRONG_ITEM = '向 ' + WRONGNAME + ' 加入单词'
AUTO_SAVE_TIME = 30

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

# @atexit.register
def commitwrong(s:str = ''):
    if itemname[cin] != ADD_WRONG_ITEM:
        return
    with open(DIRNAME + os.sep + 'words' + os.sep + WRONGNAME + '.json','w',encoding='utf-8') as f:
        temp = {'default':list(wrongdict)}
        json.dump(temp,f,indent=4)
    if s:
        print(s)

def addwrongdict():
    print(
'''这里是添加单词模式。请选择指令后回车。添加完毕后，请输入 exit 或 e 或 Ctrl+c 退出该模式。若直接关闭程序，可能不会添加单词。

添加单词：直接输入要添加的单词
撤销上一个添加的单词：输入 undo 或 u
删除单词：输入 del 与你要删除的单词，以空格键隔开
'''
    )
    last = ''
    begin = time.time()
    try:
        while True:
            if time.time() - begin > AUTO_SAVE_TIME:
                begin = time.time()
                commitwrong('已自动保存！')
            command = input()
            if command == 'exit' or command == 'e':
                # sys.exit()
                break
            if command == 'undo' or command == 'u':
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
    except KeyboardInterrupt:
        pass
    commitwrong('\n已保存添加单词，退出添加模式...')

def open_in_website(url:str):
    webbrowser.open(url,0,True)

def print_item_names():
    for i in range(len(itemname)):
        print(i,itemname[i],sep=' ')

def reciter():
    global filename
    filename = os.listdir(DIRNAME + os.sep + 'words')
    itemname.clear()
    for i in filename:
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
        return
    flag = False
    if cin >= len(filename):
        flag = True
    cin %= len(filename)
    print('当前选择：' + filename[cin])

    with open(Path(DIRNAME + os.sep + 'words' + os.sep + filename[cin])) as f:
        data = json.load(f)
        if filename[cin] == WRONGNAME + '.json':
            k = data['default']
        else:
            k = list(data.keys())
        while(1):
            if mode == 1:
                s = choices(k,k= page_nums)
                for i in s:
                    if filename[cin] == '日本語.json':
                        url = f'https://www.weblio.jp/content/{i}'
                    else:
                        url = sitelib[website % len(sitelib)] + f'{i}'
                    open_in_website(url)
                print('press enter to open next pages')
            elif mode == 2:
                try:
                    s = randint(0,len(data) - 1)
                    if filename[cin] == '日本語.json':
                        print(k[s],end='')
                    else:
                        print(k[s],end='   ')
                    if flag:
                        t2 = input()
                    if filename[cin] == '日本語.json':
                        print(data[k[s]])
                    else:
                        print(data[k[s]]['中释'].lstrip(' '),data[k[s]]['英释'],sep='  ')
                except KeyError:
                    print(list(data[k[s]].values())[0])
            t = input()

if __name__ == '__main__':
    loadjson()
    page_nums = settingdic["page_nums"]
    mode = settingdic["mode"]
    website = settingdic["website"]
    while True:
        reciter()