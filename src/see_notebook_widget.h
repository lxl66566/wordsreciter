#ifndef SEE_NOTEBOOK_WIDGET_H
#define SEE_NOTEBOOK_WIDGET_H

#include "wordschooser.h"
#include <QWidget>


namespace Ui {
class see_notebook_widget;
}

class see_notebook_widget : public QWidget {
  Q_OBJECT

public:
  explicit see_notebook_widget(QWidget *parent = nullptr);
  ~see_notebook_widget();
  void set_text(QString);

private:
  Ui::see_notebook_widget *ui;
  wordschooser *reciter;
};

#endif // SEE_NOTEBOOK_WIDGET_H
