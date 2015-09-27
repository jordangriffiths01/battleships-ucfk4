
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
      return PLAYER_TWO_S;
    }

  }
  return NORESPONSE_S;


}
