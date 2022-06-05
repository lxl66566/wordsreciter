#ifndef OFFLINE_H
#define OFFLINE_H

#include "wordschooser.h"
#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class offline;
}

class offline : public QWidget
{
    Q_OBJECT

public:
    explicit offline(QWidget *parent = nullptr);
    ~offline();
    wordschooser * reciter;
    QButtonGroup * buttongroup;
    void flush_text();
protected:
    void keyPressEvent(QKeyEvent *);
    bool flag_choose = false;

private:
    Ui::offline *ui;
};

#endif // OFFLINE_H
