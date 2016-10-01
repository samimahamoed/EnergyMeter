
/*!\file srv_cmd_handler.h
   \brief \e File:  rpi_gpio.h \n
   \brief \e Purpose: \n 
*/

#ifndef __SRV_CMD_HANDLER_H
#define __SRV_CMD_HANDLER_H


//!internal use config cmd macros, make sure the values will not 
#define ADE7880_SPI_PORT_ACTIVATE 0x01
#define ADE7880_RESET			  0x02
#define POWER_MODE 				  0x03
#define THRESHOLD				  0x04
#define SET_RAM_WR_PROTECTION	  0x05
#define DSP_RUN					  0x06


//!internal use measurment read cmd macros 
#define PHASE_A			  0x0A
#define PHASE_B			  0x0B
#define PHASE_C			  0x0C
#define LINE			  0x00

#define PHASE_VRMS		    0x01
#define PHASE_IRMS		    0x02
#define PHASE_ACTIVE_WH     0x03
#define TOTAL_ACTIVE_WH	  	0x04
#define PHASE_ACTIVE_POWER	0x05
#define TOTAL_ACTIVE_POWER	0x06


#include <stdarg.h> 

float       measure(uint8_t cmd,uint8_t channel,float samples);
uint16_t  	hex2val(uint8_t cc);
uint16_t  	make16(uint8_t* buf,uint16_t idx);
uint8_t  	make8(uint8_t* buf,uint16_t idx);
uint32_t    mask24(uint32_t val);
  

#endif
