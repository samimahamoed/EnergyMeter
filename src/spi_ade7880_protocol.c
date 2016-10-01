   
/*!\file spi_ade7880_protocol.c
   \brief \e File:  spi_ade7880_protocol.c \n
   \brief \e Purpose: \n    
*/


#include "../includes/includes.h"
#include <string.h>
#include <netinet/in.h> 


#ifdef DEBUG
uint8_t debug_print = ENABLE;
#endif

/*!---------------------------------------------------------------------------------
 \defgroup SERIAL_INTERFACE
		
-----------------------------------------------------------------------------------*/
///@{
/*!

    \page  file2 SERIAL INTERFACE
	   
	\brief spi driver application layer, The module handles the data transfer between the Rpi and ADE7880":
	
	
	\li The ADE7880 is always a slave of the communication
	
	\li The maximum serial clock frequency supported by this interface is 2.5 MHz \n\n\n
	
    \section READ SPI Read Operation procedure 
	
	\image html  spi_read_operation.PNG
	\image latex spi_read_operation.PNG
	
    \li The read operation initiated when SS/HSA pin is set to low and begins sending one byte, \n 
	    representing the address of the chip , on the MOSI line. \n
	    Notice:(Bit 0 of the address byte must be 1 for a read operation) \n
	
	\li Next, 16-bit address of the register that is read will be sent. \n
	
	\li Then ADE7880 after it receives the last bit of address of the register on a low-to-high  \n
	    transition of SCLK, it begins to transmit its contents on the MISO line when the next    \n
		SCLK high-to-low transition occurs;  \n
		
	\li After the last bit is received, set the SS and SCLK lines high and the communication ends.  \n
	
	\li The data lines, MOSI and MISO, go into a high impedance state.  \n \n
	
	Please see the datasheet \ref ADE7880 http://www.analog.com/static/imported-files/data_sheets/ADE7880.pdf for more details. \n\n\n 
	
	
   \section WRITE SPI Wirte Operation procedure 
	
	\image html  spi_write_operation.PNG
	\image latex spi_write_operation.PNG
	
    \li The write operation initiated when SS/HSA pin is set to low and begins sending one byte, \n 
	    representing the address of the chip , on the MOSI line. \n
	    Notice:(Bit 0 of the address byte must be 0 for a read operation) \n
	
	\li Next, 16-bit address of the register that is written will be sent. \n
	
	\li And then without losing any SCLK cycle, the 32-, 16-, or 8-bit value will be sent \n
		
	\li After the last bit is received, set the SS and SCLK lines high and the communication ends.  \n
	
	\li The data lines, MOSI and MISO, go into a high impedance state.  \n \n
	
	Please see the datasheet \ref ADE7880 http://www.analog.com/static/imported-files/data_sheets/ADE7880.pdf for more details. 	
	

	
   
       
        
*/






#ifdef DEBUG

/*!
    \brief  used only for debug

*/

void spi_enable_msg_debug_print(uint8_t cmd){
	debug_print =(cmd==DISABLE)?(0x08|DISABLE):ENABLE; //bit 4 is set to indecate external setting 
}
#endif


/*!---------------------------------------------------------------------------------
 @fn int8_t  spi_rd_wr(uint8_t chip_select,uint8_t * tx_buffer,uint16_t len)
 
 @param [in] chip_select    Indicates the selected chip for communication         
 @param [in] tx_buffer      Pointer to transmit buffer 
 @param [in] len	        length of the data    
 
 \brief  initalizes spi of RPi , sends out data from tx_buffer and also receive back 
         data into rc_buffer and end spi to return the normal gpio functionality.
		 
 \return function return 0 or -1, on success or failure respectively.
 
-----------------------------------------------------------------------------------*/
///@{
   
int8_t  spi_rd_wr(uint8_t chip_select,uint8_t * tx_buffer,uint16_t len)
{
   
       
       if(!spi_init(chip_select)){
	
          bcm2835_spi_transfern(tx_buffer,sizeof(tx_buffer));	   
          return 0;		  
	   }else
	      return -1;
    
	   bcm2835_spi_end();
       
	   bcm2835_gpio_write(PIN_SS, HIGH);	   
}
///@}

/*!---------------------------------------------------------------------------------
 @fn int8_t  uint32_t spi_read(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,uint8_t reg_len)
 
 @param [in] chip_select      Indicates the selected chip for communication
 @param [in] chip_address     the chip address or id of the used ADE7880  
 @param [in] target_register  the address of the register to read from 
 @param [in] reg_len	      the size of the register in byte    
 
 \brief  reads the specified register value from the selected ade7880 chip, 
         according to the recommended SPI read Operation procedure, see the 
		 device datasheet for more detail.
		 
 \return function return ther resulting value on success and on failure 0x0F000000.
 
-----------------------------------------------------------------------------------*/
///@{
uint32_t spi_read(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,uint8_t reg_len){
   
  uint32_t result= 0;

    ade7880_read_tx_buff_ut tx_buff;
	
	tx_buff.msg_fields.address_byte.address_all = chip_address;
	tx_buff.msg_fields.address_byte.bits.RD_WR = ADE7880_RD;
	tx_buff.msg_fields.target_register = htons(target_register);
	tx_buff.msg_fields.value.reg32 = DUMMY_MSG;

	#ifdef DEBUG
	 if((debug_print&0x01)){
	   printf("\nMSG SENT  : RD\n");
	   
	   printf("        Chip Address %02X\n",tx_buff.msg_fields.address_byte.address_all);
	   printf("        Target Register %04X\n",ntohs(tx_buff.msg_fields.target_register));
	   printf("        DUMMY %08X \n",tx_buff.msg_fields.value.reg32);
	   }
	#endif
	
	bcm2835_spi_transfern(tx_buff.msg_all,RD_MSG_LENGTH + reg_len);	
	
	
	          
	
	
	
	result =  (reg_len == sizeof(uint32_t))?((uint32_t)ntohl(tx_buff.msg_fields.value.reg32))
             :((reg_len == sizeof(uint16_t))?((uint32_t)ntohs(tx_buff.msg_fields.value.reg16))
     		 :((reg_len == sizeof(uint8_t))?((uint32_t)tx_buff.msg_fields.value.reg8):0x0F000000/*ERROR*/));
	
	
	#ifdef DEBUG
	 if(debug_print&0x01 || 1){
	   printf("\nMSG REPLAY : RD\n");
	   printf("        REPLAY :%X\n",result);
	 }  
	#endif
	
	


	return result;
	
}
///@}
   


/*!---------------------------------------------------------------------------------
 @fn int8_t  uint32_t spi_read(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,uint8_t reg_len)
 
 @param [in] chip_select      Indicates the selected chip for communication
 @param [in] chip_address     the chip address or id of the used ADE7880  
 @param [in] target_register  the address of the register to update 
 @param [in] value	  		  value to be written  
 @param [in] reg_len	      the size of the register in byte 
 
 \brief  update the specified register value on the selected ade7880 chip \n
         according to the recommended SPI write Operation procedure, \n
		 see the device datasheet for more detail \n
         the function will also verify the communication by reading the \n
		 LAST OPERATION,LAST ADDRESS and LAST READ WRITE DATA registers and \n 
		 outputs message to console if there is a failure.
	
 \remark the function dosen't consider 24bit signed value regisers on the device, \n
         if it become necessory to wright to regisers of this kind remember to modify.	
 
 \return function return 0 or -1, on success or failure respectively .
 
 
-----------------------------------------------------------------------------------*/
///@{
int8_t spi_write(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,int32_t value,uint8_t reg_len){
      
	  
	int8_t result = 0;  
    ade7880_write_tx_buff_ut  tx_buff;

	 tx_buff.msg_fields.address_byte.address_all = chip_address;
	 tx_buff.msg_fields.address_byte.bits.RD_WR = ADE7880_WR;
	 tx_buff.msg_fields.target_register = htons(target_register);
	 
    if(reg_len == sizeof(uint8_t))
	  tx_buff.msg_fields.value.reg8  = (uint8_t)value;
    else if(reg_len == sizeof(uint16_t))
	  tx_buff.msg_fields.value.reg16 = htons((uint16_t)value);
	else if(reg_len == sizeof(uint32_t))
	  tx_buff.msg_fields.value.reg32 = htonl((uint32_t)value);
	
	#ifdef DEBUG
	  if(debug_print&0x01){
	   printf("\nMSG SENT: WR\n");
	   printf("       Chip Address %02X\n",
	   tx_buff.msg_fields.address_byte.address_all);
	   printf("       Target Register %04X\n",
	   ntohs(tx_buff.msg_fields.target_register));
	   printf("       Value sent  %08X\n",
	   ntohl(tx_buff.msg_fields.value.reg32));
	   }
	#endif

	bcm2835_spi_transfern(tx_buff.msg_all,WR_MSG_LENGTH + reg_len);
	

	
	uint8_t  result8  =0;
    uint16_t result16 =0;
	int32_t result32 =0;	
	
	 #ifdef DEBUG 
     if(!(debug_print&0x80))
	 debug_print = DISABLE;
	   printf("\nvarifying last operation  ...\n");
	 #endif
	 if((result8 =(uint8_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_OP,sizeof(uint16_t))))
	 !=0xCA /*LAST_OP!=WR*/){
         	 
     #ifdef DEBUG
	   printf("\n                                               <---  WR failure : LAST_OP \n");
	   printf("        LAST_OP value  :%02X\n",result8);   
	 #endif
	 result = -1;
	 }else{
	  #ifdef DEBUG
	   printf("        LAST_OP value  :%02X\n",result8);   
	 #endif
	 }
	 
	 #ifdef DEBUG
	   printf("\nvarifying last accessed register  ...\n");
	 #endif
	 if((result16 =(uint16_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_ADD,sizeof(uint16_t))))!=target_register){
         	 
     #ifdef DEBUG
	   printf("\n                                               <---  WR failure : LAST_ADD\n");
	   printf("        Target Register :%04X\n", target_register);
	   printf("        LAST_ADD value  :%04X\n",result16);   
	 #endif
	 result = -1;
	 }else{
	  #ifdef DEBUG
	    printf("       LAST_ADD value  :%04X\n",result16); 
	  #endif
	 }
	 
    
     #ifdef DEBUG
	   printf("\nvarifying last accessed register  value:\n");
	 #endif
	if(reg_len == sizeof(uint8_t)){
	    result32 = (int32_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_RWDATA8,sizeof(uint8_t))&0x000000FF);
	}else 	
	if(reg_len == sizeof(uint16_t)){
	    result32 = (int32_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_RWDATA16,sizeof(uint16_t))&0x0000FFFF);
	}else
	if(reg_len == sizeof(uint32_t)){
	    result32 = (int32_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_RWDATA32,sizeof(uint32_t)));		
	}	
	//if 

     if(result32!=value)
	     result = -1;	
	 #ifdef DEBUG
	   if(result32!=value)
	   printf("\n                                               <---  WR failure : LAST_RWDATA\n");
	   printf("       Sent Value        :%08X\n", value);
	   printf("       LAST_RWDATA value :%08X\n",result32);   
	  if(!(debug_print&0x80))
	   debug_print = ENABLE;
	 #endif
	 
	 
	return result;
}	
 
 ///@}
 
 

/*!---------------------------------------------------------------------------------
 @fn int8_t spi_ram_protection(uint8_t cmd,uint8_t chip_address)
 
 @param [in] cmd              protection enable/disable command  
 @param [in] chip_select      Indicates the selected chip for communication

 \brief  Function runs RAM protection enable or disable procedure on ADE7880 \n         
	     the function also verifies the communication by reading the \n
		 LAST OPERATION,LAST ADDRESS and LAST READ WRITE DATA registers and \n 
		 outputs message to console if there is a failure.
		 
 \return function return 0 or -1, on success or failure respectively .
 
-----------------------------------------------------------------------------------*/
///@{
int8_t spi_ram_protection(uint8_t cmd,uint8_t chip_address){
      
	  
	int8_t result = 0;  
	
	uint8_t value = (cmd==ENABLE)?0x80:0x00;
    
	 ade7880_ram_lock_msg_ut tx_buff_1st_msg,tx_buff_2nd_msg;

	 tx_buff_1st_msg.msg_fields.address_byte.address_all = chip_address;
	 tx_buff_1st_msg.msg_fields.address_byte.bits.RD_WR = ADE7880_WR;
	 tx_buff_1st_msg.msg_fields.target_register = htons(0xE7FE);
	 tx_buff_1st_msg.msg_fields.value = 0xAD;
	 
	 
	 tx_buff_2nd_msg.msg_fields.address_byte.address_all = chip_address;
	 tx_buff_2nd_msg.msg_fields.address_byte.bits.RD_WR = ADE7880_WR;
	 tx_buff_2nd_msg.msg_fields.target_register = htons(0xE7E3);
	 tx_buff_2nd_msg.msg_fields.value = value;
	 

	#ifdef DEBUG
	   if(debug_print&0x01){
	   printf("\nMSG SENT: WR\n");
	   printf("       Chip Address %02X\n",
	   tx_buff_1st_msg.msg_fields.address_byte.address_all);
	   printf("       1st Target Register %04X\n",
	   ntohs(tx_buff_1st_msg.msg_fields.target_register)
	   );
	   printf("       Value sent 1st  %1X\n",
	   ntohl(tx_buff_1st_msg.msg_fields.value));
	   
	   printf("       2nd Target Register %04X\n",
	   ntohs(tx_buff_2nd_msg.msg_fields.target_register)
	   );
	   printf("       Value sent  %01X\n",
	   ntohl(tx_buff_2nd_msg.msg_fields.value));
	   }
	#endif

	
	bcm2835_spi_transfern(tx_buff_1st_msg.msg_all,RAM_LOCK_MSG_LENGTH);
	bcm2835_spi_transfern(tx_buff_2nd_msg.msg_all,RAM_LOCK_MSG_LENGTH);

	
	uint8_t  result8  =0;
	uint16_t result16 =0;
	
	 #ifdef DEBUG 
	 if(!(debug_print&0x80))
	 debug_print = DISABLE;
	   printf("\nvarifying last operation  ...\n");
	 #endif
	 if((result8 =(uint8_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_OP,sizeof(uint16_t))))
	 !=0xCA /*LAST_OP!=WR*/){
         	 
     #ifdef DEBUG
	   printf("\n                                               <---  WR failure : LAST_OP \n");
	   printf("        LAST_OP value  :%02X\n",result8);   
	 #endif
	 result = -1;
	 }else{
	  #ifdef DEBUG
	   printf("        LAST_OP value  :%02X\n",result8);   
	 #endif
	 }
	 
	 #ifdef DEBUG
	   printf("\nvarifying last accessed register  ...\n");
	 #endif
	 if((result16 =(uint16_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_ADD,sizeof(uint16_t))))!=0xE7E3){
         	 
     #ifdef DEBUG
	   printf("\n                                               <---  WR failure : LAST_ADD\n");
	   printf("        Target Register :%04X\n", 0xE7E3);
	   printf("        LAST_ADD value  :%04X\n",result16);   
	 #endif
	 result = -1;
	 }else{
	  #ifdef DEBUG
	    printf("       LAST_ADD value  :%04X\n",result16); 
	  #endif
	 }
	 
    
     #ifdef DEBUG
	   printf("\nvarifying last accessed register  value:\n");
	 #endif

	 result8 = (uint8_t)(spi_read(BCM2835_SPI_CS0,CHIP_ADDRESS1,LAST_RWDATA8,sizeof(uint8_t))&0x000000FF);
	

     if(result8!=value)
	     result = -1;	
	 #ifdef DEBUG
	   if(result8!=value)
	   printf("\n                                               <---  WR failure : LAST_RWDATA\n");
	   printf("       Sent Value        :%01X\n", value);
	   printf("       LAST_RWDATA value :%01X\n",result8);   
	   
	   if(!(debug_print&0x80))
	       debug_print = ENABLE;
	 #endif

	 
	 
	return result;
}	

///@}
///@}

 

 
 
 
   
