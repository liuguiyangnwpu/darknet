// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#include "darknet.h"

#include "image.h"
#include "layer.h"
#include "data.h"
#include "tree.h"


#ifdef GPU
float train_network_datum_gpu(network net);
float *network_predict_gpu(network net, float *input);
void pull_network_output(network net);

void forward_network_gpu(network net);
void backward_network_gpu(network net);
void update_network_gpu(network net);

float train_networks(network *nets, int n, data d, int interval);
void sync_nets(network *nets, int n, int interval);
void harmless_update_network_gpu(network net);

#endif

void compare_networks(network n1, network n2, data d);
char *get_layer_string(LAYER_TYPE a);

network make_network(int n);


float network_accuracy_multi(network net, data d, int n);
int get_predicted_class_network(network net);
void print_network(network net);
int resize_network(network *net, int w, int h);
void calc_network_cost(network net);

network load_network(char *cfg, char *weights, int clear);
network *load_network_p(char *cfg, char *weights, int clear);
load_args get_base_args(network net);
void forward_network(network net);
void backward_network(network net);
void update_network(network net);
float train_network_sgd(network net, data d, int n);
float train_network_datum(network net);
float *network_accuracies(network net, data d, int n);
void free_network(network net);
void set_batch_network(network *net, int b);
void set_temp_network(network net, float t);
int resize_network(network *net, int w, int h);
void reset_network_state(network net, int b);
void reset_network_state(network net, int b);
float train_network(network net, data d);
matrix network_predict_data(network net, data test);
image get_network_image(network net);
float *network_predict(network net, float *input);
float *network_predict_p(network *net, float *input);
int network_width(network *net);
int network_height(network *net);
float *network_predict_image(network *net, image im);
void network_detect(network *net, image im, float thresh, float hier_thresh, float nms, box *boxes, float **probs);
int num_boxes(network *net);
box *make_boxes(network *net);
void visualize_network(network net);
float get_current_rate(network net);
size_t get_current_batch(network net);
image get_network_image_layer(network net, int i);
layer get_network_output_layer(network net);
void top_predictions(network net, int n, int *index);
float network_accuracy(network net, data d);

#endif

