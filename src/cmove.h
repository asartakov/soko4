#ifndef __CMOVE_H__
#define __CMOVE_H__

#include <cboard.h>
#include <QByteArray>

class CMove
{
public:
    enum Type {
	T_run,
	T_push
    };
    CMove(Type,int,int,int,int);
    void add_box(int,int,int,int);
    //bool apply(CBoard&);
    void get_box(int&,int&,int&,int&) const;
    void get_user(int&,int&,int&,int&) const;
    QString get_lurd() const { return m_lurd;};
    Type get_type() const;
    QByteArray toQByteArray() const;
    static CMove fromString(const QString&);
    void set_lurd(const QString &s) { m_lurd=s;};
    void generate_lurd();
private:
    CMove();
    int m_ux1,m_uy1,m_ux2,m_uy2;
    int m_bx1,m_by1,m_bx2,m_by2;
    Type m_t;
    QString m_lurd;
};

#endif
