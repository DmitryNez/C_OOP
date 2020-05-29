#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent): QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}

void View::Rendering(const RiemannSolver & solver){
    ui->DensityPlot->clearGraphs();
    ui->DensityPlot->addGraph();
    ui->DensityPlot->graph(0)->setData(solver.Position,solver.rho);
    ui->DensityPlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->DensityPlot->yAxis->setRange(*std::min_element(solver.rho.begin(),solver.rho.end())
                                     ,*std::max_element(solver.rho.begin(),solver.rho.end()));
    ui->DensityPlot->replot();


    ui->PressurePlot->clearGraphs();
    ui->PressurePlot->addGraph();
    ui->PressurePlot->graph(0)->setData(solver.Position,solver.Pressure);
    ui->PressurePlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->PressurePlot->yAxis->setRange(*std::min_element(solver.Pressure.begin(),solver.Pressure.end())
                                     ,*std::max_element(solver.Pressure.begin(),solver.Pressure.end()));
    ui->PressurePlot->replot();

    ui->TemperaturePlot->clearGraphs();
    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(0)->setData(solver.Position,solver.Temperature);
    ui->TemperaturePlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->TemperaturePlot->yAxis->setRange(*std::min_element(solver.Temperature.begin(),solver.Temperature.end()) - 0.1
                                     ,*std::max_element(solver.Temperature.begin(),solver.Temperature.end()));
    ui->TemperaturePlot->replot();


    ui->VelocityPlot->clearGraphs();
    ui->VelocityPlot->addGraph();
    ui->VelocityPlot->graph(0)->setData(solver.Position,solver.Velocity);
    ui->VelocityPlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->VelocityPlot->yAxis->setRange(*std::min_element(solver.Velocity.begin(),solver.Velocity.end())
                                     ,*std::max_element(solver.Velocity.begin(),solver.Velocity.end()));
    ui->VelocityPlot->replot();
}

void View::Rendering( const QVector<double> & Pos,const QVector<double> & P,const QVector<double> &U,
                      const QVector<double> & T,const QVector<double> &D){
    ui->DensityPlot->clearGraphs();
    ui->DensityPlot->addGraph();
    ui->DensityPlot->graph(0)->setData(Pos,D);
    ui->DensityPlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->DensityPlot->yAxis->setRange(*std::min_element(D.begin(),D.end())
                                     ,*std::max_element(D.begin(),D.end()));
    ui->DensityPlot->replot();


    ui->PressurePlot->clearGraphs();
    ui->PressurePlot->addGraph();
    ui->PressurePlot->graph(0)->setData(Pos,P);
    ui->PressurePlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->PressurePlot->yAxis->setRange(*std::min_element(P.begin(),P.end())
                                     ,*std::max_element(P.begin(),P.end()));
    ui->PressurePlot->replot();

    ui->TemperaturePlot->clearGraphs();
    ui->TemperaturePlot->addGraph();
    ui->TemperaturePlot->graph(0)->setData(Pos,T);
    ui->TemperaturePlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->TemperaturePlot->yAxis->setRange(*std::min_element(T.begin(),T.end()) - 0.1
                                     ,*std::max_element(T.begin(),T.end()));
    ui->TemperaturePlot->replot();


    ui->VelocityPlot->clearGraphs();
    ui->VelocityPlot->addGraph();
    ui->VelocityPlot->graph(0)->setData(Pos,U);
    ui->VelocityPlot->xAxis->setRange(ui->Lmin->text().toDouble(), ui->Lmax->text().toDouble());
    ui->VelocityPlot->yAxis->setRange(*std::min_element(U.begin(),U.end())
                                     ,*std::max_element(U.begin(),U.end()));
    ui->VelocityPlot->replot();
}
void View::on_Calculate_clicked()
{

    double uL = ui->VelocityLeft->text().toDouble();
    double uR = ui->VelocityRight->text().toDouble();
    double pL = ui->PressureLeft->text().toDouble();
    double pR = ui->PressureRight->text().toDouble();
    double dR = ui->DensityRight->text().toDouble();
    double dL = ui->DensityLeft->text().toDouble();
    int Nx = ui->Nx->text().toInt();
    double leght = ui->Lmax->text().toDouble() - ui->Lmin->text().toDouble();

    RiemannSolver solver(uR,uL,pR,pL,dR,dL,Nx,leght,"data.txt");
    time = state.timeout;
    solver.Solve(0);

    Rendering(solver);

}

void View::on_Play_clicked()
{
    double uL = ui->VelocityLeft->text().toDouble();
    double uR = ui->VelocityRight->text().toDouble();
    double pL = ui->PressureLeft->text().toDouble();
    double pR = ui->PressureRight->text().toDouble();
    double dR = ui->DensityRight->text().toDouble();
    double dL = ui->DensityLeft->text().toDouble();
    int Nx = ui->Nx->text().toInt();
    double length = ui->Lmax->text().toDouble() - ui->Lmin->text().toDouble();

    RiemannSolver solver(uR,uL,pR,pL,dR,dL,Nx,length,"data.txt");

    solver.Solve(1);
    ui->Stop->setEnabled(true);
    time = 0;
    flag = true;
    double dt = solver.GetState().dt;
    double timeout = solver.GetState().timeout;
    int N = timeout/dt;

    for(int i = 0; i < N - 1; i++){

        QVector<double> Pos(Nx);
        QVector<double> Press(Nx);
        QVector<double> Temp(Nx);
        QVector<double> Vel(Nx);
        QVector<double> Den(Nx);

        for(int j = 0; j < Nx; j++){
            Pos[j] = solver.Position[j + i*Nx];
            Press[j] = solver.Pressure[j + i*Nx];
            Temp[j] = solver.Temperature[j + i*Nx];
            Vel[j] = solver.Velocity[j + i*Nx];
            Den[j] = solver.rho[j + i*Nx];
        }

        time = i*dt;
        Rendering(Pos,Press,Vel,Temp,Den);
        QCoreApplication::processEvents(QEventLoop::AllEvents);

        if(flag == false){
            break;
        }
    }
}

void View::on_Right_clicked()
{

    double uL = ui->VelocityLeft->text().toDouble();
    double uR = ui->VelocityRight->text().toDouble();
    double pL = ui->PressureLeft->text().toDouble();
    double pR = ui->PressureRight->text().toDouble();
    double dR = ui->DensityRight->text().toDouble();
    double dL = ui->DensityLeft->text().toDouble();
    int Nx = ui->Nx->text().toInt();
    double leght = ui->Lmax->text().toDouble() - ui->Lmin->text().toDouble();

    RiemannSolver solver(uR,uL,pR,pL,dR,dL,Nx,leght,"data.txt");

    solver.Solve(1);
    double dt = state.dt;
    double timeout = state.timeout;


    time+=dt;
    int i = time/dt - 1;

    QVector<double> Pos(Nx);
    QVector<double> Press(Nx);
    QVector<double> Temp(Nx);
    QVector<double> Vel(Nx);
    QVector<double> Den(Nx);

    if (time >= timeout){
        ui->Right->setEnabled(false);
        time = timeout;
        return;
    }
    if(time > 0){
        ui->Left->setEnabled(true);
    }

    for(int j = 0; j < Nx; j++){
        Pos[j] = solver.Position[j + i*Nx];
        Press[j] = solver.Pressure[j + i*Nx];
        Temp[j] = solver.Temperature[j + i*Nx];
        Vel[j] = solver.Velocity[j + i*Nx];
        Den[j] = solver.rho[j + i*Nx];
    }

    Rendering(Pos,Press,Vel,Temp,Den);


}

void View::on_Left_clicked()
{
    double uL = ui->VelocityLeft->text().toDouble();
    double uR = ui->VelocityRight->text().toDouble();
    double pL = ui->PressureLeft->text().toDouble();
    double pR = ui->PressureRight->text().toDouble();
    double dR = ui->DensityRight->text().toDouble();
    double dL = ui->DensityLeft->text().toDouble();
    int Nx = ui->Nx->text().toInt();
    double leght = ui->Lmax->text().toDouble() - ui->Lmin->text().toDouble();

    RiemannSolver solver(uR,uL,pR,pL,dR,dL,Nx,leght,"data.txt");

    solver.Solve(1);
    double dt = state.dt;
    double timeout = state.timeout;
    time-=dt;
    int i = time/dt - 1;

    QVector<double> Pos(Nx);
    QVector<double> Press(Nx);
    QVector<double> Temp(Nx);
    QVector<double> Vel(Nx);
    QVector<double> Den(Nx);

    if (time < 0){
        ui->Left->setEnabled(false);
        return;
    }
    if(time < timeout){
        ui->Right->setEnabled(true);
    }

    for(int j = 0; j < Nx; j++){
        Pos[j] = solver.Position[j + i*Nx];
        Press[j] = solver.Pressure[j + i*Nx];
        Temp[j] = solver.Temperature[j + i*Nx];
        Vel[j] = solver.Velocity[j + i*Nx];
        Den[j] = solver.rho[j + i*Nx];
    }

    Rendering(Pos,Press,Vel,Temp,Den);

}


void View::on_Stop_clicked()
{
    flag = false;
}

