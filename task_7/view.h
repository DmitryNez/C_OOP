#ifndef VIEW_H
#define VIEW_H
#include<qcustomplot.h>
#include <QMainWindow>
#include "ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui { class view; }
QT_END_NAMESPACE

class view : public QMainWindow
{
    Q_OBJECT
    friend class TestView;
public:
    view(QWidget *parent = nullptr);
    ~view();

private slots:
    void on_pushButton_clicked();

private:
    Ui::view *ui;

};
#endif // VIEW_H
