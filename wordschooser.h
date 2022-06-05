#ifndef WORDSCHOOSER_H
#define WORDSCHOOSER_H
#include <QString>
#include <QUrl>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>

enum filetype{notebook,englishfile,japanesefile};

class wordschooser
{
public:
    wordschooser(QString lan = "english",filetype ft = notebook);
    ~wordschooser();
    void open_a_word(QString);
    void change_language(QString);
    QJsonObject read_json(QString);
    QJsonArray get_words_array(QJsonObject);
    void get_words_set();
    QString language_str();
    QString file_str();
    void add_word(QString);
    bool del_word(QString);
    bool undo();
    bool save();
    QString seturl(const QString);
    QString get_url();
    QString select_word_randomly();
    QString get_error_message();
    QString auto_save();
    QString recite(int);
private:
    QString language;
    filetype type;
    QJsonObject * rootjsonobj;
    QSet<QString> * set;
    QString url;
    QString last;
    int error = 0;
    bool need_autosave = true;
    QString errormessage;
};

#endif // WORDSCHOOSER_H
