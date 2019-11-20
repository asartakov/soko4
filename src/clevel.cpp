#include <QDebug>
#include <clevel.h>

CLevel::CLevel()
{
}

void CLevel::append(const QString& s)
{
  body_.push_back(s);
}

// ------ CLevel::asCBoard -------
CBoard CLevel::asCBoard() const
{
#define FUNCTION "CLevel::asCBoard"
  int dx = 0;
  int dy = body_.size();
  QStringList::const_iterator it = body_.constBegin();

  for (; it != body_.constEnd(); ++it) {
    int k = (*it).size();
    if (k > dx)
      dx = k;
  }

  CBoard res(dx, dy);
  int y = 0;
  for (it = body_.constBegin(); it != body_.constEnd(); ++it) {
    int x = 0;
    QString s(*it);
    QByteArray s2(s.toUtf8());
    for (; x < s2.size(); ++x) {
      char c = s2.at(x);
      res.at(x, y)->set_value_c(c);
    }
    ++y;
  }
  // fill internal area with CCell::CCV_empty instead of CCell::CCV_outerspace
  res.fillEmpty();
  return res;
#undef FUNCTION
}

// ------- CLevel::toString ------
QString CLevel::toString() const
{
  QString res(";");
  res += header_;
  res += "\x0A";
  QStringList::const_iterator it = body_.constBegin();
  for (; it != body_.constEnd(); ++it) {
    res += (*it);
    res += "\x0A";
  }
  return res;
}

// ---------- CLevel::validate ------------
bool CLevel::validate()
{
  // remove left spaces, right spaces;
  if (body_.size() < 3)
    return false;
  QStringList::iterator it = body_.begin();
  int max_spaces = 0;
  for (; it != body_.end(); ++it) {
    if ((*it).size() > max_spaces) max_spaces = (*it).size();
  }
  for (it = body_.begin(); it != body_.end(); ++it) {
    QString tmp(*it);
    int k = 0;
    while (tmp.startsWith(" ")) {
      tmp.remove(0, 1);
      ++k;
    }
    if (k < max_spaces) max_spaces = k;
    if (k == 0) break;
  }
  if (max_spaces) {
    for (it = body_.begin(); it != body_.end(); ++it) {
      QString tmp(*it);
      tmp.remove(0, max_spaces);
      *it = tmp;
    }
  }
  for (it = body_.begin(); it != body_.end(); ++it) {
    QString tmp(*it);
    while (tmp.endsWith(" ")) {
      tmp.remove(tmp.size() - 1, 1);   //TODO: use filter
    }
    *it = tmp;
  }
  int users = 0;
  int boxes = 0;
  int targets = 0;
  for (it = body_.begin(); it != body_.end(); ++it) {
    QString tmp(*it);
    QByteArray tmp2(tmp.toUtf8());
    for (int j = 0; j < tmp2.size(); ++j) {
      char c = tmp2[j];
      if (c == '@')
        ++users;
      else if (c == '.')
        ++targets;
      else if (c == '$')
        ++boxes;
      else if (c=='+') {
        ++users;
        ++targets;
      } 
    }
  }
  return (users == 1) && (boxes == targets) && (boxes);
}
