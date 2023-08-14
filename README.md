# wordsreciter
本项目经历了几次变化：*python terminal* -> *Qt5 + qmake* -> ***Qt6 + xmake***.

基本功能：
1. 笔记本与在线背词
    * 支持添加、撤销、删除单词功能
    * 支持随机背诵（在指定网站或自定义网站中打开）
    * 支持背诵最近 1 个添加的单词
    * 支持自定义背词网址
    * 支持深色模式，默认检测主题
2. 离线背词器
    * 支持选择英语日语共 11 本词典进行随机背诵
    > 英语有大约一半的单词有中英双释

3. 迁移
    * 支持通过爬取单词释义，将笔记本记录的单词[迁移到 anki](https://github.com/lxl66566/wordsreciter/tree/anki)。

特色功能：
1. 定时保存，不用担心非正常关闭丢失数据（关闭时也会保存）
2. 最小化为不起眼的小窗，存在感不高却可在需要记下单词时快速唤起
3. 快捷键 Ctrl+← 快速唤起/隐藏；输入单词后双击回车快速添加并隐藏

![image](https://user-images.githubusercontent.com/88281489/174651573-9bb43d90-80be-472a-9dce-4650ee1e17bb.png)
![image](https://user-images.githubusercontent.com/88281489/175872596-9e683d83-ab8a-47f3-a65a-707eb2a4ce8e.png)
![image](https://user-images.githubusercontent.com/88281489/172057400-c0fe2f8b-1fc9-45ce-a820-d94d9a145230.png)
![image](https://user-images.githubusercontent.com/88281489/175872616-67539688-8561-476e-89e4-334ec6414b5b.png)
