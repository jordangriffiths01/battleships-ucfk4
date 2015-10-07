/** @file   ir_handler.c
    @authors  Jordan Griffiths (jlg108) & Jonty Trombik (jat157)
    @date   27 SEPT 2015
    
    @brief IR handling and message manipulation
*/

#include "ir_handler.h"


void ir_send_status(states status){
    ir_uart_putc(status);
}


states ir_get_status(void){
    if (ir_uart_read_ready_p ())
    {
        rcvChar =  ir_uart_getc();
        return rcvChar;
    }

    return NORESPONSE_S;
}

/** encodes pos as 0b00(pos.x)(pos.y) and sends as character **/
void ir_send_strike(tinygl_point_t pos) {
    char msg = (pos.x << 3) | pos.y;
    ir_uart_putc(msg);
}


uint8_t ir_get_position(void) {
    if (ir_uart_read_ready_p ())
    {
        char rcvdChar =  ir_uart_getc();
        return (uint8_t) rcvdChar;
    }

    return NO_POSITION;
}


tinygl_point_t ir_decode_strike(char c) {
    uint8_t y = c & 0x7;
    uint8_t x = c >> 3;
    return tinygl_point(x, y);
}
