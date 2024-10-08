/**
 * @file    button.h
 * @brief   This file contains all the function prototypes for
 *          the button.c file
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

/** Handle pressing BUTTON_1 and BUTTON_2.
 * When BUTTON_1 is pressed 1st time - matrix is turning on,
 * pressed 2nd time - matrix is turning off.
 * When BUTTON_2 is pressed 1st time - buzzer is turning on,
 * pressed 2nd time - buzzer is turning off.
 */
void press_button();

#endif /*__ BUTTON_H__ */
