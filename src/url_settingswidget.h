#ifndef URL_SETTINGSWIDGET_H
#define URL_SETTINGSWIDGET_H

#include <QDialog>

namespace Ui {
class url_settingswidget;
}

class url_settingswidget : public QDialog {
  Q_OBJECT

public:
  explicit url_settingswidget(QWidget *parent = nullptr);
  explicit url_settingswidget(QString[4], QString[4],
                              QWidget *parent = nullptr);
  ~url_settingswidget();

private:
  Ui::url_settingswidget *ui;
signals:
  void ok_url(QString, QString, QString, QString);
  void ok_text(QString, QString, QString, QString);
};

#endif // URL_SETTINGSWIDGET_H
