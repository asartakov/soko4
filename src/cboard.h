#ifndef __CBOARD_H__
#define __CBOARD_H__

#include <QObject>
#include <QVector>
#include <QByteArray>
#include "ccell.h"

class CBoard : public QObject
{
Q_OBJECT
public:
  CBoard();
  CBoard(int, int);
  CBoard(const CBoard&);
  CBoard& operator=(const CBoard&);
  CCell* at(int, int);
  const CCell* atRO(int, int) const;
  bool is_freeAt(int, int) const;
  bool is_wallAt(int, int) const;
  int width() const { return dx_; };
  int height() const { return dy_; };
  void fillEmpty();
  bool isComplete() const;
  CCell* get_neib(CCell*, int dx, int dy);
  QByteArray asQByteArray() const;
private:
  void fillEmptyRecursive(CCell*);
  void init();
  void clear();
  QVector<QVector<CCell*> > board_;
  int dx_, dy_;
};

#endif
