#include "mp3player.h"
#include "vs10xx.h"	 
#include "delay.h"
#include "key.h"
#include "lcd.h"		 
#include "malloc.h"
#include "text.h"
#include "string.h"
#include "exfuns.h"
#include "fattester.h"	 
#include "ff.h"   
#include "flac.h"	
#include "stmflash.h"
#include "mp3id3.h"
#include "piclib.h"

_f_fftdev fftdev;	//定义fft变量管理结构体

u8 lcd_bit=0;

//音乐频谱分析补丁  必须加载此补丁才能正常显示音乐频谱
const u16 VS1053_PATCH[] = {
0x0007,0x0001,0x8d00,0x0006,0x0004,0x2803,0x5b40,0x0000,0x0024,0x0007,0x0001,0x8d02,
0x0006,0x00d6,0x3e12,0xb817,0x3e12,0x3815,0x3e05,0xb814,0x3615,0x0024,0x0000,0x800a,
0x3e10,0x3801,0x0006,0x0800,0x3e10,0xb803,0x0000,0x0303,0x3e11,0x3805,0x3e11,0xb807,
0x3e14,0x3812,0xb884,0x130c,0x3410,0x4024,0x4112,0x10d0,0x4010,0x008c,0x4010,0x0024,
0xf400,0x4012,0x3000,0x3840,0x3009,0x3801,0x0000,0x0041,0xfe02,0x0024,0x2903,0xb480,
0x48b2,0x0024,0x36f3,0x0844,0x6306,0x8845,0xae3a,0x8840,0xbf8e,0x8b41,0xac32,0xa846,
0xffc8,0xabc7,0x3e01,0x7800,0xf400,0x4480,0x6090,0x0024,0x6090,0x0024,0xf400,0x4015,
0x3009,0x3446,0x3009,0x37c7,0x3009,0x1800,0x3009,0x3844,0x48b3,0xe1e0,0x4882,0x4040,
0xfeca,0x0024,0x5ac2,0x0024,0x5a52,0x0024,0x4cc2,0x0024,0x48ba,0x4040,0x4eea,0x4801,
0x4eca,0x9800,0xff80,0x1bc1,0xf1eb,0xe3e2,0xf1ea,0x184c,0x4c8b,0xe5e4,0x48be,0x9804,
0x488e,0x41c6,0xfe82,0x0024,0x5a8e,0x0024,0x525e,0x1b85,0x4ffe,0x0024,0x48b6,0x41c6,
0x4dd6,0x48c7,0x4df6,0x0024,0xf1d6,0x0024,0xf1d6,0x0024,0x4eda,0x0024,0x0000,0x0fc3,
0x2903,0xb480,0x4e82,0x0024,0x4084,0x130c,0x0006,0x0500,0x3440,0x4024,0x4010,0x0024,
0xf400,0x4012,0x3200,0x4024,0xb132,0x0024,0x4214,0x0024,0xf224,0x0024,0x6230,0x0024,
0x0001,0x0001,0x2803,0x54c9,0x0000,0x0024,0xf400,0x40c2,0x3200,0x0024,0xff82,0x0024,
0x48b2,0x0024,0xb130,0x0024,0x6202,0x0024,0x003f,0xf001,0x2803,0x57d1,0x0000,0x1046,
0xfe64,0x0024,0x48be,0x0024,0x2803,0x58c0,0x3a01,0x8024,0x3200,0x0024,0xb010,0x0024,
0xc020,0x0024,0x3a00,0x0024,0x36f4,0x1812,0x36f1,0x9807,0x36f1,0x1805,0x36f0,0x9803,
0x36f0,0x1801,0x3405,0x9014,0x36f3,0x0024,0x36f2,0x1815,0x2000,0x0000,0x36f2,0x9817,
0x0007,0x0001,0x8d6d,0x0006,0x01f6,0x3613,0x0024,0x3e12,0xb817,0x3e12,0x3815,0x3e05,
0xb814,0x3645,0x0024,0x0000,0x800a,0x3e10,0xb803,0x3e11,0x3805,0x3e11,0xb811,0x3e14,
0xb813,0x3e13,0xf80e,0x4182,0x384d,0x0006,0x0912,0x2803,0x6105,0x0006,0x0451,0x0006,
0xc352,0x3100,0x8803,0x6238,0x1bcc,0x0000,0x0024,0x2803,0x7705,0x4194,0x0024,0x0006,
0x0912,0x3613,0x0024,0x0006,0x0411,0x0000,0x0302,0x3009,0x3850,0x0006,0x0410,0x3009,
0x3840,0x0000,0x1100,0x2914,0xbec0,0xb882,0xb801,0x0000,0x1000,0x0006,0x0810,0x2915,
0x7ac0,0xb882,0x0024,0x3900,0x9bc1,0x0006,0xc351,0x3009,0x1bc0,0x3009,0x1bd0,0x3009,
0x0404,0x0006,0x0451,0x2803,0x66c0,0x3901,0x0024,0x4448,0x0402,0x4294,0x0024,0x6498,
0x2402,0x001f,0x4002,0x6424,0x0024,0x0006,0x0411,0x2803,0x6611,0x0000,0x03ce,0x2403,
0x764e,0x0000,0x0013,0x0006,0x1a04,0x0006,0x0451,0x3100,0x8024,0xf224,0x44c5,0x4458,
0x0024,0xf400,0x4115,0x3500,0xc024,0x623c,0x0024,0x0000,0x0024,0x2803,0x7691,0x0000,
0x0024,0x4384,0x184c,0x3100,0x3800,0x2915,0x7dc0,0xf200,0x0024,0x003f,0xfec3,0x4084,
0x4491,0x3113,0x1bc0,0xa234,0x0024,0x0000,0x2003,0x6236,0x2402,0x0000,0x1003,0x2803,
0x6fc8,0x0000,0x0024,0x003f,0xf803,0x3100,0x8024,0xb236,0x0024,0x2803,0x75c0,0x3900,
0xc024,0x6236,0x0024,0x0000,0x0803,0x2803,0x7208,0x0000,0x0024,0x003f,0xfe03,0x3100,
0x8024,0xb236,0x0024,0x2803,0x75c0,0x3900,0xc024,0x6236,0x0024,0x0000,0x0403,0x2803,
0x7448,0x0000,0x0024,0x003f,0xff03,0x3100,0x8024,0xb236,0x0024,0x2803,0x75c0,0x3900,
0xc024,0x6236,0x0402,0x003f,0xff83,0x2803,0x75c8,0x0000,0x0024,0xb236,0x0024,0x3900,
0xc024,0xb884,0x07cc,0x3900,0x88cc,0x3313,0x0024,0x0006,0x0491,0x4194,0x2413,0x0006,
0x04d1,0x2803,0x9755,0x0006,0x0902,0x3423,0x0024,0x3c10,0x8024,0x3100,0xc024,0x4304,
0x0024,0x39f0,0x8024,0x3100,0x8024,0x3cf0,0x8024,0x0006,0x0902,0xb884,0x33c2,0x3c20,
0x8024,0x34d0,0xc024,0x6238,0x0024,0x0000,0x0024,0x2803,0x8dd8,0x4396,0x0024,0x2403,
0x8d83,0x0000,0x0024,0x3423,0x0024,0x34e4,0x4024,0x3123,0x0024,0x3100,0xc024,0x4304,
0x0024,0x4284,0x2402,0x0000,0x2003,0x2803,0x8b89,0x0000,0x0024,0x3423,0x184c,0x34f4,
0x4024,0x3004,0x844c,0x3100,0xb850,0x6236,0x0024,0x0006,0x0802,0x2803,0x81c8,0x4088,
0x1043,0x4336,0x1390,0x4234,0x0024,0x4234,0x0024,0xf400,0x4091,0x2903,0xa480,0x0003,
0x8308,0x4336,0x1390,0x4234,0x0024,0x4234,0x0024,0x2903,0x9a00,0xf400,0x4091,0x0004,
0x0003,0x3423,0x1bd0,0x3404,0x4024,0x3123,0x0024,0x3100,0x8024,0x6236,0x0024,0x0000,
0x4003,0x2803,0x85c8,0x0000,0x0024,0xb884,0x878c,0x3900,0x8024,0x34e4,0x4024,0x3123,
0x0024,0x31e0,0x8024,0x6236,0x0402,0x0000,0x0024,0x2803,0x8b88,0x4284,0x0024,0x0000,
0x0024,0x2803,0x8b95,0x0000,0x0024,0x3413,0x184c,0x3410,0x8024,0x3e10,0x8024,0x34e0,
0xc024,0x2903,0x4080,0x3e10,0xc024,0xf400,0x40d1,0x003f,0xff44,0x36e3,0x048c,0x3100,
0x8024,0xfe44,0x0024,0x48ba,0x0024,0x3901,0x0024,0x0000,0x00c3,0x3423,0x0024,0xf400,
0x4511,0x34e0,0x8024,0x4234,0x0024,0x39f0,0x8024,0x3100,0x8024,0x6294,0x0024,0x3900,
0x8024,0x0006,0x0411,0x6894,0x04c3,0xa234,0x0403,0x6238,0x0024,0x0000,0x0024,0x2803,
0x9741,0x0000,0x0024,0xb884,0x90cc,0x39f0,0x8024,0x3100,0x8024,0xb884,0x3382,0x3c20,
0x8024,0x34d0,0xc024,0x6238,0x0024,0x0006,0x0512,0x2803,0x9758,0x4396,0x0024,0x2403,
0x9703,0x0000,0x0024,0x0003,0xf002,0x3201,0x0024,0xb424,0x0024,0x0028,0x0002,0x2803,
0x9605,0x6246,0x0024,0x0004,0x0003,0x2803,0x95c1,0x4434,0x0024,0x0000,0x1003,0x6434,
0x0024,0x2803,0x9600,0x3a00,0x8024,0x3a00,0x8024,0x3213,0x104c,0xf400,0x4511,0x34f0,
0x8024,0x6294,0x0024,0x3900,0x8024,0x36f3,0x4024,0x36f3,0xd80e,0x36f4,0x9813,0x36f1,
0x9811,0x36f1,0x1805,0x36f0,0x9803,0x3405,0x9014,0x36f3,0x0024,0x36f2,0x1815,0x2000,
0x0000,0x36f2,0x9817,0x0007,0x0001,0x1868,0x0006,0x0010,0x0032,0x004f,0x007e,0x00c8,
0x013d,0x01f8,0x0320,0x04f6,0x07e0,0x0c80,0x13d8,0x1f7f,0x3200,0x4f5f,0x61a8,0x0000,
0x0007,0x0001,0x8e68,0x0006,0x0054,0x3e12,0xb814,0x0000,0x800a,0x3e10,0x3801,0x3e10,
0xb803,0x3e11,0x7806,0x3e11,0xf813,0x3e13,0xf80e,0x3e13,0x4024,0x3e04,0x7810,0x449a,
0x0040,0x0001,0x0003,0x2803,0xa344,0x4036,0x03c1,0x0003,0xffc2,0xb326,0x0024,0x0018,
0x0042,0x4326,0x4495,0x4024,0x40d2,0x0000,0x0180,0xa100,0x4090,0x0010,0x0fc2,0x4204,
0x0024,0xbc82,0x4091,0x459a,0x0024,0x0000,0x0054,0x2803,0xa244,0xbd86,0x4093,0x2403,
0xa205,0xfe01,0x5e0c,0x5c43,0x5f2d,0x5e46,0x020c,0x5c56,0x8a0c,0x5e53,0x5e0c,0x5c43,
0x5f2d,0x5e46,0x020c,0x5c56,0x8a0c,0x5e52,0x0024,0x4cb2,0x4405,0x0018,0x0044,0x654a,
0x0024,0x2803,0xb040,0x36f4,0x5810,0x0007,0x0001,0x8e92,0x0006,0x0080,0x3e12,0xb814,
0x0000,0x800a,0x3e10,0x3801,0x3e10,0xb803,0x3e11,0x7806,0x3e11,0xf813,0x3e13,0xf80e,
0x3e13,0x4024,0x3e04,0x7810,0x449a,0x0040,0x0000,0x0803,0x2803,0xaf04,0x30f0,0x4024,
0x0fff,0xfec2,0xa020,0x0024,0x0fff,0xff02,0xa122,0x0024,0x4036,0x0024,0x0000,0x1fc2,
0xb326,0x0024,0x0010,0x4002,0x4326,0x4495,0x4024,0x40d2,0x0000,0x0180,0xa100,0x4090,
0x0010,0x0042,0x4204,0x0024,0xbc82,0x4091,0x459a,0x0024,0x0000,0x0054,0x2803,0xae04,
0xbd86,0x4093,0x2403,0xadc5,0xfe01,0x5e0c,0x5c43,0x5f2d,0x5e46,0x0024,0x5c56,0x0024,
0x5e53,0x5e0c,0x5c43,0x5f2d,0x5e46,0x0024,0x5c56,0x0024,0x5e52,0x0024,0x4cb2,0x4405,
0x0010,0x4004,0x654a,0x9810,0x0000,0x0144,0xa54a,0x1bd1,0x0006,0x0413,0x3301,0xc444,
0x687e,0x2005,0xad76,0x8445,0x4ed6,0x8784,0x36f3,0x64c2,0xac72,0x8785,0x4ec2,0xa443,
0x3009,0x2440,0x3009,0x2741,0x36f3,0xd80e,0x36f1,0xd813,0x36f1,0x5806,0x36f0,0x9803,
0x36f0,0x1801,0x2000,0x0000,0x36f2,0x9814,0x0007,0x0001,0x8ed2,0x0006,0x000e,0x4c82,
0x0024,0x0000,0x0024,0x2000,0x0005,0xf5c2,0x0024,0x0000,0x0980,0x2000,0x0000,0x6010,
0x0024,0x000a,0x0001,0x0d00,
};

u16 FFTbuf[15];    //频谱数组
//新的频率值
const u16 VS_NEW_BANDS_FREQ_TBL[14]={80,300,800,1270,2016,3200,4500,6000,7500,9000,11000,13000,15000,20000};

//显示曲目索引
//index:当前索引
//total:总文件数
void mp3_index_show(u16 index, u16 total)
{
	//显示当前曲目的索引,及总曲目数
	if (lcd_bit < 10)
	{
		LCD_ShowxNum(224 + 0, 20, index, 3, 16, 0X80);		//索引
		LCD_ShowChar(224 + 24, 20, '/', 16, 0);
		LCD_ShowxNum(224 + 32, 20, total, 3, 16, 0X80); 	//总曲目	
	}
}
//显示当前音量
void mp3_vol_show(u8 vol)
{
	if (lcd_bit < 10)
	{
		LCD_ShowString(224, 40, 32, 16, 16, "VOL:");
		LCD_ShowxNum(224 + 32, 40, vol, 2, 16, 0X80); 	//显示音量	 
	}
}
u16 f_kbps=0;//歌曲文件位率	
//显示播放时间,比特率 信息 
//lenth:歌曲总长度
void mp3_msg_show(u32 lenth)
{
	static u16 playtime = 0;//播放时间标记
	u16 time = 0;// 时间变量
	u16 temp = 0;
	if(lcd_bit == 0)
	{
		if (f_kbps == 0xffff)//未更新过
		{
			playtime = 0;
			f_kbps = VS_Get_HeadInfo();//获得比特率
		}
		time = VS_Get_DecodeTime(); //得到解码时间

		if (playtime == 0)playtime = time;
		else if ((time != playtime) && (time != 0))//1s时间到,更新显示数据
		{
			playtime = time;//更新时间 	 			
			temp = VS_Get_HeadInfo(); //获得比特率	   				 
			if (temp != f_kbps)
			{
				f_kbps = temp;//更新KBPS	  				     
			}
			//显示播放时间			 
			LCD_ShowxNum(224, 60, time / 60, 2, 16, 0X80);		//分钟
			LCD_ShowChar(224 + 16, 60, ':', 16, 0);
			LCD_ShowxNum(224 + 24, 60, time % 60, 2, 16, 0X80);	//秒钟		
			LCD_ShowChar(224 + 40, 60, '/', 16, 0);
			//显示总时间
			if (f_kbps)time = (lenth / f_kbps) / 125;//得到秒钟数   (文件长度(字节)/(1000/8)/比特率=持续秒钟数    	  
			else time = 0;//非法位率	  
			LCD_ShowxNum(224 + 48, 60, time / 60, 2, 16, 0X80);	//分钟
			LCD_ShowChar(224 + 64, 60, ':', 16, 0);
			LCD_ShowxNum(224 + 72, 60, time % 60, 2, 16, 0X80);	//秒钟	  		    
			//显示位率			   
			LCD_ShowxNum(224, 80, f_kbps, 3, 16, 0X80); 	//显示位率	 
			LCD_ShowString(224 + 24, 80, 200, 16, 16, "Kbps");
		}
		VS_Get_Spec(FFTbuf); //提取频谱数据
		FFT_post(FFTbuf);	  //进行频谱效果显示
	}
}
//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 mp3_get_tnum(u8 *path)
{
	u8 res;
	u16 rval = 0;
	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息		
	u8 *fn;
	res = f_opendir(&tdir, (const TCHAR*)path); 	//打开目录
	tfileinfo.lfsize = _MAX_LFN * 2 + 1;				//长文件名最大长度
	tfileinfo.lfname = mymalloc(tfileinfo.lfsize);//为长文件缓存区分配内?
	if (res == FR_OK && tfileinfo.fname != NULL)
	{
		while (1)//查询总的有效文件数
		{
			res = f_readdir(&tdir, &tfileinfo);       		//读取目录下的一个文件
			if (res != FR_OK || tfileinfo.fname[0] == 0)break;	//错误了/到末尾了,退出		  
			fn = (u8*)(*tfileinfo.fname ? tfileinfo.fname : tfileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X40)//取高四位,看看是不是音乐文件	
			{
				rval++;//有效文件数增加1
			}
		}
	}
	myfree(tfileinfo.fname);
	return rval;
}
//初始化频谱管理数据
void init_fft(void)
{
	u8 i;
	for (i = 0; i < FFT_BANDS; i++)	//初始化频谱管理数据
	{
		fftdev.fft_cur[i] = 0;
		fftdev.fft_top[i] = 60;
		fftdev.fft_time[i] = 1;
	}
}
//播放音乐
void mp3_play(void)
{
	u8 res;
	DIR mp3dir;	 		//目录
	FILINFO mp3fileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	u16 totmp3num; 		//音乐文件总数
	u16 curindex;		//图片当前索引
	u8 key;					//键值		  
	u16 temp;
	u16 *mp3indextbl;	//音乐索引表 
	u16 id3head;

	init_fft();
	while (f_opendir(&mp3dir, "0:/MUSIC"))//打开图片文件夹
	{
		Show_Str(30, 20, 240, 16, "MUSIC文件夹错误!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 20, 240, 226, BLACK);//清除显示	     
		delay_ms(200);
	}
	totmp3num = mp3_get_tnum("0:/MUSIC"); //得到总有效文件数
	while (totmp3num == NULL)//音乐文件总数为0		
	{
		Show_Str(30, 20, 240, 16, "没有音乐文件!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 20, 240, 226, BLACK);//清除显示	     
		delay_ms(200);
	}
	mp3fileinfo.lfsize = _MAX_LFN * 2 + 1;				//长文件名最大长度
	mp3fileinfo.lfname = mymalloc(mp3fileinfo.lfsize);//为长文件缓存区分配内存
	pname = mymalloc(mp3fileinfo.lfsize);				//为带路径的文件名分配内存
	mp3indextbl = mymalloc(2 * totmp3num);				//申请2*totmp3num个字节的内存,用于存放音乐文件索引
	while (mp3fileinfo.lfname == NULL || pname == NULL || mp3indextbl == NULL)//内存分配出错
	{
		Show_Str(30, 20, 240, 16, "内存分配失败!", 16, 0);
		delay_ms(200);
		LCD_Fill(30, 20, 240, 226, BLACK);//清除显示	     
		delay_ms(200);
	}
	VS_HD_Reset();
	VS_Soft_Reset();
	//记录索引
	res = f_opendir(&mp3dir, "0:/MUSIC"); //打开目录
	if (res == FR_OK)
	{
		curindex = 0;//当前索引为0
		while (1)//全部查询一遍
		{
			temp = mp3dir.index;								//记录当前index
			res = f_readdir(&mp3dir, &mp3fileinfo);       		//读取目录下的一个文件
			if (res != FR_OK || mp3fileinfo.fname[0] == 0)break;	//错误了/到末尾了,退出		  
			fn = (u8*)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
			res = f_typetell(fn);
			if ((res & 0XF0) == 0X40)//取高四位,看看是不是音乐文件	
			{
				mp3indextbl[curindex] = temp;//记录索引
				curindex++;
			}
		}
	}
	curindex = STMFLASH_ReadHalfWord(0X08050000);
	if (curindex > totmp3num || curindex <= 0)
	{
		curindex = 0;
		Test_Write(0X08050000, curindex);
	}

	vsset.mvol = STMFLASH_ReadHalfWord(0X08050002);
	if (vsset.mvol > 200 || vsset.mvol <= 100)
	{
		vsset.mvol = 160;
		Test_Write(0X08050002, vsset.mvol);
	}

	res = f_opendir(&mp3dir, (const TCHAR*)"0:/MUSIC"); 	//打开目录
	while (res == FR_OK)//打开成功
	{
		dir_sdi(&mp3dir, mp3indextbl[curindex]);			//改变当前目录索引	   
		res = f_readdir(&mp3dir, &mp3fileinfo);       		//读取目录下的一个文件
		if (res != FR_OK || mp3fileinfo.fname[0] == 0)break;	//错误了/到末尾了,退出
		fn = (u8*)(*mp3fileinfo.lfname ? mp3fileinfo.lfname : mp3fileinfo.fname);
		strcpy((char*)pname, "0:/MUSIC/");				//复制路径(目录)
		strcat((char*)pname, (const char*)fn);  			//将文件名接在后面	

		id3head = mp3id3_is((const TCHAR*)pname, 1);

		LCD_Fill(0, 0, 320, 16, BLACK);				//清除之前的显示
		Show_Str(0, 0, 320, 16, fn, 16, 0);				//显示歌曲名字 
		mp3_vol_show((vsset.mvol - 100) / 5);
		mp3_index_show(curindex + 1, totmp3num);

		key = mp3_play_song(pname, id3head); 				 		//播放这个MP3    
		if (key == KEY1_PRES)		//上一曲
		{
			if (curindex)curindex--;
			else curindex = totmp3num - 1;
			Test_Write(0X08050000, curindex);
		}
		else if (key == KEY0_PRES)//下一曲
		{
			curindex++;
			if (curindex >= totmp3num)curindex = 0;//到末尾的时候,自动从头开始
			Test_Write(0X08050000, curindex);
		}
		else break;	//产生了错误 	 
	}
	myfree(mp3fileinfo.lfname);	//释放内存			    
	myfree(pname);				//释放内存			    
	myfree(mp3indextbl);			//释放内存	 
}

//播放一曲指定的歌曲	
//pname:歌曲路径+名字
//返回值:0,正常播放完成
//		 1,下一曲
//       2,上一曲
//       0XFF,出现错误了
u8 mp3_play_song(u8 *pname, u16 id3head)
{
	FIL* fmp3;
	u16 br;
	u8 res, rval;
	u8 *databuf;
	u16 i = 0;
	u8 key;

	static u8 pause = 0;		//暂停标志 
	rval = 0;
	fmp3 = (FIL*)mymalloc(sizeof(FIL));	//申请内存
	databuf = (u8*)mymalloc(4096);		//开辟4096字节的内存区域
	if (databuf == NULL || fmp3 == NULL)rval = 0XFF;//内存申请失败.
	if (rval == 0)
	{
		VS_Restart_Play();  					//重启播放 
		VS_Set_All();        					//设置音量等信息 			 
		VS_Reset_DecodeTime();					//复位解码时间 	  
		VS_Load_Patch((u16*)VS1053_PATCH, 1000); 	  //加载频谱分析补丁
		VS_Set_Bands((u16*)VS_NEW_BANDS_FREQ_TBL, FFT_BANDS);//重设频谱频率
		res = f_typetell(pname);	 	 			//得到文件后缀	 			

		if (res == 0x4c)//如果是flac,加载patch
		{
			VS_Load_Patch((u16*)vs1053b_patch, VS1053B_PATCHLEN);
		}
		res = f_open(fmp3, (const TCHAR*)pname, FA_READ);//打开文件
		f_lseek(fmp3, id3head);
		if (res == 0)//打开成功.
		{
			VS_SPI_SpeedHigh();	//高速						   
			while (rval == 0)
			{
				res = f_read(fmp3, databuf, 4096, (UINT*)&br);//读出4096个字节  
				i = 0;
				do//主播放循环
				{
					if ((VS_Send_MusicData(databuf + i) == 0) && (pause == 0))//给VS10XX发送音频数据
					{
						i += 32;
					}
					else
					{
						key = KEY_Scan(0);
						switch (key)
						{
						case KEY0_PRES:
							rval = KEY0_PRES;		//下一曲
							//LCD_LED = 1;
							//lcd_bit = 0;
							break;
						case KEY1_PRES:
							rval = KEY1_PRES;		//上一曲
							//LCD_LED = 1;
							//lcd_bit = 0;
							break;
						case KEY2_PRES:
							vsset.mvol = vsset.mvol + 10;
							if (vsset.mvol == 240)
							{
								vsset.mvol = 100;
							}
							mp3_vol_show((vsset.mvol - 100) / 5);	//音量限制在:100~250,显示的时候,按照公式(vol-100)/5,显示,也就是0~30   
							VS_Set_Vol(vsset.mvol);		
							LCD_LED = 1;
							lcd_bit = 0;
							Test_Write(0X08050002, vsset.mvol);
							mp3_vol_show((vsset.mvol - 100) / 5);
							break;
						case KEY3_PRES:	   //暂停/播放
							//pause = !pause;
							//LCD_LED = 1;
						if(lcd_bit==0)
						{
							lcd_bit = 1;
							LCD_LED=0;
						}
						else if(lcd_bit==1)
						{
							lcd_bit = 0;
							LCD_LED=1;
						}
							break;
						default:
							break;
						}
						mp3_msg_show(fmp3->fsize);//显示信息	    
					}
				} while (i < 4096);//循环发送4096个字节 
				if (br != 4096 || res != 0)
				{
					rval = KEY0_PRES;
					break;//读完了.	
				}
			}
			f_close(fmp3);
		}
		else rval = 0XFF;//出现错误	   	  
	}
	myfree(databuf);
	myfree(fmp3);
	return rval;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//显示一根柱子
//x,y,width,height:位置和尺寸
//curval:当值
//topval:最大值
void fft_show_oneband(u16 x, u16 y, u16 width, u16 height, u16 curval, u16 topval)
{
	gui_fill_rectangle(x, y, width, height - curval, MP3_MAIN_BKCOLOR);					//填充背景色
	gui_fill_rectangle(x, y + height - curval, width, curval, FFT_BANDS_COLOR);	//填充柱状色
	gui_draw_hline(x, y + height - topval - 1, width, FFT_TOP_COLOR);
}
//显示FFT_BANDS根柱子
//mp3devx:MP3结构体
//pdt:频谱数据
void FFT_post(u16 *pbuf)
{
	u8 i = 0;
	u8 temp;
	for (i = 0; i < FFT_BANDS; i++)	//显示各个频谱	   循环显示14个段
	{

		temp = (pbuf[i] & 0X3F) * 3; 			//得到当前值,并乘2倍 主要为增加显示效果	因为输出的频率都相对较低

		if (fftdev.fft_cur[i] < temp) 	  //当前值小于temp
			fftdev.fft_cur[i] = temp;
		else							  //当前值大于等于temp	 开始往下降 一次降2
		{
			if (fftdev.fft_cur[i] > 1)fftdev.fft_cur[i] -= 1;
			else fftdev.fft_cur[i] = 0;
		}

		if (fftdev.fft_cur[i] > fftdev.fft_top[i])//当前值大于峰值时 更新峰值
		{
			fftdev.fft_top[i] = fftdev.fft_cur[i];
			fftdev.fft_time[i] = 1;               //重设峰值停顿时间
		}

		if (fftdev.fft_time[i])fftdev.fft_time[i]--;   //如果停顿时间大于1 即未减完
		else 										   //停顿时间已减没
		{
			if (fftdev.fft_top[i]) fftdev.fft_top[i]--;   //峰值下降1
		}


		if (fftdev.fft_cur[i] > 63)fftdev.fft_cur[i] = 63;	  //保证在范围内 因为前面有增倍效果
		if (fftdev.fft_top[i] > 63)fftdev.fft_top[i] = 63;

		fft_show_oneband(224 + i * 6, 130, 6, 80, fftdev.fft_cur[i], fftdev.fft_top[i]);//显示柱子	   
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



























