
#ifndef IMPACTCALC_H
#define IMPACTCALC_H
#include "body.h"

bool hitBall(double pos1X,double pos1Y,double pos2X,double pos2Y,double raio1,double raio2);

bool hitTable(double posX,double posY,double raio,double w, double h);

void readFile(std::vector<ball> &bolas, tableProp &table);

void calculateVecVel(std::vector<ball> &b,tableProp &table,int i,int j);

void calculateNextVelOnAxis(std::vector<ball> &b,int i,tableProp &table,double delta_t);

double calculateNextPosOnX(std::vector<ball> &b,int i,tableProp &table,double delta_t);

double calculateNextPosOnY(std::vector<ball> &b,int i,tableProp &table,double delta_t);
#endif