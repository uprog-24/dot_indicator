#ifndef __DOT_H__
#define __DOT_H__

#include "main.h"
#include <stdint.h>

void set_row_state(uint8_t row, states_t state);
void set_col_state(uint8_t col, states_t state);

void set_all_cols_state(states_t state);
void set_all_rows_state(states_t state);
void set_full_matrix_state(states_t state);

void set_matrix_by_rows();

#endif /*__DOT_H__ */

