#include "clevelset.h"
#include <QDebug>

#include <QFile>
#include <QDir>

//static
QMap<QString, CLevelSet*> CLevelSet::map_all_sets_;

//static  ------------ CLevelSet::create_by_filename ---------
CLevelSet* CLevelSet::create_by_filename(const QString& filename, const QString& levelname)
{
#define FUNCTION "CLevelSet::create_by_filename"
  CLevelSet* res = new CLevelSet(filename);
  if (res->levels2_.size()) {
    res->name_ = levelname;
    map_all_sets_[res->name_] = res;
    qDebug() << FUNCTION ": at [1] return";
    return res;
  }
  delete res;
  return NULL;
#undef FUNCTION
}

// ------- CLevelSet::CLevelSet -----
CLevelSet::CLevelSet(const QString& filename)
{
#define FUNCTION "CLevelSet::CLevelSet"
  QFile file(filename);
  file.open(QIODevice::ReadOnly);
  QString header;
  CLevel* lvl = NULL;
  for (;;) {
    // get line
    QByteArray line(file.readLine());
    if (line.size() == 0)
      break;
    // is it suitable
    //qDebug() << FUNCTION " line=" << line;
    while (line.endsWith("\x0D")||line.endsWith("\x0A"))  // TODO: replace with filter
      line.remove(line.size()-1,1);
    bool good_line = true;
    if (line.size()) {
      // non-empty
      for (int i = 0; i < line.size(); ++i) {
        char c = line.at(i);
        bool vc = ((c == ' ') || (c == '#') || (c == '@') || (c == '.') || (c == '$') || (c == '*') || (c == '+'));
        if (!vc) {
          good_line = false;
          break;
        }
      }
    } else {
      // empty
      good_line = false;
    }
    if (good_line) {
      if (!lvl) {
        lvl = new CLevel;
        lvl->set_header(header);
      }
      lvl->append(line);
    } else {
      if (lvl) {
        // print out the level
        //qDebug() << FUNCTION " at [1]" << lvl->toString();
        if (lvl->validate()) {
          levels2_.push_back(lvl);
        } else {
          delete lvl;
        }
        lvl = NULL;
      }
      if (line.startsWith(";")) {
        header = line;
        header.remove(0, 1);
        header = header.trimmed();
      }
    }
  }
  if (lvl) {
    levels2_.push_back(lvl);
    lvl = NULL;
  }
  file.close();
#undef FUNCTION
}

// ------- CLevelSet::~CLevelSet --------
CLevelSet::~CLevelSet()
{
  QMap<QString, CLevelSet*>::iterator it = map_all_sets_.find(name_);
  if (it != map_all_sets_.end()) {
    map_all_sets_.erase(it);
  }
}

// --------- CLevelSet::getLevel ----------
CLevel* CLevelSet::getLevel(int idx)
{
  if (idx<0) return NULL;
  if (idx >= levels2_.size()) return NULL;
  return levels2_[idx];
}

/*
int CLevelSet::get_num_by_name(const QString &s)
{
  // static QList<CLevelSet*> m_all_sets;
  QList<CLevelSet*>::const_iterator i;
  int res = 0;
  for (i = all_sets_.constBegin(); i != all_sets_.constEnd(); ++i) {
    if ((*i)->name_ == s)
      return res;
    ++res;
  }
  return (-1);
}
*/

//static  ----------- CLevelSet::getSet -----------
CLevelSet* CLevelSet::getSet(const QString& nm)
{
#define FUNCTION "CLevelSet::getSet"
  QMap<QString, CLevelSet*>::iterator it = map_all_sets_.find(nm);
  if (it == map_all_sets_.end()) {
    if (map_all_sets_.size() == 0) {
      qDebug() << FUNCTION ": at [1] return NULL, nm=" << nm;
      return NULL;
    }
    it = map_all_sets_.begin();
  }
  qDebug() << FUNCTION ": at [2] return";
  return it.value();
#undef FUNCTION
}
