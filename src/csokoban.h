#ifndef __CSOKOBAN_H__
#define __CSOKOBAN_H__

#include <QApplication>
#include <clevel.h>
#include <clevelset.h>
#include <textwindow.h>

class CSokoban : public QApplication
{
Q_OBJECT
 public:
  CSokoban(int&, char**);
  CLevel* getCurLevel();
  void repeatSignals();

 public slots:
  void goToTheNextLevel();
  void goToThePrevLevel();
  void goToTheNextULevel();
  void goToThePrevULevel();
  void showTextWindow();
  void handleSetLevelSet(const QString&);
  void saveLast();
  void handleLevel1();

 Q_SIGNALS:
  void repaint();
  void levelParam(const QString&, int, int, const QString&);
  void setLevelSets(const QStringList&);
  void setThemes();
  void setTheme(const QString&);
  void setLevelset(const QString&);

 private:
  void init();
  void levelInfo();
  QString commandline_;
  QString cur_levelset_name_;
  int cur_level_num_;
  static CSokoban* sokoban_;
  CTextWindow* text_window_;
  QStringList levelsets_;
  QString last_; // filename of ~/.soko4/last

 private Q_SLOTS:
  void handleClosingTextWindow();
};

#endif
