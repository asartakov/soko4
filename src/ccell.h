#ifndef __CCELL_H__
#define __CCELL_H__

#include <QDebug>
#include <cpixmap.h>

class CBoard;

class CCell
{
public:
  enum {
    CCV_empty = 0,
    CCV_target = 1,
    CCV_box = 2,
    CCV_box_on_target = CCV_box|CCV_target,
    CCV_user = 4,
    CCV_user_on_target = CCV_user|CCV_target,
    CCV_outerspace = 8,
    CCV_wall = 16
  };
  enum Direction {
    DIR_none = 0,
    DIR_left,
    DIR_right,
    DIR_up,
    DIR_down,
    DIR_lu,
    DIR_ru,
    DIR_ld,
    DIR_rd
  };
  CCell(CBoard* parent, int, int);
  CCell(CBoard* parent, int, int, int);
  static char CCV2char(int);
  static int char2CCV(char);
  void set_value_c(char c);
  void set_value_i(int c);
  CCell* get_top();
  CCell* get_bottom();
  CCell* get_left();
  CCell* get_right();
  bool is_free() const;
  bool is_box() const;
  bool is_wall() const;
  bool is_user() const;
  bool is_outerspace() const;
  bool is_unpair() const; // a box without a target or a target without a box
  void copyFrom(const CCell *);
  CCell* get_neib(Direction);
  bool seen_;
  int a_; // for A*
  void remove_user();
  void add_user();
  void remove_box();
  void add_box();
  void remove_outerspace();
  CPixmap* getPixmap(int);
  char get_value_c() const { return CCV2char(value_); };
private:
  CCell();
  bool v_is_valid(int);
  void init();
  int value_;
  quint32 wall_id_;
  CBoard *parent_;
  int x_;
  int y_;
};

#endif
