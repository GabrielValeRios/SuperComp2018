import os
num_threads = int(input("Number of threads please "))
link = input("link to product category: ")
os.system("NUM_THREADS="+ str(num_threads)+" "+"./crawler_parallel"+" "+link)
#"https://www.magazineluiza.com.br/bateria-e-percussao/instrumentos-musicais/s/im/baoa/"
#for mprof multi-thread
#"https://www.magazineluiza.com.br/computador/informatica/s/in/micr/"