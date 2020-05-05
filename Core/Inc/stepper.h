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
#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "stm32f3xx_hal.h"

#define STEPS_PER_REV	200				/* liczba krokow na pelen obrot silnika */
#define MICROSTEP_NUM	16				/* liczba mikrokrokow -> 1 = brak mikrokrokow */

#define	PULSE_TIM		htim1			/* timer odpowiedzialny za generowanie sygnalu PWM */
#define	PULSE_TIM_CH	TIM_CHANNEL_1	/* kanal generowania sygnalu PWM */

#define	STEPS_TIM		htim2			/* timer odpowiedzialny za zliczanie wykonanych krokow przez silnik */

#define	XT_TIM_CLK		72000000		/* czestotliwosc wejsciowa timera PULSE_TIM w Hz */
#define XT_TIM_PSC		31				/* prescaler timera PULSE_TIM */

#define DIR_PORT	GPIOC				/* port sygnału DIR */
#define DIR_PIN		GPIO_PIN_1			/* pin sygnału DIR */
#define RESET_PORT	GPIOA				/* port sygnału nRESET */
#define RESET_PIN	GPIO_PIN_0			/* pin sygnału nRESET */
#define SLEEP_PORT	GPIOA				/* port sygnału nSLEEP */
#define SLEEP_PIN	GPIO_PIN_1			/* pin sygnału nSLEEP */

#define STEPPER_CW		0				/* stan pinu DIR podczas ruchu CW */
#define STEPPER_CCW		1				/* stan pinu DIR podczas ruchu CCW */

void stepper_init(void);
void stepper_speed(uint16_t rpm);
void stepper_run(uint8_t dir);
void stepper_pause(void);
void stepper_stop(void);
void stepper_steps(uint16_t steps);
void stepper_rot(uint16_t ang, uint16_t rpm, uint8_t dir);

#endif /* INC_STEPPER_H_ */
