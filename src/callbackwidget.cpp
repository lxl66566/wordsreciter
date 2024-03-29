#include "callbackwidget.h"
#include <QLabel>
#include <QMouseEvent>
#include <QTextFormat>

callbackwidget::callbackwidget(QWidget *parent) : QDialog(parent) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowOpacity(0.9);
  setGeometry(0, 0, 75, 20);
  setFixedSize(75, 20);
  m_areaMovable = geometry();
  QLabel *lable = new QLabel(this);
  lable->setText("点击记单词");
  lable->setFixedWidth(75);
  lable->show();
}

callbackwidget::~callbackwidget() {}

void callbackwidget::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    m_ptPress = e->pos();
    m_bPressed = true;
    //      m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void callbackwidget::mouseMoveEvent(QMouseEvent *e) {
  if (m_bPressed) {
    moved = true;
    move(pos() + e->pos() - m_ptPress);
  }
}

void callbackwidget::mouseReleaseEvent(QMouseEvent *) {
  m_bPressed = false;
  if (!moved) {
    emit activate();
    hide();
  }
  moved = false;
}
