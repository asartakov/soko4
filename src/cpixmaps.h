#ifndef __CPIXMAPS_H__
#define __CPIXMAPS_H__

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <cplayfield.h>
#include <QIcon>

#include "ctheme.h"
#include <cpixmap.h>

class CPixmaps
{
public:
  // CPixmaps::instance()->
  static CPixmaps* instance();
  void load(const QString&);
  QStringList get_themes() const;
  int get_num_of_themes() const;
  void select_theme(int);
  void select_theme(const QString &name);
  CPixmap* get_image(quint32, int);
  QIcon&  get_app_icon();
  QString get_cur_theme_name() const;
  void get_limits(int& res12, int& res14);
private:
  CPixmaps();
  void scale(int);
  Ctheme* current_theme2_;
  QMap<quint32, CPixmap*> pixmaps2_; // non-scaled current pixmaps
  QMap<QString, Ctheme*> themes2_;
  int cdxy_;
  QMap<quint32, CPixmap*> current_pixmaps2_; // scaled current pixmaps
  QIcon app_icon_;
  int max_stone_12_;
  int max_stone_14_;
};

#endif // __CPIXMAPS_H__
