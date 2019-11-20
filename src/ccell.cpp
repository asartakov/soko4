#include "ccell.h"
#include "cboard.h"
#include "cpixmaps.h"

// ------- CCell::CCell -------
CCell::CCell(CBoard* parent, int x, int y)
  : value_(CCV_outerspace)
  , wall_id_(0)
  , parent_(parent)
  , x_(x)
  , y_(y)
{
  init();
}

CCell::CCell(CBoard* parent, int v, int x, int y)
  : value_(v)
  , parent_(parent)
  , x_(x)
  , y_(y)
{
  init();
}

// ----- CCell::init ------
void CCell::init()
{
#define FUNCTION "CCell::init"
  a_ = 0;
  if (! v_is_valid(value_)) {
    qDebug() << FUNCTION ": at [1] invalid value_:" << QString("0x%1").arg(value_ , 0, 16);
  }
#undef FUNCTION
}

void CCell::copyFrom(const CCell* src)
{
  value_ = src->value_;
}

// ----- CCell::is_free -------
bool CCell::is_free() const
{
  return ((value_ & (CCV_box | CCV_wall | CCV_outerspace)) == 0);
}

// ----- CCell::is_wall -------
bool CCell::is_wall() const
{
  return (value_ & (CCV_wall));
}

// ----- CCell::is_box -------
bool CCell::is_box() const
{
  return (value_ & (CCV_box));
}

// ----- CCell::is_user -------
bool CCell::is_user() const
{
  return (value_ & (CCV_user));
}

// ----- CCell::is_unpair -------
bool CCell::is_unpair() const
{
  return ((value_ == CCV_box)||(value_ == CCV_target));
}

// ----- CCell::is_outerspace -------
bool CCell::is_outerspace() const
{
  return (value_ & (CCV_outerspace));
}

// -------- CCell::get_neib -------
CCell* CCell::get_neib(Direction d)
{
  switch (d) {
    case DIR_none:
      return this;
    case DIR_left:
      return parent_->get_neib(this,-1, 0);
    case DIR_right:
      return parent_->get_neib(this, 1, 0);
    case DIR_up:
      return parent_->get_neib(this, 0,-1);
    case DIR_down:
      return parent_->get_neib(this, 0, 1);
    case DIR_lu:
      return parent_->get_neib(this,-1,-1);
    case DIR_ru:
      return parent_->get_neib(this, 1,-1);
    case DIR_ld:
      return parent_->get_neib(this,-1, 1);
    case DIR_rd:
      return parent_->get_neib(this, 1, 1);
    }
    return NULL;
}

void CCell::set_value_c(char c)
{
  set_value_i(char2CCV(c));
}

void CCell::set_value_i(int c)
{
  value_ = c;
}

char CCell::CCV2char(int v)
{
  switch(v&(CCV_target|CCV_box|CCV_user|CCV_outerspace|CCV_wall)) {
    case CCV_empty:
      case CCV_outerspace:
        return ' ';
      case CCV_target:
        return '.';
      case CCV_box:
        return '$';
      case CCV_box_on_target:
        return '*';
      case CCV_user:
        return '@';
      case CCV_user_on_target:
        return '+';
      case CCV_wall:
        return '#';
  }
  return '#';
}

int CCell::char2CCV(char c)
{
  switch(c) {
    case ' ':
      return CCV_outerspace;
    case '#':
      return CCV_wall;
    case '$': // box
      return CCV_box;
    case '.': // target
      return CCV_target;
    case '+': // user_on_target
      return CCV_user_on_target;
    case '*': // box_on_target
      return CCV_box_on_target;
    case '@': // user
      return CCV_user;
    }
    return CCV_outerspace;
}

void CCell::remove_outerspace()
{
  value_ &= ~CCV_outerspace;
}

void CCell::remove_user()
{
  value_ &= ~CCV_user;
}

void CCell::add_user()
{
  value_ |= CCV_user;
}

void CCell::remove_box()
{
  value_ &= ~CCV_box;
}

void CCell::add_box()
{
  value_ |= CCV_box;
}

// ---------- CCell::getPixmap ------------
CPixmap* CCell::getPixmap(int cdxy)
{
  CPixmaps* pms = CPixmaps::instance();
  CPixmap* res = NULL;

  if (value_ == CCV_target) {
    return pms->get_image(CPlayField::PIX_target, cdxy);
  }
  if (value_ == CCV_empty) {
    return pms->get_image(CPlayField::PIX_empty, cdxy);
  }
  if (value_ == CCV_box) {
    return pms->get_image(CPlayField::PIX_box, cdxy);
  }
  if (value_ == CCV_box_on_target) {
    return pms->get_image(CPlayField::PIX_box_on_target, cdxy);
  }
  if (value_ == CCV_user_on_target) {
    return pms->get_image(CPlayField::PIX_user_on_target, cdxy);
  }
  if (value_ == CCV_user) {
    return pms->get_image(CPlayField::PIX_user, cdxy);
  }
  if (value_ == CCV_wall) {
    // enhance the masonry
    if (!wall_id_) {
      int n12, n14;
      pms->get_limits(n12, n14);
      if ((!n12) || (!n14)) {
        wall_id_ = 0x08;
      } else {
        int lower_semi = 0;
        int left_semi = 0;
        int right_semi = 0;
        int left_q = 0;
        int right_q = 0;

        lower_semi = (::rand() % n12) + 1;
        CCell* l = get_left();
        if (l) {
          if (l->value_ == CCV_wall) left_semi = (l->wall_id_ >> 20) & 0x0F;
          else left_q = (::rand() % n14) + 1;
        } else {
          left_q = (::rand() % n14) + 1;
        }
        CCell* r = get_right();
        if (r) {
          if (r->value_ == CCV_wall) right_semi = (::rand() % n12) + 1;
          else right_q = (::rand() % n14) + 1;
        } else {
          right_q = (::rand() % n14) + 1;
        }
        wall_id_ = 0x08 | (lower_semi << 4) | (left_q << 8) | (right_q << 12)
               | (left_semi << 16) | (right_semi << 20);
      }
    }
    return pms->get_image(wall_id_, cdxy);
  }
  if (value_ == CCV_outerspace) {
    //return pms->get_image(CPlayField::PIX_outerspace,cdxy);
    return NULL;
  }
  return res;
}

CCell* CCell::get_top()
{
  return parent_->get_neib(this, 0, -1);
}

CCell* CCell::get_bottom()
{
  return parent_->get_neib(this, 0, 1);
}

CCell* CCell::get_left()
{
  return parent_->get_neib(this, -1, 0);
}

CCell* CCell::get_right()
{
  return parent_->get_neib(this, 1, 0);
}

bool CCell::v_is_valid(int v)
{
  if (v & 0x08) return true;
  return ((v >= 1) && (v <= 7));
}

