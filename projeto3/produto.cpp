#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "produto.h"

Produto::Produto(std::string _nome, std::string _descricao, std::string _urlFoto, std::string _urlPagina, std::string
    _categoria, std::string _precoVista, std::string _precoParcelado) :
    
    nome(_nome),
    descricao(_descricao),
    urlFoto(_urlFoto),
    urlPagina(_urlPagina),
    categoria(_categoria),
    precoVista(_precoVista),
    precoParcelado(_precoParcelado){};

std::string Produto::toJson(){

    std::stringstream ss;

    ss << "{" << "\n" << "   " << "nome: " << nome <<"\n"<<
                     "   " << "descricao: " << descricao <<"\n"<<
                     "   " << "urlFoto: " << urlFoto << "\n" << 
                     "   " << "urlPagina: " << urlPagina <<"\n"<<
                     "   " << "categoria: " << categoria <<"\n"<<
                     "   " << "precoVista: " << precoVista <<"\n"<<
                     "   " << "precoParcelado: " << precoParcelado <<"\n"
              << "}";

    std::string objJSON = ss.str();
    return objJSON;
}