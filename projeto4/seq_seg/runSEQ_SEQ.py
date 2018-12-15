import os
os.system("cmake .")
os.system("make")
img = str(input("img name with .ext for sequential segmentation: "))
os.system("./segmentacao_sequencial"+" "+"./"+img+" "+"saida.pgm")
