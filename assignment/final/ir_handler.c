

#include "ir_handler.h"
#include "tinygl.h"





static int8_t rcvChar;



void ir_send_status(char status){


  ir_uart_putc(status);
}


states ir_get_status(void){

  if (ir_uart_read_ready_p ())
  {
    rcvChar =  ir_uart_getc();

    if((states) rcvChar == READY){

      return states READY;
    }
    if((states) rcvChar == ACKNOWLEDGED){
      return states ACKNOWLEDGED;
    }
    if((states) rcvChar == PLAYER_TWO){
      return states PLAYER_TWO;
    }

  }
  return states NoResponse;


}
