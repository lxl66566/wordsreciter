#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QButtonGroup>

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

private:
    Ui::settingswidget *ui;
    QButtonGroup *buttongroup,*buttongroup2;

protected:
    void keyPressEvent(QKeyEvent *);

signals:
    void ok(int,int,QString,QString);
};

#endif // SETTINGSWIDGET_H
