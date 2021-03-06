#include <stdio.h>

#include "network.h"
#include "detection_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"

typedef struct {
    network net;
    char *filename;
    int class_id;
    int class_ides;
    float elo;
    float *elos;
} sortable_bbox;

int total_compares = 0;
int current_class_id = 0;

int elo_comparator(const void*a, const void *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    if(box1.elos[current_class_id] == box2.elos[current_class_id]) return 0;
    if(box1.elos[current_class_id] >  box2.elos[current_class_id]) return -1;
    return 1;
}

int bbox_comparator(const void *a, const void *b)
{
    ++total_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    network net = box1.net;
    int class_id   = box1.class_id;

    image im1 = load_image_color(box1.filename, net.w, net.h);
    image im2 = load_image_color(box2.filename, net.w, net.h);
    float *X  = (float*)calloc(net.w*net.h*net.c, sizeof(float));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*sizeof(float));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*sizeof(float));
    float *predictions = network_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    free(X);
    if (predictions[class_id*2] > predictions[class_id*2+1]){
        return 1;
    }
    return -1;
}

void bbox_update(sortable_bbox *a, sortable_bbox *b, int class_id, int result)
{
    int k = 32;
    float EA = 1./(1+pow(10, (b->elos[class_id] - a->elos[class_id])/400.));
    float EB = 1./(1+pow(10, (a->elos[class_id] - b->elos[class_id])/400.));
    float SA = result ? 1 : 0;
    float SB = result ? 0 : 1;
    a->elos[class_id] += k*(SA - EA);
    b->elos[class_id] += k*(SB - EB);
}

void bbox_fight(network net, sortable_bbox *a, sortable_bbox *b, int class_ides, int class_id)
{
    image im1 = load_image_color(a->filename, net.w, net.h);
    image im2 = load_image_color(b->filename, net.w, net.h);
    float *X  = (float*)calloc(net.w*net.h*net.c, sizeof(float));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*sizeof(float));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*sizeof(float));
    float *predictions = network_predict(net, X);
    ++total_compares;

    int i;
    for(i = 0; i < class_ides; ++i){
        if(class_id < 0 || class_id == i){
            int result = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, result);
        }
    }
    
    free_image(im1);
    free_image(im2);
    free(X);
}

void SortMaster3000(char *filename, char *weightfile)
{
    int i = 0;
    network net = parse_network_cfg(filename);
    if(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    char **paths = (char **)list_to_array(plist);
    int N = plist->size;
    free_list(plist);
    sortable_bbox *boxes = (sortable_bbox*)calloc(N, sizeof(sortable_bbox));
    printf("Sorting %d boxes...\n", N);
    for(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class_id = 7;
        boxes[i].elo = 1500;
    }
    clock_t time=clock();
    qsort(boxes, N, sizeof(sortable_bbox), bbox_comparator);
    for(i = 0; i < N; ++i){
        printf("%s\n", boxes[i].filename);
    }
    printf("Sorted in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

void BattleRoyaleWithCheese(char *filename, char *weightfile)
{
    int class_ides = 20;
    int i,j;
    network net = parse_network_cfg(filename);
    if(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    char **paths = (char **)list_to_array(plist);
    int N = plist->size;
    int total = N;
    free_list(plist);
    sortable_bbox *boxes = (sortable_bbox*)calloc(N, sizeof(sortable_bbox));
    printf("Battling %d boxes...\n", N);
    for(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class_ides = class_ides;
        boxes[i].elos = (float*)calloc(class_ides, sizeof(float));;
        for(j = 0; j < class_ides; ++j) {
            boxes[i].elos[j] = 1500;
        }
    }
    int round;
    clock_t time=clock();
    for(round = 1; round <= 4; ++round){
        clock_t round_time=clock();
        printf("Round: %d\n", round);
        shuffle(boxes, N, sizeof(sortable_bbox));
        for(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, class_ides, -1);
        }
        printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
    }

    int class_id;

    for (class_id = 0; class_id < class_ides; ++class_id){

        N = total;
        current_class_id = class_id;
        qsort(boxes, N, sizeof(sortable_bbox), elo_comparator);
        N /= 2;

        for(round = 1; round <= 100; ++round){
            clock_t round_time=clock();
            printf("Round: %d\n", round);

            sorta_shuffle(boxes, N, sizeof(sortable_bbox), 10);
            for(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, class_ides, class_id);
            }
            qsort(boxes, N, sizeof(sortable_bbox), elo_comparator);
            if(round <= 20) N = (N*9/10)/2*2;

            printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
        }
        char buff[256];
        sprintf(buff, "results/battle_%d.log", class_id);
        FILE *outfp = fopen(buff, "w");
        for(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class_id]);
        }
        fclose(outfp);
    }
    printf("Tournament in %d compares, %f secs\n", total_compares, sec(clock()-time));
}
