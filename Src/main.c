
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_cortex.h"
#include <stdio.h>
// #if defined(USE_FULL_ASSERT)
// #include "stm32_assert.h"
// #endif /* USE_FULL_ASSERT */

#include "gpio.h"
int val=0;
int cmpt=1;
int etat_signal=0;
int blue_mode=0;
int expe=2;
void     SystemClock_Config(void);
void     SystemClock_Config_24(void);


int main(void)
{
/* Configure the system clock */

if (expe==1){
	SystemClock_Config();
} else{
	SystemClock_Config_24();
}


// config GPIO
GPIO_init();


void config_systick_v2(void);

// init timer pour utiliser la fonction LL_mDelay() de stm32l4xx_ll_utils.c
LL_Init1msTick( SystemCoreClock );
//config_sistick_v2();
//SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk |					// interrupt local
   //     SysTick_CTRL_TICKINT_Msk;

//NVIC_SetPriority(-1,5);

config_systick_v2();
LED_GREEN(0);



while (1)
 	{
	if(blue_mode==0){
		blue_mode=BLUE_BUTTON();

		printf("je suis dans le mode blue");
	}
	if	( blue_mode ){

		// mode sleep

		LL_LPM_EnableSleep();
		__WFI();
	}

	}

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */


void config_systick_v2(void){

//	SysTick->LOAD  = 0x006f0001;  /* set reload register */
//	SysTick->VAL   = 0;                                       /* Load the SysTick Counter Value */
	SysTick->CTRL  =0b111;                 /* Enable the Systick Timer */
//	NVIC_SetPriority (SysTick_IRQn, 2);
	if(expe==1){
		SysTick_Config(8000000);
	} else{
		SysTick_Config(240000);
	}

	NVIC_EnableIRQ(SysTick_IRQn);



}


void SysTick_Handler(void){

if (expe==1){
	if(cmpt<5){

	LED_GREEN(1);
} else{
	LED_GREEN(0);

}
cmpt++;

if(cmpt==100){
	cmpt=0;
}


}else{
	if(etat_signal==0){
		SIGNAL(1);
		etat_signal=1;
	} else{
		SIGNAL(0);
		etat_signal=0;
	}

}


/*	if( BLUE_BUTTON() ){
			LED_GREEN(1);
			val=0;}
	else {
			if(val==0){
				LED_GREEN(1);
				val=1;
			}

			else{
					LED_GREEN(0);
					val=0;
				}
			}



			//LED_GREEN(1);
*/

}

void SystemClock_Config_24(void)
{
/* MSI configuration and activation */
LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

//LL_RCC_MSI_DisablePLLMode();
LL_RCC_MSI_EnableRangeSelection();

if( LL_RCC_MSI_IsEnabledRangeSelect()) LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_9);

LL_RCC_MSI_Enable();
while	(LL_RCC_MSI_IsReady() != 1)
	{ };
  
/* Main PLL configuration and activation */

  
/* Sysclk activation on the main PLL */
LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);
/*while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{ };*/

/* Set APB1 & APB2 prescaler*/
LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

/* Update the global variable called SystemCoreClock */
SystemCoreClockUpdate();
}


void SystemClock_Config(void)
{
/* MSI configuration and activation */
LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
LL_RCC_MSI_Enable();
while	(LL_RCC_MSI_IsReady() != 1)
	{ };

/* Main PLL configuration and activation */
LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
LL_RCC_PLL_Enable();
LL_RCC_PLL_EnableDomain_SYS();
while(LL_RCC_PLL_IsReady() != 1)
	{ };

/* Sysclk activation on the main PLL */
LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{ };
  
/* Set APB1 & APB2 prescaler*/
LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

/* Update the global variable called SystemCoreClock */
SystemCoreClockUpdate();
}
