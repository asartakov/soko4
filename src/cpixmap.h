#ifndef __CPIXMAP_H__
#define __CPIXMAP_H__

#include <QPixmap>
#include <QImage>

class CPixmap: public QPixmap
{
 public:
  CPixmap(quint32 id, const QImage&);
  CPixmap(const CPixmap&);
  CPixmap(int w, int h);
  quint32 get_id() const { return id_; }
  QImage& image() { return image_; }
  CPixmap& operator=(const CPixmap& another) {
    QPixmap::operator=(another);
    id_ = another.id_;
    image_ = another.image_;
    return *this;
  }
  CPixmap scaled(int);

 protected:
  quint32 id_;
  QImage image_;
};

#endif // __CPIXMAP_H__
