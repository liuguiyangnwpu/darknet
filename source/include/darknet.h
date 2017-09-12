#ifndef DARKNET_API
#define DARKNET_API

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "data_struct.h"
#include "list.h"
#include "utils.h"
#include "data.h"
#include "blas.h"
#include "box.h"
#include "image.h"
#include "matrix.h"
#include "network.h"
#include "parser.h"
#include "region_layer.h"

#define SECRET_NUM -1234
extern int gpu_index;

#ifdef GPU
void axpy_gpu(int N, float ALPHA, float * X, int INCX, float * Y, int INCY);
void fill_gpu(int N, float ALPHA, float * X, int INCX);
void scal_gpu(int N, float ALPHA, float * X, int INCX);
void copy_gpu(int N, float * X, int INCX, float * Y, int INCY);

void cuda_set_device(int n);
void cuda_free(float *x_gpu);
float *cuda_make_array(float *x, size_t n);
void cuda_pull_array(float *x_gpu, float *x, size_t n);
float cuda_mag_array(float *x_gpu, size_t n);
void cuda_push_array(float *x_gpu, float *x, size_t n);

void forward_network_gpu(network net);
void backward_network_gpu(network net);
void update_network_gpu(network net);

float train_networks(network *nets, int n, data d, int interval);
void sync_nets(network *nets, int n, int interval);
void harmless_update_network_gpu(network net);
#endif

// add by liuguiyang
void model_start_init(char* labelfile, char* cfgfile, char* weightfile, char **names);
void test_detector(char *filename, float thresh, float hier_thresh, char **names);

#endif
