#include <stdio.h>
//tmp for test , instead of #include "cvt.h"
enum stream_type_t {
	VI_H264 = 0,
	AU_MP2,
	AU_G711A,
	AU_G711U,
	AU_AAC,
	AU_AACLD,
	AU_G729A,
	ST_END
};




struct statistical_t
{
	long long recv_total;
	long long lost;
	long long delta;
	long long sequence_err;
};

struct sequ_err_t
{
	int lost_begin;
	int lost_count;
};

struct lost_t
{
	//short cur_sequ;
	long long  previous_sequ;
	long long  tmp_lost;
};

static struct statistical_t sta = {0,0,0,0};
static struct sequ_err_t se = {0,0};
static struct lost_t lo = {-1,-1};

static int sequ_err_cclt(int current_sequ , int lost_begin , int lost_count)
{
	if(current_sequ == 0)
	{
		se.lost_begin = lost_begin;
		se.lost_count = lost_count;	
		return 0;
	}


	if(lost_begin == 0  && current_sequ > se.lost_begin && current_sequ < (se.lost_begin + se.lost_count))
	{
		sta.lost--;
		sta.sequence_err++;
		return 0;
	}

}

static int lost_cclt(int cur_sequ)
{
	
	
	lo.tmp_lost = cur_sequ - lo.previous_sequ;//max cur : 127 - -1 = 128

	if(lo.previous_sequ == -1)//first test
	{				
		;
	}
	else if(lo.tmp_lost > 1 && lo.tmp_lost < 127)//lost happen
	{

		sequ_err_cclt(0 , cur_sequ , lo.tmp_lost);//new lost , reset sequ_err 
	}
	else
	{
		sequ_err_cclt(cur_sequ , 0, 0);//this pkt not lost
	}
	
	lo.previous_sequ = cur_sequ;
	return 0;

}




struct statistical_t * statistical(char * head , int type)
{
	static short ws_sequ[2] = {-1,-1};

	sta.recv_total++;

	if(type == AU_MP2)
	{
		short tmp_lost = 0;

		ws_sequ[1] = *((short*)(head + 2)) % 128;
		tmp_lost = ws_sequ[1] - ws_sequ[0];

		if(ws_sequ[0] > 128)
		{
			ws_sequ[0] = *((short*)(head + 2))%128;
		}
		else if(0 < tmp_lost  && 128 > tmp_lost )
		{
			sta.lost += tmp_lost;

		}

		ws_sequ[0] = *((short*)(head + 2))%128;



	}
	else
	{

	}
}

int main()
{
	short a = -1;
	short b = 12;
	short c = b - a ;
	printf(" %d , %x  , %d\n",c ,c , c > 128);
}
