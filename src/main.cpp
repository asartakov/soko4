#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "csokoban.h"
#include "cplayfield.h"
#include <cplayboard.h>
#include <crelateddirs.h>
#include <QDir>

#include <QFile>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void debugMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#else
void debugMsg(QtMsgType type, const char *msg);
#endif

int main (int argc, char **argv)
{
  //KCmdLineArgs::init(argc, argv, &aboutData);
  //KCmdLineArgs::addCmdLineOptions(options);

  //QApplication::setColorSpec(QApplication::ManyColor);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  qInstallMessageHandler(debugMsg);
#else
  qInstallMsgHandler(debugMsg);
#endif


  CRelatedDirs::set_commandline(argv[0]);
  CSokoban app(argc, argv);

  MainWindow w;
  CPlayField pf(NULL);

  CPlayBoard* pb = CPlayBoard::instance();
  QObject::connect(&w, SIGNAL(restartLevel()), pb, SLOT(handleRestartLevel()));
  QObject::connect(&w, SIGNAL(undo()),         pb, SLOT(handleUndo()));
  QObject::connect(&w, SIGNAL(redo()),         pb, SLOT(handleRedo()));
  QObject::connect(&w, SIGNAL(resolve()),      pb, SLOT(handleResolve()));
  QObject::connect(&w, SIGNAL(moveRight()),    pb, SLOT(handleMoveRight()));
  QObject::connect(&w, SIGNAL(moveLeft()),     pb, SLOT(handleMoveLeft()));
  QObject::connect(&w, SIGNAL(moveUp()),       pb, SLOT(handleMoveUp()));
  QObject::connect(&w, SIGNAL(moveDown()),     pb, SLOT(handleMoveDown()));

  QObject::connect(&w, SIGNAL(textWindow()),          &app, SLOT(showTextWindow()));
  QObject::connect(&w, SIGNAL(nextLevel()),           &app, SLOT(goToTheNextLevel()));
  QObject::connect(&w, SIGNAL(prevLevel()),           &app, SLOT(goToThePrevLevel()));
  QObject::connect(&w, SIGNAL(nextUnresolvedLevel()), &app, SLOT(goToTheNextULevel()));
  QObject::connect(&w, SIGNAL(gotoLevel1()),          &app, SLOT(handleLevel1()));
  QObject::connect(&w, SIGNAL(prevUnresolvedLevel()), &app, SLOT(goToThePrevULevel()));
  QObject::connect(&w, SIGNAL(setLevelSet(const QString&)), &app, SLOT(handleSetLevelSet(const QString&)));
  QObject::connect(&w, SIGNAL(mainWindowClosed()),    &app, SLOT(saveLast()));

  QObject::connect(&w, SIGNAL(themeChanged()), &pf, SLOT(reread_images()));

  QObject::connect(&app, SIGNAL(repaint()), &pf, SLOT(repaint()));
  QObject::connect(&app, SIGNAL(levelParam(const QString &, int, int, const QString &)),
    &pf,SLOT(setLevelParam(const QString &, int, int, const QString &)));
  QObject::connect(&app, SIGNAL(setLevelSets(const QStringList &)),
    &w,SLOT(handleLevelSets(const QStringList &)));
  QObject::connect(&app, SIGNAL(setThemes()), &w, SLOT(handleSetThemes()));
  QObject::connect(&app, SIGNAL(setTheme(const QString &)), &w, SLOT(handleSetTheme(const QString &)));
  QObject::connect(&app, SIGNAL(setLevelset(const QString &)), &w, SLOT(handleSetLevelset(const QString &)));

  app.repeatSignals();

  w.setCentralWidget(&pf);
  w.show();

  //KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  //if (args->count() > 0) {
  // widget->openURL(args->url(0));
  //}

  QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

  int rc = app.exec();

  return rc;
}

// ---------- debugMsg -------------
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
void debugMsg(QtMsgType type, const QMessageLogContext &, const QString &msg)
#else
void debugMsg(QtMsgType type, const char *msg)
#endif
{
  static QFile* out;
  if (!out) {
    QString filename;
    QString path_home = CRelatedDirs::get_dir(CRelatedDirs::DI_home);
    QDir xhome(path_home);
    if (!xhome.exists()) {
      xhome.mkdir(path_home);
    }
    filename = path_home;
    filename += "soko4.log";
    //////////
    out = new QFile(filename);
    out->open(QIODevice::WriteOnly);
    out->write(filename.toUtf8());
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    out->write("\x0D");
#endif
    out->write("\x0A");
  }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  out->write(msg.toUtf8().data());
#else
  out->write(msg);
#endif
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  out->write("\x0D");
#endif
  out->write("\x0A");
  out->flush();
  Q_UNUSED(type)
}
