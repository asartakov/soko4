#ifndef __CRELATEDDIRS_H__
#define __CRELATEDDIRS_H__

#include <QString>
#include <QStringList>

class CRelatedDirs
{
public:
  enum DirIDs {
    DI_home,
    DI_share,
    DI_pixmaps,
    DI_home_dot_soko4,
    DI_home_dot_soko4_bookmarks
  };

  static QString get_dir(DirIDs);
  static QStringList get_levels();
  static QStringList get_themes();
  static void set_commandline(const QString& s) { commandline_ = s;}
private:
  CRelatedDirs();
  static QString get_home();

  static QString home_;
  static QString commandline_;
};

#endif
