#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include "ir_uart.h"

typedef enum {NORESPONSE, READY, ACKNOWLEDGED,  PLAYER_TWO} states;

int ir_send_status(char status)

states ir_get_status(void);

#endif
