/** @file   ir_handler.c
    @author J. Trombik & J. Griffiths
    @date   27 SEPT 2015
    @brief IR handling and message manipulation
*/

#include "ir_handler.h"






static int8_t rcvChar;



void ir_send_status(states status){

  ir_uart_putc(status);

}


states ir_get_status(void){

  if (ir_uart_read_ready_p ())
  {
    rcvChar =  ir_uart_getc();

    if((states) rcvChar == READY_S){

      return READY_S;
    }
    if((states) rcvChar == ACKNOWLEDGED_S){
      return ACKNOWLEDGED_S;
    }
    if((states) rcvChar == PLAYER_TWO_S){
      return  PLAYER_TWO_S;
    }

  }
  return NORESPONSE_S;

}

void ir_send_strike(tinygl_point_t pos) {
  char msg = (pos.x << 4) | pos.y
  ir_uart_putc(msg);

}


uint8_t ir_get_position(){
  if (ir_uart_read_ready_p ())
  {
    char rcvdChar =  ir_uart_getc();
    return rcvdChar;

  }

  return NO_POSITION;
}

tinygl_point_t ir_decode_strike(char c) {
  uint8_t x = c & 0xf;
  uint8_t y = c >> 4;
  return tinygl_point(x, y)

}
