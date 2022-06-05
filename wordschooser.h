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
    wordschooser(QString lan = "english",filetype ft = notebook,QString route = "notebook.json");
    ~wordschooser();
    void open_a_word(QString);
    void change_language_with_save(QString);
    void change_type(filetype);
    void change_language(QString);
    void change_file(QString);
    filetype get_type();
    QString get_file();
    QJsonObject read_json();
    QJsonArray get_words_array(QJsonObject);
    void get_words_set();
    QString language_str();
//    QString file_str(QString r = "notebook.json");
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
    QVector<QString> recite_offline();
private:
    QString language;
    filetype type;
    QJsonObject * rootjsonobj;
    QSet<QString> * set;
    QString url;
    QString last;
    bool need_autosave = true;
    int error = 0;
    QString errormessage;
    QString fileroute;
};

#endif // WORDSCHOOSER_H
