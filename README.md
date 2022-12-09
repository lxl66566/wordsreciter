# anki-conversion
由于本人现在全面倒向 [anki](https://apps.ankiweb.net/)，因此写了个脚本将我的 `notebook.json` 与爬虫爬下来的翻译，转为一定格式的 txt 文件并导入 anki。

但是，wordsreciter 仍具有它的优势，它的设计初衷就是能够随时记词，因此我短时间内不会弃用 wordsreciter，脚本也会经常使用。

## 使用方法
安装所需环境。`git clone` 或 下载源码，删除所有`.json`文件，将你的`notebook.json`复制到目录下并在当前目录执行`main.py`。

## 其他
* `exceptbook.json`表示此次添加成功的单词，下次`notebook.json`变动时，此中单词将被排除。
* `failed.json`表示此次爬取释义失败的单词。需要手动添加，请在手动添加后删除条目。
* `forward.txt`以 Basic 形式导入 anki，为单词 --背--> 中英释义。
* `reverse.txt`以 Basic(type in the answer) 形式导入 anki，Front & Back设为反向，为中英释义 --背--> 单词。