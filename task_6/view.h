#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QApplication>
#include "RiemannSolver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class View; class Controller; }
QT_END_NAMESPACE

class View : public QMainWindow, public RiemannSolver
{
    Q_OBJECT
    friend class TestView;
public:
    View(QWidget *parent = nullptr);
    void Rendering( const RiemannSolver &);
    void Rendering( const QVector<double> &,const QVector<double> &,const QVector<double> &,const QVector<double> &,const QVector<double> &);
    ~View();

private slots:
    void on_Calculate_clicked();

    void on_Play_clicked();

    void on_Right_clicked();

    void on_Left_clicked();

    void on_Stop_clicked();

private:
    Ui::View *ui;
    double time;
    bool flag = true;
};
#endif // VIEW_H
