/*!\file spi_ade7880_protocol.h
   \brief \e File:  spi_ade7880_protocol.h \n
   \brief \e Purpose: \n  
*/

#ifndef __SRV_ADE7880_PROTOCOL_H
#define __SRV_ADE7880_PROTOCOL_H




#define ADE7880_RD					1
#define ADE7880_WR					0
#define DUMMY_MSG					0x00000000
#define RD_MSG_LENGTH				3			 
#define WR_MSG_LENGTH				3		
#define RAM_LOCK_MSG_LENGTH		    4	
#define REG_LENGTH(_x)              (sizeof(_x))
	
//! address byte of msg structure RPi to ADE7880  
typedef union
{
    uint8_t address_all;
    struct
    {
        uint8_t RD_WR:1;
        uint8_t chip_address:7;
    }bits;

}address_byte_ut;

typedef union{
uint8_t 		reg8;
uint16_t 		reg16;
uint32_t 		reg32;
}value_ut;

//! ADE7880 read cmd msg structure    
typedef union
{
    uint8_t msg_all[RD_MSG_LENGTH + sizeof(uint32_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte ;
	uint16_t 		target_register __attribute__ ((packed));
	uint32_t 		rc_arg __attribute__ ((packed));
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_read32_tx_buff_ut;


//! ADE7880 read cmd msg structure    
typedef union
{
    uint8_t msg_all[RD_MSG_LENGTH + sizeof(uint16_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	uint16_t 		rc_arg __attribute__ ((packed));
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_read16_tx_buff_ut;


//! ADE7880 read cmd msg structure    
typedef union
{
    uint8_t msg_all[RD_MSG_LENGTH + sizeof(uint8_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	uint8_t 		rc_arg;
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_read8_tx_buff_ut;



//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[WR_MSG_LENGTH + sizeof(uint32_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	uint32_t 		value __attribute__ ((packed));
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_write32_tx_buff_ut;



//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[WR_MSG_LENGTH + sizeof(uint16_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	uint16_t 		value __attribute__ ((packed));
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_write16_tx_buff_ut;





//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[WR_MSG_LENGTH + sizeof(uint32_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register;
		union{
		uint8_t 		reg8;
		uint16_t 		reg16 __attribute__ ((packed));
		uint32_t 		reg32 __attribute__ ((packed));
		}value;
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_write8_tx_buff_ut;




//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[WR_MSG_LENGTH + sizeof(uint32_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	union{
		uint8_t 		reg8;
		uint16_t 		reg16;
		uint32_t 		reg32;
	}value __attribute__ ((packed));

	}msg_fields  __attribute__ ((aligned));
	
}ade7880_read_tx_buff_ut;




//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[WR_MSG_LENGTH + sizeof(uint32_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	union{
		uint8_t 		reg8;
		uint16_t 		reg16;
		uint32_t 		reg32;
	}value __attribute__ ((packed));

	}msg_fields  __attribute__ ((aligned));
	
}ade7880_write_tx_buff_ut;



//! ADE7880 write cmd msg structure    
typedef union
{
    uint8_t msg_all[sizeof(uint16_t)+ 2*sizeof(uint8_t)]  __attribute__ ((aligned));
    
    struct
    {	
	address_byte_ut address_byte;
	uint16_t 		target_register __attribute__ ((packed));
	uint8_t 		value;
	}msg_fields  __attribute__ ((aligned));
	
}ade7880_ram_lock_msg_ut;




uint32_t spi_read(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,uint8_t reg_len);
int8_t   spi_write(uint8_t chip_select,uint8_t chip_address,uint16_t target_register,int32_t value,uint8_t reg_len);

#ifdef DEBUG
void spi_enable_msg_debug_print(uint8_t cmd);
#endif







#endif 


