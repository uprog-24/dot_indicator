#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h"
#include "tim.h"
#include <stdint.h>

void set_active_buzzer_state(states_t state);
void set_passive_buzzer_melody(uint16_t *freq_buff, uint8_t buff_size);

#endif /*__ BUTTON_H__ */
