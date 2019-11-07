#include "qei.hpp"

#ifdef DEBUG
#include <debug.hpp>
#endif


Qei::Qei(TIM_TypeDef* const _TIMx, int* const _err):
	TIMx(_TIMx)
{
	TIM_Encoder_InitTypeDef encoder;
	TIM_HandleTypeDef htim;
	TIM_MasterConfigTypeDef master_config;
	if (TIMx == TIM3) {
		__HAL_RCC_TIM3_CLK_ENABLE();
	} else if (TIMx == TIM4) {
		__HAL_RCC_TIM4_CLK_ENABLE();
	} else {
#ifdef DEBUG
		*_err = ERR_ENCODER_START;
#else
		*_err = -1;
#endif
	}
	htim.Instance = TIMx;
	htim.Init.Period = MAXCOUNT_PERIOD;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Prescaler = 0;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	encoder.EncoderMode = ENCODER_MODE;
	encoder.IC1Filter = 4;
	encoder.IC1Polarity = TIM_ICPOLARITY_RISING;
	encoder.IC1Prescaler = TIM_ICPSC_DIV1;
	encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	encoder.IC2Filter = 4;
	encoder.IC2Polarity = TIM_ICPOLARITY_RISING;
	encoder.IC2Prescaler = TIM_ICPSC_DIV1;
	encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_Encoder_Init(&htim, &encoder) != HAL_OK) {
#ifdef DEBUG
		*_err = ERR_ENCODER_START;
#else
		*_err = -1;
#endif
	}
	master_config.MasterOutputTrigger = TIM_TRGO_RESET;
	master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim, &master_config)
			!= HAL_OK) {
#ifdef DEBUG
		*_err = ERR_ENCODER_MASTERCONFIG;
#else
		*_err = -1;
#endif
	}
	GPIO_InitTypeDef GPIO_InitStruct;
	if (TIMx == TIM3) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}  else if (TIMx == TIM4) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else {
#ifdef DEBUG
		*_err = ERR_ENCODER_START;
#else
		*_err = -1;
#endif
	}
	if (HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL) != HAL_OK) {
#ifdef DEBUG
		*_err = ERR_ENCODER_START;
#else
		*_err = -1;
#endif
	}
	TIMx->CNT = 0;
}

Qei::~Qei()
{

}

void Qei::reset()
{
	TIMx->CNT = 0;
}

short Qei::getQei()
{
	return TIMx->CNT;
}

short Qei::getQei(short* _val)
{
	short new_val = getQei();
	short dr = new_val - *_val;
	*_val = new_val;
	return dr;
}
