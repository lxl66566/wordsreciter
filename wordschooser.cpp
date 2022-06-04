#include "wordschooser.h"
#include <QDesktopServices>
#include <QUrl>

wordschooser::wordschooser(int lan)
    : language(lan)
{

}

void wordschooser::open_a_word(QString s,QString url)
{
    QDesktopServices::openUrl(QUrl(url + s));
}
