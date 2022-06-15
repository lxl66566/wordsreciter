#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "callbackwidget.h"
#include <QDebug>
#include <QTimer>
#include <QButtonGroup>
#include <QKeyEvent>
#include <QIcon>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("reciter");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    language = "english";

    elements.push_back(ui->del);
    elements.push_back(ui->undo);
    elements.push_back(ui->word);
    elements.push_back(ui->submit);
    elements.push_back(ui->recite);
    elements.push_back(ui->english);
    elements.push_back(ui->japanese);
    elements.push_back(ui->message);

    callback = new callbackwidget(this);
    callback->hide();
    connect(callback,&callbackwidget::activate,this,[=](){
        show();
        showNormal();
        raise();
        activateWindow();
        ui->word->setFocus();
    });

    reciter = new wordschooser(language,notebook);

    setting = new settingswidget(this);
    setting->hide();
    connect(setting,&settingswidget::ok,this,&MainWindow::settings);
    connect(ui->actionsettings,&QAction::triggered,this,[=](){
        for (auto i = elements.begin();i != elements.end();++i) {
            (*i)->hide();
        }
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
    });
    connect(buttongroup->button(1),&QAbstractButton::clicked,this,[=](){
        language = "japanese";
        reciter->change_language_with_save(language);
        reciter->seturl(url_for_japanese);
    });

    ui->message->setText(reciter->get_error_message());
    connect(ui->submit,&QPushButton::clicked,this,&MainWindow::add_word);
    connect(ui->undo,&QPushButton::clicked,this,[=]()
    {
        if(reciter->undo())
            give_message("撤销成功！");
        else give_message("单词不存在");
        ui->word->clear();
    });
    connect(ui->del,&QPushButton::clicked,this,[=]()
    {
        if(ui->word->text().isEmpty()) return;
        if(reciter->del_word(ui->word->text()))
            give_message("删除成功！");
        else give_message("单词不存在");
        ui->word->clear();
    });
    connect(ui->recite,&QPushButton::clicked,this,[=](){
        give_message(reciter->recite(open_pages_num));
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
    if(ui->word->text().isEmpty()) return;
    reciter->add_word(ui->word->text());
    give_message("添加成功！");
    ui->word->clear();
    ui->word->setFocus();
}

void MainWindow::settings(int words_num, int autosavetime, QString website, QString url2)
{
    for (auto i = elements.begin();i != elements.end();++i) {
        (*i)->show();
    }
    save_time = autosavetime;
    open_pages_num = words_num;
    url_for_endlish = website;
    url_for_japanese = url2;
    ui->recite->setText("我要背" + QString::number(open_pages_num) + "个！");
    if (language == QString("english"))
        reciter->seturl(url_for_endlish);
    else reciter->seturl(url_for_japanese);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    auto temp = reciter->auto_save();
    if(temp != QString("自动保存失败"))
    {
        callback->close();
        exit(0);
    }
    else ui->message->setText("保存失败...");
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() != QEvent::WindowStateChange) return;
    if(this->windowState() == Qt::WindowMinimized)
    {
        hide();
        callback->show();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        add_word();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Insert || e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown)
    {
        hide();
        callback->show();
    }
}

