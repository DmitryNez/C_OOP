#include "RiemannSolver.h"

void States::SetFromFile(const std::string &filename){

    std::ifstream input(filename);
    input >> gamma;
    std::cout << gamma << std::endl;
    input >> Discontinuity;
    std::cout << Discontinuity<< std::endl;
    input >> timeout;
    std::cout << timeout << std::endl;
    input >> dt;
}

States RiemannSolver::GetState(){
    return state;
}

RiemannSolver::RiemannSolver()
{
    for(int i = 0;i < 4;i++){
        state.VarOnLeftSide[i] = 0.0;
        state.VarOnRightSide[i] = 0.0;
        state.VarOnStarZone[i] = 0.0;
    }
    state.VarOnLeftSide[0] = 1.0;
    state.VarOnLeftSide[1] = 0.0;
    state.VarOnLeftSide[2] = 1.0;
    
    state.VarOnRightSide[0] = 0.125;
    state.VarOnRightSide[1] = 0.0;
    state.VarOnRightSide[2] = 0.1;
    
    state.gamma = 1.4;
    gammas.gamma = 1.4;
    
    state.Lenght = 1.0;
    
    state.Nx = 10000;
    
    state.Discontinuity = 0.5;
    
    state.MPa = 1.0;
    
    state.timeout = 0.2;

    state.dt = 0.00078125;
}

RiemannSolver::RiemannSolver(const double& uR,const double& uL,const double& pR,const double& pL,
                            const double& rhoR,const double& rhoL,const int Nx,const double Lenght,
                             const std::string& filename){
    state.Nx = Nx;
    state.MPa = 1.0;
    state.Lenght = Lenght;
    state.VarOnLeftSide[0] = rhoL;
    state.VarOnLeftSide[1] = uL;
    state.VarOnLeftSide[2] = pL;
    
    state.VarOnRightSide[0] = rhoR;
    state.VarOnRightSide[1] = uR;
    state.VarOnRightSide[2] = pR;
    
    state.SetFromFile(filename);
    gammas.gamma = state.gamma;
}

void RiemannSolver::GUESSP(){
    double PPV, PMIN,PMAX,QMAX,QUSER = 2.0,PQ,PTL,PTR,GEL,GER;
    
    double CUP = 0.25 *(state.VarOnLeftSide[0] + state.VarOnRightSide[0]) * (state.VarOnLeftSide[3] + state.VarOnRightSide[3]);
    PPV = 0.5 * (state.VarOnLeftSide[2] + state.VarOnRightSide[2]) + 
            0.5 * (state.VarOnLeftSide[1] - state.VarOnRightSide[1]) * CUP;
    PPV = std::fmax(0.0,PPV);
    PMIN = std::fmin(state.VarOnLeftSide[2],state.VarOnRightSide[2]);
    PMAX = std::fmax(state.VarOnLeftSide[2],state.VarOnRightSide[2]);
    QMAX = PMAX/PMIN;
    
    if(QMAX <= QUSER && (PMIN<=PPV && PPV<=PMAX)){
        state.VarOnStarZone[2] = PPV;
    }
    else if(PPV < PMIN){
        
        PQ = std::pow(state.VarOnLeftSide[2]/state.VarOnRightSide[2],gammas.gamma0);
        state.VarOnStarZone[1] = (PQ*state.VarOnLeftSide[1]/state.VarOnLeftSide[3] +
                state.VarOnRightSide[1]/state.VarOnRightSide[3] + gammas.gamma3*(PQ - 1))/
                (PQ/state.VarOnLeftSide[3] + 1.0/state.VarOnRightSide[3]);
        
        PTL = 1.0 + gammas.gamma6 * (state.VarOnLeftSide[1] - state.VarOnStarZone[1])/state.VarOnLeftSide[3];
        PTR = 1.0 + gammas.gamma6 * (state.VarOnStarZone[1] - state.VarOnRightSide[1])/state.VarOnRightSide[3];
        state.VarOnStarZone[2] = 0.5 * (std::pow(state.VarOnLeftSide[2]*PTL,gammas.gamma2)
                + std::pow(state.VarOnRightSide[2]*PTR,gammas.gamma2));
    }
    else{
        GEL = std::sqrt((gammas.gamma4/state.VarOnLeftSide[0])/(gammas.gamma5*state.VarOnLeftSide[2] + PPV));
        GER = std::sqrt((gammas.gamma4/state.VarOnRightSide[0])/(gammas.gamma5*state.VarOnRightSide[2] + PPV));
        state.VarOnStarZone[2] = (GEL * state.VarOnLeftSide[2] + GER * state.VarOnRightSide[2] - 
                (state.VarOnRightSide[1] - state.VarOnLeftSide[1]))/(GEL + GER);
    }
}

void RiemannSolver::PREFUNL(double &F, double &FD){
    double A, B, PRATL,QRTL;
    
    if(state.VarOnStarZone[2] < state.VarOnLeftSide[2]){
        PRATL = state.VarOnStarZone[2]/state.VarOnLeftSide[2];
        F = gammas.gamma3 * state.VarOnLeftSide[3] * (pow(PRATL,gammas.gamma0) - 1.0);
        FD = (1.0/(state.VarOnLeftSide[0]*state.VarOnLeftSide[3]))/pow(PRATL,gammas.gamma1);
    }
    else{
        A = gammas.gamma4/state.VarOnLeftSide[0];
        B = gammas.gamma5 * state.VarOnLeftSide[2];
        QRTL = std::sqrt(A/(B + state.VarOnStarZone[2]));
        F = (state.VarOnStarZone[2] - state.VarOnLeftSide[2]) * QRTL;
        FD = ( 1.0 - (state.VarOnStarZone[2] - state.VarOnLeftSide[2])/(2.0 * (B + state.VarOnStarZone[2])))*QRTL;
    }
}

void RiemannSolver::PREFUNR(double &F, double &FD){
    double A, B, PRATR,QRTR;

    if(state.VarOnStarZone[2] < state.VarOnRightSide[2]){
        PRATR = state.VarOnStarZone[2]/state.VarOnRightSide[2];
        F = gammas.gamma3 * state.VarOnRightSide[3] * (pow(PRATR,gammas.gamma0) - 1.0);
        FD = (1.0/(state.VarOnRightSide[0]*state.VarOnRightSide[3]))/pow(PRATR,gammas.gamma1);
    }
    else{
        A = gammas.gamma4/state.VarOnRightSide[0];
        B = gammas.gamma5 * state.VarOnRightSide[2];
        QRTR = std::sqrt(A/(B + state.VarOnStarZone[2]));
        F = (state.VarOnStarZone[2] - state.VarOnRightSide[2]) * QRTR;
        FD = ( 1.0 - (state.VarOnStarZone[2] - state.VarOnRightSide[2])/(2.0 * (B + state.VarOnStarZone[2])))*QRTR;
    }
}

void RiemannSolver::STARPU(double &P, double &U){
    int NRITER = 10;
    
    double TOLPRE = 1.0/std::pow(10.0,6.0), UDIFF, FL, FLD, FR, FRD, CHANGE;
    
    GUESSP();
    
    UDIFF = state.VarOnRightSide[1] - state.VarOnLeftSide[1];
    
    for(int i = 1; i <= NRITER; i++){
        PREFUNL(FL,FLD);
        PREFUNR(FR,FRD);
        P = state.VarOnStarZone[2] - (FL + FR + UDIFF)/(FLD + FRD);
        CHANGE = 2.0 * fabs((P - state.VarOnStarZone[2]))/(P + state.VarOnStarZone[2]);
        
        if(CHANGE < TOLPRE){
            continue;
        }
        if(P < 0.0){
            P = TOLPRE;
        }
        state.VarOnStarZone[2] = P;
    }

    U = (state.VarOnLeftSide[1] + state.VarOnRightSide[1] + FR - FL)/2.0;
    state.VarOnStarZone[1] = U;
    
    std::cout << "Pressure in Star zone: " << P <<"; Velocity in Star zone: "<< U << std::endl;
}

void RiemannSolver::SAMPLE(double &P, double &U, double &D, double &T, double &S){
    double SHL,STL,SHR,STR,CML,CMR,PML,PMR,SL,SR,Ss;

    if(S < state.VarOnStarZone[1]){
        if(state.VarOnStarZone[2] < state.VarOnLeftSide[2]){
            SHL = state.VarOnLeftSide[1] - state.VarOnLeftSide[3];
            if(S < SHL){
                D = state.VarOnLeftSide[0];
                U = state.VarOnLeftSide[1];
                P = state.VarOnLeftSide[2];
            } else{
                CML = state.VarOnLeftSide[3] * pow((state.VarOnStarZone[2]/state.VarOnLeftSide[2]),gammas.gamma0);
                STL = state.VarOnStarZone[1] - CML;
                if(S > STL){
                    D = state.VarOnLeftSide[0] * pow((state.VarOnStarZone[2]/state.VarOnLeftSide[2]),1.0/gammas.gamma);
                    U = state.VarOnStarZone[1];
                    P = state.VarOnStarZone[2];
                } else{
                    U = gammas.gamma4 * (state.VarOnLeftSide[3] + gammas.gamma6*state.VarOnLeftSide[1] + S);
                    Ss = gammas.gamma4 * (state.VarOnLeftSide[3] + gammas.gamma6*(state.VarOnLeftSide[1] - S));
                    D = state.VarOnLeftSide[0] * std::pow((Ss/state.VarOnLeftSide[3]),gammas.gamma3);
                    P = state.VarOnLeftSide[2] * std::pow((Ss/state.VarOnLeftSide[3]),gammas.gamma2);
                }
            }
        } else{
            PML = state.VarOnStarZone[2]/state.VarOnLeftSide[2];
            SL = state.VarOnLeftSide[1] - state.VarOnLeftSide[3]*std::sqrt(gammas.gamma1*PML + gammas.gamma0);

            if(S < SL){
                D = state.VarOnLeftSide[0];
                U = state.VarOnLeftSide[1];
                P = state.VarOnLeftSide[2];
            } else{
                D = state.VarOnLeftSide[0] * (PML + gammas.gamma5)/(PML*gammas.gamma5 + 1.0);
                U = state.VarOnStarZone[1];
                P = state.VarOnStarZone[2];
            }
        }
    } else if(state.VarOnStarZone[2] > state.VarOnRightSide[2]){

        PMR = state.VarOnStarZone[2]/state.VarOnRightSide[2];
        SR = state.VarOnRightSide[1] + state.VarOnRightSide[3]*std::sqrt(gammas.gamma1*PMR + gammas.gamma0);

        if(S > SR){
            D = state.VarOnRightSide[0];
            U = state.VarOnRightSide[1];
            P = state.VarOnRightSide[2];
        } else{
            D = state.VarOnRightSide[0] * (PMR + gammas.gamma5)/(PMR*gammas.gamma5 + 1.0);
            U = state.VarOnStarZone[1];
            P = state.VarOnStarZone[2];
        }

    } else{
        SHR = state.VarOnRightSide[1] + state.VarOnRightSide[3];

        if(S > SHR){
            D = state.VarOnRightSide[0];
            U = state.VarOnRightSide[1];
            P = state.VarOnRightSide[2];
        }else{
            CMR = state.VarOnRightSide[3] * std::pow((state.VarOnStarZone[2]/state.VarOnRightSide[2]),gammas.gamma0);
            STR = state.VarOnStarZone[1] + CMR;

            if(S < STR){
                D = state.VarOnRightSide[0] * std::pow((state.VarOnStarZone[2]/state.VarOnRightSide[2]),(1.0/gammas.gamma));
                U = state.VarOnStarZone[1];
                P = state.VarOnStarZone[2];
            } else{
                U = gammas.gamma4 * (-state.VarOnRightSide[3] + gammas.gamma6 * state.VarOnRightSide[1] + S);
                Ss = gammas.gamma4 * (state.VarOnRightSide[3] - gammas.gamma6 * (state.VarOnRightSide[1] - S));
                D = state.VarOnRightSide[0] * std::pow((Ss/state.VarOnRightSide[3]),gammas.gamma3);
                P = state.VarOnRightSide[2] * std::pow((Ss/state.VarOnRightSide[3]),gammas.gamma2);
            }
        }
    }
    T = (P/(gammas.gamma - 1.0))/D;
}

void RiemannSolver::Solve(const bool flag){
    double Pef, Uef, P, U, D,T;
    double g = gammas.gamma;

    gammas.gamma0 = (g - 1.0)/(2.0 * g);
    gammas.gamma1 = (g + 1.0)/(2.0 * g);
    gammas.gamma2 = (2.0 * g)/(g - 1.0);
    gammas.gamma3 = 2.0/(g - 1.0);
    gammas.gamma4 = 2.0/(g + 1.0);
    gammas.gamma5 = (g - 1.0)/(g + 1.0);
    gammas.gamma6 = (g - 1.0)/2.0;
    gammas.gamma7 = g - 1.0;

    state.VarOnLeftSide[3] = std::sqrt(g * state.VarOnLeftSide[2] / state.VarOnLeftSide[0]);
    state.VarOnRightSide[3] = std::sqrt(g * state.VarOnRightSide[2] / state.VarOnRightSide[0]);

    if(gammas.gamma3 * (state.VarOnLeftSide[3] + state.VarOnRightSide[3]) < (state.VarOnRightSide[1] - state.VarOnLeftSide[1])){
        std::cout << "Vacuum is generated by data" << std::endl;
        return;
    }

    STARPU(Pef,Uef);

    double dx = state.Lenght/state.Nx;
    double dt = state.dt;
    std::ofstream outP("P.txt");
    std::ofstream outD("D.txt");
    std::ofstream outU("U.txt");
    std::ofstream outT("T.txt");

    for(double t = dt; t <= state.timeout; t+=dt){
        for(double i = 1.0; i <= state.Nx; i++){
            double x = (i - state.Discontinuity)*dx;
            double S = (x - state.Discontinuity)/t;

            SAMPLE(P,U,D,T,S);
            if( t >= state.timeout - dt && flag == 0){
                Pressure.push_back(P);
                Velocity.push_back(U);
                rho.push_back(D);
                Temperature.push_back(T);
                Position.push_back(S * t + state.Discontinuity);
                outP << S * t + state.Discontinuity << " " << P << std::endl;
                outD << S * t + state.Discontinuity << " " << D << std::endl;
                outU << S * t + state.Discontinuity << " " << U << std::endl;
                outT << S * t + state.Discontinuity << " " << T << std::endl;
            }
            if (flag == 1){
                Pressure.push_back(P);
                Velocity.push_back(U);
                rho.push_back(D);
                Temperature.push_back(T);
                Position.push_back(S * t + state.Discontinuity);
                /*outP << S * t + state.Discontinuity << " " << P << std::endl;
                outD << S * t + state.Discontinuity << " " << D << std::endl;
                outU << S * t + state.Discontinuity << " " << U << std::endl;
                outT << S * t + state.Discontinuity << " " << T << std::endl;*/
            }

        }
    }

}






