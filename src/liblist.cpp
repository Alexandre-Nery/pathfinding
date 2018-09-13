#include "liblist.h"

void init_list(list l)
{
#pragma HLS INLINE
    l->size = 0;
}

bool is_full(list l)
{
#pragma HLS INLINE
    if(l->size == MAX_LIST_SIZE)
            return true;

    return false;
}

bool is_empty(list l)
{
#pragma HLS INLINE
    if(l->size == 0)
            return true;

    return false;
}

void shift_list_r(list l)
{
#pragma HLS INLINE
    int i;
    if(!is_full(l))
    {
        for(i = l->size-1 ; i >= 0; i--)
        {
            l->data[i+1] = l->data[i];
            l->prior[i+1] = l->prior[i];
        }
    }
    l->data[0] = -1;
    l->prior[0] = -1;
}

int shift_list_l(list l)
{
#pragma HLS INLINE
    int i;
    int v;
    v = -1;
    if(!is_empty(l))
    {
        v = l->data[0];
        for(i = 0 ; i < l->size; i++)
        {
            l->data[i] = l->data[i+1];
            l->prior[i] = l->prior[i+1];
        }
    }
    return v;
}
void append_item(list l, int data)
{
#pragma HLS INLINE
    int i;
    int index;

    if(!is_full(l))
    {
    	l->data[l->size] = data;
        l->size++;
    }

}

bool append_item_prior(list l, int data, int p)
{
#pragma HLS INLINE
    int i;
    int index;

    if(is_empty(l))
    {
        l->data[0] = data;
        l->prior[0] = p;
        l->last_index = 0;
        l->size++;
    	//printf("empty, thus %f\n",p);
        return true;
    }
    else
    {
        if(!is_full(l))
        {
            if(p < l->prior[0]) //before first element
            {
            	//printf("%f < %f\n",p,l->prior[0]);
                shift_list_r(l);
                l->data[0] = data;
                l->prior[0] = p;
                l->last_index = 0;
                l->size++;
                return true;
            }
            else if(p >= l->prior[l->size-1]) //after last element
            {
            	//printf("%f > %f\n",p,l->prior[l->size-1]);
                l->data[l->size] = data;
                l->prior[l->size] = p;
                l->last_index = l->size;
                l->size++;
                return true;
            }
            else
            {
            	if(p < l->prior[l->last_index])
            		i = 0;
            	else
            		i = l->last_index;

            	while(p >= l->prior[i])
            		i++;

        		index = i;

                for(i = l->size-1 ; i >= index ; i--)
                {
                    l->data[i+1] = l->data[i];
                    l->prior[i+1] = l->prior[i];
                }

                l->data[index] = data;
                l->prior[index] = p;
                l->last_index = index;
                l->size++;
                return true;
            }
        }
        else
        {
        	return false;
        }
    }
    return false;
}

int remove_first(list l)
{
#pragma HLS INLINE
    int t;
    t = -1;
    if(!is_empty(l))
    {
        if(l->size == 1)
        {
            t = l->data[0];
            l->size--;
        }
        else
        {
            t = shift_list_l(l);
            l->size--;
        }
    }
    l->last_index--;
    return t;
}

int remove_last(list l)
{
	#pragma HLS INLINE

    int t;
    t = -1;
    if(!is_empty(l))
    {
        l->size--;
        t = l->data[l->size];
    }
    l->last_index--;
    return t;
}

int contains(list l, int node)
{
#pragma HLS INLINE
    int i;
    if(!is_empty(l))
    {
        for(i = 0 ; i < l->size ; i++)
        {
        	if(l->data[i] == node)
        		return (0==0);
        }
    }
    return (0==1);
}


void print_list_of_integers(list l)
{
#pragma HLS INLINE
    int i;
    if(!is_empty(l))
    {
        printf("[");
        for(i = 0 ; i < l->size ; i++)
        {
            printf("%d",l->data[i]);
            if(i < l->size-1)
                printf(",");
        }

        printf("]\n");
    }
}

void print_list_of_integers_prior(list l)
{
#pragma HLS INLINE
    int i;
    if(!is_empty(l))
    {
        printf("[");
        for(i = 0 ; i < l->size ; i++)
        {
            printf("%d(%d)",l->prior[i],l->data[i]);
            if(i < l->size-1)
                printf(",");
        }

        printf("]\n");
    }
}
