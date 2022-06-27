#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include "url_settingswidget.h"

namespace Ui {
class settingswidget;
}

class settingswidget : public QWidget
{
    Q_OBJECT

public:
    explicit settingswidget(QWidget *parent = nullptr);
    ~settingswidget();
    void OK();
    void settext(QString,QString,QString,QString);
    void seturl(QString,QString,QString,QString);
    void readjson();
    void save();
    void display();

private:
    Ui::settingswidget *ui;
    QButtonGroup *buttongroup,*buttongroup2;
    url_settingswidget * urlwidget;
    QString text[4],urls[4];
    QString selected_url[2];
    int pages = 10,auto_save_time = 30,eng_selected = 0,jap_selected = 0;

public slots:
    void setpages(int);
    void setautosavetime(int);

protected:
    void keyPressEvent(QKeyEvent *);

signals:
    void ok(int,int,QString,QString);
};

#endif // SETTINGSWIDGET_H
