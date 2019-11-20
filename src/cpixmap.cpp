#include "cpixmap.h"

CPixmap::CPixmap(quint32 id, const QImage& image)
  : QPixmap()
  , id_(id)
  , image_(image)
{
   QPixmap::operator=(QPixmap::fromImage(image_));
}

CPixmap::CPixmap(int w, int h)
  : QPixmap(w, h)
  , id_(0)
{
}

CPixmap CPixmap::scaled(int cdxy)
{
  CPixmap res(cdxy, cdxy);
  (static_cast<QPixmap*>(&res))->operator=((static_cast<QPixmap*>(this))->scaled(cdxy, cdxy));
  res.id_ = id_;
  return res;
}

CPixmap::CPixmap(const CPixmap& o)
  : QPixmap(o)
  , id_(o.id_)
  , image_(o.image_)
{
}

