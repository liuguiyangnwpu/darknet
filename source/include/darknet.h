#ifndef DARKNET_API
#define DARKNET_API

#include <iostream>
#include <string>
#include <map>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
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

// add by liuguiyang
void model_start_init(char* labelfile, char* cfgfile, char* weightfile, char **names);
void detect_single_image(char *filename, float thresh, float hier_thresh, char **names);
void handle_big_image(char *filename, std::map<std::pair<int, int>, image> &crop_images);

#endif
