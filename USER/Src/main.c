#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"
#include "Devices.h"
#include "string.h"
#include "Attitude.h"
#include "sensor.h"

void SystemClock_Config(void);

bool bool_mainLoop;
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
	USART1_UART_Init(1000000);
	Init_MPU6050();
	//USART3_UART_Init();
	
	struct TimerTemp t1;
	ReSetTimerTemp(&t1);
	struct TimerTemp t2;
	ReSetTimerTemp(&t2);
	char sendBuff[100];
	SensorInit();	//传感器初始化
	uint32 times1,times2;
	while (1)
	{
		
		if(bool_mainLoop)	//1ms进入一次
		{
			
			bool_mainLoop=0;
			ReSetTimerTemp(&t1);
			
			
			if(++times1>=2)
			{
				SensorThread();	//传感器数据读取与处理
				times1=0;
			}
			
			//imuUpdate(acc,gyro,&state,2);
			//if(WaitSysTime_UnBlocked(&t2,100,UINT_MS))
			if(++times2>=100)
			{
				times2=0;
				ReSetTimerTemp(&t2);
				Sys_LED_Negative();
				snprintf(sendBuff,100,"ACC_X:%f\t ACC_Y:%f\t ACC_Z:%f\r\n\
				GYRO_X:%f\t GYRO_Y:%f\t GYRO_Z:%f\r\n Temp:%f\r\n",
				sensor.mpu6050.acc.axisTFloat_G.axisTF.x,
				sensor.mpu6050.acc.axisTFloat_G.axisTF.y,
				sensor.mpu6050.acc.axisTFloat_G.axisTF.z,
				sensor.mpu6050.gyro.axisTFloat_DEG.axisTF.x,
				sensor.mpu6050.gyro.axisTFloat_DEG.axisTF.y,
				sensor.mpu6050.gyro.axisTFloat_DEG.axisTF.z,
				36.53+(float)sensor.mpu6050.thermometer.Temp.data/340);
				UART1_SendBytes(sendBuff,strlen(sendBuff));
				DMA_UART1_SendThread();		
			}
		}
		
	}
	
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
