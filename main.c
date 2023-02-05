#include "stm32f446xx.h"

void sysClockConfig(void){
	
	#define PLL_M 4
	#define PLL_N 180
	#define PLL_P 0 
	
	//Enable HSE and wait for the HSE to become ready
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	//SET the power enable clock and voltage regulator
	RCC->AHB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	
	//configure the FLASH PREFETCH and the LATENCY related settings
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	
	//configure the PRESCALARS HCLK< PCLK1, PCLK2
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	//APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	
	//APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	
	//configure the main pll
	RCC->PLLCFGR = ((PLL_M<<0) | (PLL_N<<6) | (PLL_P<<16) | (RCC_PLLCFGR_PLLSRC_HSE));
	
	//enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	
	//select the clock source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SWS_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

}

void GPIO_Config(void)
{ 
	//enable the GPIO clock
	RCC->AHB1ENR |= (1<<0);
	
	//set the pin as OUTPUT
	GPIOA->MODER |= (1<<10); //pin PA5(bits 11:10) as Output (01)
	
	//configure the output mode
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
}

void delay(uint32_t time)
{
	while(time--);
}
int main(void){
	sysClockConfig();
	GPIO_Config();
	while(1){
		GPIOA->BSRR |= (1<<5);
		delay(1000000);
		GPIOA->BSRR |= ((1<<5) <<16);
		delay(1000000);
	
	}
}
