# wordsreciter
## 介绍
这是一个轻量化的、简陋的英语背词python程序

支持自己添加词库(.json)文件，自带10个词库

支持阅读与背诵双模式，多数单词有中英双释

![image](https://user-images.githubusercontent.com/88281489/160550019-4aa8b438-81a0-4a6c-9b7b-a426e3112adc.png)
![image](https://user-images.githubusercontent.com/88281489/160550106-234ec3d7-df53-4b71-acfc-8b05ef729cf0.png)  
## 更新
### v1.1(20220408)
新增日语词库与日语背词功能
### v2.0(20220525)
为了更好地融入语言环境，v2.0版本将背词方式转为英-英与日-日，使用默认浏览器打开网页背词。（英：[牛津词典](https://www.oxfordlearnersdictionaries.com/) 日：[weblio词典](https://www.weblio.jp/)）

同时保留v1.1的全部功能，若想使用原先方法背词请[更改](#settingsjson)`settings.json`。
## 说明
### settings.json
`mode`：模式，1为网页打开模式，2为旧版背词模式
`page_nums`：一次性打开网页的数量