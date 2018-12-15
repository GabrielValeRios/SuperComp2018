import os

os.system("nvcc -std=c++11 border_filter.cu imagem.cpp -o border_filter")
img = input("Image name with .ext for border_file: ")
os.system("./border_filter ./" + img + " " + "./saida_bf.pgm")
