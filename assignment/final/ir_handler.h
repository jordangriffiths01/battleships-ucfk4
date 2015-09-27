#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include "ir_uart.h"

typedef enum {NORESPONSE_S, READY_S, ACKNOWLEDGED_S,  PLAYER_TWO_S} states;

void ir_send_status(states status);

states ir_get_status(void);

#endif
