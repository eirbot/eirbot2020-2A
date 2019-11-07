#include <servo.hpp>

TIM_HandleTypeDef htim10;

void servoTimerInit() {

	__HAL_RCC_TIM10_CLK_ENABLE();
	// Configuration du TIMER10 en tant que PWM OUT
	TIM_OC_InitTypeDef sConfigOC;

	htim10.Instance = TIM10;
	htim10.Init.Prescaler = SERVO_PWM_PRESCAL; // 180MHz/60 ==> 3 000 000
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = SERVO_PWM_PERIOD; // 3 000 000 / 60 000 => 50Hz
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK) {
		printf("Erreur d'initialisation du Timer 10.\n");
	}

	if (HAL_TIM_PWM_Init(&htim10) != HAL_OK) {
		printf("Erreur d'initialisation de la PWM du Timer 10.\n");
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	//sConfigOC.Pulse = SERVO_INIT; // alpha de la PWM
	sConfigOC.Pulse = SERVO_INIT; // alpha de la PWM
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		printf("Erreur d'initialisation du channel 1 du Timer 10.\n");
	}

	// Configuration du pin pwm (channel 1 du timer 10 pour avoir PWM sur PB8)
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;

	/**TIM10 GPIO Configuration
	PB8     ------> TIM10_CH1
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// On force le démarrage des modules (merci Google)
	HAL_TIM_Base_Start(&htim10);
	HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
}

void servoSetPwmDuty(int dutyCycle){

	if(dutyCycle>SERVO_PWM_PERIOD)
		dutyCycle=1200;
	if(dutyCycle<0)
		dutyCycle=0;

	// Checker le site ci dessous si la ligne suivante ne fonctionne pas :
	// https://electronics.stackexchange.com/questions/179546/getting-pwm-to-work-on-stm32f4-using-sts-hal-libraries
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, dutyCycle); //sets the PWM duty cycle
}
