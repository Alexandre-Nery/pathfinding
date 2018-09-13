#ifndef _ASTAR_H
#define _ASTAR_H

#include "ap_axi_sdata.h"
#include "liblist.h"
#include "libgraph.h"

void astar(volatile uchar *addr, volatile int *cf, int start_node, int end_node);

#endif
