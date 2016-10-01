
/*!\file includes.h
   \brief \e File:  Includes.h \n
   \brief \e Purpose: \n contains all header files  
*/

#ifndef __INCLUDES_H
#define __INCLUDES_H


#include <inttypes.h>

#define TYPE(cc)  (((cc >= '1' )) ? ((uint8_t)) : ((uint16_t)))

typedef struct
{
     float IRMS;
	 float VRMS;
	 float WH;
	 float POWER;
	 
}phase_data_t;

//! Internal 
typedef struct
{
 
	 phase_data_t phase_a;
	 phase_data_t phase_b;
	 phase_data_t phase_c;	   
}measured_data_t;







#define CHIP_ADDRESS1 			0x70
#define ENABLE 		  			1
#define DISABLE		  			0



#define DSP_START	  			0xFF
#define _VNOMAEN	  			0   			/*!<enable disable Apparent Power Calculation Using VNOM register value */ 
									

//!internal use power mode select macros 
#define PSM0	      			0
#define PSM1	      			1
#define PSM2	      			2
#define PSM3	      			3



/*!

   \page p1 IMPORTANT MACROS    
	
   \li \em CALIBRATION CONSTANTS:
       This constants calculated so that readings fetched from target registers   \n 
	   can be converted into meaningful values such as amps , volts, kwh, watt.   \n 
	   The constants should be adjusted if there is any change to Hardware design \n 
	   The calibration steps can be found from this
 \ref calibration document \n http://www.analog.com/static/imported-files/application_notes/AN-1171.pdf . \n
 \note Since all phases on the meter are matched, the same constant can be used \n 
	   for all energy, power,current rms and voltage rms readings on any Rpi 
	   
   \verbatim
VLSB_CONST	  		  	211184.71337579617834394904458599        LSB constant for voltage reading 24 volt max is assumed
ILSB_CONST	  		  	353022.88732394					         LSB constant for current reading 30 Amp max is assumed
WHLSB_CONST	  		  	(67.28)*2.275  				             LSB constant for energy reading
WATTLSB_CONST	      	8505							         LSB constant for power reading  
   \endverbatim
*/

#define VLSB_CONST	  		  	211184.71337579617834394904458599  /*!<LSB constant for voltage reading 24 volt max is assumed*/
#define ILSB_CONST	  		  	353022.88732394					   /*!<LSB constant for current reading 30 Amp max is assumed*/
#define WHLSB_CONST	  		  	(67.28)*2.275  				       /*!<LSB constant for energy reading*/
#define WATTLSB_CONST	      	8505							   /*!<LSB constant for power reading*/



/*!
   \page p1 IMPORTANT MACROS  
   \li \em TRANSFORMER_RATIO \n
        This value is the ratio of the voltage transformer \n 
		used to step down the line voltage to lower level.
        the value should be adjusted if different transformer is used.
    \verbatim	
     TRANSFORMER_RATIO     	20.56621005
    \endverbatim
*/
#define TRANSFORMER_RATIO     	20.56621005  
       



/*!

   \page p1 IMPORTANT MACROS
   \li \em CONFIGURATION COMMANDS 
       This macros determine the common configuration file  \n 
	   between the phyton script and this software. And the \n 
	   configuration command structure. \n 
	   You only need to adjust this values, to obtain the \n 
	   corresponding changes on the program execution.  
   \verbatim
	CONFIG_FILE_NAME  		"config.csv"            defines configuration           
	CONFIG_CMD_FORMAT 		"%hd %s %lu %hhu %hhu"  configuration command format specifier
   \endverbatim
*/
#define CONFIG_FILE_NAME  		"config.csv"            /*!< defines configuration file */          
#define CONFIG_CMD_FORMAT 		"%hd %s %lu %hhu %hhu"  /*!< configuration command format specifier */ 




#include <stdbool.h>
#include <unistd.h>
#include <bcm2835.h>
#include <stdio.h>



#include "ade7880_registers.h"
#include "rpi_hwio.h"
#include "spi_ade7880_protocol.h"
#include "srv_cmd_handler.h"



int8_t 		save_to_file(uint16_t rpi_address,measured_data_t data, const char *fileName,uint8_t clean) ;
int8_t 		wait_new_conversion(void);
int16_t     config_recheck(const char * config_file, const char * format, ...);
void        reading_loop();


#endif

