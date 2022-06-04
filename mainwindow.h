#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "callbackwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
//    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *event);
private:
    Ui::MainWindow * ui;
    callbackwidget * callback;
};
#endif // MAINWINDOW_H
