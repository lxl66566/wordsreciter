#include "see_notebook_widget.h"
#include "ui_see_notebook_widget.h"

see_notebook_widget::see_notebook_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::see_notebook_widget)
{
    ui->setupUi(this);
    setWindowTitle("my notebook");
    setWindowIcon(QIcon("./resource/logo.jpg"));
    reciter = new wordschooser("english",notebook,"notebook.json");
    set_text("english");
    set_text("japanese");
}

see_notebook_widget::~see_notebook_widget()
{
    delete ui;
    delete reciter;
}

void see_notebook_widget::set_text(QString language)
{
    reciter->change_language(language);
    auto wordsset = reciter->get_words_set();
    QString temp = QString();
    for (auto i = wordsset.begin();i != wordsset.end();++i)
    {
        temp += (*i) + '\n';
    }
    if (language == "english")
        ui->englishwords->setPlainText(temp);
    else ui->japanesewords->setPlainText(temp);
}
