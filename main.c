#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define RING_LENGTH 200
#define DEBUG
#ifdef DEBUG
    #define DBG(fmt, args...);   printf("" fmt, ##args)
#else   
    #define DBG(fmt, args...)
#endif

//GPS struct
typedef struct{
double timeoffix;
double latitude;
double longitude;
char NorS;
char EorW;
int SAT;
double altitude;
}GPSinfo;


typedef struct n
{
	uint8_t data;
	uint16_t index;
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
for (iter=start;i<(num-1);i++)
{
	iter->next = (node*) malloc(sizeof(node));
	iter->data = i;
	iter=iter->next;
	iter->index=i;
	//DBG("%d\n",i);
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

uint8_t read(uint8_t *buf,int i)
{
	//static uint8_t a =0;
	//return a++;
	//return rand(); 
	return buf[i];
}

void fill_ring(uint8_t data,gps_ring *gps)
{
	//DBG("%d %c\n",gps->w_ptr->index,data);
	gps->w_ptr->data = data;
	gps->w_ptr=gps->w_ptr->next;
	
}

read_ring(uint8_t* data,gps_ring *gps)
{
	*data = gps->r_ptr->data;
	gps->r_ptr=gps->r_ptr->next;
}

uint8_t find_char_ring(gps_ring *gps, node* iter,uint8_t c)
{
	uint8_t ret=0;
	//DBG("finding %c\n",c);
#if 1
	for(iter=gps->r_ptr;iter!=gps->w_ptr;iter=iter->next)
		{
			
			//DBG("%c\n",iter->data);
			if (iter->data == c)
			{
				//DBG("found %c at %d\n",iter->data,iter->index);
				ret =  0;
				break;
			}
			else
			{
				ret=1;
			}

		}
		//DBG("\n");
		#endif
		return ret;
}

uint8_t hexchartoint(uint8_t src,uint8_t* dest)
{
    if ((src <='F') && (src >= 'A'))
    {
        *dest = src - 'A'+10;
    }
    if ((src <='f') && (src >= 'a'))
    {
        *dest = src - 'a'+10;
    }

    if ((src <='9') && (src >= '0'))
    {
        *dest = src - '0';
    }
    
    return 1;
}

uint8_t converthex2int(uint8_t* hexstr)
{
    uint8_t msb, lsb;
    hexchartoint(hexstr[0],&msb);
    hexchartoint(hexstr[1],&lsb);
    return (16*msb+lsb);
}

uint8_t gps_str_checksum(node* dollar, node* star)
{
    uint8_t c = 0;
    node* iter=dollar->next;
    while(iter!=star)
        {
            c ^= iter->data;
            iter=iter->next;
        }
    return c;
}



#if 1
int find_gps_str(uint8_t* str,gps_ring* gps)
{
	node *iter = gps->r_ptr;
	node *dollar=NULL;
	node *star=NULL;
	uint8_t read_csum=0;
	uint8_t calc_csum=0;
	//find $
	for(iter=gps->r_ptr;iter!=gps->w_ptr;iter=iter->next)
		{
			if (iter->data == '$')
			{
				//DBG("found %c at %d\n",iter->data,iter->index);
				break;
			}
		}
	dollar = iter;
	//DBG("dollar %c at %d\n",dollar->data, dollar->index);
	//find star
	for(iter=dollar;iter!=gps->w_ptr;iter=iter->next)
		{
			if (iter->data == '*')
			{
				//DBG("found %c at %d\n",iter->data,iter->index);
				break;
			}
		}
	star = iter;
	//DBG("star %c at %d\n",star->data, star->index);

	
	//read checksum
    uint8_t hexstr[2];
    uint8_t i=0;
    hexstr[0]=star->next->data;
    hexstr[1]=star->next->next->data;
    //DBG("\ncsum msb%c lsb%c\n",hexstr[0],hexstr[1]);
    read_csum=converthex2int(hexstr);
    calc_csum=gps_str_checksum(dollar,star);
	//DBG("\nread_csum is %02X\n",read_csum);
	//DBG("calc csum is %02X\n",calc_csum);
	if (read_csum==calc_csum)
	{
	for(iter=dollar; iter!= star->next->next->next; iter=iter->next)
		{
			//DBG("%c",iter->data);
			str[i++]=iter->data;
		}
		gps->r_ptr=iter;
		str[i]='\0';
		//DBG("\n");
	}
	else
	{
		return 1;
	}
	#endif
	return 0;
}

int8_t nmeaProcessGPGGA(GPSinfo *gps, char* packet)
 {
     
     uint8_t i;
     char* endptr;
     //int c = gpgga_checksum((packet+1));
     if (!strncmp(packet,"$GPGGA",6)==0){
        return 0;
     }
     //printf("start parsing\n");
     DBG("parsing %s\n",packet);
     // start parsing just after "GPGGA,"
     i = 7;
   
     // get UTC time [hhmmss.sss]
     //GpsInfo.PosLLA.TimeOfFix.f = strtod(&packet[i], &endptr);
     gps->timeoffix = strtod(&packet[i], &endptr);
     DBG("time is %f\n",gps->timeoffix);

     while(packet[i++] != ',');              // next field: latitude
     
     // get latitude [ddmm.mmmmm]
     gps->latitude = strtod(&packet[i], &endptr);
     DBG("latitude %f",gps->latitude);
     while(packet[i++] != ',');              // next field: N/S indicator
     
     // correct latitute for N/S
     gps->NorS=packet[i];
     DBG("%c\n",gps->NorS);
     while(packet[i++] != ',');              // next field: longitude
     
     // get longitude [ddmm.mmmmm]
     gps->longitude = strtod(&packet[i], &endptr);
     DBG("longitude %f",gps->longitude);
     while(packet[i++] != ',');              // next field: E/W indicator
 
     // correct latitute for E/W
     gps->EorW=packet[i];
     DBG("%c\n",gps->EorW);
     while(packet[i++] != ',');              // next field: position fix status
 
     // position fix status
     // 0 = Invalid, 1 = Valid SPS, 2 = Valid DGPS, 3 = Valid PPS
     // check for good position fix
     if( (packet[i] != '0') && (packet[i] != ',') )
         //GpsInfo.PosLLA.updates++;
     while(packet[i++] != ',');              // next field: satellites used
     
     // get number of satellites used in GPS solution
     gps->SAT = atoi(&packet[i]);
     DBG("SAT %d\n",gps->SAT);
     while(packet[i++] != ',');              // next field: HDOP (horizontal dilution of precision)
     while(packet[i++] != ',');              // next field: altitude
     
     // get altitude (in meters)
     gps->altitude = strtod(&packet[i], &endptr);
     DBG("Altitude %f\n",gps->altitude);

     while(packet[i++] != ',');              // next field: altitude units, always 'M'
     while(packet[i++] != ',');              // next field: geoid seperation
     while(packet[i++] != ',');              // next field: seperation units
     while(packet[i++] != ',');              // next field: DGPS age
     while(packet[i++] != ',');              // next field: DGPS station ID
     while(packet[i++] != '*');              // next field: checksum
     
    return 1;
 }



int main()
{
uint8_t data;
uint32_t i=0;
srand(time(NULL));


gps_ring gps1;
gps_init(&gps1);
GPSinfo gps_info;
DBG("len %d\n",ring_len(&gps1));
char str[100];
FILE *fp=NULL;
fp = fopen( "GPS.txt" , "r+" );
int ctr=0;
uint8_t byte_buf[200];
node iter;
#if 1
while(1)
{
	i = (unsigned int) fread(byte_buf, 1, 100, fp);
    if(i==0)
    	exit(0);
	//i=((uint8_t)rand())/4;
	//printf("bytes to be read %d\n",i);
	while(i--)
		fill_ring(read(byte_buf,ctr++),&gps1);
	ctr=0;
	
	find_gps_str(str,&gps1);
	//printf("%s\n",str);
	nmeaProcessGPGGA(&gps_info, str);
	//DBG("timeoffix %g\n",gps_info.timeoffix);
	
}
#endif
return 0;
}

