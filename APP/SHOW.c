#include "show.h"
#include "lunar.h"
#include "image2lcd.h"
#include "image1.h"
#include "lcd.h"
#include "text.h"
#include "rtc.h"
#include "delay.h"
#include "key.h"
#include "vs10xx.h"
#include "mp3player.h"

u8 lcd_bit=0;

_lunar_obj moon;
/*显示生肖-----------------------------------------------------*/
void displaysx(void) {
	unsigned char LunarYearD, ReYear, tiangan, dizhi;        //?????????  ??????? 
	   //?????,??????,?c_moon????,year_moon???,month_moon,day_moon????BCD??
	LunarYearD = (moon.year_moon / 16) * 10 + moon.year_moon % 16;        //??????10??? 
	ReYear = LunarYearD % 12;                             //????12,????
	tiangan = LunarYearD % 10;         //??  ???.....
	dizhi = LunarYearD % 12;         //??  ???.....

	switch (tiangan)	//天干
	{
	case 0: Show_Str(10, 130, 24, 24, "庚", 24, 0); break;   //余0即整除 天干  庚
	case 1: Show_Str(10, 130, 24, 24, "辛", 24, 0); break;   //
	case 2: Show_Str(10, 130, 24, 24, "壬", 24, 0); break;
	case 3: Show_Str(10, 130, 24, 24, "癸", 24, 0); break;
	case 4: Show_Str(10, 130, 24, 24, "甲", 24, 0); break;
	case 5: Show_Str(10, 130, 24, 24, "乙", 24, 0); break;
	case 6: Show_Str(10, 130, 24, 24, "丙", 24, 0); break;
	case 7: Show_Str(10, 130, 24, 24, "丁", 24, 0); break;
	case 8: Show_Str(10, 130, 24, 24, "戊", 24, 0); break;
	case 9: Show_Str(10, 130, 24, 24, "已", 24, 0); break;
	}

	switch (dizhi) //地支
	{
	case 0: Show_Str(34, 130, 24, 24, "辰", 24, 0); break;   //余0即整除 地支 辰
	case 1: Show_Str(34, 130, 24, 24, "巳", 24, 0); break;   //
	case 2: Show_Str(34, 130, 24, 24, "午", 24, 0); break;
	case 3: Show_Str(34, 130, 24, 24, "未", 24, 0); break;
	case 4: Show_Str(34, 130, 24, 24, "申", 24, 0); break;
	case 5: Show_Str(34, 130, 24, 24, "酉", 24, 0); break;
	case 6: Show_Str(34, 130, 24, 24, "戌", 24, 0); break;
	case 7: Show_Str(34, 130, 24, 24, "亥", 24, 0); break;
	case 8: Show_Str(34, 130, 24, 24, "子", 24, 0); break;
	case 9: Show_Str(34, 130, 24, 24, "丑", 24, 0); break;
	case 10:Show_Str(34, 130, 24, 24, "寅", 24, 0); break;
	case 11:Show_Str(34, 130, 24, 24, "卯", 24, 0); break;
	}

	switch (ReYear)	//生肖
	{
	case 0: Show_Str(58, 130, 24, 24, "龙", 24, 0); break;   //余0即整除 农历 龙年
	case 1: Show_Str(58, 130, 24, 24, "蛇", 24, 0); break;   //蛇年
	case 2: Show_Str(58, 130, 24, 24, "马", 24, 0); break;
	case 3: Show_Str(58, 130, 24, 24, "羊", 24, 0); break;
	case 4: Show_Str(58, 130, 24, 24, "猴", 24, 0); break;
	case 5: Show_Str(58, 130, 24, 24, "鸡", 24, 0); break;
	case 6: Show_Str(58, 130, 24, 24, "狗", 24, 0); break;
	case 7: Show_Str(58, 130, 24, 24, "猪", 24, 0); break;
	case 8: Show_Str(58, 130, 24, 24, "鼠", 24, 0); break;
	case 9: Show_Str(58, 130, 24, 24, "牛", 24, 0); break;
	case 10:Show_Str(58, 130, 24, 24, "虎", 24, 0); break;
	case 11:Show_Str(58, 130, 24, 24, "兔", 24, 0); break;
	}
	Show_Str(84, 130, 24, 24, "年", 24, 0);
}

/*显示农历日期---------------------------------------------------------------*/
void displaynl(void)
{
	unsigned char a, b, c;
	a = calendar.w_year % 1000 % 100 % 10;
	a = a + (calendar.w_year % 1000 % 100 / 10) * 16;
	b = calendar.w_month % 10;
	b = b + (calendar.w_month / 10) * 16;
	c = calendar.w_date % 10;
	c = c + (calendar.w_date / 10) * 16;
	Conversion(0, a, b, c);        //?????
/*****	显示农历月份  *****/

	if (moon.month_moon == 1) { Show_Str(108, 130, 24, 24, "正", 24, 0); }
	if (moon.month_moon == 2) { Show_Str(108, 130, 24, 24, "二", 24, 0); }
	if (moon.month_moon == 3) { Show_Str(108, 130, 24, 24, "三", 24, 0); }
	if (moon.month_moon == 4) { Show_Str(108, 130, 24, 24, "四", 24, 0); }
	if (moon.month_moon == 5) { Show_Str(108, 130, 24, 24, "五", 24, 0); }
	if (moon.month_moon == 6) { Show_Str(108, 130, 24, 24, "六", 24, 0); }
	if (moon.month_moon == 7) { Show_Str(108, 130, 24, 24, "七", 24, 0); }
	if (moon.month_moon == 8) { Show_Str(108, 130, 24, 24, "八", 24, 0); }
	if (moon.month_moon == 9) { Show_Str(108, 130, 24, 24, "九", 24, 0); }
	if (moon.month_moon == 10) { Show_Str(108, 130, 24, 24, "十", 24, 0); }
	if (moon.month_moon == 11) { Show_Str(108, 130, 24, 24, "冬", 24, 0); }
	if (moon.month_moon == 12) { Show_Str(108, 130, 24, 24, "腊", 24, 0); }
	Show_Str(132, 130, 24, 24, "月", 24, 0);

	/*****	显示农历日的十位  *****/
	if (moon.day_moon / 10 == 0) { Show_Str(156, 130, 24, 24, "初", 24, 0); }    //
	if (moon.day_moon / 10 == 1)
		if (moon.day_moon / 10 == 1 & moon.day_moon % 10 == 0) { Show_Str(156, 130, 24, 24, "初", 24, 0); }
		else { Show_Str(156, 130, 24, 24, "十", 24, 0); }    //
	if (moon.day_moon / 10 == 2)
		if (moon.day_moon / 10 == 2 & moon.day_moon % 10 == 0) { Show_Str(156, 130, 24, 24, "二", 24, 0); }
		else { Show_Str(156, 130, 24, 24, "廿", 24, 0); }   //
	if (moon.day_moon / 10 == 3) { Show_Str(156, 130, 24, 24, "三", 24, 0); }	  //"三"字的代码

/*****	显示农历日的个位  *****/

	if (moon.day_moon % 10 == 1) { Show_Str(180, 130, 24, 24, "一", 24, 0); }    //
	if (moon.day_moon % 10 == 2) { Show_Str(180, 130, 24, 24, "二", 24, 0); }    //
	if (moon.day_moon % 10 == 3) { Show_Str(180, 130, 24, 24, "三", 24, 0); }	  //"三"字的代码
	if (moon.day_moon % 10 == 4) { Show_Str(180, 130, 24, 24, "四", 24, 0); }    //
	if (moon.day_moon % 10 == 5) { Show_Str(180, 130, 24, 24, "五", 24, 0); }    //
	if (moon.day_moon % 10 == 6) { Show_Str(180, 130, 24, 24, "六", 24, 0); }    //
	if (moon.day_moon % 10 == 7) { Show_Str(180, 130, 24, 24, "七", 24, 0); }    //
	if (moon.day_moon % 10 == 8) { Show_Str(180, 130, 24, 24, "八", 24, 0); }    //
	if (moon.day_moon % 10 == 9) { Show_Str(180, 130, 24, 24, "九", 24, 0); }    //
	if (moon.day_moon % 10 == 0) { Show_Str(180, 130, 24, 24, "十", 24, 0); }    //
}

/*----------显示节气---------------------------------------------*/
void displayjieqi(void)
{
	unsigned char j, a, b, c;
	a = calendar.w_year % 1000 % 100 % 10;
	a = a + (calendar.w_year % 1000 % 100 / 10) * 16;
	b = calendar.w_month % 10;
	b = b + (calendar.w_month / 10) * 16;
	c = calendar.w_date % 10;
	c = c + (calendar.w_date / 10) * 16;
	j = jieqi(a, b, c);
	if (j == 1) { Show_Str(204, 130, 96, 24, "今天小寒", 24, 0); }
	if (j == 2) { Show_Str(204, 130, 96, 24, "今天大寒", 24, 0); }
	if (j == 3) { Show_Str(204, 130, 96, 24, "今天立春", 24, 0); }
	if (j == 4) { Show_Str(204, 130, 96, 24, "今天雨水", 24, 0); }
	if (j == 5) { Show_Str(204, 130, 96, 24, "今天惊蛰", 24, 0); }
	if (j == 6) { Show_Str(204, 130, 96, 24, "今天春分", 24, 0); }
	if (j == 7) { Show_Str(204, 130, 96, 24, "今天清明", 24, 0); }
	if (j == 8) { Show_Str(204, 130, 96, 24, "今天谷雨", 24, 0); }
	if (j == 9) { Show_Str(204, 130, 96, 24, "今天立夏", 24, 0); }
	if (j == 10) { Show_Str(204, 130, 96, 24, "今天小满", 24, 0); }
	if (j == 11) { Show_Str(204, 130, 96, 24, "今天芒种", 24, 0); }
	if (j == 12) { Show_Str(204, 130, 96, 24, "今天夏至", 24, 0); }
	if (j == 13) { Show_Str(204, 130, 96, 24, "今天小暑", 24, 0); }
	if (j == 14) { Show_Str(204, 130, 96, 24, "今天大暑", 24, 0); }
	if (j == 15) { Show_Str(204, 130, 96, 24, "今天立秋", 24, 0); }
	if (j == 16) { Show_Str(204, 130, 96, 24, "今天处暑", 24, 0); }
	if (j == 17) { Show_Str(204, 130, 96, 24, "今天白露", 24, 0); }
	if (j == 18) { Show_Str(204, 130, 96, 24, "今天秋分", 24, 0); }
	if (j == 19) { Show_Str(204, 130, 96, 24, "今天寒露", 24, 0); }
	if (j == 20) { Show_Str(204, 130, 96, 24, "今天霜降", 24, 0); }
	if (j == 21) { Show_Str(204, 130, 96, 24, "今天立冬", 24, 0); }
	if (j == 22) { Show_Str(204, 130, 96, 24, "今天小雪", 24, 0); }
	if (j == 23) { Show_Str(204, 130, 96, 24, "今天大雪", 24, 0); }
	if (j == 24) { Show_Str(204, 130, 96, 24, "今天冬至", 24, 0); }
	if (j == 0) { Show_Str(216, 130, 96, 24, "无节气", 24, 0); }
}

void timeplay(void)
{
	Show_Str(58, 0, 24, 24, "年", 24, 0);
	Show_Str(106, 0, 24, 24, "月", 24, 0);
	Show_Str(154, 0, 24, 24, "日", 24, 0);
	Show_Str(180, 0, 48, 24, "星期", 24, 0);
	LCD_ShowxNum(10, 0, calendar.w_year, 4, 24, 0);//显示年
	LCD_ShowxNum(82, 0, calendar.w_month, 2, 24, 0);//显示月
	LCD_ShowxNum(130, 0, calendar.w_date, 2, 24, 0);//显示日

	switch (calendar.week)
	{
	case 0:
		Show_Font(228, 0, "天", 24, 0);
		break;
	case 1:
		Show_Font(228, 0, "一", 24, 0);
		break;
	case 2:
		Show_Font(228, 0, "二", 24, 0);
		break;
	case 3:
		Show_Font(228, 0, "三", 24, 0);
		break;
	case 4:
		Show_Font(228, 0, "四", 24, 0);
		break;
	case 5:
		Show_Font(228, 0, "五", 24, 0);
		break;
	case 6:
		Show_Font(228, 0, "六", 24, 0);
		break;
	}

	if (calendar.hour / 10 != 0)
	{
		image_display(0, 30, (u8*)gImage[calendar.hour / 10]);//在指定地址显示图?
	}
	else { image_display(0, 30, (u8*)gImage[11]); }

	image_display(50, 30, (u8*)gImage[calendar.hour % 10]);//在指定地址显示图片?

	if (calendar.sec % 10 == 0) { image_display(100, 30, (u8*)gImage[10]); }
	if (calendar.sec % 10 == 2) { image_display(100, 30, (u8*)gImage[10]); }
	if (calendar.sec % 10 == 4) { image_display(100, 30, (u8*)gImage[10]); }
	if (calendar.sec % 10 == 6) { image_display(100, 30, (u8*)gImage[10]); }
	if (calendar.sec % 10 == 8) { image_display(100, 30, (u8*)gImage[10]); }
	if (calendar.sec % 10 == 1) { image_display(100, 30, (u8*)gImage[12]); }
	if (calendar.sec % 10 == 3) { image_display(100, 30, (u8*)gImage[12]); }
	if (calendar.sec % 10 == 5) { image_display(100, 30, (u8*)gImage[12]); }
	if (calendar.sec % 10 == 7) { image_display(100, 30, (u8*)gImage[12]); }
	if (calendar.sec % 10 == 9) { image_display(100, 30, (u8*)gImage[12]); }

	image_display(150, 30, (u8*)gImage[calendar.min / 10]);
	image_display(200, 30, (u8*)gImage[calendar.min % 10]);

	LCD_ShowChar(250, 86, ':', 24, 0);
	LCD_ShowxNum(262, 86, calendar.sec, 2, 24, 0x80);
}
          
/**********************************************************
正常界面
**********************************************************/
void time_go(void)
{
	RTC_Get();
	timeplay();
	displaynl();
	displayjieqi();
	displaysx();
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

//显示曲目索引
//index:当前索引
//total:总文件数
void mp3_index_show(u16 index, u16 total)
{
	//显示当前曲目的索引,及总曲目数
	if (lcd_bit ==1)
	{
		LCD_ShowxNum(224 + 0, 20, index, 3, 16, 0X80);		//索引
		LCD_ShowChar(224 + 24, 20, '/', 16, 0);
		LCD_ShowxNum(224 + 32, 20, total, 3, 16, 0X80); 	//总曲目	
	}
}
//显示当前音量
void mp3_vol_show(u8 vol)
{
	if (lcd_bit ==1 )
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
	if(lcd_bit == 1)
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



