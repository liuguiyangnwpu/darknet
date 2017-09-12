#ifndef BOX_H
#define BOX_H
#include "darknet.h"

typedef struct{
    float dx, dy, dw, dh;
} dbox;

float box_rmse(box a, box b);
dbox diou(box a, box b);
box decode_box(box b, box anchor);
box encode_box(box b, box anchor);
float box_iou(box a, box b);
void do_nms(box *boxes, float **probs, int total, int classes, float thresh);
data load_all_cifar10();
box_label *read_boxes(char *filename, int *n);
box float_to_box(float *f, int stride);
void do_nms_sort(box *boxes, float **probs, int total, int classes, float thresh);
void do_nms_obj(box *boxes, float **probs, int total, int classes, float thresh);

#endif
