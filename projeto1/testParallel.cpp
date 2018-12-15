
#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "impactCalc.h"

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;
void testParallel(std::vector<ball> &bodies, tableProp table, double delta_t, long iter){

    for(long l = 0; l<iter; l++){

        double* posXDepois = new double[bodies.size()];
        double* posYDepois = new double[bodies.size()];
        double pX,pY;

        #pragma omp parallel for
        for (unsigned i = 0; i < bodies.size(); i++){
            //get possible new positions of every ball
            calculateNextVelOnAxis(bodies,bodies[i].id,table,delta_t);
            pX = calculateNextPosOnX(bodies,bodies[i].id,table,delta_t);
            pY = calculateNextPosOnY(bodies,bodies[i].id,table,delta_t);
            posXDepois[bodies[i].id] = pX;
            posYDepois[bodies[i].id] = pY;
        }

        //hit with bounderies
        #pragma omp parallel for
        for (unsigned a = 0; a < bodies.size(); a++) {
            if(hitTable(posXDepois[bodies[a].id],posYDepois[bodies[a].id],bodies[a].raio,table.width,table.height)){
                //calculate new geometric vel orientation
                calculateVecVel(bodies,table,bodies[a].id,-1);
                bodies[a].hit = true;
            }
        }

        //check if ball will hit any bounderies
        #pragma omp parallel for collapse(2)
        for (unsigned j = 0; j < bodies.size(); j++) {
            //check if any ball will hit another ball
            for (unsigned k = 0; k < bodies.size(); k++) {
                if(bodies[j].id != bodies[k].id){
                    if(hitBall(posXDepois[bodies[j].id],posYDepois[bodies[j].id],posXDepois[bodies[k].id],posYDepois[bodies[k].id],bodies[j].raio,bodies[k].raio)){
                        //calculate new geometric vel orientation
                        calculateVecVel(bodies,table,bodies[j].id,bodies[k].id);
                        bodies[j].hit = true;
                    }
                } 
            }        
        }
        //if there is no hit at all, the ball will move normally
        #pragma omp parallel for
        for (unsigned z = 0; z < bodies.size(); z++) {
            if(!bodies[z].hit){
                bodies[z].posX = posXDepois[bodies[z].id];
                bodies[z].posY = posYDepois[bodies[z].id];
            }
            else{
                bodies[z].hit = false;
            }
        }
    }
}   

int main(int argc, char const *argv[])
{   
    
    unsigned long iter = atoi(argv[1]);

    std::cout << "RUNNING:"<< iter<<" "<< "ITERATIONS"<< '\n';
    double delta_t = 0.01;
    //vector of balls
    std::vector<ball> b;
    // struct of table prop
    tableProp t;
    //read func
    readFile(b,t); 

    Time::time_point t1 = Time::now();
    testParallel(b,t,delta_t,iter);
    Time::time_point t2 = Time::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Time:" << time_span.count() << '\n';

    return 0;
}