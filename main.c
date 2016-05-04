#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#define RING_LENGTH 4096
#define DEBUG
#ifdef DEBUG
    #define DBG(fmt, args...);   printf("" fmt, ##args)
#else   
    #define DBG(fmt, args...)
#endif



typedef struct n
{
	uint8_t data;
	struct n *next;
}node;

typedef struct buf
{
	node* w_ptr;
	node* r_ptr;
}gps_ring;

node* init_ring(uint32_t num)
{

node* start = (node*) malloc(sizeof(node));
node* iter=NULL;
uint32_t i=0;
for (iter=start;i<num;i++)
{
	iter->next = (node*) malloc(sizeof(node));
	iter->data = i;
	iter=iter->next;
	DBG("%d\n",i);
}
iter->next=start;
return start;

}

void gps_init(gps_ring* gps)
{
	gps->w_ptr=NULL;
	gps->r_ptr=NULL;
	gps->w_ptr=init_ring(RING_LENGTH);
	gps->r_ptr=gps->w_ptr;

}

uint32_t ring_len(gps_ring *gps)
{
	node* iter=NULL;
	uint32_t len=0;
	iter=gps->w_ptr->next;
	#if 1
	for (; (iter!=gps->w_ptr); iter=iter->next)
	{
		len++;
	}
	#endif
return len;
}

uint8_t read()
{
	static uint8_t a =0;
	return a++;
	//return rand(); 
}

void fill_ring(uint8_t data,gps_ring *gps)
{
	gps->w_ptr->data = data;
	gps->w_ptr=gps->w_ptr->next;
}

read_ring(uint8_t* data,gps_ring *gps)
{
	*data = gps->r_ptr->data;
	gps->r_ptr=gps->r_ptr->next;
}


int main()
{
uint8_t data;
uint32_t i=0;
srand(time(NULL));
gps_ring gps1;
gps_init(&gps1);
printf("len %d\n",ring_len(&gps1));

#if 1
while(1)
{
	i=((uint8_t)rand())/4;
	printf("bytes to be read %d\n",i);
	while(i--)
		fill_ring(read(),&gps1);
	
	while(gps1.r_ptr!=gps1.w_ptr)
	{
	    read_ring(&data , &gps1);
		printf("%d\n",data);
		usleep(10000);
	}

}
#endif
return 0;
}

#if 0
void find_gps_str(node* read, uint8_t* str)
{
	node* iter = read;
	while(iter->data = 0xe0)
		iter=iter->next;
	
}
#endif