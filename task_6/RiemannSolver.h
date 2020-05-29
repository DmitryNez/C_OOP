#ifndef RIEMANNSOLVER_H
#define RIEMANNSOLVER_H

#include <string>
#include <QVector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <qcustomplot.h>

struct States{
    double gamma;
    double Lenght;
    double Discontinuity;
    double Nx;
    double timeout;
    double dt;
    double MPa;
    double VarOnRightSide[4]; // density,velocity,pressure,speed sound
    double VarOnLeftSide[4];
    double VarOnStarZone[4];
    void SetFromFile(const std::string &);
};

struct Gammas{
    double gamma;
    double gamma0;
    double gamma1;
    double gamma2;
    double gamma3;
    double gamma4;
    double gamma5;
    double gamma6;
    double gamma7;
};

class RiemannSolver
{
protected:
    States state;
    Gammas gammas;
public:
    QVector<double> Pressure;
    QVector<double> Velocity;
    QVector<double> rho;
    QVector<double> Temperature;
    QVector<double> Position;

    RiemannSolver();
    RiemannSolver(const double& uR,const double& uL,const double& pR,const double& pL,const double& rhoR,const double& rhoL,
                  const int Nx,const double Lenght,const std::string& filename);
    void GUESSP();
    void PREFUNL(double &F,double &FD);
    void PREFUNR(double &F,double &FD);
    void STARPU(double &P,double &U);
    void SAMPLE(double &P,double &U,double &D,double &S,double &T);
    void Solve(const bool flag);
    States GetState();
};

#endif // RIEMANNSOLVER_H
