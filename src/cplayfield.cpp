#include <QDebug>
#include <cplayfield.h>
#include <cplayboard.h>
#include <ccell.h>
#include <QPainter>
#include <QPaintEvent>
#include <cpixmaps.h>

// ------- CPlayField::CPlayField --------
CPlayField::CPlayField(QWidget* parent)
  : QWidget(parent)
{
  CPlayBoard* pb = CPlayBoard::instance();
  number_of_levels_ = 0;
  curent_level_ = 0;
  cdxy_ = 0;
  QObject::connect(pb, SIGNAL(repaint()), this, SLOT(repaint()));
}

// ------ CPlayField::paintEvent ------
void CPlayField::paintEvent(QPaintEvent *e)
{
#define FUNCTION "CPlayField::paintEvent"
  QPainter paint(this);
  const int info_panel_height = 32;

  //paint.setBrushOrigin(0, 0);
  //if (empty_.width()==0) {
  //  reread_images();
  //}
  paint.setClipRegion(e->region());
  paint.setClipping(true);

  //paintPainter(paint, e->rect());
  //paint.drawText(QPointF(10,10),"Hello");
  int w = width();
  int h = height() - info_panel_height;

  CPixmap* outerspace = CPixmaps::instance()->get_image(CPlayField::PIX_outerspace, -1);
  int w1 = outerspace->width();
  int h1 = outerspace->height();
  for (int y = 0; y < height(); y += h1)
    for (int x = 0; x < w; x += w1) {
      paint.drawPixmap(x, y, *outerspace);
    }

  dx_ = w;
  dy_ = h;
  CPlayBoard* pb = CPlayBoard::instance();
  //
  int c_dx = pb->width();
  int c_dy = pb->height();
  nw_ = c_dx;
  nh_ = c_dy;
  int kx = w / c_dx;
  int ky = h / c_dy;
  if (kx < ky) ky = kx;
  if (ky < kx) kx = ky;
  int x_offset = (w - c_dx * kx) / 2;
  int y_offset = (h - c_dy * ky) / 2;
  x_off_ = x_offset;
  y_off_ = y_offset;
  cdxy_ = kx;
  for (int y = 0; y < c_dy; ++y) {
    for (int x = 0; x < c_dx; ++x) {
      int xx = x_offset + x * kx;
      int yy = y_offset + y * ky;
      //char c = pb->at(x, y)->get_value_c();
      //qDebug() << FUNCTION << " at [2] x=" << x << " y=" << y << " c=" << c;
      CPixmap* p = pb->at(x, y)->getPixmap(cdxy_);
      if (p) paint.drawPixmap(xx, yy, *p);
    }
  }
  //paint.drawText(QPointF(w/2,h/2),"Hello");
  // <levelset name> : <level number>/<total> (name)
  paint.setFont(QFont(QString("Helvetica"), 26));

  paint.setPen(QColor(0, 255, 0));
  QString s(collection_name_);
  s += ":";
  QString tmp;
  tmp.sprintf("%d/%d", curent_level_ + 1, number_of_levels_);
  s +=  tmp;
  if (level_name_.size()) {
    s+=" (";
    s+=level_name_;
    s+=")";
  }
  paint.drawText(QPointF(5, y_off_ + nh_ * cdxy_ + info_panel_height - 6), s);

  paint.setPen(QColor(255, 0, 0));
  tmp.sprintf("%d/%d ", pb->get_cur_move(), pb->get_moves_size());
  int text_w = paint.fontMetrics().width(tmp);

  int x0 = w - text_w;
  int y0 = y_off_ + nh_ * cdxy_ + info_panel_height - 6;
  paint.drawText(QPointF(x0, y0), tmp);

  tmp = "Moves:";
  x0 -= paint.fontMetrics().width(tmp);
  paint.setPen(QColor(127, 127, 127));
  paint.drawText(QPointF(x0, y0), tmp);

  if (pb->does_solution_exist()) {
    paint.setPen(QColor(255, 255, 0));
    tmp = "Resolved ";
    x0 -= paint.fontMetrics().width(tmp);
    paint.drawText(QPointF(x0, y0), tmp);
  }
#undef FUNCTION
}

// -------- CPlayField::keyPressEvent ----------
void CPlayField::keyPressEvent (QKeyEvent *)
{
#define FUNCTION "CPlayField::keyPressEvent"
  qDebug() << FUNCTION " at [0]";
#undef FUNCTION
}

// ------- CPlayField::mousePressEvent --------
void CPlayField::mousePressEvent (QMouseEvent *e)
{
#define FUNCTION "CPlayField::mousePressEvent"
  Qt::MouseButton bt = e->button();
  int x = e->x();
  int y = e->y();
  int nx = -1;
  int ny = -1;
  if (bt == Qt::LeftButton) {
    reCompute(x, y, nx, ny);
    if ((nx >= 0) && (ny >= 0)) {
      //qDebug() << FUNCTION " at [1] left nx=" << nx << " ny=" << ny;
      CPlayBoard::instance()->goTo(nx, ny);
    } else {
      //qDebug() << FUNCTION " at [1.1] left nx=" << nx << " ny=" << ny;
    }
  } else if (bt == Qt::RightButton) {
    reCompute(x, y, nx, ny);
    if ((nx >= 0) && (ny >= 0)) {
      //qDebug() << FUNCTION " at [2] left nx=" << nx << " ny=" << ny;
      CPlayBoard::instance()->pushTo(nx, ny);
    } else {
      //qDebug() << FUNCTION " at [2.1] left nx=" << nx << " ny=" << ny;
    }
  }
#undef FUNCTION
}

// ---------- CPlayField::reCompute ----------
void CPlayField::reCompute(int x, int y, int& nx, int& ny)
{
  if ((x >= x_off_) && (x < x_off_ + cdxy_ * nw_)) {
    nx=(x - x_off_) / cdxy_;
  } else {
    nx = -1;
  }
  if ((y >= y_off_) && (y < y_off_ + cdxy_ * nh_)) {
    ny = (y - y_off_) / cdxy_;
  } else {
    ny = -1;
  }
}


// ---------- CPlayField::reread_images ----------
void CPlayField::reread_images()
{
  repaint();
}

// ------- CPlayField::setLevelParam ------
void CPlayField::setLevelParam(const QString &lsname, int total, int n, const QString &name)
{
#define FUNCTION "CPlayField::setLevelParam"
  qDebug() << FUNCTION " at [0] lsname=" << lsname << " total=" << total << " n=" << n << " name=" << name;
  collection_name_ = lsname;
  level_name_ = name;
  number_of_levels_ = total;
  curent_level_ = n;
#undef FUNCTION
}
