#include <QDebug>
#include <textwindow.h>
#include <QCloseEvent>
#include "textwindow.h"
#include "ui_textwindow.h"
#include <cplayboard.h>


CTextWindow::CTextWindow(QWidget *parent)
    :QDialog(parent)
    , ui(new Ui::TextWindow)
{
    ui->setupUi(this);
    ui->textEdit->setFontFamily("monospace");
    show();
    QObject::connect(ui->pb_get,SIGNAL(clicked(bool)),this,SLOT(handleGet(bool)));
    QObject::connect(ui->pb_get_sol,SIGNAL(clicked(bool)),this,SLOT(handleGetSol(bool)));
    //QObject::connect(ui->pb_save,SIGNAL(clicked(bool)),this,SLOT(handleSave(bool)));
    QObject::connect(ui->pb_play,SIGNAL(clicked(bool)),this,SLOT(handlePlay(bool)));
}

CTextWindow::~CTextWindow()
{
    delete ui;
}

void CTextWindow::closeEvent(QCloseEvent *e)
{
    emit closing();
    e->accept();
}

// ---------- CTextWindow::handleGet ----------
void CTextWindow::handleGet(bool)
{
#define FUNCTION "CTextWindow::handleGet"
    qDebug() << FUNCTION " at [0]";
    CPlayBoard *pb=CPlayBoard::instance();
    int dx=pb->width();
    int dy=pb->height();
    ui->textEdit->clear();
    for (int y=0; y<dy; ++y) {
	QString line;
	for (int x=0; x<dx; ++x) {
	    //int v= pb->valueAt(x,y);
	    char c=pb->at(x,y)->get_value_c();
	    line += c;
	}
	while (line.endsWith(" "))
	    line.remove(line.size()-1,1);
	ui->textEdit->append(line);	    
    }
#undef FUNCTION
}

// ---------- CTextWindow::handleGetSol ----------
void CTextWindow::handleGetSol(bool)
{
#define FUNCTION "CTextWindow::handleGetSol"
    qDebug() << FUNCTION " at [0]";
    ui->textEdit->clear();
    CPlayBoard *pb=CPlayBoard::instance();
    QStringList list(pb->get_all_lurds());
    qDebug() << FUNCTION " at [1] list.size()=" << list.size();
    QStringList::const_iterator i;
    QString sub;
    for (i=list.constBegin(); i!=list.constEnd(); ++i) {
        QString line(*i);
	qDebug() << FUNCTION " at [2] line=" << line;
        line=line.trimmed();
        if (sub.size()>32) {
            ui->textEdit->append(sub);
            sub.clear();
        }
        sub += line;
    }
    if (sub.size()>0) {
        ui->textEdit->append(sub);
    }
#undef FUNCTION
}

// -------- CTextWindow::handleSave ----------
void CTextWindow::handleSave(bool)
{
#define FUNCTION "CTextWindow::handleSave"
    qDebug() << FUNCTION " at [0]";
#undef FUNCTION
}

// -------- CTextWindow::handlePlay ----------
void CTextWindow::handlePlay(bool)
{
#define FUNCTION "CTextWindow::handlePlay"
    qDebug() << FUNCTION " at [0]";
    CPlayBoard *pb=CPlayBoard::instance();
    QString plain=ui->textEdit->toPlainText();
    QStringList list=plain.split("\x0A");
    CLevel lvl;
    QStringList::const_iterator i;
    for (i=list.constBegin(); i!=list.constEnd(); ++i) {
	lvl.append(*i);
    }
    if (lvl.validate()) {
	pb->reset(lvl);
    }
    emit repaint();
#undef FUNCTION
}
