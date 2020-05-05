/* The MIT License
 *
 * Copyright (c) 2020 Piotr Duba
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "stepper.h"
#include "tim.h"
#include "gpio.h"

/* inicjalizacja biblioteki */
void stepper_init(void)
{
	HAL_TIM_Base_Start_IT(&STEPS_TIM);

	HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SLEEP_PORT, SLEEP_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);

	HAL_Delay(2);
}

/* ustawia predkosc obrotowa silnika krokowego w rpm */
void stepper_speed(uint16_t rpm)
{
	if(rpm > 0)
	{
		uint16_t arr_val = XT_TIM_CLK / (((XT_TIM_PSC+1) * rpm * STEPS_PER_REV * MICROSTEP_NUM) / 60) - 1;
		uint16_t pulse_val = arr_val / 2;
		__HAL_TIM_SET_AUTORELOAD(&PULSE_TIM, arr_val);
		__HAL_TIM_SET_COMPARE(&PULSE_TIM, PULSE_TIM_CH, pulse_val);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&PULSE_TIM, PULSE_TIM_CH, 0);
	}
}

/* rozpoczyna ruch silnika z kierunkiem obrotu dir */
void stepper_run(uint8_t dir)
{
	if(dir)
	{
		HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(SLEEP_PORT, SLEEP_PIN, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_TIM_PWM_Start(&PULSE_TIM, PULSE_TIM_CH);
}

/* wstrzymuje ruch silnika - pozostawia moment trzymajacy na wale silnika */
void stepper_pause(void)
{
	HAL_TIM_PWM_Stop(&PULSE_TIM, PULSE_TIM_CH);
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
}

/* zatrzymuje silnik - wylacza wyjscia kontrolera silnika */
void stepper_stop(void)
{
	stepper_pause();
	HAL_GPIO_WritePin(SLEEP_PORT, SLEEP_PIN, GPIO_PIN_RESET);
}

/* ustawia zadana liczbe krokow do wykonania */
void stepper_steps(uint16_t steps)
{
	__HAL_TIM_SET_AUTORELOAD(&STEPS_TIM, steps);
}

/* obraca wal silnika o zadny kat z zadana predkoscia w kierunku dir */
void stepper_rot(uint16_t ang, uint16_t rpm, uint8_t dir)
{
	stepper_steps(((STEPS_PER_REV * MICROSTEP_NUM * ang) / 360));
	stepper_speed(rpm);
	stepper_run(dir);
}

