#coding=utf-8
import json,os,webbrowser,time #,atexit,sys
from random import choices,randint
from pathlib import Path
sitelib = ['https://www.merriam-webster.com/dictionary/','https://www.oxfordlearnersdictionaries.com/definition/english/']
settingdic = {
    "mode" : 1,
    "page_nums" : 8,
    "website" : 0,
    "auto_save_time" : 30
    }
LANGUAGE_MAP = ['english','japanese']
DIRNAME = os.path.dirname(__file__)
filename = os.listdir(DIRNAME + os.sep + 'words')
itemname = []
cin = -1
WRONGNAME = 'notebook'
ADD_WRONG_ITEM = ['向 ' + WRONGNAME + ' 加入英语单词','向 ' + WRONGNAME + ' 加入日语单词']
wrongdict = {}
language_set = [set(),set()]

def loadjson():
    global settingdic,wrongdict,language_set
    try:
        with open(DIRNAME + os.sep + 'settings.json','r',encoding='utf-8') as f:
            settingdic = json.load(f)
    except FileNotFoundError:
        with open(DIRNAME + os.sep + 'settings.json','w',encoding='utf-8') as f:
            json.dump(settingdic,f,indent=4)
    try:
        with open(DIRNAME + os.sep + 'words' + os.sep + WRONGNAME + '.json','r',encoding='utf-8') as f:
            wrongdict = json.load(f)
            language_set = [set(wrongdict[LANGUAGE_MAP[i]]['default']) for i in range(2)]
    except FileNotFoundError:
        for i in LANGUAGE_MAP:
            wrongdict[i] = {'default' : []}

# @atexit.register
def commitwrong(s:str = ''):
    if itemname[cin] not in ADD_WRONG_ITEM:
        return
    with open(DIRNAME + os.sep + 'words' + os.sep + WRONGNAME + '.json','w',encoding='utf-8') as f:
        for i in range(2):
            wrongdict[LANGUAGE_MAP[i]]['default'] = list(language_set[i])
        json.dump(wrongdict,f,indent=4)
    if s:
        print(s)

def addwrongdict(language : int):# 0,1
    language_str = LANGUAGE_MAP[language]
    print(
f'''这里是添加 {language_str} 单词模式。请选择指令后回车。添加完毕后，请输入 exit 或 e 或 Ctrl+c 退出该模式。若直接关闭程序，可能不会添加单词。

添加单词：直接输入要添加的单词
撤销上一个添加的单词：输入 undo 或 u
删除单词：输入 del 与你要删除的单词，以空格键隔开
'''
    )
    last = ''
    begin = time.time()
    try:
        while True:
            if time.time() - begin > settingdic['auto_save_time']:
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
                language_set[language].discard(last)
                print('撤销成功！')
                last = ''
            elif command.find('del') != -1:
                try:
                    language_set[language].remove(command.split()[-1].strip())
                    print('删除成功！')
                except KeyError:
                    print('删除失败：未找到该单词')
            else:
                last = command
                language_set[language].add(last)
                print('添加成功！')
    except KeyboardInterrupt:
        pass
    commitwrong('\n已保存添加单词，退出添加模式...')

def open_in_website(url:str):
    webbrowser.open(url,0,True)

def print_item_names():
    for i in range(len(itemname)):
        print(i,itemname[i],sep=' ')

def choose_language() -> int:
    print('please choose a language:')
    for i in range(len(LANGUAGE_MAP)):
        print(i,LANGUAGE_MAP[i],sep=' ')
    while True:
        try:
            temp = (int(input())) % len(LANGUAGE_MAP)
            break
        except ValueError:
            print('please write a correct number! choose a language:')

    return temp

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
    for i in ADD_WRONG_ITEM:
        itemname.append(i)
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

    if itemname[cin] in ADD_WRONG_ITEM:    # 添加notebook
        addwrongdict((cin - len(itemname) + 2) % len(ADD_WRONG_ITEM))# 0,1
        return
    flag = False
    if cin >= len(filename):
        flag = True
    cin %= len(filename)
    print('当前选择：' + filename[cin])

    with open(Path(DIRNAME + os.sep + 'words' + os.sep + filename[cin])) as f:
        data = json.load(f)
        temp = 0
        if filename[cin] == WRONGNAME + '.json':
            temp = choose_language()
            k = list(language_set[temp])
        else:
            k = list(data.keys())
        while(1):
            if mode == 1:
                s = choices(k,k= page_nums)
                for i in s:
                    if filename[cin] == '日本語.json' or temp:
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