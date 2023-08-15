#include "wordschooser.h"
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QSet>
#include <QString>
#include <QTime>
#include <QUrl>
#include <algorithm>
#include <iterator>

QString wordschooser::get_error_str(const i32 x) const {
  switch (x) {
  case 0:
    return "";
  case 1:
    return "open file failed";
  case 2:
    return "read json error";
  default:
    return "unknown error";
  }
}

wordschooser::wordschooser(QString lan, filetype ft, QString route)
    : language(lan), type(ft), fileroute(route) {
  set = QSet<QString>();
  if (ft == filetype::notebook) {
    rootjsonobj = new QJsonObject(read_json());
    if (rootjsonobj.has_value() && !rootjsonobj.value()->empty())
      get_words_set();
  }
}

wordschooser::~wordschooser() {
  if (rootjsonobj.has_value()) {
    delete rootjsonobj.value();
  }
}

void wordschooser::open_a_word(const QString &s) const {
  if (s.isEmpty())
    return;
  QString temp = url;
  QDesktopServices::openUrl(QUrl(temp.replace("{}", s)));
}

void wordschooser::change_language_with_save(const QString &s) {
  auto_save();
  change_language(s);
  get_words_set();
}

void wordschooser::change_type(const filetype t) noexcept { type = t; }

void wordschooser::change_language(const QString s) noexcept { language = s; }

void wordschooser::change_file(QString s) {
  if (rootjsonobj.has_value()) {
    delete rootjsonobj.value();
  }
  fileroute = s;
  rootjsonobj = new QJsonObject(read_json());
}

void wordschooser::clear_last() { last = QString(); }

filetype wordschooser::get_type() { return type; }

QString wordschooser::get_file() { return fileroute; }

QJsonObject wordschooser::read_json() {
  QFile file(fileroute);
  i32 tempflag = 4;
  while (!file.open(QIODevice::ReadOnly) && --tempflag) {
    save(1);
  }
  if (tempflag == 0) {
    error = 1;
    return QJsonObject();
  }
  QByteArray data(file.readAll());
  file.close();
  QJsonParseError jError;
  QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
  if (jError.error != QJsonParseError::NoError) {
    error = 2;
    return QJsonObject();
  }
  return jDoc.object();
}

QJsonArray wordschooser::get_words_array(QJsonObject rootobj) {
  if (type == filetype::notebook) {
    QJsonObject jObj2 = rootobj[language].toObject();
    return jObj2["default"].toArray();
  }
  return QJsonArray();
}

QSet<QString> wordschooser::get_words_set() {
  if (!rootjsonobj.has_value())
    return QSet<QString>();
  set.clear();
  set.squeeze();
  if (type == filetype::notebook) {
    QJsonObject jObj2 = (*rootjsonobj.value())[language].toObject();
    QJsonArray temp = jObj2["default"].toArray();
    for (auto i = temp.begin(); i != temp.end(); ++i)
      set.insert(i->toString());
  } else {
    auto temp = (*rootjsonobj.value()).keys();
    set = QSet<QString>(temp.begin(), temp.end());
  }
  need_autosave = false;
  return set;
}

QString wordschooser::language_str() { return language; }

void wordschooser::add_word(QString s) {
  if (s.isEmpty())
    return;
  set.insert(s);
  last = s;
  need_autosave = true;
}

bool wordschooser::del_word(QString s) {
  if (s.isEmpty())
    return false;
  need_autosave = true;
  return set.remove(s);
}

QString wordschooser::undo() {
  if (last.isEmpty())
    return QString();
  auto temp = last;
  del_word(last);
  last = QString();
  need_autosave = true;
  return temp;
}

bool wordschooser::save(i32 mode) {
  QJsonDocument jDoc;
  if (mode == 0) {
    QJsonArray newarray;
    std::ranges::transform(set, std::back_inserter(newarray),
                           [](const QString &s) { return s; });
    QJsonObject objdefault;
    objdefault.insert("default", newarray);
    rootjsonobj.value()->insert(language, objdefault);
    jDoc = QJsonDocument(*rootjsonobj.value());
  } else if (mode == 1) {
    QJsonObject roottemp, objdefault;
    objdefault.insert("default", QJsonArray());
    roottemp.insert("english", objdefault);
    roottemp.insert("japanese", objdefault);
    jDoc = QJsonDocument(roottemp);
  }

  QFile file(fileroute);
  if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
    error = 3;
    return false;
  }
  QByteArray data(jDoc.toJson());
  file.write(data);
  file.close();
  need_autosave = false;
  return true;
}

QString wordschooser::get_last() { return last; }

QString wordschooser::select_word_randomly() const {
  if (set.empty())
    return QString();
  auto iter = set.begin();
  std::advance(iter, QRandomGenerator::global()->bounded(set.size()));
  return *iter;
}

QString wordschooser::seturl(const QString s) {
  url = s;
  return url;
}

QString wordschooser::get_url() { return url; }

QString wordschooser::get_error_str() const noexcept {
  return get_error_str(error);
}

QString wordschooser::auto_save() {
  if (!need_autosave)
    return "无需自动保存";
  return save() ? "自动保存成功" : "自动保存失败";
}

QString wordschooser::recite(i32 x) {
  QString temp;
  if (type == notebook) {
    for (; x > 0; --x) {
      temp = select_word_randomly();
      open_a_word(temp);
    }
    return temp;
  }
  return temp;
}

QString wordschooser::recite_last() {
  if (last.isEmpty())
    return QString();
  return last;
}

// returns a vector of 2 or 3 QStrings, represent the word, definition {1,2}
QVector<QString> wordschooser::recite_offline() {
  QVector<QString> a;
  a.push_back(select_word_randomly());
  if (type == englishfile) {
    const QJsonObject temp = (*rootjsonobj.value())[*a.begin()].toObject();
    if (temp.keys().size() == 0)
      return recite_offline();
    a.push_back(temp["中释"].toString());
    if (temp.keys().size() >= 2)
      a.push_back(temp["英释"].toString());
  } else if (type == japanesefile) {
    a.push_back((*rootjsonobj.value())[*a.begin()].toString());
  }
  return a;
}
