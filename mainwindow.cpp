#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "callbackwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->english->setEnabled(true);
    setWindowTitle("reciter");
    setWindowFlag(Qt::WindowStaysOnTopHint);
    callback = new callbackwidget(this);
    callback->setAttribute(Qt::WA_DeleteOnClose);
    callback->hide();
    connect(callback,&callbackwidget::activate,this,[=](){
        show();
        showNormal();
        raise();
        activateWindow();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete callback;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange) return;
    if(this->windowState() == Qt::WindowMinimized)
    {
        hide();
        callback->show();
    }
}

