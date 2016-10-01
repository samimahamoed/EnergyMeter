/*!\file ade7880_registers.h
   \brief \e File:  srv_ade7880_registers.h \n
   \brief \e Purpose: \n   
*/

#ifndef __ADE7880_REGISTERS_H 
#define __ADE7880_REGISTERS_H 






/*!see the datasheet about this const
\ref ADE7880 http://www.analog.com/static/imported-files/data_sheets/ADE7880.pdf .
*/
#define GAIN_1     			0b000       
#define GAIN_2     			0b001   
#define GAIN_4     			0b010 
#define GAIN_8     			0b011
#define GAIN_16    			0b100




#define MODE_0_0     		0b00       
#define MODE_0_1     		0b01   
#define MODE_1_0     		0b10 
#define MODE_1_1     		0b11

#define DUMMIY   0xEBFF    
#define CHECKSUM 0xE51F 



//!Threshold registers 
#define WTHR_DEFULT         0x03 		/*!<Threshold used in phase total /fundamental active power data path.*/ 
#define VARTHR_DEFULT		0x03 		/*!<Threshold used in phase total /fundamental reactive power data path.*/ 
#define VATHR_DEFULT		0x03 		/*!<Threshold used in phase apparent power data path.*/
#define VLEVEL_DEFULT		0x38000	    /*!<use Equation 26, Equation 37, Equation 44, Equation 22, and Equation 42
										in the datasheet to determine this values */
//!Physical Address
#define WTHR 				0xEA02 
#define VARTHR 				0xEA03 
#define VATHR 				0xEA04 
#define VLEVEL 				0x439F 
#define LAST_RWDATA8	    0xE7FD
#define LAST_RWDATA16	    0xE9FF
#define LAST_RWDATA32	    0xE5FF

//!DSP run control register
#define RUN 				0xE228




  
//! ADE7880 Gain Register Physical address and structure  
typedef enum pga_et
{ 
   gain_1  = GAIN_1,       
   gain_2  = GAIN_2,   
   gain_4  = GAIN_4, 
   gain_8  = GAIN_8,
   gain_16 = GAIN_16
}pga_et;

#define GAIN 0xE60F 
typedef union
{
    uint16_t reg_all;
    struct
    {
	   enum pga_et PGA1:3;
       enum pga_et PGA2:3; 	   
       enum pga_et PGA3:3; 
    }bits;

}gain_reg_u;

//! ADE7880 PHSIGN Register Physical address and structure   
#define CONFIG 0xE618 
typedef union
{
    uint16_t reg_all;
    struct
    {
	   uint16_t INTEN     	:1;
	   uint16_t RESERVED    :1;
       uint16_t CF2DIS      :1;
       uint16_t SWAP	   	:1;	
       uint16_t MOD1SHORT  	:1;
	   uint16_t MOD2SHORT 	:1;
       uint16_t HSDCEN		:1;
       uint16_t SWRST	 	:1;	   
	   uint16_t VTOIA	 	:2;	
	   uint16_t VTOIB	 	:2;
       uint16_t VTOIC	 	:2;		   
	   uint16_t RESERVED2 	:2;	
       
    }bits;

}config_reg_u;


//! ADE7880 CONFIG2 Register Physical address and structure    
#define CONFIG2 0xEC01 
typedef union
{
    uint8_t reg_all;
    struct
    {
        uint8_t EXTREFEN:1;
        uint8_t I2C_LOCK:1;
		uint8_t RESERVED:6;
    }bits;

}config2_reg_u;



//! ADE7880  Register Physical address and structure   
typedef enum
{  //see the datasheet about this const
   mode_0_0 = MODE_0_0,       
   mode_0_1 = MODE_0_1,   
   mode_1_0 = MODE_1_0, 
   mode_1_1 = MODE_1_1
  
}mode_et;
  
#define ACCMODE 0xE701 
typedef union accmode_reg_u
{
    uint8_t reg_all;
    struct
    {
	   mode_et 		WATTACC :2;
       mode_et 		VARACC  :2; 	   
       mode_et 		CONSEL  :2; 
	   uint8_t 		REVAPSEL:1;
	   uint8_t 		RESERVED:1;
	   
    }bits;

}accmode_reg_u;

//! ADE7880 PHSIGN Register Physical address and structure   
#define PHSIGN 0xE617  
typedef union
{
    uint16_t reg_all;
    struct
    {
	   uint16_t AWSIGN     	:1;
	   uint16_t BWSIGN     	:1;
       uint16_t CWSIGN     	:1;
       uint16_t SUM1SIGN   	:1;	
       uint16_t AFVARSIGN  	:1;
	   uint16_t BFVARSIGN 	:1;
       uint16_t CFVARSIGN	:1;
       uint16_t SUM2SIGN 	:1;	   
	   uint16_t SUM3SIGN 	:1;	
	   uint16_t RESERVED 	:7;	
       
    }bits;

}phsign_reg_u;





//! ADE7880 CONFIG3 Register Physical address and structure   

#define CONFIG3 0xEA00 
#define CONFIG3_DEFULT         0x01 		
typedef union
{
    uint8_t reg_all;
    struct
    {
	    uint8_t HPFEN		:1;
        uint8_t LPFSEL  	:1;
		uint8_t INSEL   	:1;
		uint8_t ININTEN	    :1;
        uint8_t RESERVED    :1;
		uint8_t RESERVED2	:3;
       
    }bits;

}config3_reg_u;


//! ADE7880 LCYCMODE Register Physical address and structure   
#define LCYCMODE 0xE702  
typedef union
{
    uint8_t reg_all;
    struct
    {
	    uint8_t LWATT		:1;
        uint8_t LVAR 		:1;
		uint8_t LVA  		:1;
		uint8_t ZXSEL_A	    :1;
        uint8_t ZXSEL_B 	:1;
		uint8_t ZXSEL_C  	:1;
		uint8_t RSTREAD	    :1;
        uint8_t PFMODE 	    :1;
    }bits;

}lcycmode_reg_u;


#define STATUS0 0xE502 
typedef union
{
    uint32_t reg_all;
    struct
    {
	  uint32_t NOT_USED:17;
	  uint32_t DREADY  :1;
	  uint32_t REVPSUM3:1;
	  uint32_t HREADY  :1;
	  uint32_t RESERVED:13;
	  
       
    }bits;

}status0_reg_u;




#define COMPMODE 0xE60E 

typedef union
{
    uint16_t reg_all;
    struct
    {
    	uint16_t TERMSEL1_0 :1;
        uint16_t TERMSEL1_1 :1;
    	uint16_t TERMSEL1_2 :1;
    	uint16_t TERMSEL2_0 :1;
        uint16_t TERMSEL2_1 :1;
    	uint16_t TERMSEL2_2 :1;
    	uint16_t TERMSEL3_0 :1;
        uint16_t TERMSEL3_1 :1;
    	uint16_t TERMSEL3_2 :1;	
        uint16_t ANGLESEL	:2;
        uint16_t VNOMAEN    :1;
        uint16_t VNOMBEN    :1;
        uint16_t VNOMCEN    :1;
        uint16_t SELFREQ    :1;	
        uint16_t RESERVED	:2;
        
       
    }bits;

}compmode_reg_u;


#define STATUS1 0xE503 
typedef union
{
    uint32_t reg_all;
    struct
    {
	
       
    }bits;

}status1_reg_u;



#define MASK0 0xE50A 
typedef union
{
    uint32_t reg_all;
    struct
    {
    
	
       
    }bits;

}mask0_reg_u;


#define MASK1 0xE50B 
typedef union
{
    uint32_t reg_all;
    struct
    {
	 uint32_t NOT_USED :24;
	 uint32_t CRC      :1;
	 uint32_t RESERVED :6;
       
    }bits;

}mask1_reg_u;



//!  Registers




#define LAST_ADD 0xE9FE 

#define LAST_OP 0xEA01 

#define LAST_RWDATA_8bit 0xE7FD 

#define LAST_RWDATA_16bit 0xE9FF 

#define LAST_RWDATA_24bit 0xE5FF 


//! Energy Registers

//! phase magnitude registers 
#define AVA 0xE519 
#define BVA 0xE51A 
#define CVA 0xE51B 


 
#define AIGAIN 0x4380 
#define AVGAIN 0x4381 
#define BIGAIN 0x4382 
#define BVGAIN 0x4383 
#define CIGAIN 0x4384 
#define CVGAIN 0x4385 
#define NIGAIN 0x4386 
#define DICOEFF 0x4388 
#define APGAIN 0x4389 
#define AWATTOS 0x438A 
#define BPGAIN 0x438B 
#define BWATTOS 0x438C 
#define CPGAIN 0x438D 
#define CWATTOS 0x438E 
#define AIRMSOS 0x438F 
#define AVRMSOS 0x4390 
#define BIRMSOS 0x4391 
#define BVRMSOS 0x4392 
#define CIRMSOS 0x4393 
#define CVRMSOS 0x4394 
#define NIRMSOS 0x4395 
#define HPGAIN 0x4398 
#define ISUMLVL 0x4399 

#define AFWATTOS 0x43A2 
#define BFWATTOS 0x43A3 
#define CFWATTOS 0x43A4 
#define AFVAROS 0x43A5 
#define BFVAROS 0x43A6 
#define CFVAROS 0x43A7 
#define AFIRMSOS 0x43A8 
#define BFIRMSOS 0x43A9 
#define CFIRMSOS 0x43AA 
#define AFVRMSOS 0x43AB 
#define BFVRMSOS 0x43AC 
#define CFVRMSOS 0x43AD 
#define HXWATTOS 0x43AE 
#define HYWATTOS 0x43AF 
#define HZWATTOS 0x43B0 
#define HXVAROS 0x43B1 
#define HYVAROS 0x43B2 
#define HZVAROS 0x43B3 
#define HXIRMSOS 0x43B4 
#define HYIRMSOS 0x43B5 
#define HZIRMSOS 0x43B6 
#define HXVRMSOS 0x43B7 
#define HYVRMSOS 0x43B8 
#define HZVRMSOS 0x43B9 
#define AIRMS 0x43C0 
#define AVRMS 0x43C1 
#define BIRMS 0x43C2 
#define BVRMS 0x43C3 
#define CIRMS 0x43C4 
#define CVRMS 0x43C5 
#define NIRMS 0x43C6 
#define ISUM 0x43C7 
 
#define AWATTHR 0xE400 
#define BWATTHR 0xE401 
#define CWATTHR 0xE402 
#define AFWATTHR 0xE403 
#define BFWATTHR 0xE404 
#define CFWATTHR 0xE405 
#define AFVARHR 0xE409 
#define BFVARHR 0xE40A 
#define CFVARHR 0xE40B 
#define AVAHR 0xE40C 
#define BVAHR 0xE40D 
#define CVAHR 0xE40E 
#define IPEAK 0xE500 
#define VPEAK 0xE501 

#define AIMAV 0xE504 
#define BIMAV 0xE505 
#define CIMAV 0xE506 
#define OILVL 0xE507 
#define OVLVL 0xE508 
#define SAGLVL 0xE509 

#define IAWV 0xE50C 
#define IBWV 0xE50D 
#define ICWV 0xE50E 
#define INWV 0xE50F 
#define VAWV 0xE510 
#define VBWV 0xE511 
#define VCWV 0xE512 
#define AWATT 0xE513 
#define BWATT 0xE514 
#define CWATT 0xE515 
#define AFVAR 0xE516 
#define BFVAR 0xE517 
#define CFVAR 0xE518 

#define VNOM 0xE520 

#define PHSTATUS 0xE600 
#define ANGLE0 0xE601 
#define ANGLE1 0xE602 
#define ANGLE2 0xE603 
#define PHNOLOAD 0xE608 
#define LINECYC 0xE60C 
#define ZXTOUT 0xE60D 


#define CFMODE 0xE610 
#define CF1DEN 0xE611 
#define CF2DEN 0xE612 
#define CF3DEN 0xE613 
#define APHCAL 0xE614 
#define BPHCAL 0xE615 
#define CPHCAL 0xE616 

 
#define MMODE 0xE700 


#define PEAKCYC 0xE703 
#define SAGCYC 0xE704 
#define CFCYC 0xE705 
#define HSDC_CFG 0xE706 
#define Version 0xE707 

#define FVRMS 0xE880 
#define FIRMS 0xE881 
#define FWATT 0xE882 
#define FVAR 0xE883 
#define FVA 0xE884 
#define FPF 0xE885 
#define VTHDN 0xE886 
#define ITHDN 0xE887 
#define HXVRMS 0xE888 
#define HXIRMS 0xE889 
#define HXWATT 0xE88A 
#define HXVAR 0xE88B 
#define HXVA 0xE88C 
#define HXPF 0xE88D 
#define HXVHD 0xE88E 
#define HXIHD 0xE88F 
#define HYVRMS 0xE890 
#define HYIRMS 0xE891 
#define HYWATT 0xE892 
#define HYVAR 0xE893 
#define HYVA 0xE894 
#define HYPF 0xE895 
#define HYVHD 0xE896 
#define HYIHD 0xE897 
#define HZVRMS 0xE898 
#define HZIRMS 0xE899 
#define HZWATT 0xE89A 
#define HZVAR 0xE89B 
#define HZVA 0xE89C 
#define HZPF 0xE89D 
#define HZVHD 0xE89E 
#define HZIHD 0xE89F 
#define HCONFIG 0xE900 
#define APF 0xE902 
#define BPF 0xE903 
#define CPF 0xE904 
#define APERIOD 0xE905 
#define BPERIOD 0xE906 
#define CPERIOD 0xE907 
#define APNOLOAD 0xE908 
#define VARNOLOAD 0xE909 
#define VANOLOAD 0xE90A 


#define HX_reg 0xEA08 
#define HY_reg 0xEA09 
#define HZ_reg 0xEA0A 
#define LPOILVL 0xEC00 

 
#endif /* __ADE7880_REGISTERS_H */ 
 
 
 
 
 
 