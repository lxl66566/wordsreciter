#include "mainwindow.h"
#include "qshortcut.h"
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QShortCut>
#include <QStyleFactory>

void set_dark() {
  qApp->setStyle(QStyleFactory::create("Fusion"));
  QPalette darkPalette;
  QColor darkColor = QColor(45, 45, 45);
  QColor disabledColor = QColor(127, 127, 127);
  darkPalette.setColor(QPalette::Window, darkColor);
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  qApp->setPalette(darkPalette);
  QFile f(":qdarkstyle/dark/darkstyle.qss");
  if (!f.exists()) {
    printf("Unable to set stylesheet, file not found\n");
  } else {
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    qApp->setStyleSheet(ts.readAll());
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

#ifdef Q_OS_WIN
  QSettings settings(
      "HKEY_CURRENT_"
      "USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
      QSettings::NativeFormat);
  if (settings.value("AppsUseLightTheme") == 0)
    set_dark();
#else
  // on other platform, it would be dark mode by default. (just for my vision)
  set_dark();
#endif

  // QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+LEFT"), &w);
  QShortcut shortcut = QShortcut(QKeySequence("Ctrl+LEFT"), &w);
  QObject::connect(&shortcut, &QShortcut::activated, &w,
                   &MainWindow::activated);
  return a.exec();
}
