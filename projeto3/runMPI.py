import os
import argparse
parser = argparse.ArgumentParser()                                               

parser.add_argument("--file", "-f", type=str, required=True)
args = parser.parse_args()

link = input("link to product category: ")
hf = args.file
nP = str(2*int(hf[-1]))
print("number of processes: "+ nP)

os.system("mpiexec -n"+" "+nP+" "+"-hostfile"+" "+hf+" "+ "./controller"+" "+link+" "+"2>testes.txt")
