#include "Har_SysTick.h"


/**
  * @brief  配置系统滴答定时器 SysTick,将产生中断
  * @param  无
  * @retval 1 = failed, 0 = successful
  */
bool SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000     10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */ 
	uint32_t ticks= SystemCoreClock / 1000000;						/* 设置定时周期为1ms  */

	if ((ticks - 1) > SysTick_LOAD_RELOAD_Msk)  return (1);      /* Reload value impossible */

	SysTick->LOAD  = ticks - 1;                                  /* set reload register */
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Systick Interrupt */
	SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
						SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
	

	//HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//选择系统时钟源216MHz
	//HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);   //1毫秒产生一次中断
	//NVIC_SetPriority(SysTick_IRQn, 15);  //配置优先级

	return (0);
}
