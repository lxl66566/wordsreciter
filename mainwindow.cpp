#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "callbackwidget.h"
#include <QDebug>
#include <QTimer>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QIcon>
#include <QVector>
bool is_alpha(QString s)
{
    for(auto i = s.begin();i != s.end();++i)
        if(!((*i) >= 'a' && (*i) <= 'z') && !((*i) >= 'A' && (*i) <= 'Z'))
            return false;
    return true;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("reciter");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
//    setFocusPolicy(Qt::StrongFocus);
    language = "english";

    callback = new callbackwidget(this);
    callback->hide();
    connect(callback,&callbackwidget::activate,this,&MainWindow::activated);

    reciter = new wordschooser(language,notebook);

    setting = new settingswidget(this);
    setting->hide();
    settings(setting->pages,setting->auto_save_time,
            !setting->eng_selected ? setting->urls[0] : setting->urls[1],
            !setting->jap_selected ? setting->urls[2] : setting->urls[3]);
    connect(setting,&settingswidget::ok,this,&MainWindow::settings);
    connect(ui->actionsettings,&QAction::triggered,this,[=](){
        ui->elements->hide();
        ui->menubar->hide();
        setting->show();
        setting->raise();
    });

    buttongroup = new QButtonGroup(this);
    buttongroup->addButton(ui->english,0);
    buttongroup->addButton(ui->japanese,1);
    buttongroup->button(0)->setChecked(true);
    reciter->seturl(url_for_endlish);
    connect(buttongroup->button(0),&QAbstractButton::clicked,this,[=](){
        language = "english";
        reciter->change_language_with_save(language);
        reciter->seturl(url_for_endlish);
        reciter->clear_last();
    });
    connect(buttongroup->button(1),&QAbstractButton::clicked,this,[=](){
        language = "japanese";
        reciter->change_language_with_save(language);
        reciter->seturl(url_for_japanese);
        reciter->clear_last();
    });

    ui->message->setText(reciter->get_error_message());
    connect(ui->submit,&QPushButton::clicked,this,&MainWindow::add_word);
    connect(ui->undo,&QPushButton::clicked,this,[=]()
    {
        auto temp = reciter->undo();
        if(!temp.isEmpty())
            give_message("???????????????");
        else give_message("???????????????");
        ui->word->insert(temp);
        ui->word->setFocus();
    });
    connect(ui->del,&QPushButton::clicked,this,[=]()
    {
        if(ui->word->text().isEmpty()) return;
        if(reciter->del_word(ui->word->text()))
            give_message("???????????????");
        else give_message("???????????????");
        ui->word->clear();
    });
    connect(ui->recite,&QPushButton::clicked,this,[=](){
        give_message(reciter->recite(open_pages_num));
    });
    connect(ui->recite_last,&QPushButton::clicked,this,[=](){
        QString temp = reciter->get_last();
        if(temp.isEmpty())
        {
            give_message("?????????????????????");
            return;
        }
        reciter->open_a_word(temp);
    });

    connect(ui->actionoffline_mode,&QAction::triggered,this,[=]()
    {
        offlinewidget = new offline();
        offlinewidget->show();
        offlinewidget->raise();
    });
    connect(ui->actionread_notebook,&QAction::triggered,this,[=]()
    {
        notebook_widget = new see_notebook_widget();
        notebook_widget->show();
        notebook_widget->raise();
    });

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::clear_message);
    timer_save = new QTimer(this);
    connect(timer_save,&QTimer::timeout,this,[=](){
        timer_save->start(save_time * 1000);
        give_message(reciter->auto_save());
    });
    timer_save->start(save_time * 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::give_message(QString s)
{
    ui->message->setText(s);
    timer->start(show_message_time * 1000);
}

void MainWindow::clear_message()
{
    ui->message->clear();
}

void MainWindow::add_word()
{
    if(ui->word->text().isEmpty())
    {
//        give_message("??????????????????");
        activated();
        return;
    }
    if(is_alpha(ui->word->text()))
    {
        if (language != "english")
        {
            language = "english";
            reciter->change_language_with_save("english");
            reciter->seturl(url_for_endlish);
            ui->english->setChecked(true);
        }
    }
    else{
        if (language != "japanese")
        {
            language = "japanese";
            reciter->change_language_with_save("japanese");
            reciter->seturl(url_for_japanese);
            ui->japanese->setChecked(true);
        }
    }
    reciter->add_word(ui->word->text());
    give_message("???????????????");
    ui->word->clear();
    ui->word->setFocus();
}

void MainWindow::activated()
{
    switch (isVisible()) {
        case true:
            this->hide();
            callback->show();
            break;
        default:
            show();
            showNormal();
            raise();
            activateWindow();
            ui->word->setFocus();
            callback->hide();
            break;
        }
}

void MainWindow::settings(int words_num, int autosavetime, QString website, QString url2)
{
    ui->elements->show();
    ui->menubar->show();
    save_time = autosavetime;
    open_pages_num = words_num;
    url_for_endlish = website;
    url_for_japanese = url2;
    ui->recite->setText("???" + QString::number(open_pages_num) + "???");
    if (language == QString("english"))
        reciter->seturl(url_for_endlish);
    else reciter->seturl(url_for_japanese);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    auto temp = reciter->auto_save();
    if(temp != QString("??????????????????"))
    {
        callback->close();
        exit(0);
    }
    else ui->message->setText("????????????...");
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() != QEvent::WindowStateChange) return;
    if(this->windowState() == Qt::WindowMinimized)
    {
        hide();
        callback->show();
    }
    else if(this->windowState() == Qt::WindowMaximized)
    {
        ui->word->setFocus();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        add_word();
}

//void MainWindow::keyReleaseEvent(QKeyEvent *e)
//{
//    if(e->key() == Qt::Key_Insert || e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown)
//    {
//        hide();
//        callback->show();
//    }
//}

void MainWindow::focusInEvent(QFocusEvent *)
{
    ui->word->setFocus();
}

