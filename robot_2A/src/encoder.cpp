/**
 * Ce fichier gère les encodeurs.
 */

#include "encoder.hpp"

/**
 *  Initialisation de l'encodeur.
 */
void TIM4_EncoderInit() {
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOD_CLK_ENABLE();
  __TIM4_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  TIM4->SMCR = 3;       // Encoder mode 3
  TIM4->CCER = 0;       // rising edge polarity
  TIM4->ARR = 0xFFFF;   // count from 0-ARR or ARR-0
  TIM4->CCMR1 = 0xC1C1; // f_DTS/16, N=8, IC1->TI1, IC2->TI2
  TIM4->EGR = 1;        // Generate an update event
  TIM4->CR1 = 1;        // Enable the counter
  TIM4->CNT = 0;        // Initialize counter
}

void TIM3_EncoderInit() {
  __GPIOA_CLK_ENABLE();
  __TIM3_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  TIM3->SMCR = 3; // Encoder mode 3
  TIM3->CCER = 0; // rising edge polarity
  TIM3->CCER |= (1 << 1);
  TIM3->ARR = 0xFFFF;   // count from 0-ARR or ARR-0
  TIM3->CCMR1 = 0xC1C1; // f_DTS/16, N=8, IC1->TI1, IC2->TI2
  TIM3->EGR = 1;        // Generate an update event
  TIM3->CR1 = 1;        // Enable the counter
  TIM3->CNT = 0;        // Initialize counter
}

Encoder::Encoder(TIM_TypeDef* timer)
  : TIM(timer), lastValue(0) {

  if (TIM == TIM4)
    TIM4_EncoderInit();
  if (TIM == TIM3)
    TIM3_EncoderInit();

}

void Encoder::reset() {
  TIM->CNT = 0;
}

int32_t Encoder::getDistance() {
  return TIM->CNT;
}

short Encoder::diff() {
  short currentVal = TIM->CNT;
  short diff = (currentVal - lastValue);
  lastValue = currentVal;
  return diff;
}
