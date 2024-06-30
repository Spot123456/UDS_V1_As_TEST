/*
 * UDS_APP_Diag_Cfg.h
 *
 *  Created on: Jun 4, 2024
 *     Author: Omnia
 */

#ifndef INC_UDS_APP_DIAG_CFG_H_
#define INC_UDS_APP_DIAG_CFG_H_

#define Num_of_Services 5


typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef	unsigned int		uint32;
typedef	unsigned long int	uint64;

typedef signed char			int8;
typedef signed short		int16;
typedef	signed int			int32;
typedef	signed long int		int64;

typedef enum {
	Default_Session_Menu = 0,
	Control_Session_Menu,
	Read_Write_Data_Menu,
	Security_Access_Menu,

} ClientMenu;


typedef enum {
	Default_Session = 0,
	Extended_Session,

} SessionType;


typedef enum {
	Control_Service = 0x10,
	Read_Service = 0x22,              // 2*16 +2 32+2 = 34
	Security_Service = 0x27,
	Write_Service = 0x2E,
	Tester_Representer_Service = 0x3E,

} Services;


typedef enum {
	Not_Available = 0x00,
	Available = 0x01,

} Availability;


typedef enum {
	Control_Service_Availability = Available,
	Read_Service_Availability = Available,
	Security_Service_Availability = Available,
	Write_Service_Availability = Available,
	Tester_Representer_Service_Availability = Available,

} Services_Availability;


typedef enum {
	Oil_Temp = 0xF13D,
	Oil_Pressure = 0xF53D,

}DID;




typedef enum {
	Secure = 0x01,
	Un_Secure = 0x00,

}Security_Access_State;


typedef enum {
	DefaultSession = 0x00,
	ExtendedSession = 0x01,
	Seed = 0x02,
	Key = 0x03,

}Sub_Fun;



// Struct For:
//void UDS_Send_Pos_Res(void);
//void UDS_Send_Neg_Res(void);

typedef struct {
	Services SID;
	DID did;
	Sub_Fun sub_fun;
	uint8* data;

}Frame_Info;
typedef struct
{
	uint8 SID;
	uint8 SUB_FUNC;
	uint8 DID;
	uint8 DID_Length;
	uint8* Data;
	uint8 Data_Length;
}ServiceInfo;



typedef struct
{
	uint8* data; 
	uint8 length;
}ST_TP_pduID;


typedef struct
{
	uint8* Data;
	uint32 Lenght;
}PduInfoType;

typedef enum
{
	E_OK = 0x00,
	E_NOK
}Std_ReturnType;


typedef enum
{
	NRC_WRITE_secuirty = 10,
	NRC_SID = 20,
	NRC_sub_fun = 30,
	NRC_sec_key_seed = 40

}NRC_VAR;

#endif /* INC_UDS_APP_DIAG_CFG_H_ */
