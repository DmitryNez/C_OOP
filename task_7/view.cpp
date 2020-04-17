#include "view.h"
#include "ui_view.h"
#include <QVector>

view::view(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::view)
{
    ui->setupUi(this);
}

view::~view()
{
    delete ui;
}


void view::on_pushButton_clicked()
{
    double a = ui->lineEditA->text().toDouble(); // coefficient before x*x
    double b = ui->lineEditB->text().toDouble(); // coefficient before x
    double c = ui->lineEditC->text().toDouble(); // free coefficient
    int n = 20 , k = 0;
    QVector<double> x,y;

    for(double i = -n/2; i <= n/2; i+=0.1){
        x.push_back(i);
        y.push_back(a*pow(x[k],2) + b*x[k] + c);
        k++;
    }

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");


    ui->widget->xAxis->setRange(-10, 10);

    ui->widget->yAxis->setRange(*std::min_element(y.begin(),y.end()) - 5,*std::min_element(y.begin(),y.end()) + 20);

    ui->widget->replot();
}


