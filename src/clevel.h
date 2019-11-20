#ifndef __CLEVEL_H__
#define __CLEVEL_H__

#include <QByteArray>
#include <QList>
#include <QString>
#include "cboard.h"

class CLevel
{
 public:
  CLevel();
  void append(const QString&);
  void set_header(const QString& h) { header_ = h; }
  QString getName() const { return header_; }
  CBoard asCBoard() const;
  QString toString() const;
  bool validate();
private:
  QStringList body_;
  QString header_;
};

#endif
