import requests,json
from bs4 import BeautifulSoup
from fake_useragent import UserAgent
from contextlib import suppress
from pathlib import Path

useragent = UserAgent()
header = {'User-Agent': useragent.random}
notebook = set()
exceptbook = set()

forward = []    
reversed = []   # 反向词书不需要例句

with suppress(FileNotFoundError):
    with open('./notebook.json','r',encoding='utf-8') as f:
        notebook = set(json.load(f)['english']['default'])
with suppress(FileNotFoundError):
    with open('./exceptbook.json','r',encoding='utf-8') as f:
        exceptbook = set(json.load(f))
notebook.difference_update(exceptbook)

j = 9999  # test
for word in notebook:
    if j <= 0:
        break
    j -= 1

    print(''.join([str(len(exceptbook) + 1),' : ',word]))
    exceptbook.add(word)

    url = "https://dictionary.cambridge.org/dictionary/english-chinese-simplified/{}".replace("{}",word)
    try:
        response = requests.get(url,headers=header) # ,proxies={"http":"http://127.0.0.1:10359"}
    except:
        break

    soup = BeautifulSoup(response.content, 'html.parser')
    for i in soup.find_all(class_="pr phrase-block dphrase-block"): # 排除短语框
        i.decompose()
    
    forward.append(word)
    forward.append('\t')
    reversed.append(word)
    reversed.append('\t')

    englishtranslation = []
    # english translation
    definition_pos = soup.find_all('div',class_="pr entry-body__el")
    try:
        for i in definition_pos:
            treatise_definition = i.find_all('div', class_='def ddef_d db')
            for one_definition in treatise_definition:
                englishtranslation.append(one_definition.text.rstrip().rstrip(':').replace('\n',' '))
    except AttributeError:
        continue

    examples = []
    # examples
    try:
        for i in soup.find_all('span',class_="deg",limit=10):
            examples.append(i.text.strip().replace('\n',' '))
    except AttributeError:
        continue

    chinesetranslation = []
    #chinese translation
    for i in soup.find_all('div',class_="examp dexamp"):
        i.decompose()
    try:
        for i in soup.find_all('span',class_=['trans','dtrans','dtrans-se' ,'break-cj']):
            chinesetranslation.append(i.text.strip().replace('\n',' '))
    except AttributeError:
        continue
    
    temp = []
    temp.append('<br>'.join(chinesetranslation))
    temp.append('<br><br><br>')
    temp.append('<br>'.join(englishtranslation))
    reversed.append(''.join(temp))
    reversed.append('\n')

    temp.append('<br><br><br>')
    temp.append('<br>'.join(examples))
    forward.append(''.join(temp))
    forward.append('\n')


Path("forward.txt").write_text(''.join(forward),encoding='utf-8')
Path("reverse.txt").write_text(''.join(reversed),encoding='utf-8')

with open('./exceptbook.json','w',encoding='utf-8') as f:
    json.dump(list(exceptbook),f,indent=4, ensure_ascii=False)

import find_invalid
for i in range(2):
    find_invalid.go()