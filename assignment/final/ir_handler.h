/** @file   ir_handler.c
    @author J. Trombik & J. Griffiths
    @date   27 SEPT 2015
    @brief IR handler headers
*/

#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include "ir_uart.h"
#include "tinygl.h"
#define NO_POSITION  0xff


typedef enum {NORESPONSE_S, READY_S, ACKNOWLEDGED_S,  PLAYER_TWO_S, HIT_S, MISS_S, LOOSER_S, PLAYON_S} states;


void ir_send_status(states status);

states ir_get_status(void);

void ir_send_strike(tinygl_point_t pos);

tinygl_point_t ir_decode_strike(char c);

uint8_t ir_get_position(void);

#endif
