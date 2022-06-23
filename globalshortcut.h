#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include <QApplication>
#include <windows.h>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>

class GlobalShortCut;

class WinEventFilter : public QAbstractNativeEventFilter
{
public:
    WinEventFilter(GlobalShortCut *shortcut);
    ~WinEventFilter();
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *);
private:
    GlobalShortCut *m_shortcut;
};

class GlobalShortCut : public QObject
{
    Q_OBJECT
public:
    GlobalShortCut(QString key, QObject*);
    ~GlobalShortCut();
    void activateShortcut();
    bool registerHotKey();
    bool unregisterHotKey();
    QHash<QPair<quint32, quint32>, GlobalShortCut*> shortcuts;
private:
    QApplication *m_app;
    WinEventFilter *m_filter;
    QKeySequence m_key;
    Qt::Key key;
    Qt::KeyboardModifiers mods;
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
signals:
    void activated();

};

#endif // GLOBALSHORTCUT_H

