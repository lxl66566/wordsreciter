#include "url_settingswidget.h"
#include "ui_url_settingswidget.h"

url_settingswidget::url_settingswidget(QString text[4], QString urls[4],
                                       QWidget *parent)
    : QDialog(parent), ui(new Ui::url_settingswidget) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle("change urls for recite");
  ui->engtext1->setText(text[0]);
  ui->engtext2->setText(text[1]);
  ui->japtext1->setText(text[2]);
  ui->japtext2->setText(text[3]);
  ui->engurl1->setText(urls[0]);
  ui->engurl2->setText(urls[1]);
  ui->japurl1->setText(urls[2]);
  ui->japurl2->setText(urls[3]);
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=]() {
    emit ok_url(ui->engurl1->text(), ui->engurl2->text(), ui->japurl1->text(),
                ui->japurl2->text());
    emit ok_text(ui->engtext1->text(), ui->engtext2->text(),
                 ui->japtext1->text(), ui->japtext2->text());
    close();
  });
  connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [=]() { close(); });
}

url_settingswidget::url_settingswidget(QWidget *parent)
    : QDialog(parent), ui(new Ui::url_settingswidget) {}

url_settingswidget::~url_settingswidget() { delete ui; }
