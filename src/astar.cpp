#include "astar.h"

static int path[N_NODES];
static int costs[N_NODES];

void init()
{
#pragma HLS INLINE
	for(int i = 0 ; i < N_NODES ; i=i+4)
	{
		#pragma HLS PIPELINE
		path[i] = -1;
		path[i+1] = -1;
		path[i+2] = -1;
		path[i+3] = -1;
		costs[i] = -1;
		costs[i+1] = -1;
		costs[i+2] = -1;
		costs[i+3] = -1;
	}
}

void neighborhood(int *neig, int node)
{
#pragma HLS INLINE

	int row = get_node_row(node);
	int col = get_node_col(node);

/*	neig[0] = (row > 0 && col > 0) ? (node - (WIDTH+1)):(-1); //NorthWest
	neig[1] = (row > 0) ? (node - (WIDTH)):(-1); //North
	neig[2] = (row > 0 && col < WIDTH-1) ? (node - (WIDTH-1)) : (-1); //NorthEast
	neig[3] = (col > 0) ? (node - 1): (-1); //West
	neig[4] = (col < WIDTH-1) ? (node + 1) : (-1); //East
	neig[5] = (row < HEIGHT-1 && col < WIDTH-1) ? (node + (WIDTH+1)) : (-1); //SouthEast
	neig[6] = (row < WIDTH-1) ? (node + WIDTH) : (-1); //South
	neig[7] = (row < WIDTH-1 && col > 0) ? (node + (WIDTH-1)) : (-1); //SouthWest
*/

	neig[0] = (row > 0) ? (node - (WIDTH)):(-1); //North
	neig[1] = (col > 0) ? (node - 1): (-1); //West
	neig[2] = (col < WIDTH-1) ? (node + 1) : (-1); //East
	neig[3] = (row < WIDTH-1) ? (node + WIDTH) : (-1); //South
}

int m_abs(int x)
{
#pragma HLS INLINE
	if(x < 0)
		return -x;

	return x;
}

int heuristic(int v, int n)
{
#pragma HLS INLINE
	int x1 = get_node_row(v);
	int y1 = get_node_col(v);
	int x2 = get_node_row(n);
	int y2 = get_node_col(n);
	return (m_abs(x2 - x1) + m_abs(y2 - y1));
}

void astar(volatile uchar *g, volatile int *p, int start_node, int end_node) {
	#pragma HLS INTERFACE m_axi depth=262144 port=g offset=slave
	#pragma HLS INTERFACE m_axi depth=262144 port=p offset=slave
	#pragma HLS INTERFACE s_axilite port=g bundle=AXI_Lite1
	#pragma HLS INTERFACE s_axilite port=p bundle=AXI_Lite1
	#pragma HLS INTERFACE s_axilite port=start_node bundle=AXI_Lite1
	#pragma HLS INTERFACE s_axilite port=end_node bundle=AXI_Lite1
	#pragma HLS INTERFACE s_axilite port=return bundle=AXI_Lite1

	#pragma HLS ARRAY_PARTITION variable=graph cyclic factor=4 dim=1
	#pragma HLS ARRAY_PARTITION variable=path cyclic factor=4 dim=1
	#pragma HLS ARRAY_PARTITION variable=costs cyclic factor=4 dim=1

	memcpy(graph,(const int*)g,262144*sizeof(uchar));

	int i;
	int n1,n2,n3,n4;//,n5,n6,n7,n8;
	int new_cost1,new_cost2,new_cost3,new_cost4;//,new_cost5,new_cost6,new_cost7,new_cost8;
	int18 prior1,prior2,prior3,prior4;//,prior5,prior6,prior7,prior8;

	init();

	//for(i = 0 ; i < 100 ; i++)
	//	printf("%d = %d,%d\n",i,x_coord[i],y_coord[i]);

	list_t perim;

	//int neig[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	int neig[4] = {-1,-1,-1,-1};

	init_list(&perim);

	append_item_prior(&perim, start_node,0);

	costs[start_node] = 0;

	frontier_loop: while (!is_empty(&perim)) {
		#pragma HLS LOOP_TRIPCOUNT min=65000 max=131072
		int v = remove_first(&perim);

		if (v == end_node)
			break;

		neighborhood(neig, v);

		n1 = neig[0];
		n2 = neig[1];
		n3 = neig[2];
		n4 = neig[3];

		if (n1 >= 0) {
			if (graph[n1] > 0 && graph[n1] < 255) {
				new_cost1 = costs[v] + graph[n1];
				prior1 = new_cost1 + heuristic(n1, end_node);
				if (costs[n1] < 0 || new_cost1 < costs[n1]) {
					if(append_item_prior(&perim, n1, prior1))
					{
						costs[n1] = new_cost1;
						path[n1] = v;
					}
				}
			}
		}

		if (n2 >= 0) {
			if (graph[n2] > 0 && graph[n2] < 255) {
				new_cost2 = costs[v] + graph[n2];
				prior2 = new_cost2 + heuristic(n2, end_node);
				if (costs[n2] < 0 || new_cost2 < costs[n2]) {
					if(append_item_prior(&perim, n2, prior2))
					{
						costs[n2] = new_cost2;
						path[n2] = v;
					}
				}
			}
		}

		if (n3 >= 0) {
			if (graph[n3] > 0 && graph[n3] < 255) {
				new_cost3 = costs[v] + graph[n3];
				prior3 = new_cost3 + heuristic(n3, end_node);
				if (costs[n3] < 0 || new_cost3 < costs[n3]) {
					if(append_item_prior(&perim, n3, prior3))
					{
						costs[n3] = new_cost3;
						path[n3] = v;
					}
				}
			}
		}

		if (n4 >= 0) {
			if (graph[n4] > 0 && graph[n4] < 255) {
				new_cost4 = costs[v] + graph[n4];
				prior4 = new_cost4 + heuristic(n4, end_node);
				if (costs[n4] < 0 || new_cost4 < costs[n4]) {
					if(append_item_prior(&perim, n4, prior4))
					{
						costs[n4] = new_cost4;
						path[n4] = v;
					}
				}
			}
		}

/*
		if (n5 >= 0) {
			if (graph[n5] > 0 && graph[n5] < 255) {
				new_cost5 = costs[v] + graph[n5];
				prior5 = new_cost5 + heuristic(n5, end_node);
				if (costs[n5] < 0 || new_cost5 < costs[n5]) {
					if(append_item_prior(&perim, n5, prior5))
					{
						costs[n5] = new_cost5;
						path[n5] = v;
					}
				}
			}
		}

		if (n6 >= 0) {
			if (graph[n6] > 0 && graph[n6] < 255) {
				new_cost6 = costs[v] + graph[n6];
				prior6 = new_cost6 + heuristic(n6, end_node);
				if (costs[n6] < 0 || new_cost6 < costs[n6]) {
					if(append_item_prior(&perim, n6, prior6))
					{
						costs[n6] = new_cost6;
						path[n6] = v;
					}
				}
			}
		}

		if (n7 >= 0) {
			if (graph[n7] > 0 && graph[n7] < 255) {
				new_cost7 = costs[v] + graph[n7];
				prior7 = new_cost7 + heuristic(n7, end_node);
				if (costs[n7] < 0 || new_cost7 < costs[n7]) {
					if(append_item_prior(&perim, n7, prior7))
					{
						costs[n7] = new_cost7;
						path[n7] = v;
					}
				}
			}
		}


		if (n8 >= 0) {
			if (graph[n8] > 0 && graph[n8] < 255) {
				new_cost8 = costs[v] + graph[n8];
				prior8 = new_cost8 + heuristic(n8, end_node);
				if (costs[n8] < 0 || new_cost8 < costs[n8]) {
					if(append_item_prior(&perim, n8, prior8))
					{
						costs[n8] = new_cost8;
						path[n8] = v;
					}
				}
			}
		}*/
	}

	memcpy((int*)p,path,N_NODES*sizeof(int));
}

