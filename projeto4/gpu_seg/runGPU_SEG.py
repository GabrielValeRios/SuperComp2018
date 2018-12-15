import os

os.system("nvcc -std=c++11 main.cpp imagem.cpp -o sssp -lnvgraph")
img = input("Image name with .ext for sssp: ")
os.system("./sssp ./" + img + " " + "./saida_sssp.pgm")
