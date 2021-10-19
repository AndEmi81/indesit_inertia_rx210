#define __DRIVER_H
#include "HAL_Interface.h"
#include  HAL_CFG
#include  HAL
#include "IO_driver.h"

/* Function's pointer for Erase Page Command Executed in RAM  */
typedef void (*pFLS_ErasePageCmdIsInRam)(FLS_paramType param);
/* Function's pointer for Burst Writing Command Executed in RAM  */
typedef void (*pFLS_BurstProgCmdIsInRam)(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);

volatile static int HAL_Error;
void HAL_ErrorHandler( int err )
{
    HAL_Error = err;
    while(HAL_Error!=0)
    {
        HAL_Error = err;
    }
}

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */


bool_t DIO_IsNotDummyIO(const IO_ModuleIOMapType* ptr)
{
  return (bool_t) ( ((ptr->PortAddress.basic) != (DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR) && ((ptr->PinMask) != 0))?(bool_t)1:(bool_t)0;
}
/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/**
 * Function: CheckClearSourceIdFailed
 * Description: this function fails if the clock source for modulo (clear source) is correct
 * Note:
 * Status:
 */

bool_t TMR_CheckClearSourceIdFailed(TMR_CNTAddressType tmrID, uint8_t clrSrcId) 
{
    (void)tmrID;
    (void)clrSrcId;
    return 0;
}

/*
 * Function: TMR_SetPrescaler
 * Description:
 */
void TMR_SetPrescaler(TMR_CNTAddressType tmrID, uint8_t prescaler) 
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  module->sc = (uint8_t)((module->sc & ~TPM3SC_PS_MASK) | (((prescaler) << TPM3SC_PS_BITNUM) & TPM3SC_PS_MASK)); 
}

/*
 * Function: TMR_SetPrescalerDfltValue
 * Description: Set Prescaler to the default value
 * Note: 
 * Status: 
 */
void TMR_SetPrescalerDfltValue(TMR_CNTAddressType tmrID) 
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  module -> sc &= (~TPM3SC_PS_MASK);
}

/*
 * Function: TMR_GetCounter
 * Description: return the counter value
 * Note:
 * Status: 
 */
IO_HwTimerType TMR_GetCounter( TMR_CNTAddressType tmrID) 
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;  
  return module->cnt;
}

/*
 * Function: TMR_SetCounter
 * Description: Set the timer counter register value
 * Note:
 * Status: 
 */
void TMR_SetCounter( TMR_CNTAddressType tmrID, uint16_t value)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;  
  module->cnt = value;
}

/*
 * Function: TMR_SetCounterDfltValue
 * Description:
 * Note:
 * Status:
 */
void TMR_SetCounterDfltValue(TMR_CNTAddressType tmrID)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  module->cnt = 0;
}


/*
 * Function: TMR_SetCountDir
 * Description:
 * Note: 
 * Status:  
 */
void TMR_SetCountDir( TMR_CNTAddressType tmrID, uint8_t direction ) 
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;

  if (direction == 2){    
    module->sc |= TPM3SC_CPWMS_MASK; 
  }else{
    module->sc &= ~TPM3SC_CPWMS_MASK;  
  }
}

/*
 * Macro: TMR_SetCountDirDfltValue
 * Description:
 * Note: Only up counter direction
 */
void TMR_SetCountDirDfltValue(TMR_CNTAddressType tmrID)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  module->sc &= ~TPM3SC_CPWMS_MASK;
}


void TMR_SetModulo(TMR_CNTAddressType tmrID, Period_ValueType period, uint8_t clrSrcId) 
{
  
    tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;  
	(void)clrSrcId;
    module->mod = period;  
}

/*
 * Function: Period_ValueType
 * Description:
 * Note: 
 * Status:
 */
Period_ValueType TMR_GetModulo(TMR_CNTAddressType tmrID, uint8_t channel) 
{
    
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;    
  (void)channel;
  return module->mod;
}

/*
 * Function: TMR_SetModuloDfltValue
 * Description: Set Timer Counter Modulo Reg. to the default value
 * Note: 
 * Status: 
 */
void TMR_SetModuloDfltValue(TMR_CNTAddressType tmrID, uint8_t clrSrcId)
{

  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  (void)clrSrcId;
  module->mod = 0;
}


/*
 * Function: TMR_SetClockSource
 * Description:  Start the timer, enable on & reset off
 * Status: 
 */
void TMR_SetClockSource(TMR_CNTAddressType tmrID, uint8_t source)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  module->sc = (uint8_t)((module -> sc & ~TPM3SC_CLKSx_MASK) | (((source + 1) << TPM3SC_CLKSx_BITNUM) & TPM3SC_CLKSx_MASK));
}
/*
 * Function: TMR_StopClockSource
 * Description: Disable Clock source, TSTR Register Start/Stop timer counter
 * Note:
 * Status: 
 */
void TMR_StopClockSource( TMR_CNTAddressType tmrID)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;    
  module->sc &= ~TPM3SC_CLKSx_MASK;
}

void TMR_EnableTI( TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId)
{
    
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  (void)clrSrcId;      
  module->sc |= TPM3SC_TOIE_MASK;
}

void TMR_DisableTI(TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId) 
{  

  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID; 
  (void)clrSrcId;     
  module->sc &= ~TPM3SC_TOIE_MASK;
}


uint8_t TMR_ReadTOF(TMR_CNTAddressType tmrID) 
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  return ((module->sc & TPM3SC_TOF_MASK) != 0);
}

bool_t TMR_GetTOF(TMR_CNTAddressType tmrID, uint8_t channelId)
{
  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  (void) channelId;
  return  (bool_t) ((module->sc & TPM3SC_TOF_MASK) ? 1 : 0);
}

/*lint -emacro(522,TMR_ClearTOF) due to dummy read */
void TMR_ClearTOF(TMR_CNTAddressType tmrID, TMR_CHAddressType ch)
{

  tmrcntreg_struct_t *module = (tmrcntreg_struct_t *)tmrID;
  (void)ch;
  module->sc &   TPM3SC_TOF_MASK;
  module->sc &= ~TPM3SC_TOF_MASK;
}

/*
 * Function: TMR_GetDelta
 * Description:
 */
Period_ValueType TMR_GetDelta(TMR_CNTAddressType trmId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR)
{
  Period_ValueType period;
	if( CntrVal_CURR > CntrVal_OLD){
    return ((Period_ValueType) ((uint16_t) CntrVal_CURR - (uint16_t) CntrVal_OLD));
  }

  period = TMR_GetModulo(trmId, 0);
  return ((period - CntrVal_OLD)+CntrVal_CURR);
}




/*
 * Function: TMR_SetChannelInputChapture
 * Description: Set a channel in Input Capture Mode
 * Note: 
 * Status:
 */

void TMR_SetChannelInputChapture( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  pChannel->sc &= ~TPM3C0SC_MS0x_MASK;
}


/*
 * Function: TMR_SetThreshold1
 * Description: This function programs the match value
 * Note: Threshold of Counter
 * Status: 
 */
void TMR_SetThreshold1( TMR_CHAddressType chID, Period_ValueType value)
{ 
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  pChannel->v = value;
}



/*
 * Macro: TMR_SetChannelPulses
 * Description: Turns on the PWM mode. Enable the output of PWM on PIN.
 * Note: Enable PWM Mode, inverted not used
 * Status:  
 */
void TMR_SetChannelPulses(TMR_CHAddressType chID, uint8_t inverted) 
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;

  if (inverted){    
    pChannel->sc |= (uint8_t)TPM3C0SC_ELS0A_MASK; 
  }else{
    pChannel->sc = (uint8_t)((pChannel->sc & ~TPM3C0SC_ELS0A_MASK) | TPM3C0SC_ELS0B_MASK);   
  }  
}

/*
 * Function: TMR_SetChannelPWM
 * Description: Setting of PWM Channel
 * Note:
 * Status:
 */
void TMR_SetChannelPWM( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  pChannel->sc |= TPM3C0SC_MS0B_MASK;
}

/*
 * Function: TMR_SetTimeOn
 * Description: This function defines the match point for the PWM.
 *              When the counter reaches value	the outputs are switched high.
 * Note:
 * Status:
 */
void TMR_SetTimeOn( TMR_CHAddressType chID, Period_ValueType tOn)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  pChannel->v = tOn;
}
/*
 * Function: TMR_GetTimeOn
 * Description: This function get the match point for the PWM.
 * Note:
 * Status:
 */
void TMR_GetTimeOn( TMR_CHAddressType chID, Period_ValueType *tOn)
{
    *tOn = TMR_GetChValue(chID);
}

/*
 * Function: TMR_SetChannelEdgeMode
 * Description: This function configures the channel as InputCapture
 * Note:
 */
void TMR_SetChannelEdgeMode( TMR_CHAddressType chID, uint8_t mode, bool_t inversion)
{

  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  
	pChannel -> sc &= (~TPM3C0SC_ELS0x_MASK);

	switch(mode)
	{
		/* Rising Edge */
	case 1:
		if (inversion)
		{
			/* Switch to falling edge */
			pChannel -> sc |= (TPM3C0SC_ELS0B_MASK);
		}
		else
		{
			pChannel -> sc |= (TPM3C0SC_ELS0A_MASK);
		}
		break;
		/* Falling Edge */
	case 2:
		if (inversion)
		{
			/* Switch to rising edge */
			pChannel -> sc |= (TPM3C0SC_ELS0A_MASK);
		}
		else
		{
			pChannel -> sc |= (TPM3C0SC_ELS0B_MASK);
		}
		break;
	case 3:
		/* inversion is meaningless */
		pChannel -> sc |= (TPM3C0SC_ELS0x_MASK);
		break;
	default:
		break;
	}
}

void TMR_SetChannelOutputMode(TMR_CHAddressType chID, uint8_t mode, bool_t inversion)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
	switch(mode)
	{
	case 4:
		/* Software compare only */
		pChannel -> sc &= (uint8_t)~TPM3C0SC_ELS0x_MASK;
		break;
	case 5:
		/* Toggle output on compare */
		pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM3C0SC_ELS0B_MASK) | TPM3C0SC_ELS0A_MASK);
		break;
	case 7:
		/* Set output on compare */
		if (inversion)
		{
			pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM3C0SC_ELS0A_MASK) | TPM3C0SC_ELS0B_MASK);
		}
		else
		{
			pChannel -> sc |= (uint8_t)TPM3C0SC_ELS0x_MASK;
		}
		break;
	case 8:
		/* Clear output on compare */
		if (inversion)
		{
			pChannel -> sc |= (uint8_t)TPM3C0SC_ELS0x_MASK;
		}
		else
		{
			pChannel -> sc = (uint8_t)((pChannel -> sc & ~TPM3C0SC_ELS0A_MASK) | TPM3C0SC_ELS0B_MASK);
		}

		break;
	}
}

/*
 * Function: OutputCompareModeUnsupported
 * Description: This function checkes if Output Compare Mode is supported
 * Note:
 * Status:
 */
bool_t OutputCompareModeUnsupported(uint8_t mode, bool_t inversion)
{
	if ((6 == mode) || (5 == mode) && (1 == inversion))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Function: TMR_GetChValue
 * Description: This function return the cannel register value.
 */
Period_ValueType TMR_GetChValue( TMR_CHAddressType chID ) 
{     
  tmrchreg_struct_t *channel = (tmrchreg_struct_t *)chID;     
  return channel->v;
}

/*
 * Function: TMR_EnableCHI
 * Description: Enables the interrupt for the channel, output compare/input compare
 * Note: 
 * Status: 
 */
void TMR_EnableCHI( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *channel = (tmrchreg_struct_t *)chID;     
  channel->sc |= TPM3C0SC_CH0IE_MASK;
}

/*
 * Function: TMR_DisableCHI
 * Description: Disable the interrupt for the channel
 * Note:
 * Status:
 */
void TMR_DisableCHI( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *channel = (tmrchreg_struct_t *)chID;     
  channel -> sc &= ~TPM3C0SC_CH0IE_MASK;
}

/*
 * Function: TMR_SetChannelDefaultMode
 * Description: This function configures default modes for the channel
 * Note: 
 * Status:
 */
static void TMR_SetChannelDefaultMode( TMR_CHAddressType chID) 
{
  tmrchreg_struct_t *channel = (tmrchreg_struct_t *)chID;       
  channel -> sc &= ~TPM3C0SC_ELS0x_MASK;
}

/*
 * Function: TMR_SetChannelOutputCompare
 * Description: Set a channel in Output Compare Mode (Software compare only)
 * Notes:
 * Status:
 */
void TMR_SetChannelOutputCompare( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *channel = (tmrchreg_struct_t *)chID;
  channel->sc = (uint8_t)((channel->sc & ~TPM3C0SC_MS0B_MASK) | TPM3C0SC_MS0A_MASK) ;
}

/*
 * Function: TMR_ToggleChannelEdgeMode
 * Description: Toogle edged type to falling or rising
 * Note:
 * Status:
 */
void TMR_ToggleChannelEdgeMode( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  
	if( (pChannel->sc & TPM3C0SC_ELS0x_MASK) != TPM3C0SC_ELS0x_MASK)
	{
		pChannel->sc ^= (TPM3C0SC_ELS0x_MASK);
	}
}

/*
 * Function: TMR_GetChannelEdgeMode
 * Description:
 * Note:
 * Status: 
 */
uint8_t TMR_GetChannelEdgeMode( TMR_CHAddressType chID)
{
    tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
    uint8_t res=0;      /* unknown */
    
    switch( (pChannel -> sc & TPM3C0SC_ELS0x_MASK) )
    {
    case TPM3C0SC_ELS0A_MASK:
        res=1;
        break;
    case TPM3C0SC_ELS0B_MASK:
        res=2;
        break;
    case TPM3C0SC_ELS0x_MASK:
        res=3;
        break;
    default:;

	}
	return res;
}

void TMR_SetValue(TMR_CHAddressType chID, Period_ValueType value) 
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  pChannel->v = value;
}


void TMR_ClearCHF( TMR_CHAddressType chID)
{
  tmrchreg_struct_t *pChannel = (tmrchreg_struct_t *)chID;
  /*lint -emacro(522,TMR_ClearCHF) due to dummy read */
  pChannel->sc & TPM3C0SC_CH0F_MASK;
  pChannel->sc &= ~TPM3C0SC_CH0F_MASK;
}



/*
 * Function: TMR_DeInitChannel
 * Description: This function is used to bring the channel to its orginal state
 * Note:
 * Status: 
 */
void TMR_DeInitChannel( TMR_CHAddressType chID) 
{
    TMR_DisableCHI(chID);
    TMR_SetChannelDefaultMode(chID);
    TMR_SetChannelOutputCompare(chID);
    TMR_SetValue(chID,0);
    TMR_SetChannelInputChapture(chID);
    TMR_ClearCHF(chID);
}

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */
/*
 * Function: FLS_Erase
 * Description: This function is used to erase a flash block by its position
 * Note:
 * Status: 
 */
uint8_t FLS_Erase(FLS_positionType position)
{
	FLS_EraseCmdInRamType RamCmd;

  (void)memcpy(&(RamCmd.cmd), &FLS_ErasePageCmdIsInRam, SIZE_CMD_ERASE_IN_RAM);
  ((pFLS_ErasePageCmdIsInRam)&(RamCmd.cmd))(position);
  
  return FLASH_SUCCESS;
}



/*
 * Function: FLS_Programming
 * Description: This function is used to program a specific flash block
 * Note:
 * Status: 
 */

uint8_t FLS_Programming(uint8_t* pSource, uint32_t size, FLS_positionType address)
{
  unsigned char *srcPtr=(unsigned char *)pSource;
  FLS_BurstCmdInRamType RamCmd;
  int len=size;
  int dest=address;
  int headOffset;
  int headLen=0;
  int tailLen;
  int i;
  uint32_t writeBuff;
  
  headOffset = (dest & 0x03);
  (void)memcpy(&(RamCmd.cmd), &FLS_BurstProgCmdIsInRam, SIZE_BURST_CMD_IN_RAM);
  
	if(headOffset > 0)
	{
	  headLen = 4-headOffset;
	}
  /* head ------------------------------------------*/
  writeBuff    = 0xFFFFFFFF;
  
  if(headLen >0 || len < 4)
  {
    for( i=headOffset; i<4 && i < (headOffset+len); i++, srcPtr++ )
    {
      ((unsigned char*)&writeBuff)[i]=*srcPtr;
    }
    
    ((pFLS_BurstProgCmdIsInRam)&(RamCmd.cmd))(&writeBuff, 4, dest-headOffset);
  }
  
  /* body -----------------------------------------*/
  {
    int writePos= dest+headLen;
    int nByte=len;
    tailLen = (len + headOffset) & 0x03;
    
    if(headLen>0)
    {
      nByte -= headLen;
    }
    
    nByte -= tailLen;
    
    if(nByte>0)
    {
      ((pFLS_BurstProgCmdIsInRam)&(RamCmd.cmd))((uint32_t*)srcPtr, nByte, writePos );
      srcPtr += nByte;
    }
  }
  
  /* tail ------------------------------------------*/
  writeBuff    = 0xFFFFFFFF;
  {
    int beginTail = dest+(len-tailLen);
    
    if( tailLen > 0 && dest < beginTail)
    {
      for(i=0; i<tailLen; i++)
      {
        ((unsigned char*)&writeBuff)[i] = *srcPtr++;
      }
      
      ((pFLS_BurstProgCmdIsInRam)&(RamCmd.cmd))((&writeBuff), 4, beginTail);
    }
  }
  
  return FLASH_SUCCESS;
}

/* ********************************************************************************************** */
/*                                  END FLS Class HAL Support                                     */
/* ********************************************************************************************** */

const boot_short_info_t BootShortInfo;

#pragma define_section rst_info ".systemResetStatus" far_absolute
#if _lint
#define RESET_INFO_BLOCK
#else   /* _lint */
#define RESET_INFO_BLOCK __declspec(rst_info)
#endif  /* _lint */

RESET_INFO_BLOCK struct _ResetInfo ResetInfo;
