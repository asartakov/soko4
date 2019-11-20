#ifndef __CLEVELSET_H__
#define __CLEVELSET_H__

#include "clevel.h"
class QString;

class CLevelSet
{
 public:
  static CLevelSet* create_by_filename(const QString&, const QString&);
  ~CLevelSet();
  int get_num_of_levels() const { return levels2_.size(); }
  CLevel* getLevel(int);
  QString getName() const { return name_; }

  //static int get_num_of_sets()  { return map_all_sets_.keys().size(); }
  static CLevelSet* getSet(const QString&); // CLevelSet* set = CLevelSet::getSet(name);

 private:
  explicit CLevelSet(const QString&);
  static QMap<QString, CLevelSet*> map_all_sets_;
  QVector<CLevel*> levels2_;
  QString name_;
};

#endif
