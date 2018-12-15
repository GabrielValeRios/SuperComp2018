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


/* Este arquivo deve conter a definição da sua estrutura/classe que
 * representa um corpo na simulação. A estrutura abaixo é somente um exemplo 
 * que deve ser alterado para que seu programa seja integrado com o 
 * visualizador. 
 */
#ifndef BODY_H
#define BODY_H
struct tableProp{
    double width,height,mu,alphaW,alphaB;
};

class ball {
public:
	long int id;
	double raio,massa,posX,posY,velX,velY;
	bool hit;
	double stopPx,stopPy;
	ball(long int _id,double _r,double _m,double _posX,double _posY,double _velX,double _velY,bool _hit,double stopPx,double stopPy);

    double getBallVel();
};
#endif