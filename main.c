#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#define RING_LENGTH 4096



typedef struct n
{
	uint8_t data;
	struct n *next;
}node;

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
}
iter->next=start;
return start;

}

uint32_t ring_len(node* start)
{
	node* iter=NULL;
	uint32_t len=0;
	for (iter=start->next; (iter!=start); iter=iter->next)
	{
		len++;
	}
return len;
}

uint8_t read()
{
	static uint8_t a =0;
	return a++;
	//return rand(); 
}

node* fill_ring(uint8_t data,node* write)
{
	write->data = data;
	return write->next;
}

node* read_ring(uint8_t* data,node* read)
{
	*data = read->data;
	return read->next;
}


int main()
{
node* r_ptr=NULL;
node* w_ptr=NULL;
uint8_t data=0;
srand(time(NULL));

node* start = init_ring(RING_LENGTH);
r_ptr=start;
w_ptr=start;

printf("len %d\n",ring_len(start));
int i=0;

while(1)
{
	i=((uint8_t)rand())/4;
	printf("bytes to be read %d\n",i);
	while(i--)
		w_ptr = fill_ring(read(),w_ptr);
	
	while(r_ptr!=w_ptr)
	{
		r_ptr = read_ring(&data , r_ptr);
		printf("%d\n",data);
		usleep(10000);
	}

}
return 0;
}

#if 0
void find_gps_str(node*node* read, uint8_t* str)
{
	node* iter = read;
	while(iter->data = 0xe0)
		iter=iter->next;
	
}
#endif