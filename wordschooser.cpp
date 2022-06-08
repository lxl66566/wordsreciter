#include "wordschooser.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>
#include <QTime>
#include <QSet>
#include <QDebug>

wordschooser::wordschooser(QString lan,filetype ft,QString route/*,bool only_read_json*/)
    : language(lan)
    , type(ft)
    , fileroute(route)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    set = new QSet<QString>();
    if (ft == filetype::notebook/* && !only_read_json*/)
    {
        rootjsonobj = new QJsonObject(read_json());
        if(!rootjsonobj->empty())
            get_words_set();
    }
}

wordschooser::~wordschooser()
{
    delete set;
}

int randint(int x,int y)
{
    return qrand() % (y - x + 1) + x;
}

void wordschooser::open_a_word(QString s)
{
    if (!s.isEmpty())
        QDesktopServices::openUrl(QUrl(url + s));
}

void wordschooser::change_language_with_save(QString s)
{
    auto_save();
    change_language(s);
    get_words_set();
}

void wordschooser::change_type(filetype t)
{
    type = t;
}

void wordschooser::change_language(QString s)
{
    language = s;
}

void wordschooser::change_file(QString s)
{
//    if(rootjsonobj)
//        delete rootjsonobj;
    fileroute = s;
    rootjsonobj = new QJsonObject(read_json());
}

filetype wordschooser::get_type()
{
    return type;
}

QString wordschooser::get_file()
{
    return fileroute;
}

QJsonObject wordschooser::read_json()
{
    QFile file(fileroute);
    if(!file.open(QIODevice::ReadOnly)){
        error = 1;
        errormessage = QString("open file failed.");
        qDebug() << error;
        return QJsonObject();
    }
    QByteArray data(file.readAll());
    file.close();
    QJsonParseError jError;
    QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
    if(jError.error != QJsonParseError::NoError){
        error = 2;
        errormessage = QString("read json error.");
        qDebug() << error;
        return QJsonObject();
    }
    return jDoc.object();
}

QJsonArray wordschooser::get_words_array(QJsonObject rootobj)
{
    if (type == filetype::notebook)
    {
        QJsonObject jObj2 = rootobj[language].toObject();
        return jObj2["default"].toArray();
    }
    return QJsonArray();
}

QSet<QString> wordschooser::get_words_set()
{
    set->clear();
    set->squeeze();
    if (type == filetype::notebook)
    {
        QJsonObject jObj2 = (*rootjsonobj)[language].toObject();
        QJsonArray temp = jObj2["default"].toArray();
        for(auto i = temp.begin();i != temp.end();++i)
            set->insert(i->toString());
    }
    else
    {
        *set = (*rootjsonobj).keys().toSet();
    }
    need_autosave = false;
    return *set;
}

QString wordschooser::language_str()
{
    return language;
}

//QString wordschooser::file_str(QString r)
//{
//    fileroute = r;
//    return r;
//}

void wordschooser::add_word(QString s)
{
    int temp = set->size();
    if(s.isEmpty()) return;
    set->insert(s);
    if(temp != set->size())//一次有效添加
        last = s;
    need_autosave = true;
}

bool wordschooser::del_word(QString s)
{
    if (s.isEmpty()) return false;
    need_autosave = true;
    return set->remove(s);
}

bool wordschooser::undo()
{
    if (last.isEmpty()) return false;
    bool temp = del_word(last);
    last = QString();
    need_autosave = true;
    return temp;
}

bool wordschooser::save()
{
    QJsonArray newarray = QJsonArray();
    for(auto i = set->begin();i != set->end();++i)
        newarray.push_back(*i);
    QJsonObject objdefault;
    objdefault.insert("default",newarray);
    rootjsonobj->insert(language,objdefault);
    QJsonDocument jDoc(*rootjsonobj);
    QFile file(fileroute);
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
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

QString wordschooser::select_word_randomly()
{
    if(set->empty()) return QString();
    return set->toList().at(randint(0,set->size() - 1));
}

QString wordschooser::seturl(const QString s)
{
    url = s;
    return url;
}

QString wordschooser::get_url()
{
    return url;
}

QString wordschooser::get_error_message()
{
    if(error)
        return errormessage;
    else return QString();
}

QString wordschooser::auto_save()
{
    if (!need_autosave) return "无需自动保存";
    return save() ? "自动保存成功" : "自动保存失败";
}

QString wordschooser::recite(int x)
{
    QString temp;
    if (type == notebook)
    {
        for(;x > 0;--x)
        {
            temp = select_word_randomly();
            open_a_word(temp);
        }
        return temp;
    }
    return temp;
}

QVector<QString> wordschooser::recite_offline()
{
    QVector<QString> a;
    a.push_back(select_word_randomly());
    if(type == englishfile)
    {
        QJsonObject temp = (*rootjsonobj)[*a.begin()].toObject();
        if(temp.keys().size() == 0)
            return recite_offline();
        a.push_back(temp["中释"].toString());
        if(temp.keys().size() >= 2)
            a.push_back(temp["英释"].toString());
    }
    else if(type == japanesefile)
    {
        a.push_back((*rootjsonobj)[*a.begin()].toString());
    }
    return a;
}
