#include <iostream>

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

void handle_big_image(char *filename, map<pair<int, int>, image> &crop_images) {
    image im = load_image_color(filename, 0, 0);
    // image crop_image(image im, int dx, int dy, int w, int h);
    cout << "width=" << im.w << ", height=" << im.h << endl;
    int crop_width = stoi(g_ConfMap["WIDTH"]);
    int crop_height = stoi(g_ConfMap["HEIGHT"]);
    cout << "crop_height:" << crop_height << ", crop_width:" << crop_width << endl;
    // width=2048, height=1536
    // crop_height:608, crop_width:608
    int w_stride = crop_width - 60;
    int h_stride = crop_height - 60;
    int w_nums = im.w / w_stride;
    int h_nums = im.h / h_stride;
    for(int i=0;i<=h_nums;i++) {
        int row = i * h_stride;
        if(row + crop_height >= im.h) {
            row = im.h - crop_height;
        }
        for(int j=0;j<=w_nums;j++) {
            int col = j * w_stride;
            if(col + crop_width >= im.w) {
                col = im.w - crop_width;
            }
            image c_img = crop_image(im, col, row, crop_width, crop_height);
            crop_images[make_pair(col, row)] = c_img;
        }
    }
    cout << "Crop Images Num is " << crop_images.size() << endl;
}

void detect_single_image(image im, float thresh, float hier_thresh, char **names) {
    srand(2222222);
    float nms=.3;
    layer l = net.layers[net.n-1];
    box *boxes = (box*)calloc(l.w*l.h*l.n, sizeof(box));
    float **probs = (float**)calloc(l.w*l.h*l.n, sizeof(float *));
    for(int j = 0; j < l.w*l.h*l.n; ++j) 
        probs[j] = (float*)calloc(l.classes + 1, sizeof(float *));
    float **masks = 0;
    if (l.coords > 4) {
        masks = (float**)calloc(l.w*l.h*l.n, sizeof(float*));
        for(int j = 0; j < l.w*l.h*l.n; ++j) 
            masks[j] = (float*)calloc(l.coords-4, sizeof(float *));
    }

    image sized = letterbox_image(im, net.w, net.h);
    float *X = sized.data;
    double time=what_time_is_it_now();
    network_predict(net, X);
    printf("Predicted %f seconds.\n", what_time_is_it_now()-time);
    get_region_boxes(l, im.w, im.h, net.w, net.h, thresh, probs, boxes, masks, 0, 0, hier_thresh, 1);
    if (nms)
        do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
    draw_detections(im, l.w*l.h*l.n, thresh, boxes, probs, masks, names, l.classes);
    save_image(im, "predictions");
    free_image(sized);
    free(boxes);
    free_ptrs((void **)probs, l.w*l.h*l.n);
    if(masks) {
        free_ptrs((void **)masks, l.w*l.h*l.n);
    }
}
