#include <QDebug>
#include <QKeyEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cpixmaps.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setWindowIcon(CPixmaps::instance()->get_app_icon());
  QObject::connect(ui->actionRestart_level,  SIGNAL(triggered(bool)), this, SLOT(handleRestartLevel(bool)));
  QObject::connect(ui->actionUndo,           SIGNAL(triggered(bool)), this, SLOT(handleUndo(bool)));
  QObject::connect(ui->actionRedo,           SIGNAL(triggered(bool)), this, SLOT(handleRedo(bool)));
  QObject::connect(ui->actionResolve,        SIGNAL(triggered(bool)), this, SLOT(handleResolve(bool)));
  QObject::connect(ui->actionNext_level,     SIGNAL(triggered(bool)), this, SLOT(handleNextLevel(bool)));
  QObject::connect(ui->actionPrevious_level, SIGNAL(triggered(bool)), this, SLOT(handlePrevLevel(bool)));
  QObject::connect(ui->actionText_window,    SIGNAL(triggered(bool)), this, SLOT(handleTextWindow(bool)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::handleRestartLevel(bool)
{
  emit restartLevel();
}

void MainWindow::handleUndo(bool)
{
  emit undo();
}

void MainWindow::handleNextLevel(bool)
{
  emit nextLevel();
}

void MainWindow::handlePrevLevel(bool)
{
  emit prevLevel();
}

void MainWindow::keyPressEvent (QKeyEvent* e)
{
#define FUNCTION "MainWindow::keyPressEvent"
  qDebug() << FUNCTION " at [0]";
  int key = e->key();
  int mod = e->modifiers();
  if ((key == 'N') && ((mod & Qt::ShiftModifier) == 0)) {
    qDebug() << FUNCTION " at [1] 'n'";
    emit nextLevel();
  } else if ((key == 'N') && (mod & Qt::ShiftModifier)) {
    qDebug() << FUNCTION " at [2] 'N'";
    emit nextUnresolvedLevel();
  } else if (key == Qt::Key_Backspace) {
    emit undo();
  } else if (key == Qt::Key_Escape) {
    emit restartLevel();
  } else if ((key == 't') || (key == 'T')) {
    emit textWindow();
  } else if (key == '1') {
    emit gotoLevel1();
  } else if ((key == 'r') || (key == 'R')) {
    emit redo();
  } else if ((key == 'P') && ((mod & Qt::ShiftModifier) == 0)) {
    qDebug() << FUNCTION " at [3] 'p'";
    emit prevLevel();
  } else if ((key == 'P') && (mod & Qt::ShiftModifier)) {
    qDebug() << FUNCTION " at [4] 'P'";
    emit prevUnresolvedLevel();
  } else if (key == Qt::Key_Right) {
    emit moveRight();
  } else if (key == Qt::Key_Left) {
    emit moveLeft();
  } else if (key == Qt::Key_Up) {
    emit moveUp();
  } else if (key == Qt::Key_Down) {
    emit moveDown();
  }
#undef FUNCTION
}

// -------- MainWindow::handleTextWindow ----------
void MainWindow::handleTextWindow(bool)
{
#define FUNCTION "MainWindow::handleTextWindow"
  //qDebug() << FUNCTION " at [0]";
  emit textWindow();
#undef FUNCTION
}

// -------- MainWindow::handleLevelSets -------
void MainWindow::handleLevelSets(const QStringList &list)
{
#define FUNCTION "MainWindow::handleLevelSets"
  qDebug() << FUNCTION " at [0]";
  QMenu* menu = new QMenu();
  QActionGroup* group = new QActionGroup(this);
  QStringList::const_iterator i = list.constBegin();
  for (; i != list.constEnd(); ++i) {
    QAction* tmp = menu->addAction(*i);
    tmp->setData(QVariant(*i));
    tmp->setCheckable(true);
    group->addAction(tmp);
    QObject::connect(tmp, SIGNAL(triggered(bool)), this, SLOT(handleLevelset()));
  }
  ui->actionLevel_collection->setMenu(menu);
#undef FUNCTION
}

void MainWindow::handleSetTheme(const QString &name)
{
#define FUNCTION "MainWindow::handleSetTheme"
  qDebug() << FUNCTION " name=" << name;
  QMenu* menu = ui->actionThemes->menu();
  QList<QAction*> actions(menu->actions());
  QList<QAction*>::const_iterator i = actions.constBegin();
  for (; i != actions.constEnd(); ++i) {
    QAction* tmp = *i;
    if (tmp->text()==name) {
      tmp->setChecked(true);
      break;
    }
  }
#undef FUNCTION
}

void MainWindow::handleSetLevelset(const QString &name)
{
#define FUNCTION "MainWindow::handleSetLevelset"
  qDebug() << FUNCTION "name=" << name;
  QMenu* menu = ui->actionLevel_collection->menu();
  QList<QAction*> actions(menu->actions());
  QList<QAction*>::const_iterator i = actions.constBegin();
  for (; i != actions.constEnd(); ++i) {
    QAction* tmp= *i;
    if (tmp->text() == name) {
      tmp->setChecked(true);
      break;
    }
  }
#undef FUNCTION
}

// -------- MainWindow::handleLevelset --------
void MainWindow::handleLevelset()
{
#define FUNCTION "MainWindow::handleLevelset"
  qDebug() << FUNCTION " at [0]";
  QAction* src = static_cast<QAction *>(QObject::sender());
  QString data = src->data().toString();
  src->setChecked(true);
  emit setLevelSet(data);
#undef FUNCTION
}

// --------- MainWindow::handleSetThemes ---------
void MainWindow::handleSetThemes()
{
#define FUNCTION "MainWindow::handleSetThemes"
  //qDebug() << FUNCTION " at [0]";
  CPixmaps* pms = CPixmaps::instance();
  QStringList list = pms->get_themes();
  QActionGroup* group = new QActionGroup(this);

  QMenu* menu = new QMenu();
  QStringList::const_iterator i=list.constBegin();
  for (; i != list.constEnd(); ++i) {
    QAction* tmp = menu->addAction(*i);
    tmp->setData(QVariant(*i));
    tmp->setCheckable(true);
    group->addAction(tmp);
    QObject::connect(tmp,SIGNAL(triggered(bool)),this,SLOT(handleTheme()));
  }
  ui->actionThemes->setMenu(menu);
#undef FUNCTION
}

void MainWindow::handleTheme()
{
#define FUNCTION "MainWindow::handleTheme"
  //qDebug() << FUNCTION " at [0]";
  QAction* src = static_cast<QAction *>(QObject::sender());
  QString data = src->data().toString();
  CPixmaps* pms = CPixmaps::instance();
  pms->select_theme(data);
  emit themeChanged();
#undef FUNCTION
}

void MainWindow::handleRedo(bool)
{
#define FUNCTION "MainWindow::handleRedo"
  //qDebug() << FUNCTION " at [0]";
  emit redo();
#undef FUNCTION
}

void MainWindow::handleResolve(bool)
{
#define FUNCTION "MainWindow::handleResolve"
  //qDebug() << FUNCTION " at [0]";
  emit resolve();
#undef FUNCTION
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  emit mainWindowClosed();
  e->accept();
}
