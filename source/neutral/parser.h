#ifndef PARSER_H
#define PARSER_H
#include "darknet.h"
#include "network.h"

void save_network(network net, char *filename);
void save_weights_double(network net, char *filename);

list *read_cfg(char *filename);
network parse_network_cfg(char *filename);
void save_weights(network net, char *filename);
void load_weights(network *net, char *filename);
void save_weights_upto(network net, char *filename, int cutoff);
void load_weights_upto(network *net, char *filename, int start, int cutoff);

#endif
