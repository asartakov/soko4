#include <cmove.h>

CMove::CMove(Type t, int x1, int y1, int x2, int y2)
    :m_ux1(x1)
    ,m_uy1(y1)
    ,m_ux2(x2)
    ,m_uy2(y2)
    ,m_t(t)
{
}

void CMove::add_box(int x1, int y1, int x2, int y2)
{
    m_bx1=x1;
    m_by1=y1;
    m_bx2=x2;
    m_by2=y2;
}

void CMove::get_box(int & x1, int & y1, int & x2, int & y2) const
{
    x1=m_bx1;
    y1=m_by1;
    x2=m_bx2;
    y2=m_by2;
}

void CMove::get_user(int & x1, int & y1, int & x2, int & y2) const
{
    x1=m_ux1;
    y1=m_uy1;
    x2=m_ux2;
    y2=m_uy2;
}

CMove::Type CMove::get_type() const
{
    return m_t;
}

QByteArray CMove::toQByteArray() const
{
  QString s;
  if (m_t==T_run) {
    s.sprintf("R,%d,%d,%d,%d,%s\n", m_ux1, m_uy1, m_ux2, m_uy2, m_lurd.toUtf8().data());
  } else {
    s.sprintf("P,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", m_ux1, m_uy1, m_ux2, m_uy2, m_bx1,
      m_by1, m_bx2, m_by2, m_lurd.toUtf8().data());
  }
  return s.toUtf8();
}

// --------- CMove::fromString ---------
CMove CMove::fromString(const QString&s)
{
#define FUNCTION "CMove::fromString"
    qDebug() << FUNCTION ": at [0] s=" << s;
    QStringList list=s.split(',');
    int x1=list.at(1).toInt();
    int y1=list.at(2).toInt();
    int x2=list.at(3).toInt();
    int y2=list.at(4).toInt();
    qDebug() << FUNCTION ": at [1] x1=" << x1 << " y1=" << y1 << " x2=" << x2 << " y2=" << y2;
    if (s.startsWith("R")) {
	QString tmp;
	if (list.size()==6)
	    tmp=list.at(5);
	qDebug() << FUNCTION ": at [2] tmp=" << tmp;
	tmp.remove("\0x0D");
	tmp.remove("\0x0A");
	qDebug() << FUNCTION ": at [3] tmp=" << tmp;
	CMove mov(T_run,x1,y1,x2,y2);
	if (tmp.size())
	    mov.set_lurd(tmp);
	else
	    mov.generate_lurd();
	return mov;
    }
    int x3=list.at(5).toInt();
    int y3=list.at(6).toInt();
    int x4=list.at(7).toInt();
    int y4=list.at(8).toInt();
    CMove res(T_push,x1,y1,x2,y2);
    res.add_box(x3,y3,x4,y4);
    QString tmp;
    if (list.size()==10)
	tmp=list.at(9);
    tmp.remove("\0x0D");
    tmp.remove("\0x0A");
    if (tmp.size())
	res.set_lurd(tmp);
    else
	res.generate_lurd();
    return res;
#undef FUNCTION
}

// ---------- CMove::generate_lurd --------
void CMove::generate_lurd()
{
    if (m_t==T_run) {
    } else {
    }
}
