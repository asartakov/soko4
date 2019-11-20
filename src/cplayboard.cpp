#include <QCryptographicHash>
#include <cplayboard.h>
#include <crelateddirs.h>
#include <QFile>
#include <QDir>

// -------- CPlayBoard::CPlayBoard -------
CPlayBoard::CPlayBoard()
  : cur_idx_(0)
{
}

// --------- CPlayBoard::instance --------
CPlayBoard* CPlayBoard::instance()
{
  static CPlayBoard* res;
  if (!res) {
    res = new CPlayBoard();
  }
  return res;
}

// ---------- CPlayBoard::reset ----------
void CPlayBoard::reset(const CLevel& lvl)
{
  moves_.clear();
  cur_idx_ = 0;
  solution_exists_ = false;
  board_ = lvl.asCBoard();
  board0_ = board_;
  get_level_info();
  for (int y = 0; y < board_.height(); ++y) {
    for (int x = 0; x < board_.width(); ++x) {
      if (board_.atRO(x,y)->is_user()) {
        user_x_ = x;
        user_y_ = y;
        return;
      }
    }
  }
  user_x_ = -1;
  user_y_ = -1;
}

// --------- CPlayBoard::goTo ---------
bool CPlayBoard::goTo(int x_dest, int y_dest)
{
#define FUNCTION "CPlayBoard::goTo"
  int x0 = user_x_;
  int y0 = user_y_;
  bool res = true;
  if (!board_.at(x_dest,y_dest)->is_free()) {
    //qDebug() << FUNCTION " at [1] destination is not free";
    return false;
  }
  // phase 1) reset a_
  int max = height() * width();
  for (int y1 = 0; y1 < height(); ++y1) {
    for (int x1 = 0; x1 < width(); ++x1) {
      board_.at(x1,y1)->a_ = max;
    }
  }
  board_.at(x_dest, y_dest)->a_ = 0;
  // phase 2) lower
  int nc = 0;
  for (;;) {
    ++nc;
    bool complete = true;
    for (int y = 0; y < height(); ++y) {
      for (int x = 0; x < width(); ++x) {
        if (board_.at(x, y)->is_free()) {
          CCell* cur = board_.at(x, y);
          CCell* tmp = cur->get_top();
          if (tmp && tmp->is_free() && (tmp->a_ < (cur->a_ - 1))) {    // TODO: remove indian code
            cur->a_ = tmp->a_ + 1;
            complete = false;
          }
          tmp = cur->get_bottom();
          if (tmp && tmp->is_free() && (tmp->a_ < (cur->a_ - 1))) {
            cur->a_ = tmp->a_ + 1;
			complete=false;
		    }
		    tmp=cur->get_left();
		    if (tmp && tmp->is_free() && (tmp->a_<(cur->a_-1))) {
			cur->a_ = tmp->a_ + 1;
			complete=false;
		    }
		    tmp=cur->get_right();
		    if (tmp && tmp->is_free() && (tmp->a_<(cur->a_-1))) {
			cur->a_ = tmp->a_ + 1;
			complete=false;
		    }
		}
	    }
	}
	if (complete)
	    break;
    }
  //qDebug() << FUNCTION " at [1.9] nc=" << nc;
  // phase 3) goto target
  CCell* cur = board_.at(user_x_, user_y_);
  //qDebug() << FUNCTION " at [2] cur->get_value()=" << cur->get_value();
  int a_cur = cur->a_;

  QString lurd;

  LURD direction = lurd_none;
  LURD direct2;
  int steps = 0;

  while ((user_x_ != x_dest) || (user_y_ != y_dest)) {
    CCell* tmp_l = cur->get_left();
    CCell* tmp_t = cur->get_top();
    CCell* tmp_r = cur->get_right();
    CCell* tmp_b = cur->get_bottom();
    CCell* old = cur;
    if (tmp_l && (tmp_l->a_ == a_cur - 1)) {
      //qDebug() << FUNCTION " at [2.2] (left)";
      cur = tmp_l;
      --user_x_;
      --a_cur;
      direct2 = lurd_l;
    } else if (tmp_t && (tmp_t->a_ == a_cur - 1)) {
      //qDebug() << FUNCTION " at [2.3] (top)";
      cur=tmp_t;
      --user_y_;
      --a_cur;
      direct2=lurd_u;
    } else if (tmp_r && (tmp_r->a_ == a_cur - 1)) {
      //qDebug() << FUNCTION " at [2.4] (right)";
      cur = tmp_r;
      ++user_x_;
      --a_cur;
      direct2 = lurd_r;
    } else if (tmp_b && (tmp_b->a_ == a_cur - 1)) {
      //qDebug() << FUNCTION " at [2.5] (bottom)";
      cur = tmp_b;
      ++user_y_;
      --a_cur;
      direct2 = lurd_d;
    } else {
      //qDebug() << FUNCTION " at [2] unable to get to destination";
      return false;
    }
    old->remove_user();
    cur->add_user();
    if (direction == direct2) {
      ++steps;
    } else {
      if (steps) {
        QString tmp;
        tmp.sprintf("%d%c",steps,(char)direction);
        lurd += tmp;
      }
      steps = 1;
      direction = direct2;
    }
    // TODO: animation speed
    emit repaint();
  }
  if (steps) {
    QString tmp;
    tmp.sprintf("%d%c", steps, static_cast<char>(direction));
    lurd += tmp;
  }
  //qDebug() << FUNCTION " at [3] all OK";
  // todo: no animation mode
  // create move
  CMove mov(CMove::T_run, x0, y0, x_dest, y_dest);
  mov.set_lurd(lurd);
  if (cur_idx_ < moves_.size()) {
    moves_.replace(cur_idx_, mov);
    moves_ = moves_.mid(0, cur_idx_ + 1);
    solution_exists_ = false;
  } else {
    moves_.push_back(mov);
  }
  ++cur_idx_;
  //qDebug() << FUNCTION " at [4] all OK";
  emit repaint();
  return res;
#undef FUNCTION
}

// --------- CPlayBoard::pushTo ---------
bool CPlayBoard::pushTo(int x_dest, int y_dest)
{
#define FUNCTION "CPlayBoard::pushTo"
  bool res = false;
  int x0 = user_x_;
  int y0 = user_y_;
  int dx = 0;
  int dy = 0;
  LURD direction = lurd_none;
  if (user_x_ == x_dest) {
    // vert
    if (y_dest < user_y_) {
      // up
      res = true;
      dy = -1;
      direction = lurd_u;
    }
    if (y_dest > user_y_) {
      // down
      res = true;
      dy = 1;
      direction=lurd_d;
    }
  }
  if (user_y_ == y_dest) {
    // horiz
    if (x_dest < user_x_) {
      // left
      res = true;
      dx = -1;
      direction=lurd_l;
    }
    if (x_dest > user_x_) {
      // right
      res = true;
      dx = 1;
      direction = lurd_r;
    }
  }
  if (!res) return res;
  CCell* cur = board_.at(user_x_, user_y_);

  int steps = 0;

  int x_box0 = -1;
  int y_box0 = -1;

  while ((user_x_ != x_dest) || (user_y_ != y_dest)) {
    CCell* next = board_.at(user_x_ + dx, user_y_ + dy);
    if (next->is_free()) {
      cur->remove_user();
      next->add_user();
      user_x_ += dx;
      user_y_ += dy;
      cur = next;
      ++steps;
    } else if (next->is_wall()) {
      break;
    } else if ((next->is_box())) {
      if (x_box0 < 0) {
        x_box0 = user_x_ + dx;
        y_box0=user_y_+dy;
      }
      CCell* next2 = board_.at(user_x_ + dx + dx, user_y_ + dy + dy);
      if (next2->is_free()) {
        next->remove_box();
        next2->add_box();
        cur->remove_user();
        next->add_user();
        cur = next;
        ++steps;
      } else {
        break;
      }
    }
    emit repaint();
  }

  if (steps) {
    CMove mov(CMove::T_push, x0, y0, user_x_, user_y_);
    mov.add_box(x_box0, y_box0, user_x_ + dx, user_y_ + dy);
    QString tmp;
    tmp.sprintf("%d%c", steps, static_cast<char>(direction));
    mov.set_lurd(tmp);
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_, mov);
      moves_ = moves_.mid(0, cur_idx_ + 1);
      solution_exists_ = false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    if (board_.isComplete()) {
      qDebug() << FUNCTION " level is complete!";
      save_level_info();
    }
    emit repaint();
    return true;
  } else
    return false;
#undef FUNCTION
}

// -------- CPlayBoard::aAt -----------
int CPlayBoard::aAt(int x, int y)
{
    return board_.at(x,y)->a_;
}

void CPlayBoard::handleRestartLevel()
{
#define FUNCTION "CPlayBoard::handleRestartLevel"
  qDebug() << FUNCTION " at [0]";
  moves_.clear();
  cur_idx_ = 0;
  solution_exists_ = false;
  board_ = board0_;
  for (int y = 0; y < board_.height(); ++y) {
    for (int x = 0; x < board_.width(); ++x) {
      if (board_.atRO(x, y)->is_user()) {
        user_x_ = x;
        user_y_ = y;
        emit repaint();
        return;
      }
    }
  }
  user_x_ = -1;
  user_y_ = -1;
  emit repaint();
#undef FUNCTION
}

// ---------- CPlayBoard::handleUndo ----------
void CPlayBoard::handleUndo()
{
#define FUNCTION "CPlayBoard::handleUndo"
  qDebug() << FUNCTION " at [0]";
  if (cur_idx_ == 0) return;
  CMove mov = moves_.at(--cur_idx_);

  int x_u1, x_u2, y_u1, y_u2;
  mov.get_user(x_u1, y_u1, x_u2, y_u2);
  CCell* user1 = board_.at(x_u1, y_u1);
  CCell* user2 = board_.at(x_u2, y_u2);
  user2->remove_user();
  user_x_ = x_u1;
  user_y_ = y_u1;
  CMove::Type t = mov.get_type();
  if (t == CMove::T_push) {
    int x_b1, x_b2, y_b1, y_b2;
    mov.get_box(x_b1, y_b1, x_b2, y_b2);
    CCell* box1 = board_.at(x_b1, y_b1);
    CCell* box2 = board_.at(x_b2, y_b2);
    box2->remove_box();
    box1->add_box();
  }
  user1->add_user();
  emit repaint();
#undef FUNCTION
}

void CPlayBoard::handleMoveRight()
{
#define FUNCTION "CPlayBoard::handleMoveRight"
  qDebug() << FUNCTION " at [0]";
  CCell* c1 = board_.at(user_x_,user_y_);
  CCell* c2=c1->get_right();
  if (!c2) {
    qDebug() << FUNCTION " return at [1]";
    return;
  }
  if (c2->is_free()) {
    // goto
    CMove mov(CMove::T_run, user_x_, user_y_, user_x_+1, user_y_);
    mov.set_lurd(QString("1r"));
    c1->remove_user();
    c2->add_user();
    ++user_x_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_,mov);
      moves_ = moves_.mid(0,cur_idx_+1);
      solution_exists_ = false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    //moves_.push_back(mov);
    qDebug() << FUNCTION " return at [2]";
    emit repaint();
    return;
  }
  if (!(c2->is_box())) {
    qDebug() << FUNCTION " return at [3]";
    return;
  }
  CCell* c3 = c2->get_right();
  if (!c3) {
    qDebug() << FUNCTION " return at [4]";
  }
  if (c3->is_free()) {
    // push
    CMove mov(CMove::T_push, user_x_, user_y_, user_x_ + 1, user_y_);
    mov.add_box(user_x_+1, user_y_, user_x_+2, user_y_);
    mov.set_lurd(QString("1r"));
    c2->remove_box();
    c3->add_box();
    c1->remove_user();
    c2->add_user();
    ++user_x_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_, mov);
      moves_ = moves_.mid(0, cur_idx_ + 1);
      solution_exists_ = false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    if (board_.isComplete()) {
      qDebug() << FUNCTION " level is complete!";
      save_level_info();
    }
    qDebug() << FUNCTION " return at [5]";
    emit repaint();
    return;
  } 
  qDebug() << FUNCTION " return at [6]";
#undef FUNCTION
}

void CPlayBoard::handleMoveLeft()
{
#define FUNCTION "CPlayBoard::handleMoveLeft"
  qDebug() << FUNCTION " at [0]";
  CCell* c1 = board_.at(user_x_, user_y_);
  CCell* c2 = c1->get_left();
  if (!c2) {
    return;
  }
  if (c2->is_free()) {
    // goto
    CMove mov(CMove::T_run, user_x_, user_y_, user_x_ - 1, user_y_);
    mov.set_lurd(QString("1l"));
    c1->remove_user();
    c2->add_user();
    --user_x_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_, mov);
      moves_=moves_.mid(0, cur_idx_ + 1);
      solution_exists_ = false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    emit repaint();
    return;
  }
  if (!(c2->is_box())) {
    qDebug() << FUNCTION " return at [3]";
    return;
  }
  CCell* c3 = c2->get_left();
  if (!c3) {
    qDebug() << FUNCTION " return at [4]";
  }
  if (c3->is_free()) {
    // push
    CMove mov(CMove::T_push, user_x_, user_y_, user_x_ - 1, user_y_);
    mov.add_box(user_x_-1,user_y_,user_x_-2,user_y_);
    mov.set_lurd(QString("1l"));
    c2->remove_box();
    c3->add_box();
    c1->remove_user();
    c2->add_user();
    --user_x_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_,mov);
      moves_=moves_.mid(0,cur_idx_+1);
      solution_exists_=false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    if (board_.isComplete()) {
      qDebug() << FUNCTION " level is complete!";
      save_level_info();
    }
    qDebug() << FUNCTION " return at [5]";
    emit repaint();
    return;
  }
  qDebug() << FUNCTION " return at [6]";
#undef FUNCTION
}

void CPlayBoard::handleMoveDown()
{
#define FUNCTION "CPlayBoard::handleMoveDown"
  qDebug() << FUNCTION " at [0]";
  CCell* c1 = board_.at(user_x_,user_y_);
  CCell* c2 = c1->get_bottom();
  if (!c2) {
    qDebug() << FUNCTION " return at [1]";
    return;
  }
  if (c2->is_free()) {
    // goto
    CMove mov(CMove::T_run, user_x_, user_y_, user_x_, user_y_ + 1);
    mov.set_lurd(QString("1d"));
    c1->remove_user();
    c2->add_user();
    ++user_y_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_,mov);
      moves_=moves_.mid(0,cur_idx_+1);
      solution_exists_=false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    if (board_.isComplete()) {
      qDebug() << FUNCTION " level is complete!";
      save_level_info();
    }
    qDebug() << FUNCTION " return at [2]";
    emit repaint();
    return;
  }
  if (!(c2->is_box())) {
    qDebug() << FUNCTION " return at [3]";
    return;
  }
  CCell* c3 = c2->get_bottom();
  if (!c3) {
    qDebug() << FUNCTION " return at [4]";
  }
  if (c3->is_free()) {
    // push
    CMove mov(CMove::T_push, user_x_, user_y_, user_x_, user_y_ + 1);
    mov.add_box(user_x_, user_y_+1, user_x_, user_y_ + 2);
    mov.set_lurd(QString("1d"));
    c2->remove_box();
    c3->add_box();
    c1->remove_user();
    c2->add_user();
    ++user_y_;
    if (cur_idx_ < moves_.size()) {
      moves_.replace(cur_idx_, mov);
      moves_=moves_.mid(0, cur_idx_ + 1);
      solution_exists_ = false;
    } else {
      moves_.push_back(mov);
    }
    ++cur_idx_;
    //moves_.push_back(mov);
    if (board_.isComplete()) {
      qDebug() << FUNCTION " level is complete!";
      save_level_info();
    }
    qDebug() << FUNCTION " return at [5]";
    emit repaint();
    return;
  }
  qDebug() << FUNCTION " return at [6]";
#undef FUNCTION
}

void CPlayBoard::handleMoveUp()
{
#define FUNCTION "CPlayBoard::handleMoveUp"
  qDebug() << FUNCTION " at [0]";
  CCell* c1=board_.at(user_x_,user_y_);
  CCell* c2=c1->get_top();
  if (!c2) {
    qDebug() << FUNCTION " return at [1]";
    return;
  }
  if (c2->is_free()) {
    // goto
	CMove mov(CMove::T_run,user_x_,user_y_,user_x_,user_y_-1);
	mov.set_lurd(QString("1u"));
	c1->remove_user();
	c2->add_user();
	--user_y_;
        if (cur_idx_ < moves_.size()) {
    	    moves_.replace(cur_idx_,mov);
	    moves_=moves_.mid(0,cur_idx_+1);
	    solution_exists_=false;
	} else {
	    moves_.push_back(mov);
	}
	++cur_idx_;
	//moves_.push_back(mov);
	qDebug() << FUNCTION " return at [2]";
	emit repaint();
	return;
    }
    if (!(c2->is_box())) {
	qDebug() << FUNCTION " return at [3]";
	return;
    }
    CCell *c3=c2->get_top();
    if (!c3) {
	qDebug() << FUNCTION " return at [4]";
    }
    if (c3->is_free()) {
	// push
	CMove mov(CMove::T_push,user_x_,user_y_,user_x_,user_y_-1);
	mov.add_box(user_x_,user_y_-1,user_x_,user_y_-2);
	mov.set_lurd(QString("1u"));
	c2->remove_box();
	c3->add_box();
	c1->remove_user();
	c2->add_user();
	--user_y_;
        if (cur_idx_ < moves_.size()) {
    	    moves_.replace(cur_idx_,mov);
	    moves_=moves_.mid(0,cur_idx_+1);
	    solution_exists_=false;
	} else {
	    moves_.push_back(mov);
	}
	++cur_idx_;
	//moves_.push_back(mov);
	if (board_.isComplete()) {
	    qDebug() << FUNCTION " level is complete!";
	    save_level_info();
	}
	qDebug() << FUNCTION " return at [5]";
	emit repaint();
	return;
    } 
    qDebug() << FUNCTION " return at [6]";
    
#undef FUNCTION
}

// ---------- CPlayBoard::get_level_info --------
void CPlayBoard::get_level_info()
{
#define FUNCTION "CPlayBoard::get_level_info"
  QCryptographicHash hash(QCryptographicHash::Sha1);
  //CBoard board_;
  hash.addData(board_.asQByteArray());
  QByteArray res = hash.result();
  QString s;
  for (int j = 0; j < res.size(); ++j) {
    QString tmp;
    tmp.sprintf("%02x",res.at(j)&0xFF);
    s += tmp;
  }
  qDebug() << FUNCTION " hash: " << s;
  signature_ = s;

  QString path_home(CRelatedDirs::get_dir(CRelatedDirs::DI_home));

  label_path_ = path_home;
  label_path_ += signature_;
  qDebug() << FUNCTION " label_path: " << label_path_;
  QFile file(label_path_);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly);
    for (;;) {
      QByteArray in(file.readLine());
      s = in;
      if (s.startsWith("R") || s.startsWith("P")) {
        moves_.push_back(CMove::fromString(s));
      } else {
        if (s.startsWith("<<<complete>>>")) {
          solution_exists_ = true;
        }
        break;
      }
    }
    file.close();
  }
  qDebug() << FUNCTION " moves_.size()=" << moves_.size();
#undef FUNCTION
}

// --------- CPlayBoard::save_level_info ---------
void CPlayBoard::save_level_info()
{
  QFile file(label_path_);
  file.open(QIODevice::WriteOnly);
  //QString s;
  QList<CMove>::const_iterator i;
  for (i = moves_.constBegin(); i != moves_.constEnd(); ++i) {
    file.write((*i).toQByteArray());
  }
  if (board_.isComplete()) {
    file.write("<<<complete>>>\n");
    solution_exists_=true;
  }
  file.close();
}

// ------- CPlayBoard::get_moves_size --------
int CPlayBoard::get_moves_size() const
{
  return moves_.size();
}

// ------- CPlayBoard::get_cur_move ----------
int CPlayBoard::get_cur_move() const
{
  return cur_idx_;
}

// ------- CPlayBoard::handleRedo ---------
void CPlayBoard::handleRedo()
{
#define FUNCTION "CPlayBoard::handleRedo"
  qDebug() << FUNCTION " at [0]";
  if (cur_idx_ >= moves_.size()) {
    qDebug() << FUNCTION " no redo, return";
    return;
  }
  CMove mov = moves_.at(cur_idx_);
  applyMove(mov);
#undef FUNCTION
}

// ------- CPlayBoard::applyMove -------
void CPlayBoard::applyMove(const CMove &mov)
{
#define FUNCTION "CPlayBoard::applyMove"
  qDebug() << FUNCTION " at [0]";
  int x1, y1, x2, y2;
  mov.get_user(x1, y1, x2, y2);
  board_.at(x1, y1)->remove_user();

  if (mov.get_type() == CMove::T_push) {
    int x1, y1, x2, y2;
    mov.get_box(x1, y1, x2, y2);
    board_.at(x1, y1)->remove_box();
    board_.at(x2, y2)->add_box();
  }
  board_.at(x2, y2)->add_user();
  user_x_ = x2;
  user_y_ = y2;
  ++cur_idx_;
  repaint();
#undef FUNCTION
}


// --------- CPlayBoard::handleResolve ---------
void CPlayBoard::handleResolve()
{
#define FUNCTION "CPlayBoard::handleResolve"
  qDebug() << FUNCTION " at [0]";
#undef FUNCTION
}

// -------- CPlayBoard::get_all_lurds -------
QStringList CPlayBoard::get_all_lurds() const
{
#define FUNCTION "CPlayBoard::get_all_lurds"
  QStringList res;
  QList<CMove>::const_iterator i;
  for (i = moves_.constBegin(); i != moves_.constEnd(); ++i) {
    QString s = (*i).get_lurd();
    qDebug() << FUNCTION ": at [1] s=" << s;
    res.push_back(s);
  }
  return res;
#undef FUNCTION
}
