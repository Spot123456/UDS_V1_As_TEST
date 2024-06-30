






// croky \|/ 0_0 \|/


//in server callback
//{
	//recieve request frame from tp
	//check SID
	//if (SID == control) --> void UDS_Process_Session(void);
	//if (SID == read) -->  void UDS_Read_Data_Server(void);
	//if (SID == write) --> void UDS_Write_Data_Server(void);
	//if (SID == security) --> void send_ser_sec();
	//if (SID == testerPresent) --> void UDS_Tester_Present(void); // reset time out

	//Note: any function should have itsown logic by +ve or -ve response -->void UDS_Send_Pos_Res(void); -->void UDS_Send_Neg_Res(void);
	//else: SID invalid -ve reponse invalid SID



/***************************************************************
						 INCLUDES
 ***************************************************************/
#include "../Inc/UDS_APP_Diag_Cfg.h"
#include "../Inc/UDS_APP_Diag.h"
#include "../Inc/UDS_APP_Diag_Pri.h"
#include <stdio.h>
#include <stdlib.h>


 /***************************************************************
						  defines
  ***************************************************************/

// there are many ser. has sub,  this var indecate for which sub servise
#define sub_func_control		 1 


 /***************************************************************
						  global var
  ***************************************************************/
uint8 global_sec_flag = 0;
uint8 global_session = Default_Session;
uint8 flag_sub_fun = 0;
uint8 Security_Service_Availability_flag = 0;
uint32 seed = 40;
uint32 oil_temp = 50; 
uint32 oil_pre = 0;
uint32 oil_temp_var =50;
uint32 oil_pressure_var = 60;

/***************************************************************
						functions
 ***************************************************************/

Std_ReturnType CanTp_Transmit(uint8 TxPduId, PduInfoType* PduInfoPtr);
/**************************************************************/

Std_ReturnType CanTp_RxIndication(uint8 RxPduId, PduInfoType* PduInfoPtr);

/**************************************************************/


/***************************************************************************************/

// function used to edit the value of term or pres in app 

uint8 check_DID(uint16 DID, uint32 data)
{
	if (Oil_Temp == DID)
	{
		oil_temp_var = (data>>16); // update the 2 bytes data for Oil temperature
		return 1; // Indicating success
	}
	else if (Oil_Pressure == DID)
	{
		oil_pressure_var = data ; // update the 4 bytes data for Oil pressure
		return 1; // Indicating success
	}
	else
	{
		return 0; // Indicating failure
	}
}


void UDS_Write_Data_Server(uint8* received_data, uint16 received_length)
{
	/*???????????????????????*/
	uint8 received_data_l = sizeof(received_data);
	if (received_length != received_data_l - 1)
	{
		//return -ive response NRC data length != length that assign to the frame
	}

	// Extract the DID from the received data
	uint16 DID = (received_data[2] << 8) | received_data[3];

	// Extract the data from the received data
	uint32 data;
	data = (received_data[4] << 24) | (received_data[5] << 16) | (received_data[6] << 8) | received_data[7];


	// update the data in the required DID
	uint8 returnType = check_DID( DID, data);
	// according to the returnType if it is equal to 1 --> positive response - but if it is equal to 0 --> negative response

	/*********************************
		this is the logic of +ive resp ( we need edit ) 
	*************************************/
	// Define the positive response array
	uint8 arr[7]; // Adjust size as needed
	uint8  SID_response = 0x6E; // Positive response SID (0x2E + 0x40)

	// Fill the local array with SID_response, DID, and data
	arr[0] = SID_response;
	arr[1] = (DID >> 8) & 0xFF; // Most significant byte of DID
	arr[2] = DID & 0xFF;        // Least significant byte of DID

	// Assuming Data is 4 bytes
	arr[3] = (data >> 24) & 0xFF; // Most significant byte of data
	arr[4] = (data >> 16) & 0xFF;
	arr[5] = (data >> 8) & 0xFF;
	arr[6] = data & 0xFF;		  // Least significant byte of data

	PduInfoType hamada_write;
	
	// Prepare the TP structure
	hamada_write.Data = arr;
	hamada_write.Lenght = sizeof(arr);

	// Transmit the data through CAN_TP using this function
//	CanTP_Transmit(0, &hamada_write);
}
/******************************************************************************/




/*************************************************************/
/*
void UDS_Read_Data_Server( uint8 Data[])
{
	uint8 Send_counter_Temp;
	if (Data[1] == Read_Service)
	{

		// void read_DID (DID)
		switch (Data[2])
		{

		case Oil_Temp:
			//HAL_UART_Transmit(&huart2, (const uint8*)"\r\n Oil Temp .\r\n", 50, HAL_MAX_DELAY);
			//UDS_Read_Data_Client(Frame_Info.DID);
			Data[4] = oil_temp_var >> 8;
			Data[5] = (uint8)oil_temp_var;
		
			// send +ive resp with data
		//	ServiceInfo nour;

		//	UDS_Send_Pos_Res(nour);

			// send +ive with data
			break;
		case Oil_Pressure:
			// save the data 
			Data[4] = oil_temp_var >> 24;
			Data[5] = oil_temp_var >> 16;
			Data[6] = oil_temp_var >> 8;
			Data[7] = (uint8)oil_temp_var;
			break;

			//send +ive resp with data
		}

	}
			


}
*/
/**************************************************************/


/*************************************************
 void server_call_back()

 Description	:	
 *	- The function initializes a flag to check the validity of the received SID.
 *  - The function then validates the received SID against a set of predefined services.
 *  - Based on the validated SID, it executes corresponding service actions, such as Control Service,
 *    Read Service, Security Service, Write Service, and Tester Representative Service.
 *  - If the SID is valid and matches Control Service, it further checks the session type and changes
 *    the session state accordingly.
 Synch/Asynch	:	Synchronouse performs operations sequentially and returns once all actions are completed.
 Reentrance		:	non reeentrant because global variable
 parameter in	:	null	(void)
 parameter out	:	null	(void)
 return value	:	void

***************************************************/
void server_call_back()
{
	// create flag for check SID
	uint8 local_sid_flag = 0;
	
	uint8 data[20] = { Control_Service , ExtendedSession };
	// creat str to save the data into it  
	PduInfoType hamada; 
	// recieve response frame from tp
	//CanTp_RxIndication(0,& hamada);
	// let for example the return is this
	hamada.Data = data;
	hamada.Lenght = 2;
	// read from this array 
	printf("%d\n", *hamada.Data);
	printf("%d\n", hamada.Lenght);

	// for validation 
	if (hamada.Data[0] == Control_Service || hamada.Data[0]== Read_Service || hamada.Data[0] == Write_Service || hamada.Data[0] == Security_Service || hamada.Data[0] == Tester_Representer_Service)
	{
		// tmam 
		local_sid_flag = 1; 
		printf(" +ive there is SID valid \n");

	} 

	else
	{
		// return -ive response
		printf(" -ive there is no SID has this name \n");
		return;
	}

	if (local_sid_flag)
	{
		if (hamada.Data[0] == Control_Service)
		{
			// call this func
			printf("u are in Control_Service \n");
		
			flag_sub_fun = 1;
		}
		else if (hamada.Data[0] == Read_Service)
		{
			//  for test
			printf("u are in Read_Service\n");
			// send read function (rad resp as the actual data of temp or pressure)
			printf("UDS_Read_Data_Server() // read the value of temp or pressure oil  ");
		//	UDS_Read_Data_Server(data);
		}
		else if (hamada.Data[0] == Security_Service && Security_Service_Availability_flag == Available)
		{
			// if sid of sec true and security flag is available 
			// this printf for test only
			printf("u are in Security_Service\n");
			// send +ive resp 
			printf("send_ser_sec() +ive resp  \n");
		}
		else if (hamada.Data[0] == Security_Service && Security_Service_Availability_flag == Not_Available)
		{
			// if sid of sec true and security flag is not available 
			printf("u are in Security_Service\n");
			// send -ive resp 
			printf("send_ser_sec() //send the seead and compare it after return the response  \n");
		}
		else if (seed != seed + 5)
		{
			// send - ive response  i.e. the seed is the global var that access the time for security
			//HAL_UART_Transmit(&huart2, (const uint8_t*)" send -ive resp for wrong key ", 50, HAL_MAX_DELAY);
			printf(" send -ive resp for wrong key ");
			// 

		}
		else if (hamada.Data[0] == Write_Service && global_sec_flag ==1 )
		{
			printf("u are in Write_Service\n");
			// send write response
			printf("UDS_Write_Data_Server() \n");
			// use the function of write 
			// the fram + length in first data array 
			UDS_Write_Data_Server(data, data[0]);

		}
		else if (hamada.Data[0] == Write_Service && global_sec_flag == 0)
		{
			printf("u are not in Write_Service\n");
			// send -ive response 
			printf("UDS_Write_Data_Server() \n");
			// send write data with -ive response (we need security 
			//------------------->
		}
		else if (hamada.Data[0] == Tester_Representer_Service)
		{
			printf("u are in Tester_Representer_Service\n");
			// call the fun of tester Representer
			printf("void UDS_Tester_Present(void) \n");
		}
	}
	// check sub fun 
	if (flag_sub_fun== sub_func_control)
	{
		// true sub fun 
		if (data[0]== Control_Service && data[1] == DefaultSession)
		{
			// change the state to default 
			printf(" UDS_Process_Session(void); \n "); 
			printf(" changed to DefaultSession ");
			global_session = Default_Session;
			
		}
		else if (data[0] == Control_Service && data[1] == ExtendedSession)
		{
			// change to extended 
			printf(" changed to ExtendedSession ");
			global_session = Extended_Session;
		}
		else
		{
			printf(" this sub not supported ");
			// return - ive response 
		}
	}
	else
	{

		// error in sub func 
		
	}
	
}

/**************************************************************/

void send_ses_Def()
{
	// init struct var  to use for send TP
	ST_TP_pduID hamada;
	// def the length of array that be send
	uint8 length = 2; 
	// the data that be send
	uint8 data[2] = { Control_Service , DefaultSession }; 

	// fill the struct data 
	hamada.data = data;
	hamada.length = length;

	// send to can tp 
	//CanTp_Transmit(0, &hamada);
}

/**************************************************************/
void send_ses_ext()
{
	// init struct var  to use for send TP
	ST_TP_pduID hamada;

	// def the length of array 
	uint8 length = 2;

	// the data that be send
	uint8 data[2] = { Control_Service , ExtendedSession };

	// fill the struct data 
	hamada.data = data;
	hamada.length = length;

	// send to can tp 
	//CanTp_Transmit(0, &hamada);
}

/**************************************************************/

/*typedef struct
{
	uint8 Data[4096];
	uint32 Length;
}PduInfoType;
*/






int main()
{
	server_call_back();
	//printf("hamada");
	printf("the data befor write oil_temp_var %d\n", oil_temp_var);
	printf("the data befor write oil_pressure_var %d\n", oil_pressure_var);
	uint8 data[] = { 7,Write_Service ,0xF5,0x3D, 0x0,0x1 ,0,0 };
	//UDS_Write_Data_Server(data, 7);
	printf("the data after write oil_temp_var %d\n", oil_temp_var);
	printf("the data after write  oil_pressure_var %d\n", oil_pressure_var);
	return 0;
}






//}

/* USE_FULL_ASSERT */
