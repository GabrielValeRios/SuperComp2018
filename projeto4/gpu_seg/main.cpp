#include <algorithm>
#include <numeric>
#include <iostream>
#include <queue>
#include <vector>
#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include "nvgraph.h"
#include "imagem.h"
#include <chrono>

/*
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */



/* Single Source Shortest Path (SSSP)
 *  Calculate the shortest path distance from a single vertex in the graph
 *  to all other vertices.
 */

typedef std::chrono::high_resolution_clock Time;
using namespace std::chrono;

void buildVectors(imagem *img, int ghost_seed ,std::vector<int> &seeds,std::vector<int> &dest_offset, std::vector<int> &src_indices,std::vector<float> &weights){
   
    dest_offset.push_back(0);
    int dest_counter;
    //int g_fg = img->total_size+1;
    //int g_bg = img->total_size + 2;
    //ghost_fg = g_fg;
    //ghost_bg = g_bg;

    for (int i = 0; i < img->total_size; i++) {

        dest_counter = dest_offset[i];
        

        if (find(begin(seeds), end(seeds), i) != end(seeds)) {
              src_indices.push_back(ghost_seed);
              weights.push_back(0.0);
              dest_counter++;
        }

        int acima = i - img->cols;
        if (acima >= 0) {
            dest_counter+=1;
            src_indices.push_back(acima);
            double custo_acima = get_edge(img, i, acima);
            weights.push_back(custo_acima);
        }

        int abaixo = i + img->cols;
        if (abaixo < img->total_size) {
            dest_counter+=1;
            src_indices.push_back(abaixo);
            double custo_abaixo = get_edge(img, i, abaixo);
            weights.push_back(custo_abaixo);
        }

        int direita = i + 1;
        if (direita < img->total_size) {
            dest_counter+=1;
            src_indices.push_back(direita);
            double custo_direita = get_edge(img, i, direita);
            weights.push_back(custo_direita);
        }

        int esquerda = i - 1;
        if (esquerda >= 0) {
            dest_counter+=1;
            src_indices.push_back(esquerda);
            double custo_esquerda = get_edge(img, i, esquerda);
            weights.push_back(custo_esquerda);
        }

        dest_offset.push_back(dest_counter);
    }

    dest_offset.push_back(src_indices.size());

}

// Nvidia stuff
void check_status(nvgraphStatus_t status)
{
    if ((int)status != 0)
    {
        printf("ERROR : %d\n",status);
        exit(0);
    }
}

int SSSP(int num_pixels, int source_vert,std::vector<int> &destination_offsets_h, std::vector<int> &source_indices_h,std::vector<float> &weights_h,std::vector<int> &sssp_output){

    int* destination_offsets  = new int[destination_offsets_h.size()];
    for(unsigned i=0; i<destination_offsets_h.size();i++)
        destination_offsets[i] = destination_offsets_h[i];

    int* source_indices  = new int[source_indices_h.size()];
    for(unsigned s=0; s<source_indices_h.size();s++)
        source_indices[s] = source_indices_h[s];

    float* weights  = new float[weights_h.size()];
    for(unsigned w=0; w<weights_h.size();w++)
        weights[w] = weights_h[w];

    const size_t n = num_pixels, nnz = weights_h.size(), vertex_numsets = 1, edge_numsets = 1;
    int i;
    float *sssp_1_h;
    void** vertex_dim;

    // nvgraph variables
    nvgraphStatus_t status;
    nvgraphHandle_t handle;
    nvgraphGraphDescr_t graph;
    nvgraphCSCTopology32I_t CSC_input;
    cudaDataType_t edge_dimT = CUDA_R_32F;
    cudaDataType_t* vertex_dimT;

    sssp_1_h = (float*)malloc(n*sizeof(float));
    vertex_dim  = (void**)malloc(vertex_numsets*sizeof(void*));
    vertex_dimT = (cudaDataType_t*)malloc(vertex_numsets*sizeof(cudaDataType_t));
    CSC_input = (nvgraphCSCTopology32I_t) malloc(sizeof(struct nvgraphCSCTopology32I_st));

    vertex_dim[0]= (void*)sssp_1_h; vertex_dim[1];
    vertex_dimT[0] = CUDA_R_32F; vertex_dimT[1]= CUDA_R_32F;

    check_status(nvgraphCreate(&handle));
    check_status(nvgraphCreateGraphDescr (handle, &graph));

    CSC_input->nvertices = n;
    CSC_input->nedges = nnz;
    CSC_input->destination_offsets = destination_offsets;
    CSC_input->source_indices = source_indices;

    // Set graph connectivity and properties (tranfers)
    check_status(nvgraphSetGraphStructure(handle, graph, (void*)CSC_input, NVGRAPH_CSC_32));
    check_status(nvgraphAllocateVertexData(handle, graph, vertex_numsets, vertex_dimT));
    check_status(nvgraphAllocateEdgeData  (handle, graph, edge_numsets, &edge_dimT));
    check_status(nvgraphSetEdgeData(handle, graph, (void*)weights, 0));

    // Solve
    std::cout<<"&source_vert:" <<" "<<&source_vert<<" "<<"source_vert: "<<source_vert<<"\n";
    check_status(nvgraphSssp(handle, graph, 0,  &source_vert, 0));
    
    check_status(nvgraphGetVertexData(handle, graph, (void*)sssp_1_h, 0));
    
    for (i = 0; i<n; i++)  sssp_output.push_back(sssp_1_h[i]);


    free(destination_offsets);
    free(source_indices);
    free(weights);
    free(sssp_1_h);
    //free(sssp_2_h);
    free(vertex_dim);
    free(vertex_dimT);
    free(CSC_input);

    //Clean 
    check_status(nvgraphDestroyGraphDescr (handle, graph));
    check_status(nvgraphDestroy (handle));


    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    
    if (argc < 3) {
        std::cout << "Uso:  segmentacao_sequencial entrada.pgm saida.pgm\n";
        return -1;
    }
    std::string path(argv[1]);
    std::string path_output(argv[2]);
    imagem *img = read_pgm(path);
    
    int n_fg, n_bg;
    int x, y;
     
    // n of seeds, fg and bg        
    std::cin >> n_fg >> n_bg;
    
    std::vector<int> seeds_fg;
    std::vector<int> seeds_bg;
    for (int a = 0; a < n_fg; a++){
       std::cin >> x >> y;
       seeds_fg.push_back(y * img-> cols + x);

    }
  
    for (int b =0; b < n_bg; b++){
       std::cin >> x >> y;
       seeds_bg.push_back(y * img-> cols + x);
    }

    //build vectors for nvidia algorithm
    std::vector<int> dest_offset_fg, dest_offset_bg;
    std::vector<int> src_indices_fg, src_indices_bg;
    std::vector<float> weights_fg, weights_bg;

    int ghost_fg = img->total_size+1;
    int ghost_bg = img->total_size+2;
   
    Time::time_point t1buildVec = Time::now();
    buildVectors(img,ghost_fg,seeds_fg,dest_offset_fg,src_indices_fg,weights_fg);
    buildVectors(img,ghost_bg,seeds_bg,dest_offset_bg,src_indices_bg,weights_bg);
    Time::time_point t2buildVec = Time::now();
    duration<double> tempo = duration_cast<duration<double>>(t2buildVec - t1buildVec);
    double buildVecTime = tempo.count();   
    std::cout << "buildVecTime: " << buildVecTime << "\n";
    
    std::vector<int> sssp_output_1, sssp_output_2;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    SSSP(img->total_size,ghost_fg,dest_offset_fg,src_indices_fg,weights_fg,sssp_output_1);
    SSSP(img->total_size,ghost_bg,dest_offset_bg,src_indices_bg,weights_bg,sssp_output_2);
    
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);   
    float elapsed_time;
    cudaEventElapsedTime(&elapsed_time, start, stop);
    std::cout << "SSSP TIME: "<<elapsed_time<<"\n";

    imagem *saida = new_image(img->rows, img->cols);
    Time::time_point t1buildImage = Time::now();

    for (int k = 0; k < saida->total_size; k++) {
        if (sssp_output_1[k] > sssp_output_2[k]) {
            saida->pixels[k] = 0;
        } else {
            saida->pixels[k] = 255;
        }
    }
    
    write_pgm(saida, path_output);
    Time::time_point t2buildImage = Time::now();
    duration<double> tempo_2 = duration_cast<duration<double>>(t2buildImage - t1buildImage);
    double buildImageTime = tempo_2.count();
    std::cout << "buildImageTime: " << buildImageTime << "\n";
 
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}
