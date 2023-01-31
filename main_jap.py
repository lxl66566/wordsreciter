import requests,json
from bs4 import BeautifulSoup
from fake_useragent import UserAgent
from contextlib import suppress
from pathlib import Path
import re

useragent = UserAgent()
header = {'User-Agent': useragent.random}
notebook = set()
exceptbook = set()
failed_words = set()

forward = []    
# reversed = []   # 反向词书不需要例句 由于不常用，取消反向词书

with suppress(FileNotFoundError):
    with open('./notebook.json','r',encoding='utf-8') as f:
        notebook = set(json.load(f)['japanese']['default'])
with suppress(FileNotFoundError):
    with open('./exceptbook_jap.json','r',encoding='utf-8') as f:
        exceptbook = set(json.load(f))
with suppress(FileNotFoundError):
    with open('./failed_jap.json','r',encoding='utf-8') as f:
        failed_words = set(json.load(f))
notebook.difference_update(exceptbook)

j = 9999  # test
for word in notebook:
    if j <= 0:
        break
    j -= 1

    print(''.join([str(len(exceptbook) + 1),' : ',word]))
    exceptbook.add(word)

    url = "https://www.weblio.jp/content/{}".replace("{}",word)
    try:
        response = requests.get(url,headers=header) # ,proxies={"http":"http://127.0.0.1:10359"}
    except:
        break
    soup = BeautifulSoup(response.content, 'html.parser')

    with suppress(AttributeError):
        soup.find(text = re.compile('読み方')).insert_after('<br><br>')

    japanesetranslation = []
    definition_pos = soup.find('div',class_="kijiWrp")
    if definition_pos is None:
        failed_words.add(word)
        continue

    forward.append(word)
    forward.append('\t')
    for i in definition_pos.find_all('div',class_="synonymsUnderDictWrp"):
        i.decompose()
    text = definition_pos.text

    text_list = text.split()
    text_list[0] = re.sub(r'〔.*〕','',text_list[0])
    if re.match(r'の例文・使い方・用例・文例', text_list[0]) is not None:
        text_list[0] = '例文だけ'
    if re.match(r'読み方', text_list[1]) is not None:
        text_list[1].replace('１','<br>')
    text_list.insert(1,"<br>")
    for i in range(1,len(text_list)):
        if len(text_list[i]) == 1:
            text_list[i] = r'<br>'
        # text_list[i] = re.sub(r'―',word,text_list[i])

    forward.extend(text_list)
    forward.append('\n')

Path("forward_jap.txt").write_text(''.join(forward),encoding='utf-8')

with open('./exceptbook_jap.json','w',encoding='utf-8') as f:
    json.dump(list(exceptbook),f,indent=4, ensure_ascii=False)
with open('./failed_jap.json','w',encoding='utf-8') as f:
    json.dump(list(failed_words),f,indent=4, ensure_ascii=False)