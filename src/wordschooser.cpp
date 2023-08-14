#include "wordschooser.h"
#include <QDebug>
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

wordschooser::wordschooser(QString lan, filetype ft, QString route)
    : language(lan), type(ft), fileroute(route) {
  set = new QSet<QString>();
  if (ft == filetype::notebook) {
    rootjsonobj = new QJsonObject(read_json());
    if (!rootjsonobj->empty())
      get_words_set();
  }
}
wordschooser::~wordschooser() { delete set, delete rootjsonobj; }
void wordschooser::open_a_word(const QString s) {
  if (s.isEmpty())
    return;
  QString temp = url;
  QDesktopServices::openUrl(QUrl(temp.replace("{}", s)));
}

void wordschooser::change_language_with_save(QString s) {
  auto_save();
  change_language(s);
  get_words_set();
}

void wordschooser::change_type(filetype t) { type = t; }

void wordschooser::change_language(QString s) { language = s; }

void wordschooser::change_file(QString s) {
  //    if(rootjsonobj)
  //        delete rootjsonobj;
  fileroute = s;
  rootjsonobj = new QJsonObject(read_json());
}

void wordschooser::clear_last() { last = QString(); }

filetype wordschooser::get_type() { return type; }

QString wordschooser::get_file() { return fileroute; }

QJsonObject wordschooser::read_json() {
  QFile file(fileroute);
  int tempflag = 4;
  while (!file.open(QIODevice::ReadOnly) && --tempflag) {
    save(1);
  }
  if (tempflag == 0) {
    error = 1;
    errormessage = QString("open file failed.");
    qDebug() << error;
    return QJsonObject();
  }
  QByteArray data(file.readAll());
  file.close();
  QJsonParseError jError;
  QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
  if (jError.error != QJsonParseError::NoError) {
    error = 2;
    errormessage = QString("read json error.");
    qDebug() << error;
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
  set->clear();
  set->squeeze();
  if (type == filetype::notebook) {
    QJsonObject jObj2 = (*rootjsonobj)[language].toObject();
    QJsonArray temp = jObj2["default"].toArray();
    for (auto i = temp.begin(); i != temp.end(); ++i)
      set->insert(i->toString());
  } else {
    auto temp = (*rootjsonobj).keys();
    *set = QSet<QString>(temp.begin(), temp.end());
  }
  need_autosave = false;
  return *set;
}

QString wordschooser::language_str() { return language; }

// QString wordschooser::file_str(QString r)
//{
//     fileroute = r;
//     return r;
// }

void wordschooser::add_word(QString s) {
  //    int temp = set->size();
  if (s.isEmpty())
    return;
  set->insert(s);
  //    if(temp != set->size())//一次有效添加（已弃用
  last = s;
  need_autosave = true;
}

bool wordschooser::del_word(QString s) {
  if (s.isEmpty())
    return false;
  need_autosave = true;
  return set->remove(s);
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

bool wordschooser::save(int mode) {
  QJsonDocument jDoc;
  if (mode == 0) {
    QJsonArray newarray;
    std::ranges::transform(*set, std::back_inserter(newarray),
                           [](const QString &s) { return s; });
    QJsonObject objdefault;
    objdefault.insert("default", newarray);
    rootjsonobj->insert(language, objdefault);
    jDoc = QJsonDocument(*rootjsonobj);
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
    errormessage = QString("write json error.");
    qDebug() << error;
    return false;
  }
  QByteArray data(jDoc.toJson());
  file.write(data);
  file.close();
  need_autosave = false;
  return true;
}

QString wordschooser::get_last() { return last; }

QString wordschooser::select_word_randomly() {
  if (set->empty())
    return QString();
  auto iter = set->begin();
  std::advance(iter, QRandomGenerator::global()->bounded(set->size()));
  return *iter;
}

QString wordschooser::seturl(const QString s) {
  url = s;
  return url;
}

QString wordschooser::get_url() { return url; }

QString wordschooser::get_error_message() noexcept {
  return error ? errormessage : QString();
}

QString wordschooser::auto_save() {
  if (!need_autosave)
    return "无需自动保存";
  return save() ? "自动保存成功" : "自动保存失败";
}

QString wordschooser::recite(int x) {
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

QVector<QString> wordschooser::recite_offline() {
  QVector<QString> a;
  a.push_back(select_word_randomly());
  if (type == englishfile) {
    QJsonObject temp = (*rootjsonobj)[*a.begin()].toObject();
    if (temp.keys().size() == 0)
      return recite_offline();
    a.push_back(temp["中释"].toString());
    if (temp.keys().size() >= 2)
      a.push_back(temp["英释"].toString());
  } else if (type == japanesefile) {
    a.push_back((*rootjsonobj)[*a.begin()].toString());
  }
  return a;
}
