#include "app_start.h"
#include "mp3player.h"
#include "includes.h"
#include "show.h"
#include "GUIx.h"
#include "key.h"
#include "lcd.h"
#include "fontupd.h"
#include "data.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//任务优先级
#define START_TASK_PRIO 3
//任务堆栈大小
#define START_STK_SIZE 64
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//MP3任务
//设置任务优先级
#define MUSIC_PLAY_TASK_PRIO 1
//设置任务堆栈大小
#define MUSIC_PLAY_STK_SIZE 128
//任务控制块
OS_TCB MUSICTaskTCB;
//任务堆栈
CPU_STK MUSIC_PLAY_TASK_STK[MUSIC_PLAY_STK_SIZE];
//任务函数
void mp3_play(void *pdata);

//图片显示任务
//设置任务优先级
#define PIC_SHOW_TASK_PRIO 1
//设置任务堆栈大小
#define PIC_SHOW_STK_SIZE 128
//任务控制块
OS_TCB PICTaskTCB;
//任务堆栈
CPU_STK PIC_SHOW_TASK_STK[PIC_SHOW_STK_SIZE];
//任务函数
void show_mp3_pic(void *pdata);

//开始任务
void start_task(void *pdata)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	pdata = pdata;

	CPU_Init();
	/*
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
	CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif
	*/
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE); //开启CRC时钟

	OS_CRITICAL_ENTER(); //进入临界区
						 //创建MP3任务
	OSTaskCreate((OS_TCB *)&MUSICTaskTCB,
				 (CPU_CHAR *)"mp3 task",
				 (OS_TASK_PTR)mp3_play,
				 (void *)0,
				 (OS_PRIO)MUSIC_PLAY_TASK_PRIO,
				 (CPU_STK *)&MUSIC_PLAY_TASK_STK[0],
				 (CPU_STK_SIZE)MUSIC_PLAY_STK_SIZE / 10,
				 (CPU_STK_SIZE)MUSIC_PLAY_STK_SIZE,
				 (OS_MSG_QTY)0,
				 (OS_TICK)0,
				 (void *)0,
				 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
				 (OS_ERR *)&err);
	OSTaskCreate((OS_TCB *)&PICTaskTCB,
				 (CPU_CHAR *)"pic_show task",
				 (OS_TASK_PTR)show_mp3_pic,
				 (void *)0,
				 (OS_PRIO)PIC_SHOW_TASK_PRIO,
				 (CPU_STK *)&PIC_SHOW_TASK_STK[0],
				 (CPU_STK_SIZE)PIC_SHOW_STK_SIZE / 10,
				 (CPU_STK_SIZE)PIC_SHOW_STK_SIZE,
				 (OS_MSG_QTY)0,
				 (OS_TICK)0,
				 (void *)0,
				 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
				 (OS_ERR *)&err);
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err); //挂起开始任务
	OS_CRITICAL_EXIT();							   //进入临界区
}

void APP_start(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OSInit(&err);		 //初始化UCOSIII
	OS_CRITICAL_ENTER(); //进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB *)&StartTaskTCB,							//任务控制块
				 (CPU_CHAR *)"start task",							//任务名字
				 (OS_TASK_PTR)start_task,							//任务函数
				 (void *)0,											//传递给任务函数的参数
				 (OS_PRIO)PIC_SHOW_TASK_PRIO,						//任务优先级
				 (CPU_STK *)&START_TASK_STK[0],						//任务堆栈基地址
				 (CPU_STK_SIZE)START_STK_SIZE / 10,					//任务堆栈深度限位
				 (CPU_STK_SIZE)START_STK_SIZE,						//任务堆栈大小
				 (OS_MSG_QTY)0,										//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
				 (OS_TICK)0,										//当使能时间片轮转时的时间片长度，为0时为默认长度，
				 (void *)0,											//用户补充的存储区
				 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
				 (OS_ERR *)&err);									//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();												//退出临界区
	OSStart(&err);													//开启UCOSIII
	while (1)
		;
}

void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE); //使能IO口时钟，ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	//PC4 作为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_DeInit(ADC1); //复位ADC1

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC工作模式：独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//魔术转换工作在单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换有软件启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//顺序进行规则转换
	ADC_Init(ADC1, &ADC_InitStructure);									//根据ADC_InitStruct中指定 的参数初始化外设ADCx的寄存器

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1, ENABLE);		//使能指定的ADC1
	ADC_ResetCalibration(ADC1); //使能复位校准
	while (ADC_GetResetCalibrationStatus(ADC1))
		;						//等待复位校准结束
	ADC_StartCalibration(ADC1); //开启AD校准
	while (ADC_GetCalibrationStatus(ADC1))
		; //等待校准结束
}
u16 Get_Adc_Average(void)
{
	u16 temp_val;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		;
	temp_val = ADC_GetConversionValue(ADC1);
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	return temp_val;
}
