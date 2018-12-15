
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "impactCalc.h"

ball::ball(long int _id,double _r,double _m,double _posX,double _posY,double _velX,double _velY,bool _hit,double _stopPx,double _stopPy) :
    
    id(_id),
    raio(_r),
    massa(_m),
    posX(_posX),
    posY(_posY),
    velX(_velX),
    velY(_velY),
    hit(_hit),
    stopPx(_stopPx),
    stopPy(_stopPy){};

double ball::getBallVel(){ 
    return sqrt(pow(velX,2) + pow(velY,2));
}

bool hitBall(double pos1X,double pos1Y,double pos2X,double pos2Y,double raio1,double raio2){
    if(sqrt(pow(pos1X - pos2X,2) + pow(pos1Y - pos2Y,2)) <= raio1 + raio2){
    	return true;  
    } 
    return false;
}

bool hitTable(double posX,double posY,double raio,double w,double h){
    if((posX + raio >= w) or (posX - raio <= 0) or
      (posY + raio >= h) or (posY - raio <= 0)){
        return true;  
    } 
    return false;
}

void readFile(std::vector<ball> &bolas, tableProp &table){
    int id,n;
    double w,h,mu,alphaW,alphaB,raio,massa,x0,y0,vx0,vy0;
    //table properties setup
    std::cin >> w >> h >> n >> mu >> alphaW >> alphaB;
    table.width = w;
    table.height = h;
    table.mu = mu;
    table.alphaW = alphaW;
    table.alphaB = alphaB;

    for(int i=0; i<n; i++){
        std::cin >> id >> raio >> massa >> x0 >> y0 >> vx0 >> vy0;
        ball b(id,raio,massa,x0,y0,vx0,vy0,false,vx0*pow(10,-1),vy0*pow(10,-1));
        bolas.push_back(b);
    }
}

void calculateVecVel(std::vector<ball> &b,tableProp &table,int i,int j){

    //calcular projecao de V1 no vetor que liga os centros.Primeiro, o modulo 'w'
    double wMagnetude = (b[i].velX*(b[j].posX - b[i].posX)+b[i].velY*(b[j].posY - b[i].posY))/((pow((b[j].posX - b[i].posX),2))+(pow((b[j].posY - b[i].posY),2)));
    //apos calculo do valor do vetor projetado, calcular as direcoes
    std::pair <double,double> cartesianDirec;
    cartesianDirec = std::make_pair(b[j].posX-b[i].posX,b[j].posY-b[i].posY);
    std::pair <double,double> wVector;
    wVector = std::make_pair(cartesianDirec.first*wMagnetude,cartesianDirec.second*wMagnetude);
    //agora, o calculo do vetor 'p' que fecha o triangulo retangulo
    std::pair <double,double> pVector;
    pVector = std::make_pair(b[i].velX-wVector.first,b[i].velY-wVector.second);
    //calcular o vetor resultante

    //hit table
	if(j < 0){
		if(b[i].posY - b[i].raio-1 <= 0 or b[i].posY + b[i].raio+1 >= table.height){
			b[i].velY = -b[i].velY;
		}
		if(b[i].posX - b[i].raio-1 <= 0 or b[i].posX + b[i].raio+1 >= table.width){
			b[i].velX = -b[i].velX;
		}	
	}
	// hit ball
	else{
	    b[i].velX = -wVector.first + pVector.first;
	    b[i].velY = -wVector.second + pVector.second;
	}
}

void calculateNextVelOnAxis(std::vector<ball> &b,int i,tableProp &table,double delta_t){


	//primeiro, calcular sen e cos do angulo entre as velocidades
	double senAng,cosAng;

	senAng = b[i].velY/b[i].getBallVel();
	cosAng = b[i].velX/b[i].getBallVel();
	//std::cout << "sen" << senAng << " " << "cos" << cosAng << '\n';
	//calcular agora o atrito pra cada eixo
	double atritoX = cosAng*(table.mu*b[i].massa*9.81);
	double atritoY = senAng*(table.mu*b[i].massa*9.81);
	// mudar as velocidades nos eixos

	b[i].velX -= atritoX*delta_t;
	b[i].velY -= atritoY*delta_t;


}

double calculateNextPosOnX(std::vector<ball> &b,int i,tableProp &table,double delta_t){
    double newPosX = b[i].posX + b[i].velX*delta_t;
    return newPosX;
}
double calculateNextPosOnY(std::vector<ball> &b,int i,tableProp &table,double delta_t){
    double newPosY=  b[i].posY + b[i].velY*delta_t;
    return newPosY;
}