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

#include "table.h"

#define CH1N_LED GPIO_PIN_8
#define CH1_LED GPIO_PIN_9
#define CH2N_LED GPIO_PIN_10
#define CH2_LED GPIO_PIN_11
#define CH3N_LED GPIO_PIN_12
#define CH3_LED GPIO_PIN_13

#define PERIOD (260 -1)
// PERIOD = F_CPU/PRESCALER/F_PWM
#define CH1_PULSE 50
#define CH2_PULSE 100
#define CH3_PULSE 250
// PULSE = DutyCycle * (PERIOD -1) /100


static TIM_HandleTypeDef Timer_Instance = {
    .Instance = TIM2
};

static TIM_HandleTypeDef PWM_Instance = {
		.Instance = TIM1
};

uint8_t position1 = 0;
uint8_t position2 = 64;
uint8_t position3 = 128;

static TIM_OC_InitTypeDef sConfigOC;

void init_Timer(int period){
	__TIM2_CLK_ENABLE();
	Timer_Instance.Init.Prescaler = 1000;
	Timer_Instance.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer_Instance.Init.Period = period;
	Timer_Instance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Timer_Instance.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&Timer_Instance);
	HAL_TIM_Base_Start_IT(&Timer_Instance);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
}

void init_PWM(){
	__HAL_RCC_TIM1_CLK_ENABLE();
	PWM_Instance.Init.Prescaler = 9;
	PWM_Instance.Init.CounterMode = TIM_COUNTERMODE_UP;
	PWM_Instance.Init.Period = PERIOD;
	PWM_Instance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&PWM_Instance);


	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.Pulse = CH1_PULSE;
	HAL_TIM_PWM_ConfigChannel(&PWM_Instance, &sConfigOC, TIM_CHANNEL_1);

	sConfigOC.Pulse = CH2_PULSE;
	HAL_TIM_PWM_ConfigChannel(&PWM_Instance, &sConfigOC, TIM_CHANNEL_2);

	sConfigOC.Pulse = CH3_PULSE;
	HAL_TIM_PWM_ConfigChannel(&PWM_Instance, &sConfigOC, TIM_CHANNEL_3);

	//HAL_TIM_PWM_MspInit(&PWM_Instance);
	HAL_TIM_PWM_Start(&PWM_Instance, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&PWM_Instance, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&PWM_Instance, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&PWM_Instance, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&PWM_Instance, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&PWM_Instance, TIM_CHANNEL_3);
}

void init_PWM_LED(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef PWM_LED;
	PWM_LED.Pin = CH1N_LED | CH1_LED | CH2N_LED | CH2_LED | CH3N_LED | CH3_LED;
	PWM_LED.Mode = GPIO_MODE_AF_PP;
	PWM_LED.Alternate = GPIO_AF2_TIM1;
	PWM_LED.Speed = GPIO_SPEED_FREQ_HIGH;
	PWM_LED.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &PWM_LED);
}

void init_LED(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	//PORTE pin 14 = orange LED
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
}

/*
void init_UserButton(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Struct;
	//PORTA pin 0 = User Button
	GPIO_Struct.Pin = GPIO_PIN_0;
	GPIO_Struct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_Struct.Pull = GPIO_PULLUP;
	GPIO_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_Struct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
}*/

void init_UserButton(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Struct;
	//PORTA pin 0
	GPIO_Struct.Pin = GPIO_PIN_0;
	GPIO_Struct.Mode = GPIO_MODE_INPUT;
	GPIO_Struct.Pull = GPIO_NOPULL;
	GPIO_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_Struct);
}

void TIM2_IRQHandler(){
	HAL_TIM_IRQHandler(&Timer_Instance);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2)
		position1 += 10;
		position2 += 20;
		position3 += 30;

		TIM1->CCR1 = table[position1];
		TIM1->CCR2 = table[position2];
		TIM1->CCR3 = table[position3];
}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == USER_BUTTON_PIN){
		for(int i=0; i<0x7FFFF; i++);
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
	}
}*/

int main(void)
{
	uint8_t flag = 0;
	HAL_Init();
	init_LED();
	init_UserButton();
	init_PWM_LED();
	init_PWM(500);
	init_Timer(500);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	//HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	while(1){
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){
			flag++;
			//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);

		}

	}
}









