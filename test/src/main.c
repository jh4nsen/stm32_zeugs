/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3_discovery.h"
#include "stm32f3xx_hal.h"
#include <stm32f3xx_it.h>

static TIM_HandleTypeDef Timer_Instance = {
    .Instance = TIM2
};

static TIM_HandleTypeDef PWM_Instance = {
		.Instance = TIM1
};

void init_Timer( int period){
	__TIM2_CLK_ENABLE();
	Timer_Instance.Init.Prescaler = 40000;
	Timer_Instance.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer_Instance.Init.Period = period;
	Timer_Instance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Timer_Instance.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&Timer_Instance);
	HAL_TIM_Base_Start_IT(&Timer_Instance);
}

void init_PWM(int period){
	__HAL_RCC_TIM1_CLK_ENABLE();
	PWM_Instance.Init.Prescaler = 40000;
	PWM_Instance.Init.CounterMode = TIM_COUNTERMODE_UP;
	PWM_Instance.Init.Period = period;
	PWM_Instance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&PWM_Instance);

	TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.Pulse = 200;

	HAL_TIM_PWM_ConfigChannel(&PWM_Instance, &sConfigOC, TIM_CHANNEL_1);
	//HAL_TIM_PWM_MspInit(&PWM_Instance);
	HAL_TIM_PWM_Start(&PWM_Instance, TIM_CHANNEL_1);
}

void init_PWM_LED(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef PWM_LED;
	PWM_LED.Pin = GPIO_PIN_9;
	PWM_LED.Mode = GPIO_MODE_AF_PP;
	PWM_LED.Alternate = GPIO_AF2_TIM1;
	PWM_LED.Speed = GPIO_SPEED_FREQ_HIGH;
	PWM_LED.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &PWM_LED);
}

void init_LED(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	//PORTE pin 8 = blue LED
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
}

void TIM2_IRQHandler(){
	HAL_TIM_IRQHandler(&Timer_Instance);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	//if(htim->Instance == TIM2)
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
}

int main(void)
{
	HAL_Init();
	init_LED();
	init_PWM_LED();
	init_PWM(500);
	init_Timer(500);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	while(1){

	}
}









