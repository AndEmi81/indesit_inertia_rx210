/*!
//==========================================================================================
//
// Filename:     SetProfile.h
// Author:       L. Morbidelli
// Revision:     1.0.0
//
// Date: 		 21/10/2009
//
// Description:  
//
// Notes:		 
//				 
//==========================================================================================
*/

#ifndef __UP_SETPROFILE_H
#define __UP_SETPROFILE_H



#if (CHECKERRORS_ENABLE == 1)
	#define _SETPROFILE_CHECKERRORS_
#endif

/*DEFINES*/

#define SETPROFILE_INIT			0
#define SETPROFILE_EXE			1
#define SETPROFILE_TRUE			2

#define VEL_FAN_ON_MASK			0x0C
#define VEL_FAN_OFF_MASK		0x03

#define PROFILE_C_MASK			0x0f	
#define PROFILE_NS_MASK			0xf0

#define PROFILE_CF_MASK			0x80
/* Per la base tempi impieghiamo tutto un byte , così abbiamo una discretizzazione pi� fine.*/

#define SEG_TIME_BASE			OFFS_B(Tab_HWCSPR_TimeBase.Data,1)



#define PROFILE_ON				1
#define PROFILE_OFF				0

/*PROTOTYPES*/

uint8_t	SPF_SetProfile_Exe(uint8_t highStatement, uint8_t lowStatement);
void SPF_SetProfileLoadMng(uint8_t nr_load_profile_temp,uint8_t activation_value_temp, Loader_Struct uPSetProfile_setting_data_tmp_loc);




#endif  /* __UP_SETPROFILE_H */
