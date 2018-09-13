#ifndef _LIBGRAPH_H
#define _LIBGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblist.h"
//#include "util.h"

#define DELIM " ;,\"!={}[]/\n"

#define WIDTH 512
#define HEIGHT 512
#define N_NODES (WIDTH*HEIGHT)

typedef unsigned char uchar;

static uchar graph[N_NODES];

int get_node_row(int x);
int get_node_col(int x);

#endif
