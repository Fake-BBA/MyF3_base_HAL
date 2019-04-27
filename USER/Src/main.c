#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"
#include "Devices.h"
#include "string.h"
#include "Attitude.h"
#include "sensor.h"
#include "Communication.h"
#include "PID.h"

void SystemClock_Config(void);

uint32 loopTimeIMU1,loopTimeIMU2,loopTimeMPU1,loopTimeMPU2;
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

int main(void)
{
	HAL_Init();				//初始化库
	SystemClock_Config();	//重新设置时钟为72M
	SysTick_Init();	//
	Sys_LED_Init();
	Buzzer_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	USART1_UART_Init(500000);
	Init_MPU6050();
	Init_PWM();	//初始化PWM，包括引脚
	//USART3_UART_Init();
	
	SensorInit();	//传感器初始化
	
	struct TimerTemp t_IMU_Update;
	InitTimerTemp(&t_IMU_Update);
	
	struct TimerTemp tSendThread;
	InitTimerTemp(&tSendThread);
	
	struct TimerTemp tRecv;
	InitTimerTemp(&tRecv);
	
	while (1)
	{
		if(WaitSysTime(&t_IMU_Update,2,UINT_MS))	//2ms进入一次
		{
			ReSetTimerTemp(&t_IMU_Update);
			loopTimeIMU1=GetSystemTime()-loopTimeIMU2;
			SensorThread();	//处理传感器数据
			imuUpdate(0.002);	//2ms执行一次 500HZ
			//PID_Thread();	//PID控制
			loopTimeIMU2=GetSystemTime();
		}
		
		//50ms进行一次发送
		if(WaitSysTime(&tSendThread,50,UINT_MS))
		{
			ReSetTimerTemp(&tSendThread);
			//Sys_LED_Negative();	//系统灯取反
			
			SendStatus();	//发送飞行器基本姿态给上位机
			SendSensor();	//发送传感器信息给上位机
			//SendLoopTime();
			DMA_UART1_SendThread();	//启动DMA发送
		}
		
		//20ms处理一次接收到的数据
		if(WaitSysTime(&tRecv,20,UINT_MS))
		{
			ReSetTimerTemp(&tRecv);
			RecvMessageThread();	//处理收到的数据
		}
	}
	//_WFI();	//进入睡眠模式，任意中断可唤醒
}

void Error_Handler()
{
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;	//配置IIC为内部8MHz时钟
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


