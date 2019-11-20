#ifndef __CTHEME_H__
#define __CTHEME_H__

class CPixmap;
#include <QString>
#include <QMap>

class Ctheme
{
 public:
  static Ctheme* create_theme(const QString&); // Ctheme* theme = Ctheme::create_theme(path);

  void get_pixmaps(QMap<quint32, CPixmap*>&);
  QString get_name() { return name_; };
  quint32 provide(quint32, QMap<quint32, CPixmap*>&);
  int get_max_stone_12() const {return max_stone_12_;}
  int get_max_stone_14() const {return max_stone_14_;}
  ~Ctheme();

 protected:
  Ctheme();
  QString name_;
  QMap<quint32, CPixmap*> pixmaps_;
  int max_stone_12_;
  int max_stone_14_;
  int standard_w_;
};

#endif // __CTHEME_H__
