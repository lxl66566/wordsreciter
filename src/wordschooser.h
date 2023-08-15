#ifndef WORDSCHOOSER_H
#define WORDSCHOOSER_H
#include "absx.h"
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QUrl>
#include <cstddef>
#include <optional>

enum filetype { notebook, englishfile, japanesefile };

class wordschooser {
public:
  wordschooser(
      QString lan = "english", filetype ft = notebook,
      QString route = "notebook.json" /*,bool only_read_json = false*/);
  ~wordschooser();
  void open_a_word(const QString &) const;
  void change_language_with_save(const QString &);
  void change_type(const filetype) noexcept;
  void change_language(const QString) noexcept;
  void change_file(QString);
  void clear_last();
  filetype get_type();
  QString get_file();
  QJsonObject read_json();
  QJsonArray get_words_array(QJsonObject);
  QSet<QString> get_words_set();
  QString language_str();
  void add_word(QString);
  bool del_word(QString);
  bool save(i32 mode = 0);
  QString undo();
  QString get_last();
  QString seturl(const QString);
  QString get_url();
  QString select_word_randomly() const;
  QString get_error_str() const noexcept; // public
  QString auto_save();
  QString recite(i32);
  QString recite_last();
  QVector<QString> recite_offline();

private:
  QString language;
  filetype type;
  std::optional<QJsonObject *> rootjsonobj;
  QSet<QString> set;
  QString url;
  QString last;
  bool need_autosave = true;
  i32 error = 0;
  QString get_error_str(const i32) const;
  QString fileroute;
};

#endif // WORDSCHOOSER_H
