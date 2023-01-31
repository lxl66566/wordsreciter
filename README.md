# anki-conversion
由于本人现在全面倒向 [anki](https://apps.ankiweb.net/)，因此写了个脚本将我的 `notebook.json` 与爬虫爬下来的翻译，转为一定格式的 txt 文件并导入 anki。

但是，wordsreciter 仍具有它的优势，它的设计初衷就是能够方便地随时记词，因此我短时间内不会弃用 wordsreciter，脚本也能够重复使用。

## 使用方法
1. 安装所需 python 环境。
2. `git clone` 或 下载源码。
3. 删除所有`.json`文件。（cmd: `del *.json`）
4. 将你的`notebook.json`复制到目录下并在当前目录执行`main.py`（日语：`main_jap.py`）。

## 其他
* `_jap` 结尾的为日语爬虫程序与中间件。其余为英语。
* `exceptbook.json`表示此次添加成功的单词，当`notebook.json`变动时，此中单词将被排除，避免重复搜索。
* `failed.json`表示此次爬取释义失败的单词。需要手动添加，请在手动添加后删除条目。
* `forward.txt`以 Basic 形式导入 anki，为单词 --背--> 释义。
* `reverse.txt`以 Basic(type in the answer) 形式导入 anki，Front & Back设为反向，为释义 --背--> 单词。（日语程序废除了 `reverse.txt` 的设定）