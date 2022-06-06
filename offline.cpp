#include "offline.h"
#include "ui_offline.h"
#include <QDebug>
#include <QDir>

offline::offline(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::offline)
{
    ui->setupUi(this);
    setWindowTitle("offline recite mode");
    setWindowIcon(QIcon("./resource/logo.jpg"));
    buttongroup = new QButtonGroup(this);
    buttongroup->addButton(ui->eng1,0);
    buttongroup->addButton(ui->eng2,1);
    buttongroup->addButton(ui->eng3,2);
    buttongroup->addButton(ui->eng4,3);
    buttongroup->addButton(ui->eng5,4);
    buttongroup->addButton(ui->eng6,5);
    buttongroup->addButton(ui->eng7,6);
    buttongroup->addButton(ui->eng8,7);
    buttongroup->addButton(ui->eng9,8);
    buttongroup->addButton(ui->eng10,9);
    buttongroup->addButton(ui->jap1,10);
//    ui->word->setText(QDir::currentPath());
    reciter = new wordschooser("english",englishfile);
    for (int i = 0;i <= 9;++i) {
        connect(buttongroup->button(i),&QRadioButton::toggled,this,[=]()
        {
            flag_choose = true;
            if(!buttongroup->button(i)->isChecked()) return;
            reciter->change_language("english");
            reciter->change_type(englishfile);
            reciter->change_file(QString("./words/") + buttongroup->button(i)->text() + QString(".json"));
            if(!reciter->get_error_message().isEmpty()){ui->word->setText(reciter->get_error_message());return;}
            reciter->get_words_set();
            flush_text();
        });
    }
    connect(buttongroup->button(10),&QRadioButton::toggled,this,[=]()
    {
        flag_choose = true;
        if(!buttongroup->button(10)->isChecked()) return;
        reciter->change_language("japanese");
        reciter->change_type(japanesefile);
        reciter->change_file("./words/日本語.json");
        if(!reciter->get_error_message().isEmpty()){ui->word->setText(reciter->get_error_message());return;}
        reciter->get_words_set();
        flush_text();
    });
    connect(ui->next,&QPushButton::clicked,this,[=]()
    {
        flush_text();
    });
}

offline::~offline()
{
    delete ui;
    delete reciter;
    delete buttongroup;
}

void offline::flush_text()
{
    if(!flag_choose) return;
    QVector<QString> temp = reciter->recite_offline();
    if(temp.size() <= 1) QVector<QString> temp = reciter->recite_offline();
    else if (temp.size() == 2)
    {
        ui->word->setText(temp.at(0));
        ui->chinesemeaning->setPlainText(temp.at(1));
        ui->languagemeaning->setPlainText("");
    }
    else
    {

        ui->word->setText(temp.at(0));
        ui->chinesemeaning->setPlainText(temp.at(1));
        ui->languagemeaning->setPlainText(temp.at(2));
    }
}

void offline::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        flush_text();
}


