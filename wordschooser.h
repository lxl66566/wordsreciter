#ifndef WORDSCHOOSER_H
#define WORDSCHOOSER_H
#include <QString>
#include <QUrl>

class wordschooser
{
public:
    wordschooser(int lan = 0);
    void open_a_word(QString,QString);
private:
    int language = 0;
};

#endif // WORDSCHOOSER_H
