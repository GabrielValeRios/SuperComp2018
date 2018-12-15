#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include "imagem.h"
#include <string.h>


#define MAX(y,x) (y>x?y:x)    // Calcula valor maximo
#define MIN(y,x) (y<x?y:x)    // Calcula valor minimo

__global__ void border_filter(unsigned char *in, unsigned char *out, int rowStart, int rowEnd, int colStart, int colEnd) {
	int i=blockIdx.x*blockDim.x+threadIdx.x;
	int j=blockIdx.y*blockDim.y+threadIdx.y;
	int di, dj;
	if (i < rowEnd && j < colEnd) {
           // entendi lendo a aula 24 que estes for's sao desnecessarios
           // o proprio kernel do Cuda ja divide em blocos de threads, entao nao faz o menor sentido ter os loops
           // for(i = rowStart; i < rowEnd; ++i) {
               // for(j = colStart; j < colEnd; ++j) {
           int min = 256;
           int max = 0;
           for(di = MAX(rowStart, i - 1); di <= MIN(i + 1, rowEnd - 1); di++) {
               for(dj = MAX(colStart, j - 1); dj <= MIN(j + 1, colEnd - 1); dj++) {
                   if(min>in[di*(colEnd-colStart)+dj]) min = in[di*(colEnd-colStart)+dj];
                       if(max<in[di*(colEnd-colStart)+dj]) max = in[di*(colEnd-colStart)+dj]; 
            	}
           }	
           out[i*(colEnd-colStart)+j] = max-min;
      	}
}
		

int main(int argc, char **argv)
{

    if (argc < 3) {
        std::cout << "Uso: border_filter entrada.pgm saida.pgm\n";
        return -1;
    }

    std::string path(argv[1]);
    std::string out_path(argv[2]);

    imagem *img = read_pgm(path);
    int nrows = img->rows;
    int ncols = img->cols;

   
    imagem *saida = new_image(nrows, ncols);
    thrust::device_vector<unsigned char> input(img->pixels, img->pixels + img->total_size );
    thrust::device_vector<unsigned char> output(saida->pixels, saida->pixels + saida->total_size );

    dim3 dimGrid(ceil(nrows/16.0), ceil(ncols/16.0), 1);
    // 16 x 16 block size
    dim3 dimBlock(16, 16, 1);
    border_filter<<<dimGrid,dimBlock>>>(thrust::raw_pointer_cast(input.data()), thrust::raw_pointer_cast(output.data()), 0, nrows, 0, ncols);
    
    thrust::host_vector<unsigned char> output_data(output);
    for(int i = 0; i < output_data.size(); i++) {
        saida->pixels[i] = output_data[i];
    }

write_pgm(saida, out_path);


}
