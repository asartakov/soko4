#include "ctheme.h"
#include <QDebug>
#include <QString>
#include <QDir>
#include <QPainter>

#include "cpixmap.h"

Ctheme::Ctheme()
  : max_stone_12_(0)
  , max_stone_14_(0)
  , standard_w_(0)
{
}

Ctheme::~Ctheme()
{
  Q_FOREACH(quint32 id, pixmaps_.keys()) {
    CPixmap* p = pixmaps_[id];
    delete p;
  }
}

// static  ------------ Ctheme::create_theme -----------
Ctheme* Ctheme::create_theme(const QString& p3)
{
#define FUNCTION "Ctheme::create_theme"
  qDebug() << FUNCTION ": at [1] p3==" << p3;
  QDir dir2(p3);
  QStringList list2(dir2.entryList());
  QStringList::const_iterator xk = list2.constBegin();

  Ctheme* res = new Ctheme();
  QRegExp rexp("([0-9a-f]{8,8})\\.png", Qt::CaseInsensitive);
  for (; xk != list2.constEnd(); ++xk) {
    if ( (*xk).startsWith(".")) continue;
    QString p4(p3);
    p4 += QDir::separator();
    p4 += *xk;

    qDebug() << FUNCTION ": at [1.5] p4==" << p4;

    if (rexp.exactMatch(*xk)) {
      bool ok;
      qDebug() << FUNCTION ": at [1.6] rexp.cap(0)==" << rexp.cap(0);
      quint32 v = rexp.cap(0).left(8).toUInt(&ok, 16);
      if (!ok) {
        delete res;
        qDebug() << FUNCTION ": at [2] return NULL";
        return NULL;
      }
      if (v & 0xF0) {
        ++res->max_stone_12_;
        v = (res->max_stone_12_ << 4) | 0x08;
        if (res->standard_w_ == 0) res->standard_w_ = -1; // flag: get the width
      } else if (v & 0xF00) {
        ++res->max_stone_14_;
        v = (res->max_stone_14_ << 8) | 0x08;
      }
      QImage image(p4);
      CPixmap* p = new CPixmap(v, image);
      res->pixmaps_[v] = p;
      qDebug() << FUNCTION ": at [2.5] v=0x" << QString("%1").arg(v , 0, 16);;
      if (res->standard_w_ < 0) {
        res->standard_w_ = p->width(); // work on the flag
      }
    }
  }
  qDebug() << FUNCTION ": at [3] res->pixmaps_.size()==" << res->pixmaps_.size()
           << " res->max_stone_12_=" << res->max_stone_12_
           << " res->max_stone_14_=" << res->max_stone_14_;
  Q_FOREACH (quint32 key, res->pixmaps_.keys()) {
    qDebug() << FUNCTION ": at [3.1] key=" << QString("0x%1").arg(key , 0, 16);
  }
  return res;
#undef FUNCTION
}

// --------------- Ctheme::get_pixmaps -----------------
void Ctheme::get_pixmaps(QMap<quint32, CPixmap*>& dest)
{
  dest.clear();
  Q_FOREACH(quint32 v, pixmaps_.keys()) {
    dest[v] = pixmaps_[v];
  }
}

// ------------ Ctheme::provide --------------
quint32 Ctheme::provide(quint32 id, QMap<quint32, CPixmap*>& dest)
{
#define FUNCTION "Ctheme::provide"
  if (! (id & 0x08)) {
    if (!dest.contains(id)) {
      CPixmap* pm = pixmaps_[id];
      if (pm) {
        dest[id] = pm;
      } else {
        qDebug() << FUNCTION ": at [0.5] pm is NULL";
      }
    }
    qDebug() << FUNCTION ": at [1] return id:" << QString("0x%1").arg(id , 0, 16);
    return id;
  }
  int lower_semi = (id >> 4) & 0x0F;
  int left_q = (id >> 8) & 0x0F;
  int right_q = (id >> 12) & 0x0F;
  int left_semi = (id >> 16) & 0x0F;
  int right_semi = (id >> 20) & 0x0F;

  if (lower_semi > max_stone_12_) lower_semi = max_stone_12_;
  if (left_semi > max_stone_12_) left_semi = max_stone_12_;
  if (right_semi > max_stone_12_) right_semi = max_stone_12_;

  if (left_q > max_stone_14_) left_q = max_stone_14_;
  if (right_q > max_stone_14_) right_q = max_stone_14_;

  /*
  if ( (lower_semi == 0)
   || ((left_semi == 0) && (left_q == 0))
   || ((right_semi == 0) && (right_q == 0)) ) {
    if (!dest.contains(id)) {
      CPixmap* pm = pixmaps_[id];
      if (pm) dest[id] = pm;
    }
    qDebug() << FUNCTION ": at [2] return id=8";
    return 0x08;
  }
  */

  quint32 v = (right_semi << 20) | (left_semi << 16) | (right_q << 12)
            | (left_q << 8) | (lower_semi << 4) |  0x08;

  CPixmap* p1 = NULL;
  if (pixmaps_.contains(v)) {
    p1 = pixmaps_[v];
  } else {
    p1 = new CPixmap(standard_w_, standard_w_); // a square
    pixmaps_[v] = p1;
    qDebug() << FUNCTION ": at [2.1] build the image for id=" << QString("%1").arg(v , 0, 16);
    QPainter painter1(p1);
    {
      quint32 id = (lower_semi << 4) | 0x08;  // [sic]
      CPixmap* p2 = pixmaps_[id];
      painter1.drawImage(0, standard_w_ / 2, p2->image());
    }
    if (left_q) {
      quint32 id = (left_q << 8) | 0x08;
      CPixmap* p2 = pixmaps_[id];
      if (p2) {
        painter1.drawImage(0, 0, p2->image());
      } else {
        qDebug() << FUNCTION ": at [2.2] id=" << QString("0x%1").arg(id , 0, 16);
      }
    }
    if (right_q) {
      quint32 id = (right_q << 8) | 0x08;
      CPixmap* p2 = pixmaps_[id];
      if (p2) {
        painter1.drawImage(standard_w_ / 2, 0, p2->image());
      } else {
        qDebug() << FUNCTION ": at [2.3] id=" << QString("0x%1").arg(id , 0, 16);
      }
    }
    if (left_semi) {
      quint32 id = (left_semi << 4) | 0x08;  // [sic]
      CPixmap* p2 = pixmaps_[id];
      if (p2) {
        painter1.drawImage(0, 0, p2->image(), standard_w_ / 2, 0, standard_w_ / 2, standard_w_ / 2);
      } else {
        qDebug() << FUNCTION ": at [2.4] id=" << QString("0x%1").arg(id , 0, 16);
      }
    }
    if (right_semi) {
      quint32 id = (right_semi << 4) | 0x08;  // [sic]
      CPixmap* p2 = pixmaps_[id];
      if (p2) {
        painter1.drawImage(standard_w_ / 2, 0, p2->image(), 0, 0, standard_w_ / 2, standard_w_ / 2);
      } else {
        qDebug() << FUNCTION ": at [2.5] id=" << QString("0x%1").arg(id , 0, 16);
      }
    }
  }
  dest[v] = p1;
  return v;
#undef FUNCTION
}
