#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
 public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

 public slots:
  void handleRestartLevel(bool);
  void handleUndo(bool);
  void handleRedo(bool);
  void handleResolve(bool);
  void handleNextLevel(bool);
  void handlePrevLevel(bool);
  void handleTextWindow(bool);
  void handleLevelSets(const QStringList &);
  void handleSetThemes();
  void handleSetLevelset(const QString &);
  void handleSetTheme(const QString &);

 signals:
  void restartLevel();
  void undo(); 
  void redo();
  void resolve();
  void nextLevel();
  void prevLevel();
  void nextUnresolvedLevel();
  void prevUnresolvedLevel();
  void textWindow();
  void setLevelSet(const QString&);
  void themeChanged();
  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void mainWindowClosed();
  void gotoLevel1();

 private:
  Ui::MainWindow* ui;

 private slots:
  void handleLevelset();
  void handleTheme();

protected:
  void keyPressEvent(QKeyEvent*);
  void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
