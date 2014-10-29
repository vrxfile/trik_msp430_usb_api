/*
 * Trik_ProtocolHandler.c
 *
 *  Created on: 21 ���. 2014 �.
 *      Author: Rostislav Varzar
 */

#include "Trik_Devices/trik_protocolhandler.h"

//Error response
void PROTOCOL_errResponse(char *r_str, uint8_t dev_addr, uint8_t func_code, uint8_t err_code)
{
	char stmp1[MAX_STRING_LENGTH]; //Temp string
	uint8_t crc;
	memset(r_str,0,MAX_STRING_LENGTH);
	if (dev_addr<16)
	    sprintf(r_str,":0%x",dev_addr);
	else
	    sprintf(r_str,":%x",dev_addr);
	if (func_code<0x80) func_code+=0x80;
	if (func_code<16)
	    sprintf(stmp1,"0%x",func_code);
	else
	    sprintf(stmp1,"%x",func_code);
	strcat(r_str,stmp1);
	if (err_code<16)
	    sprintf(stmp1,"0%x",err_code);
	else
	    sprintf(stmp1,"%x",err_code);
	strcat(r_str,stmp1);
	crc=0-(dev_addr+func_code+err_code);
	if (crc<16)
	    sprintf(stmp1,"0%x",crc);
	else
	    sprintf(stmp1,"%x",crc);
	strcat(r_str,stmp1);
	sprintf(stmp1,"\n");
	strcat(r_str,stmp1);
}

//Write register response response
void PROTOCOL_transResponse(char *r_str, uint8_t dev_addr, uint8_t resp_code)
{
    char stmp1[MAX_STRING_LENGTH]; //Temp string
    uint8_t crc;
    memset(r_str,0,MAX_STRING_LENGTH);
    if (dev_addr<16)
        sprintf(r_str,":0%x",dev_addr);
    else
        sprintf(r_str,":%x",dev_addr);
    if (resp_code<16)
        sprintf(stmp1,"0%x",resp_code);
    else
        sprintf(stmp1,"%x",resp_code);
    strcat(r_str,stmp1);
    crc=0-(dev_addr+resp_code);
    if (crc<16)
        sprintf(stmp1,"0%x",crc);
    else
        sprintf(stmp1,"%x",crc);
    strcat(r_str,stmp1);
    sprintf(stmp1,"\n");
    strcat(r_str,stmp1);
}

//Protocol handler
uint8_t PROTOCOL_hadler(char *in_str, char *out_str)
{
	char stmp1[MAX_STRING_LENGTH]; //Temp string
	uint8_t devaddr1; //Device address
	uint8_t func1; //Function number
	uint8_t regaddr1; //Register address
	uint32_t regval1; //Register value
	uint8_t crc1 = 0; //Cheksum
	uint8_t crc2; //Calculated checksum
	uint8_t errhandler; //Result code of handlers

	//Clear output string
	memset(out_str,0,MAX_STRING_LENGTH);

	//Start condition error
	if (in_str[0]!=':')
	{
		sprintf(out_str,":000013ED\n");
		return START_ERROR;
	}

	//Incorrect packet length
	if ((strlen(in_str)!=9) && (strlen(in_str)!=13) && (strlen(in_str)!=17))
	{
		sprintf(out_str,":000014EC\n");
		return LENGTH_ERROR;
	}

	//Get device address
	sprintf(stmp1,"%c%c",in_str[1],in_str[2]);
	devaddr1=strtoul(stmp1,&stmp1[2],16);

	//Get function
	sprintf(stmp1,"%c%c",in_str[3],in_str[4]);
	func1=strtoul(stmp1,&stmp1[2],16);

	//Get register address
	sprintf(stmp1,"%c%c",in_str[5],in_str[6]);
	regaddr1=strtoul(stmp1,&stmp1[2],16);

	//Get register value
	if (func1==0x03)
	{
	    sprintf(stmp1,"%c%c%c%c",in_str[7],in_str[8],in_str[9],in_str[10]);
	    regval1=strtoul(stmp1,&stmp1[4],16);
	}
	if (func1==0x04)
	{
	    sprintf(stmp1,"%c%c%c%c%c%c%c%c",in_str[7],in_str[8],in_str[9],in_str[10],in_str[11],in_str[12],in_str[13],in_str[14]);
	    regval1=strtoul(stmp1,&stmp1[8],16);
	}

	//Device addresses range
	if ((devaddr1>MAX_DEVICES) && (devaddr1!=BSL))
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,DEV_ADDR_ERROR);
		return DEV_ADDR_ERROR;
	}

	//Motor registers addresses range
	if (((devaddr1>=MOTOR1) && (devaddr1<=MOTOR4)) && (regaddr1>0x06))
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,REG_ADDR_ERROR);
		return REG_ADDR_ERROR;
	}

	//Sensor registers addresses range
	if (((devaddr1>=SENSOR1) && (devaddr1<=SENSOR14)) && (regaddr1>0x04))
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,REG_ADDR_ERROR);
		return REG_ADDR_ERROR;
	}

	//Actuator registers addresses range
	if (((devaddr1>=ACTUATOR1) && (devaddr1<=ACTUATOR20)) && (regaddr1>0x04))
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,REG_ADDR_ERROR);
		return REG_ADDR_ERROR;
	}

	//Function number check
	if ((func1!=0x03) && (func1!=0x04) && (func1!=0x05) && (func1!=0x06))
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,FUNC_CODE_ERROR);
		return FUNC_CODE_ERROR;
	}

	//CRC check
	if (func1==0x03)
	{
	    sprintf(stmp1,"%c%c",in_str[11],in_str[12]);
	    crc1=strtoul(stmp1,&stmp1[2],16);
	}
	if (func1==0x04)
	{
	    sprintf(stmp1,"%c%c",in_str[15],in_str[16]);
	    crc1=strtoul(stmp1,&stmp1[2],16);
	}
	if ((func1==0x05) || (func1==0x06))
	{
	    sprintf(stmp1,"%c%c",in_str[11],in_str[12]);
	    crc1=strtoul(stmp1,&stmp1[2],16);
	}
	if ((func1==0x03) || (func1==0x04)) crc2=0-(devaddr1+func1+regaddr1+
	        (uint8_t)(regval1 & 0x000000FF)+(uint8_t)((regval1 & 0x0000FF00) >> 8)+
	        (uint8_t)((regval1 & 0x00FF0000) >> 16)+(uint8_t)((regval1 & 0xFF000000) >> 24));
	if ((func1==0x05) || (func1==0x06)) crc2=0-(devaddr1+func1+regaddr1);
	if (crc1!=crc2)
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,CRC_ERROR);
	    return CRC_ERROR;
	}

	//Hadle of function 0x03 - write single 16 bit register
	if (((func1==0x03) && (strlen(in_str)==13)) || ((func1==0x04) && (strlen(in_str)==17)))
	{
	    //Motors
	    if ((devaddr1>=MOTOR1) && (devaddr1<=MOTOR4))
	    {
	        if (regaddr1==0x00) MOT[devaddr1].MCTL=regval1;
	        if (regaddr1==0x01) MOT[devaddr1].MPWR=regval1;
	        if (regaddr1==0x02) MOT[devaddr1].MFRQ=regval1;
	        if (regaddr1==0x03) MOT[devaddr1].MANG=regval1;
	        if (regaddr1==0x04) MOT[devaddr1].MTMR=regval1;
	        //Error register value
	        if (((MOT[devaddr1].MFRQ==0) || (MOT[devaddr1].MPWR>MOT[devaddr1].MFRQ)) && (MOT[devaddr1].MCTL & 0x0004))
                {
	                PROTOCOL_errResponse(out_str,devaddr1,func1,REG_VAL_ERROR);
	                return REG_VAL_ERROR;
                }
	        else
	            {
	                errhandler=MOTOR_hadler(devaddr1);
	                PROTOCOL_transResponse(out_str,devaddr1,errhandler);
	                //sprintf(out_str,"%x %x %x %x %x %x %x %x\r\n",errhandler,MOT[devaddr1].MCTL,MOT[devaddr1].MFRQ,MOT[devaddr1].MPWR,
	                //        MOT[devaddr1].MOT_EN,MOT[devaddr1].MOT_PWR,MOT[devaddr1].MOT_PWM,MOT[devaddr1].MOT_DIR);

	                return NO_ERROR;
	            }
	    }

	    //BSL
	    if ((devaddr1==BSL) && (regaddr1==0x00))
	    {
	        errhandler=BSL_enterBSL(regval1);
	        PROTOCOL_transResponse(out_str,devaddr1,errhandler);
	        //sprintf(out_str,"%x %x %x\r\n",devaddr1,regaddr1,(regval1==0xA480E917));
	        return NO_ERROR;
	    }

	    //If not found any devices
	    PROTOCOL_errResponse(out_str,devaddr1,func1,DEV_ADDR_ERROR);
	    return DEV_ADDR_ERROR;
	}
	else
	{
	    PROTOCOL_errResponse(out_str,devaddr1,func1,LENGTH_ERROR);
	    return LENGTH_ERROR;
	}













/*

	//Function 0x05 - read single register
    if ((func1==0x05) && (strlen(in_str)!=11))
    {
        PROTOCOL_errResponse(out_str,devaddr1,func1,LENGTH_ERROR);
        return LENGTH_ERROR;
    }
    else
    {

    }

    //Function 0x06 - read single register
    if ((func1==0x06) && (strlen(in_str)!=11))
    {
        PROTOCOL_errResponse(out_str,devaddr1,func1,LENGTH_ERROR);
        return LENGTH_ERROR;
    }
    else
    {

    }
*/
    sprintf(out_str,":FFFFFF03\r\n");
	return UNDEF_ERROR;
}

