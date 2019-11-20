#ifndef __CPLAYBOARD_H__
#define __CPLAYBOARD_H__

#include <QObject>
#include <clevel.h>
#include <cmove.h>

class CPlayBoard : public QObject
{
Q_OBJECT
 public:
  static CPlayBoard* instance(); // CPlayBoard* pn = CPlayBoard::instance();
  void reset(const CLevel&);
  bool goTo(int x_dest, int y_dest);
  bool pushTo(int x_dest, int y_dest);
  CCell* at(int x, int y) { return board_.at(x, y); }
  int aAt(int x, int y);
  int width() const { return board_.width(); }
  int height() const { return board_.height(); }
  void get_level_info();
  QString get_signature() const { return signature_; }
  int get_moves_size() const;
  int get_cur_move() const;
  bool does_solution_exist() const { return solution_exists_; }
  QStringList get_all_lurds() const;

 public Q_SLOTS:
  void handleRestartLevel();
  void handleUndo();
  void handleMoveRight();
  void handleMoveLeft();
  void handleMoveUp();
  void handleMoveDown();
  void handleRedo();
  void handleResolve();

 Q_SIGNALS:
  void repaint();

 private:
  enum LURD {
    lurd_none = 0,
    lurd_l = 'l',
    lurd_u = 'u',
    lurd_r = 'r',
    lurd_d = 'd'
  };
  CPlayBoard();
  void save_level_info();
  void applyMove(const CMove&);

  QList<CMove> moves_;
  CBoard board_;
  CBoard board0_; // for quick restart
  int user_x_;
  int user_y_;
  QString signature_;
  QString label_path_;
  bool solution_exists_;
  int cur_idx_;
};

#endif
