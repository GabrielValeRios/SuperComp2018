
/*
 * Copyright (c) 2018 Igor Montagner igordsm@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>
#include "visualizador.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include <stdio.h>
#include <omp.h>
#include "impactCalc.h"
#include <iostream>
#include <vector>
#include <fstream>

Visualizador::Visualizador(std::vector<ball> &bodies,tableProp table, double delta_t, int gui) :
    gui(gui),
    delta_t(delta_t),
    bodies(bodies),
    table(table) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    double ratio = (double) table.width / table.height;
    if (ratio > 1) {
        win_width = max_dimension;
        win_height = max_dimension / ratio;
    } else {
        win_width = max_dimension * ratio;
        win_height = max_dimension;
    }
    win = SDL_CreateWindow("Visualizador SUPERCOMP", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, win_width, win_height, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    iter = 0;
    quit = false;
    timeIter = 0.01;
    //file to write gui results
    file.open ("gui.txt");
    if(gui == 0){
        file <<"BALLS_STATE_LIST"<<'\n';
    }
}

Visualizador::~Visualizador() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


void Visualizador::draw() {
    SDL_SetRenderDrawColor(renderer, 160, 160, 185, 255);
    SDL_RenderClear(renderer);
    for (auto i = bodies.begin(); i != bodies.end(); i++) {
        filledCircleRGBA(renderer, i->posX / table.width * win_width,
                         i->posY / table.height* win_height,
                         i->raio / table.width * win_width,
                         255, 0 ,0, 255);
    }
    SDL_RenderPresent(renderer);
}

void Visualizador::run() {
    SDL_ShowWindow(win);
    draw();
    while (!quit) {
        if(SDL_QuitRequested()){
            break;
        }
        do_iteration();   
        draw();
    }
    //close gui.txt file
    file.close();  
}

void Visualizador::do_iteration() {

    //write balls state o gui.txt  
    if(gui == 0 and int(timeIter)%100 == 0){
        file <<"DELTA_T:"<<timeIter<<'\n';
        file << "[";
    }

    for (unsigned b = 0; b < bodies.size(); b++){
        //write balls state o gui.txt 
        if((gui == 0 and int(timeIter)%100 == 0)){
            //ID,RAIO,MASSA,POSX,POSY,VELX,VELY
            file <<"("<<bodies[b].id<<","<<bodies[b].raio<<","<<bodies[b].massa<<","
                 <<bodies[b].posX<<","<<bodies[b].posY<<","<<bodies[b].velX
                 <<","<<bodies[b].velY<<")"<<",";
        }
    }

    //vectors of positions(x,y)
    double* posXDepois = new double[bodies.size()];
    double* posYDepois = new double[bodies.size()];

    #pragma omp parallel for
    for (unsigned i = 0; i < bodies.size(); i++){
        //get possible new positions of every ball
        calculateNextVelOnAxis(bodies,bodies[i].id,table,delta_t);

        double pX = calculateNextPosOnX(bodies,bodies[i].id,table,delta_t);
        double pY = calculateNextPosOnY(bodies,bodies[i].id,table,delta_t);
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
                int maiorID,menorID;
    			if(hitBall(posXDepois[bodies[j].id],posYDepois[bodies[j].id],posXDepois[bodies[k].id],posYDepois[bodies[k].id],bodies[j].raio,bodies[k].raio)){
                    if(bodies[j].id > bodies[k].id){
                        maiorID = bodies[j].id;
                        menorID = bodies[k].id;
                    }
                    else{
                        maiorID = bodies[k].id;
                        menorID = bodies[j].id; 
                    }
                    // print HIT action
                    
                    #pragma omp critical
                    {
                        std::cout <<"CHOQUE"<<" "<<"ID_0:"<<menorID <<" "<<"ID_1:"<<maiorID<<" "<<"DELTA_T:"<<timeIter<<'\n';
                    }
                    
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

    //write balls state o gui.txt 
    if ((gui == 0 and int(timeIter)%100 == 0)){
        file << "]" << '\n';
    }
    iter++;
    timeIter++;
}