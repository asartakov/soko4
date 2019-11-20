#include <QDebug>
#include <crelateddirs.h>
#include <QDir>

QString CRelatedDirs::home_;
QString CRelatedDirs::commandline_;

// -------- CRelatedDirs::get_dir -----------
QString CRelatedDirs::get_dir(DirIDs id)
{
#define FUNCTION "CRelatedDirs::get_dir"
  switch (id) {
    case DI_home:
      return get_home();
    case DI_share:
      {
        QString path_share;
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    // C:\Documents and settings\johndoe\.soko4         //
    // C:\Program files\soko4\                          //
    // %runpath%\                                       //
    // %runpath%\..\                                    //
        path_share = getenv("ProgramFiles");
        path_share += QDir::separator();
        path_share += "soko4";
        path_share += QDir::separator();
#else
        path_share ="/usr/share/soko4/";
#endif
        path_share.replace(QChar('/'),QDir::separator());
        return path_share;
      }
    case DI_pixmaps:
      {
        QString path1=get_dir(DI_share);
        path1 += "pixmaps";
        path1 += QDir::separator();
        if (QDir(path1).exists()) return path1;
        // ./
        QFileInfo info(commandline_);
        QString path_here = info.absolutePath();

        path_here.replace(QChar('/'),QDir::separator());
        path_here += QDir::separator();

        path_here += "..";
        path_here += QDir::separator();

        path_here += "pixmaps";
        if (QDir(path1).exists()) return path1;
      }
      qDebug() << FUNCTION "(DI_pixmaps): unable to locate pixmaps";
      exit(1);
      break;
    case DI_home_dot_soko4:
      {
        QString res = get_home();
        res += ".soko4";
        res += QDir::separator();
        return res;
      }
    case DI_home_dot_soko4_bookmarks:
      {
        QString res = get_dir(DI_home_dot_soko4);
        res += "bookmarks";
        res += QDir::separator();
        return res;
      }
  }
  return QString();
#undef FUNCTION
}

// ------------ CRelatedDirs::get_levels -----------
QStringList CRelatedDirs::get_levels()
{
#define FUNCTION "CRelatedDirs::get_levels"
#warning CRelatedDirs::get_levels implementation
  return QStringList();
#undef FUNCTION
}

// -------------- CRelatedDirs::get_themes -----------
QStringList CRelatedDirs::get_themes()
{
#define FUNCTION "CRelatedDirs::get_themes"
#warning CRelatedDirs::get_themes implementation
  return QStringList();
#undef FUNCTION
}

// ----------- CRelatedDirs::get_home -----------
QString CRelatedDirs::get_home()
{
  if (home_.length() == 0) {
    QString path_home(QDir::homePath());
    if (!path_home.endsWith(QDir::separator())) {
      path_home += QDir::separator();
    }
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    path_home += "Application Data";
    path_home += QDir::separator();
    path_home += "soko4";
#else
    path_home += ".soko4";
#endif
    path_home += QDir::separator();
    path_home.replace(QChar('/'),QDir::separator());
    home_ = path_home;
  }
  return home_;
}
