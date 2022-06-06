#include "settingswidget.h"
#include "ui_settingswidget.h"

settingswidget::settingswidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingswidget)
{
    ui->setupUi(this);
    setWindowTitle("settings");
    setWindowIcon(QIcon("./resource/logo.jpg"));
    buttongroup = new QButtonGroup(this);
    buttongroup->addButton(ui->web1,0);
    buttongroup->addButton(ui->web2,1);
    connect(ui->okbutton,&QPushButton::clicked,this,[=]()
    {
        hide();
        QString url;
        if(buttongroup->button(0)->isChecked())
            url = QString("https://www.merriam-webster.com/dictionary/");
        else if(buttongroup->button(1)->isChecked())
            url = QString("https://www.oxfordlearnersdictionaries.com/definition/english/");
        emit ok(ui->page->value(),ui->autosavetime->value(),url);
    });
}

settingswidget::~settingswidget()
{
    delete ui;
}
