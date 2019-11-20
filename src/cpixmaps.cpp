#include <QDebug>
#include <cpixmaps.h>

#include <QString>
#include <QDir>

#include <cplayfield.h>
#include <cpixmap.h>

CPixmaps::CPixmaps()
  : current_theme2_(NULL)
  , cdxy_(0)
{
}

// -------- CPixmaps::instance ------
CPixmaps* CPixmaps::instance()
{
  static CPixmaps* res;
  if (!res) {
    res = new CPixmaps;
  }
  return res;
}

// ------- CPixmaps::load ---------
void CPixmaps::load(const QString& path)
{
#define FUNCTION "CPixmaps::load"
  // /usr/share/soko4/themes or
  // ~/.soko4/themes or
  // ~/work/sokoban/soko4/themes
  qDebug() << FUNCTION ": at [0] path=" << path;
  QDir dir1(path);
  if (!dir1.exists()) {
    dir1.mkpath(path);
    qDebug() << FUNCTION ": at [1] return";
    return;
  }
  QStringList list(dir1.entryList());
  QStringList::const_iterator xj = list.constBegin();
  for (; xj != list.constEnd(); ++xj) {
    if ( (*xj).startsWith("."))
      continue;
    if (themes2_.contains(*xj)) {
      qDebug() << FUNCTION ": at [2] " << (*xj) << " already loaded, skip it";
      continue;
    }
    QString p3(path);
    p3 += QDir::separator();
    p3 += *xj;
    qDebug() << FUNCTION ": at [3] p3==" << p3;
    Ctheme* theme = Ctheme::create_theme(p3);

    if (theme) {
      themes2_[*xj] = theme;
    }
  }

  // default theme: "ksokoban"
  // TODO(asartakov): theme from the config
  QMap<QString, Ctheme*>::iterator it = themes2_.find(QString("ksokoban"));
  if (it == themes2_.end()) {
    it = themes2_.begin();
    if (it == themes2_.end()) {
      qDebug() << FUNCTION ": at [4] no themes, abort";
      abort();
    }
  }
  current_theme2_ = it.value();

  QDir dir3;
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  dir3.setPath("../pixmaps");
  if (!dir3.exists()) {
    dir3.setPath("/usr/share/soko4/pixmaps");
  }
#else
  dir3.setPath("pixmaps");
  if (!dir3.exists()) {
    dir3.setPath("/usr/share/soko4/pixmaps");
  }
#endif
  if (dir3.exists()) {
    QString filename=dir3.path();
    filename += QDir::separator();
    filename += "hi32-app-ksokoban.png";
    qDebug() << FUNCTION ": at [4] filename=" << filename;
    QFile file1(filename);
    if (file1.exists()) {
      app_icon_.addFile(filename);
    }
  }
#undef FUNCTION
}

// -------- CPixmaps::get_image -------
CPixmap* CPixmaps::get_image(quint32 key, int cdxy)
{
#define FUNCTION "CPixmaps::get_image"
  CPixmap* res = NULL;
  qDebug() << FUNCTION ": at [0]" << QString("0x%1").arg(key , 0, 16);
  if (!current_theme2_) {
    qDebug() << FUNCTION ": at [1]";
    return res;
  }

  bool modified = false;
  if (!pixmaps2_.contains(key)) {
    current_theme2_->provide(key, pixmaps2_);
    modified = true;
  }
  if (cdxy <= 0) {
    qDebug() << FUNCTION ": at [2], pixmaps2_.size()==" << pixmaps2_.size();
    return pixmaps2_[key];
  }
  if ((cdxy_ != cdxy) || (modified)) {
    scale(cdxy);
  }
  //return pixmaps_[current_theme_][idx];
  res = current_pixmaps2_[key];
  if (!res) {
    qDebug() << FUNCTION ": at [3] key=" << QString("0x%1").arg(key , 0, 16) << " return NULL";
  }
  return res;
#undef FUNCTION
}

// ------------ CPixmaps::scale -----------
void CPixmaps::scale(int cdxy)
{
#define FUNCTION "CPixmaps::scale"
  int nn = 0;
  Q_FOREACH(quint32 key, pixmaps2_.keys()) {
    CPixmap* p1 = current_pixmaps2_[key];
    if (p1) {
      *p1 = pixmaps2_[key]->scaled(cdxy);
    } else {
      current_pixmaps2_[key] = new CPixmap(pixmaps2_[key]->scaled(cdxy));
    }
    ++nn;
  }
  qDebug() << FUNCTION ": at [1] nn=" << nn << " cdxy=" << cdxy;
  cdxy_ = cdxy;
#undef FUNCTION
}

// --------- CPixmaps::get_themes ------
QStringList CPixmaps::get_themes() const
{
  return themes2_.keys();
}

// ------------- CPixmaps::select_theme -------------
void CPixmaps::select_theme(const QString& name)
{
  QMap<QString, Ctheme*>::iterator it = themes2_.find(name);
  if (it != themes2_.end()) {
    current_theme2_ = it.value();
    cdxy_ = 0;
  }
}

// ------- CPixmaps::get_app_icon -------------
QIcon& CPixmaps::get_app_icon()
{
  return app_icon_;
}

void CPixmaps::get_limits(int& res12, int& res14)
{
  if (current_theme2_) {
    res12 = current_theme2_->get_max_stone_12();
    res14 = current_theme2_->get_max_stone_14();
  } else {
    res12 = 0;
    res14 = 0;
  }
}

// ---------- CPixmaps::get_cur_theme_name -----------
QString CPixmaps::get_cur_theme_name() const
{
  if (current_theme2_) {
    return current_theme2_->get_name();
  } else {
    return QString("");
  }
}
