#ifndef CALLBACKWIDGET_H
#define CALLBACKWIDGET_H

#include <QDialog>

class callbackwidget : public QDialog {
  Q_OBJECT
public:
  callbackwidget(QWidget *parent = nullptr);
  ~callbackwidget();

protected:
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

private:
  QRect m_areaMovable; // 可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
  bool m_bPressed = false; // 鼠标按下标志（不分左右键）
  QPoint m_ptPress;        // 鼠标按下的初始位置
  bool moved = false;
signals:
  void activate();
};

#endif // CALLBACKWIDGET_H
