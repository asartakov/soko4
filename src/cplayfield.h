#ifndef __CPLAYFIELD_H__
#define __CPLAYFIELD_H__

#include <QWidget>

class CPlayField : public QWidget
{
Q_OBJECT
 public:
  enum PIX {
    PIX_outerspace     = 0x01,
    PIX_empty          = 0x02,
    PIX_user           = 0x03,
    PIX_user_on_target = 0x04,
    PIX_box            = 0x05,
    PIX_box_on_target  = 0x06,
    PIX_wall           = 0x08,
    PIX_target         = 0x07
  };
  CPlayField(QWidget* parent);

 public Q_SLOTS:
  void reread_images();
  void setLevelParam(const QString&, int, int, const QString&);
  //void repaint();

 protected:
  void paintEvent(QPaintEvent*);
  void keyPressEvent(QKeyEvent*);
  void mousePressEvent(QMouseEvent*);

 private:
  void reCompute(int, int, int&, int&);

  int nw_, nh_; // in cells
  int dx_, dy_; // by pixels
  int x_off_, y_off_; // in pixels
  int cdxy_; // in pixels
  QString collection_name_;
  QString level_name_;
  int number_of_levels_;
  int curent_level_;
};

#endif
