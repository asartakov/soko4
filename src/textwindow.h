#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QDialog>

namespace Ui
{
    class TextWindow;
}

class CTextWindow : public QDialog
{
    Q_OBJECT

public:
    CTextWindow(QWidget *parent = 0);
    ~CTextWindow();
public slots:
    void handleGet(bool);
    void handleGetSol(bool);
    void handleSave(bool);
    void handlePlay(bool);
signals:
    void closing();
    void repaint();
private:
    Ui::TextWindow *ui;
protected:
    void closeEvent(QCloseEvent *);
};

#endif // TEXTWINDOW_H
