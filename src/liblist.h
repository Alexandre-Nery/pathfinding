#ifndef _LIBLIST_H
#define _LIBLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ap_cint.h>

#define MAX_LIST_SIZE (512*512)

typedef unsigned int uint;

/*typedef struct _item_t{
    int data;
    float priority;
} item_t;

typedef item_t * item;*/

typedef struct{
    int18 size;
    int18 last_index;
    int18 data[MAX_LIST_SIZE];
    int18 prior[MAX_LIST_SIZE];
} list_t;

typedef list_t * list;

void init_list(list l);
bool is_full(list l);
bool is_empty(list l);
void append_item(list l, int data);
bool append_item_prior(list l, int data, int p);
int remove_last(list l);
int remove_first(list l);
int shift_list_l(list l);
void shift_list_r(list l);
void print_list_of_integers(list l);
void print_list_of_integers_prior(list l);
int contains(list l, int node);

#endif
