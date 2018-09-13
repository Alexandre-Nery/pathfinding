#include "libgraph.h"

int get_node_row(int x)
{
#pragma HLS INLINE
	return x >> 9;
}

int get_node_col(int x)
{
#pragma HLS INLINE
	return x - ((x >> 9)*512);
}
