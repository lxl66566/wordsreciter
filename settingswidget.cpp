#include "settingswidget.h"
#include "ui_settingswidget.h"

#include <QKeyEvent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

settingswidget::settingswidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingswidget)
{
    ui->setupUi(this);
    setWindowTitle("settings");

    buttongroup = new QButtonGroup(this);
    buttongroup->addButton(ui->web1,0);
    buttongroup->addButton(ui->web2,1);
    buttongroup2 = new QButtonGroup(this);
    buttongroup2->addButton(ui->web3,0);
    buttongroup2->addButton(ui->web4,1);

    settext("Merriam","Oxford","weblio","goo");
    seturl(QString("https://www.merriam-webster.com/dictionary/{}"),
           QString("https://www.oxfordlearnersdictionaries.com/definition/english/{}"),
           QString("https://www.weblio.jp/content/{}"),
           QString("https://dictionary.goo.ne.jp/srch/jn/{}/m6u/"));

    readjson();
    display();

    connect(ui->okbutton,&QPushButton::clicked,this,&settingswidget::OK);
    connect(ui->diyurl,&QPushButton::clicked,this,[=]()
    {
        urlwidget = new url_settingswidget(text,urls,this);
        connect(urlwidget,&url_settingswidget::ok_url,this,&settingswidget::seturl);
        connect(urlwidget,&url_settingswidget::ok_text,this,&settingswidget::settext);
        urlwidget->show();
    });
    for (int i = 0; i <= 1;++i) {
        connect(buttongroup->button(i),&QAbstractButton::toggled,this,[=]()
        {
            eng_selected = buttongroup->button(0)->isChecked() ? 0 : 1;
        });
        connect(buttongroup2->button(i),&QAbstractButton::toggled,this,[=]()
        {
            jap_selected = buttongroup2->button(0)->isChecked() ? 0 : 1;
        });
    }
    connect(ui->page,SIGNAL(valueChanged(int)),this,SLOT(setpages(int)));
    connect(ui->autosavetime,SIGNAL(valueChanged(int)),this,SLOT(setautosavetime(int)));

}

settingswidget::~settingswidget()
{
    delete ui;
}

void settingswidget::OK()
{
    QString url,url2;
    url = !eng_selected ? urls[0] : urls[1];
    url2 = !jap_selected ? urls[2] : urls[3];
    emit ok(pages,auto_save_time,url,url2);
    hide();
    save();
}

void settingswidget::settext(QString t1, QString t2, QString t3, QString t4)
{
    text[0] = t1,text[1] = t2,text [2] = t3,text[3] = t4;
    display();
}

void settingswidget::seturl(QString t1, QString t2, QString t3, QString t4)
{
    urls[0] = t1,urls[1] = t2,urls [2] = t3,urls[3] = t4;
}

void settingswidget::readjson()
{
    QFile file("settings.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "open file failed.";
        return;
    }
    QByteArray data(file.readAll());
    file.close();
    QJsonParseError jError;
    QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
    if(jError.error != QJsonParseError::NoError){
        qDebug() << "read json error.";
        return;
    }
    QJsonObject rootobj = jDoc.object();

    pages = rootobj["pages"].toInt();
    auto_save_time = rootobj["auto_save_time"].toInt();
    eng_selected = rootobj["eng_selected"].toInt();
    jap_selected = rootobj["jap_selected"].toInt();
    QJsonArray temp = rootobj["text"].toArray();
    for(int i = 0;i < 4;++i)
        text[i] = temp.at(i).toString();
    QJsonArray temp2 = rootobj["urls"].toArray();
    for(int i = 0;i < 4;++i)
        urls[i] = temp2.at(i).toString();
}

void settingswidget::save()
{
    QJsonObject rootobj;

    rootobj.insert("pages",QJsonValue(pages));
    rootobj.insert("auto_save_time",QJsonValue(auto_save_time));
    rootobj.insert("eng_selected",QJsonValue(eng_selected));
    rootobj.insert("jap_selected",QJsonValue(jap_selected));
    QJsonArray temp;
    for (int i = 0;i < 4;++i) {
        temp.push_back(QJsonValue(text[i]));
    }
    rootobj.insert("text",temp);
    QJsonArray temp2;
    for (int i = 0;i < 4;++i) {
        temp2.push_back(QJsonValue(urls[i]));
    }
    rootobj.insert("urls",temp2);

    QJsonDocument jDoc(rootobj);
    QFile file("settings.json");
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "write json error.";
        return;
    }
    QByteArray data(jDoc.toJson());
    file.write(data);
    file.close();
}

void settingswidget::display()
{
    ui->page->setValue(pages);
    ui->autosavetime->setValue(auto_save_time);
    ui->web1->setText(text[0]);
    ui->web2->setText(text[1]);
    ui->web3->setText(text[2]);
    ui->web4->setText(text[3]);
    if(!eng_selected) ui->web1->setChecked(true);
    else ui->web2->setChecked(true);
    if(!jap_selected) ui->web3->setChecked(true);
    else ui->web4->setChecked(true);
}

void settingswidget::setpages(int i)
{
    pages = i;
}

void settingswidget::setautosavetime(int i)
{
    auto_save_time = i;
}

void settingswidget::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        OK();
}
