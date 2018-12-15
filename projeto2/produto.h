#ifndef PRODUTO_H
#define PRODUTO_H
#include <string>
#include <sstream>

class Produto{
public:
	std::string nome,descricao,urlFoto,urlPagina,categoria,precoVista,precoParcelado;

	Produto(std::string _nome, std::string _descricao, std::string _urlFoto, std::string _urlPagina, std::string
    _categoria, std::string _precoVista, std::string _precoParcelado);

    std::string toJson();
    
};
#endif