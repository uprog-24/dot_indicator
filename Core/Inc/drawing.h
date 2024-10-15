/**
 * @file    drawing.h
 * @brief   This file contains all the function prototypes for
 *          the drawing.c file
 */
#ifndef __DRAWING_H__
#define __DRAWING_H__

#include <stdint.h>

/**
 * Stores direction of movement
 */
typedef enum {
	UPWARD, DOWNWARD, STOP
} direction_t;

/**
 * @brief  Setting symbols for current floor.
 * @param  *str Pointer to the output string with symbols for the floor.
 * @param  current_floor Number of the current floor.
 * @param  direction Direction of the movement of lift: UPWARD, DOWNWARD, STOP.
 * @retval None
 */
void setting_symbols_floor(char *str, uint8_t current_floor, direction_t direction);

/**
 * @brief  Draw a symbol on matrix.
 * @param  symbol
 * @param  pos Start position (index of column) for symbol
 * @retval None
 */
void draw_symbol_on_matrix(char symbol, uint8_t pos, uint8_t shift);

/**
 * @brief  Draw string on matrix.
 * @param  *str_symbols String to be set on matrix
 * @retval None
 */
void draw_str_on_matrix(char *str_symbols);

#endif /*__ DRAWING_H__ */
