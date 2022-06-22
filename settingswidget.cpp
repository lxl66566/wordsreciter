#include "settingswidget.h"
#include "ui_settingswidget.h"

#include <QKeyEvent>

settingswidget::settingswidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingswidget)
{
    ui->setupUi(this);
    setWindowTitle("settings");
//    setWindowIcon(QIcon(":/icon/icon.ico"));
//    setAttribute(Qt::WA_DeleteOnClose);
    buttongroup = new QButtonGroup(this);
    buttongroup->addButton(ui->web1,0);
    buttongroup->addButton(ui->web2,1);
    buttongroup2 = new QButtonGroup(this);
    buttongroup2->addButton(ui->web3,0);
    buttongroup2->addButton(ui->web4,1);
    connect(ui->okbutton,&QPushButton::clicked,this,&settingswidget::OK);
}

settingswidget::~settingswidget()
{
    delete ui;
}

void settingswidget::OK()
{
    QString url,url2;
    if(buttongroup->button(0)->isChecked())
        url = QString("https://www.merriam-webster.com/dictionary/");
    else if(buttongroup->button(1)->isChecked())
        url = QString("https://www.oxfordlearnersdictionaries.com/definition/english/");
    if(buttongroup2->button(0)->isChecked())
        url2 = QString("https://www.weblio.jp/content/");
    else if(buttongroup2->button(1)->isChecked())
        url2 = QString("https://dictionary.goo.ne.jp/srch/jn/");
    emit ok(ui->page->value(),ui->autosavetime->value(),url,url2);
    hide();
}

void settingswidget::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        OK();
}
