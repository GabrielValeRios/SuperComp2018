
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

#include "visualizador.h"
#include "impactCalc.h"
#include <vector>
#include <stdlib.h>  

int main(int argc, char ** argv) {
    //gui variable,check if it's a number
    int gui;
    double delta_t;
    if(const char* env_p = std::getenv("GUI")){
		gui = atoi(env_p);
	}
	else{
		gui = 1;
	}

    if(const char* env_p2 = std::getenv("DELTA")){
    	delta_t = atof(env_p2);     
    }
    else{
    	delta_t = 0.01;
            
    }

    //vector of balls
    std::vector<ball> bolas;
    // struct of table prop
    tableProp table;
    //read func
    readFile(bolas,table);

    
    Visualizador v(bolas, table, delta_t, gui);
	v.run();

    return 0; 
}
