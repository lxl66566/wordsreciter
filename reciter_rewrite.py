import json,os,webbrowser,time
from random import choices,randint
from pathlib import Path
settingdic = {
    "mode" : 1,
    "page_nums" : 8,
    "website" : 0,
    "auto_save_time" : 30
    }
sitelib = {
    'english' : ['https://www.merriam-webster.com/dictionary/',
                'https://www.oxfordlearnersdictionaries.com/definition/english/'],
    'japanese' : ['https://www.weblio.jp/content/']
}
items = []
WRONGNAME = 'notebook'
LANGUAGE = ['english', 'japanese']
language : str
wrongdict = {'english' : [],'japanese' : []}

def init_environment():
    global settingdic,wrongdict
    try:
        with open('settings.json','r',encoding='utf-8') as f:
            settingdic = json.load(f)
    except FileNotFoundError:
        with open('settings.json','w',encoding='utf-8') as f:
            json.dump(settingdic,f,indent=4)
    try:
        with open('words' + os.sep + WRONGNAME + '.json','r',encoding='utf-8') as f:
            wrongdict = json.load(f)
    except FileNotFoundError:
        pass

def online_reciter():
    print('enter a book:')
    for (i,item) in enumerate(items):
        print(i,item,sep=' ')
    print(len(items),'add words into notebook',sep=' ')
    book = input_filter(0,len(items))
    if book != len(items):
        with open('words' + os.sep + language + os.sep + items + '.json', 'r') as f: 
            wordlist = list(json.load(f).keys())
            while True:
                words = choices(wordlist,k = settingdic['page_nums'])
                for word in words:
                    open_in_browser(word)
    else:
        addwrongdict()

def commitwrong(s:str = ''):
    with open('words' + os.sep + WRONGNAME + '.json','w',encoding='utf-8') as f:
        for l in LANGUAGE:
            wrongdict[l] = list(wrongdict[l])
        json.dump(wrongdict,f,indent=4)
    if s:
        print(s)

def addwrongdict():
    for l in LANGUAGE:
        wrongdict[l] = set(wrongdict[l])
    print(
f'''这里是添加 {language} 单词模式。请选择指令后回车。添加完毕后，请输入 exit 或 e 或 Ctrl+c 退出该模式。若直接关闭程序，可能不会添加单词。

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
                break
            if command == 'undo' or command == 'u':
                if last == '':
                    print('撤销失败：未找到上一个单词')
                    continue
                wrongdict[language].discard(last)
                print('撤销成功！')
                last = ''
            elif command.find('del') != -1:
                try:
                    wrongdict[language].remove(command.split()[-1].strip())
                    print('删除成功！')
                except KeyError:
                    print('删除失败：未找到该单词')
            else:
                last = command
                wrongdict[language].add(last)
                print('添加成功！')
    except KeyboardInterrupt:
        pass
    commitwrong('\n已保存添加单词，退出添加模式...')

def input_filter(x:int,y:int) -> int:
    while True:
        try:
            temp = (int(input()) % (y - x + 1)) + x
            break
        except ValueError:
            print('input a correct number of arguments!')
    return temp

def open_in_browser(word:str):
    webbrowser.open(sitelib[language][0] + word,0,True)

if __name__ == '__main__':
    init_environment()
    print('choose language:\n0 english\n1 japanese\n')
    language = LANGUAGE[input_filter(0,1)]
    
    for i in os.listdir('words' + os.sep + language):
        items.append(i.rstrip('.json'))
    
    online_reciter()
    