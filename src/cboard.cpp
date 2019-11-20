#include <QDebug>
#include <cboard.h>

// ------- CBoard::CBoard -------
CBoard::CBoard()
    :QObject()
{
  dx_ = 1;
  dy_ = 1;
  init();
}

CBoard::CBoard(int dx, int dy)
  : QObject()
  , dx_(dx)
  , dy_(dy)
{
  if (dx_ < 1) dx_ = 1;
  if (dy_ < 1) dy_ = 1;
  init();
}

CBoard::CBoard(const CBoard& src)
  : QObject()
{
  dx_ = src.dx_;
  dy_ = src.dy_;
  init();
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      CCell* c1 = at(x, y);
      const CCell* c2 = src.atRO(x, y);
      c1->copyFrom(c2);
      //at(x,y)->copyFrom(src.atRO(x,y));
    }
  }
}

// -------- CBoard::init --------
void CBoard::init()
{
  board_.resize(dy_);
  for (int y = 0; y < dy_; ++y) {
    board_[y].resize(dx_);
    for (int x = 0; x < dx_; ++x) {
      board_[y][x] = new CCell(this, x, y);
    }
  }
}

// ------- CBoard::clear ---------
void CBoard::clear()
{
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      delete board_[y][x];
      board_[y][x] = NULL;
    }
  }
}

// ----------- CBoard::at -----------
CCell* CBoard::at(int x, int y)
{
  return board_[y][x];
}

// ------------- CBoard::atRO -------
const CCell* CBoard::atRO(int x, int y) const
{
  return board_[y][x];
}

// ----------- CBoard::is_freeAt -------------
bool CBoard::is_freeAt(int x, int y) const
{
    return board_[y][x]->is_free();
}

// ------------- CBoard::is_wallAt ------------
bool CBoard::is_wallAt(int x, int y) const
{
    return board_[y][x]->is_wall();
}

// ------------ CBoard::operator= ----------
CBoard& CBoard::operator=(const CBoard& src)
{
  if (this == &src) return *this;
  clear();
  dx_ = src.dx_;
  dy_ = src.dy_;
  init();
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      at(x, y)->copyFrom(src.atRO(x, y));
    }
  }
  return *this;
}

// -------- CBoard::fillEmpty -------
void CBoard::fillEmpty()
{
  CCell* start = NULL;
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      CCell* c1 = at(x, y);
      c1->seen_ = false;
      if (c1->is_user()) {
        start = c1;
      }
    }
  }
  if (start) fillEmptyRecursive(start);
}

// ---------- CBoard::fillEmptyRecursive -----------
void CBoard::fillEmptyRecursive(CCell* p)
{
  p->seen_ = true;
  // TODO: remove the indian code
  if (p->is_outerspace()) p->remove_outerspace();
  CCell* p2 = p->get_top();
  if (p2 && !(p2->seen_)) {
    //c=p2->get_value();
    if (!(p2->is_wall())) fillEmptyRecursive(p2);
  }
  p2 = p->get_bottom();
  if (p2 && !(p2->seen_)) {
    if (!(p2->is_wall())) fillEmptyRecursive(p2);
  }
  p2 = p->get_left();
  if (p2 && !(p2->seen_)) {
    if (!(p2->is_wall())) fillEmptyRecursive(p2);
  }
  p2 = p->get_right();
  if (p2 && !(p2->seen_)) {
    if (!(p2->is_wall())) fillEmptyRecursive(p2);
  }
}

bool CBoard::isComplete() const
{
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      if (atRO(x, y)->is_unpair()) return false;
    }
  }
  return true;
}

CCell* CBoard::get_neib(CCell* p, int dx, int dy)
{
#define FUNCTION "CBoard::get_neib"
// TODO: get the (x_,y_) from the cell
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      if (atRO(x, y) == p) {
        if ((x + dx >= 0) && (x + dx < dx_) && (y + dy >= 0) && (y + dy < dy_)) {
          return at(x + dx, y + dy);
        } else {
          return NULL;
        }
      }
    }
  }
  qDebug() << FUNCTION " p not found ";
  return NULL;
#undef FUNCTION
}

QByteArray CBoard::asQByteArray() const
{
  QByteArray res;
  res.reserve((dx_ + 1) * dy_);
  int idx = 0;
  for (int y = 0; y < dy_; ++y) {
    for (int x = 0; x < dx_; ++x) {
      res[idx++] = atRO(x, y)->get_value_c();
    }
    res[idx++] = '\x0A';
  }
  return res;
}
