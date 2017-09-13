#include <iostream>
#include <string>
#include <map>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <grpc++/grpc++.h>
#include "darknet.h"

using namespace std;

extern map<string, string> g_ConfMap;
network net;
void model_start_init(char *labelfile, char* cfgfile, char* weightfile, char **names) {
    names = get_labels(labelfile);
    net = parse_network_cfg(cfgfile);
    load_weights(&net, weightfile);
    set_batch_network(&net, 1);
}

void detect_single_image(char *filename, float thresh, float hier_thresh, char **names) {
    image **alphabet = NULL;
    srand(2222222);
    float nms=.3;
    image im = load_image_color(filename, 0, 0);
    image sized = letterbox_image(im, net.w, net.h);
    layer l = net.layers[net.n-1];

    box *boxes = (box*)calloc(l.w*l.h*l.n, sizeof(box));
    float **probs = (float**)calloc(l.w*l.h*l.n, sizeof(float *));
    for(int j = 0; j < l.w*l.h*l.n; ++j) 
        probs[j] = (float*)calloc(l.classes + 1, sizeof(float *));
    float **masks = 0;
    if (l.coords > 4){
        masks = (float**)calloc(l.w*l.h*l.n, sizeof(float*));
        for(int j = 0; j < l.w*l.h*l.n; ++j) 
            masks[j] = (float*)calloc(l.coords-4, sizeof(float *));
    }

    float *X = sized.data;
    double time=what_time_is_it_now();
    network_predict(net, X);
    printf("%s: Predicted in %f seconds.\n", filename, what_time_is_it_now()-time);
    get_region_boxes(l, im.w, im.h, net.w, net.h, thresh, probs, boxes, masks, 0, 0, hier_thresh, 1);
    if (nms)
        do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
    draw_detections(im, l.w*l.h*l.n, thresh, boxes, probs, masks, names, alphabet, l.classes);
    save_image(im, "predictions");

    free_image(im);
    free_image(sized);
    free(boxes);
    free_ptrs((void **)probs, l.w*l.h*l.n);
}
