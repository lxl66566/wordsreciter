#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "callbackwidget.h"
#include "offline.h"
#include "qtmetamacros.h"
#include "see_notebook_widget.h"
#include "settingswidget.h"
#include "wordschooser.h"
#include <QMainWindow>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

const float show_message_time = 1;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void give_message(QString);
  void clear_message();
  void add_word();
private slots:
  void settings(int, int, QString, QString);
public slots:
  void activated();

protected:
  void closeEvent(QCloseEvent *);
  void changeEvent(QEvent *);
  void keyPressEvent(QKeyEvent *);
  //    void keyReleaseEvent(QKeyEvent *);
  void focusInEvent(QFocusEvent *);

private:
  Ui::MainWindow *ui;
  callbackwidget *callback;
  QButtonGroup *buttongroup;
  QString language = "english";
  QString url_for_endlish =
      QString("https://www.merriam-webster.com/dictionary/{}");
  QString url_for_japanese = QString("https://www.weblio.jp/content/{}");
  wordschooser *reciter;
  QTimer *timer;
  QTimer *timer_save;
  settingswidget *setting;
  offline *offlinewidget;
  see_notebook_widget *notebook_widget;
  int open_pages_num = 10;
  int save_time = 30;
  //    QVector<QWidget*> elements;
};
#endif // MAINWINDOW_H
