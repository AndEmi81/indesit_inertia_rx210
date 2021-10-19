#include "Config.h"
#include "OCTPM_drv.h"
#include "OCWDT_drv.h"
#if !defined(RENESAS_300H_39079)
#include "OCCLK_drv.h"	/* Clock HAL not done yet for this target */
#endif

extern IO_ICU_CfgType IcuTmr0Ch0_cfg;
extern IO_ICU_CfgType IcuTmr0Ch1_cfg;
extern IO_OCU_CfgType OcuTmr0Ch2_cfg;
extern IO_OCU_CfgType OcuTmr1Ch0_cfg;
extern IO_PWD_CfgType PwdTmr1Ch4_cfg;
extern IO_PWM_CfgType PwmTmr1Ch3_cfg;

extern IO_HwCntrCfgType HwCntr0_cfg;
extern IO_HwCntrCfgType HwCntr1_cfg;
bool_t InitTimers;
IO_ErrorType Err;
IO_TimingValueType Time_nSec;
uint16_t NumISR_Tmr0;
IO_HwTimerType CaptureInTicks;
IO_TimingValueType CaptureInNsec;
uint16_t NumCaptureEvent;
uint16_t NumOutCompareEvent;
IO_TimingValueType Tmr0Ch2OCUThreshold = (IO_TimingValueType)TMR_ID0_TICK_RES*20000;
IO_HwTimerType Tmr1Ch0OCUThreshold = (IO_HwTimerType)TMR_ID1_PERIOD_VALUE/2;
IO_PWD_MeasureType PwdMeasureOnTmr1Ch4;
IO_PWD_RawMeasureType  PwdMeasureOnTmr1Ch4Inticks;
bool_t ActivateIsrLatency = FALSE;
bool_t Tmr1Ch4SuspendMeasure = FALSE;
bool_t Tmr1Ch4ActivateMeasure = FALSE;
bool_t Tmr1Ch4Deinit = FALSE;
bool_t Tmr1Ch3PWM_Enable = FALSE;
bool_t Tmr1Ch3PWM_Deinit = FALSE;
bool_t Tmr1Ch3PWM_NewPeriod = FALSE;
bool_t Tmr1Ch3PWM_EndPeriodISR = FALSE;


/* Prototypes */
static IO_ErrorType callbackTmr0ModuloReached (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
static IO_ErrorType callbackTmrChInputCapture (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
static IO_ErrorType callbackTmrChOutputCapture (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
static IO_ErrorType callbackPwdEvent (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
static void GeneratePWM_OnTmr1Ch3(void);
static IO_ErrorType callbackPwmEndPeriod (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);

void main(void)
{


    while(NumISR_Tmr0>=0)
    {
        if (FALSE == InitTimers)
        {
        	#if !defined(RENESAS_300H_39079)     
            (void)OCWDT_InitSync();
            (void)OCWDT_RefreshSync();
        	#endif
        	#if !defined(RENESAS_300H_39079) 
            (void)OCCLK_SetFLLInternalClockSync(20000000);
        	#endif
        	#if defined(RENESAS_300H_39079)
        	set_imask_ccr(0);
        	#elif defined(FREESCALE_S08_GB60)	
            EnableInterrupts
        	#else
        	#endif

            /* Init Hw Counter id 0 */
            Err = OCHWCNTR_InitSync(&HwCntr0_cfg);
            /* Start Counting */
            Err = OCHWCNTR_Enable(HW_TMR_ID_0, TMR_CNT_ENABLE);               
            /* Overflow/Underflow Interrupt enable */
            Err = OCHWCNTR_EnableISR(HW_TMR_ID_0, TMR_ISR_ENABLE, &callbackTmr0ModuloReached);
            
            /* Init Hw Counter id 1 */
            Err = OCHWCNTR_InitSync(&HwCntr1_cfg);
            /* Start Counting */
            Err = OCHWCNTR_Enable(HwCntr1_cfg.hwCntrlId, TMR_CNT_ENABLE);               


            /* Input Capture Init. for Tmr0 Ch1 */
            Err = OCICU_InitSync(&IcuTmr0Ch1_cfg);
            /* Select Input Capture Edge for Tmr0 Ch1 */
            Err = ICU_SetModeSync(TMR_CH_ID_1, TMR_CH_IC_POS_EDGE);
            /* Enable Interrupt On Edge */
            Err = OCICU_EnableISR(TMR_CH_ID_1, TMR_CH_ISR_ENABLE, &callbackTmrChInputCapture);
            
            /* Output Compare Init. for Tmr0 Ch2 */
            Err = OCOCU_InitSync(&OcuTmr0Ch2_cfg);
            /* Set Threshold 1 for Tmr0 Ch2 */
            OCOCU_SetThreshold1Sync(OcuTmr0Ch2_cfg.OCU_Id, Tmr0Ch2OCUThreshold);
            /* Toggle Pin On Compare */
            Err = OCU_SetModeSync(OcuTmr0Ch2_cfg.OCU_Id, TMR_CH_OC_TOGGLE_T1);
            /* Enable Interrupt on Compare */
            Err = OCOCU_EnableISR(OcuTmr0Ch2_cfg.OCU_Id, TMR_CH_ISR_ENABLE, &callbackTmrChOutputCapture);


            /* Output Compare Init. for Tmr1 Ch0 */
            Err = OCOCU_InitSync(&OcuTmr1Ch0_cfg);
            /* Set Threshold 1 for Tmr1 Ch0 in ticks */
            OCOCU_SetThreshold1TickResolutionSync(OcuTmr1Ch0_cfg.OCU_Id, Tmr1Ch0OCUThreshold );
            /* Pin On Compare: High*/
            Err = OCU_SetModeSync(OcuTmr1Ch0_cfg.OCU_Id, TMR_CH_OC_PIN_HIGH);
            /* Enable Interrupt on Compare */
            Err = OCOCU_EnableISR(OcuTmr1Ch0_cfg.OCU_Id, TMR_CH_ISR_ENABLE, &callbackTmrChOutputCapture);

            /* PWD Init. Tmr1 Ch4 */
            Err = OCPWD_InitSync(&PwdTmr1Ch4_cfg);
            Err = OCPWD_StartSampleSync(PwdTmr1Ch4_cfg.PWD_Id, &callbackPwdEvent);

            /* PWM Init. Tmr1 Ch3 */
            Err = OCPWM_InitSync(&PwmTmr1Ch3_cfg);

            InitTimers = TRUE;        
        }
        (void)OCWDT_RefreshSync();
        
        /* Get The Timer Value */
        Time_nSec = OCHWCNTR_GetSync(HW_TMR_ID_0);
        
        if (Tmr1Ch4SuspendMeasure)
        {
            OCPWD_StopSampleSync(PwdTmr1Ch4_cfg.PWD_Id);
            Tmr1Ch4SuspendMeasure = FALSE;
        }
        else if (Tmr1Ch4ActivateMeasure)
        {
            Err = OCPWD_StartSampleSync(PwdTmr1Ch4_cfg.PWD_Id, &callbackPwdEvent);  
            Tmr1Ch4ActivateMeasure = FALSE;          
        }
        else if (Tmr1Ch4Deinit)
        {
            Err = OCPWD_DeInitSync(PwdTmr1Ch4_cfg.PWD_Id);
            Tmr1Ch4Deinit = FALSE;
        }
        if (Tmr1Ch3PWM_Enable)
        {
            GeneratePWM_OnTmr1Ch3();                
        }
        if (Tmr1Ch3PWM_Deinit)
        {
            Err = OCPWM_DeInitSync(PwmTmr1Ch3_cfg.PWM_Id);

            Tmr1Ch3PWM_Deinit = FALSE;    
        }
        if (Tmr1Ch3PWM_NewPeriod)
        {
            OCPWM_SetPeriodSync(PwmTmr1Ch3_cfg.PWM_Id, 50000000);
            Tmr1Ch3PWM_NewPeriod = FALSE;
        }
        if (Tmr1Ch3PWM_EndPeriodISR)
        {
            Err = OCPWM_EnableT1ISR(PwmTmr1Ch3_cfg.PWM_Id, TRUE, callbackPwmEndPeriod);
            Tmr1Ch3PWM_EndPeriodISR = FALSE;
        }
    }



    /* Disable Interrupt in Input Capture on Tmr0 Ch1 */
    Err = OCICU_EnableISR(TMR_CH_ID_1, TMR_CH_ISR_DISABLE, NULL);
    
    /* DeInit Tmr0 Ch1 Config */
    Err = OCICU_DeInitSync(TMR_CH_ID_1);
                    
    /* Stop Tmr0 counting */
    Err = OCHWCNTR_Enable(HW_TMR_ID_0, TMR_CNT_DISABLE);
    
    /* Overflow/Underflow Interrupt disable */
    Err = OCHWCNTR_EnableISR(HW_TMR_ID_0, TMR_ISR_DISABLE, NULL);

    /* Deinit Hw Counter id 0 */
    Err = OCHWCNTR_DeInitSync(HW_TMR_ID_0);
    
    Err = OCOCU_DeInitSync(OcuTmr1Ch0_cfg.OCU_Id);

}

static IO_ErrorType callbackTmr0ModuloReached (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
uint16_t delay;
    switch(chid)
    {
        case HW_TMR_ID_0:
            #if defined(FREESCALE_S08_GB60)
            PTFDD_PTFDD5 = 1;
            PTFD_PTFD5 ^=1;
            #endif
            NumISR_Tmr0++;
            if (ActivateIsrLatency)
            {
                for(delay=0;delay<0xFF;delay++)
                {
                        (void)OCWDT_RefreshSync();
        
                }                
            }
            break;
        case HW_TMR_ID_1:
            break;
        default:
            break;
    }
    return 0;
}
static IO_ErrorType callbackTmrChInputCapture (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{

    switch(chid)
    {
        case TMR_CH_ID_1:
            
            CaptureInTicks = ICU_GetSyncTickResolution(TMR_CH_ID_1);
            CaptureInNsec = ICU_GetSync(TMR_CH_ID_1);
            NumCaptureEvent++;
            break;
    }
    return 0;
}
static IO_ErrorType callbackTmrChOutputCapture (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
static bool_t Tmr1Ch0Isr=FALSE;
    if(chid == OcuTmr0Ch2_cfg.OCU_Id)
    {
        /* Change Threshold */
        Tmr0Ch2OCUThreshold += (IO_TimingValueType)TMR_ID0_TICK_RES*50;
        OCOCU_SetThreshold1Sync(OcuTmr0Ch2_cfg.OCU_Id, Tmr0Ch2OCUThreshold);

        NumOutCompareEvent++;
    }
    else if(chid == OcuTmr1Ch0_cfg.OCU_Id)
    {
        if (FALSE == Tmr1Ch0Isr)
        {
            /* Change Threshold (ticks) */
            Tmr1Ch0OCUThreshold+=5000;
            /* Pin On Compare: Low */
            Err = OCU_SetModeSync(OcuTmr1Ch0_cfg.OCU_Id, TMR_CH_OC_PIN_LOW);
            /* Update Th */ 
            OCOCU_SetThreshold1TickResolutionSync(OcuTmr1Ch0_cfg.OCU_Id, Tmr1Ch0OCUThreshold );
            Tmr1Ch0Isr = TRUE;
            
        }
        else
        {
            /* Change Threshold (ticks) */
            Tmr1Ch0OCUThreshold-=5000;
            /* Pin On Compare: Low */
            Err = OCU_SetModeSync(OcuTmr1Ch0_cfg.OCU_Id, TMR_CH_OC_PIN_HIGH);
            /* Update Th */ 
            OCOCU_SetThreshold1TickResolutionSync(OcuTmr1Ch0_cfg.OCU_Id, Tmr1Ch0OCUThreshold );
            Tmr1Ch0Isr = FALSE;

        }
        
    }
    return 0;
}
static IO_ErrorType callbackPwdEvent (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
    if (PwdTmr1Ch4_cfg.PWD_Id == chid)
    {
        OCPWD_GetAllSync(chid, &PwdMeasureOnTmr1Ch4);
        OCPWD_GetAllSyncInTicks(chid, &PwdMeasureOnTmr1Ch4Inticks);
    }
    return 0;    
}
#define UPDATE_PWM    10
#define DELTA_INC_PWM   100000
static void GeneratePWM_OnTmr1Ch3(void)
{
IO_TimingValueType TonPWM;
IO_TimingValueType PeriodPWM; 
static uint16_t PWM_cntr = 0;
static bool_t UpDown = TRUE;
    
    PeriodPWM = OCPWM_GetPeriodSync(PwmTmr1Ch3_cfg.PWM_Id);
    TonPWM = OCPWM_GetSync(PwmTmr1Ch3_cfg.PWM_Id);

    if (PWM_cntr>UPDATE_PWM)
    {
        PWM_cntr = 0;

        if (UpDown)
        {
            if (TonPWM <= PeriodPWM)
            {
                /* Set PWM tOn value */
                TonPWM+=DELTA_INC_PWM;
                OCPWM_SetSync(PwmTmr1Ch3_cfg.PWM_Id, TonPWM);                        
            }
            else
            {
                UpDown = FALSE;    
            }                    
        }
        else
        {
            if (TonPWM >= DELTA_INC_PWM )
            {
                /* Set PWM tOn value */
                TonPWM-=DELTA_INC_PWM;
                OCPWM_SetSync(PwmTmr1Ch3_cfg.PWM_Id, TonPWM);                        
            }
            else
            {
                UpDown = TRUE;    
            }                                
        }
    }
    PWM_cntr++;
}
static IO_ErrorType callbackPwmEndPeriod (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
IO_TimingValueType TonPWM;
IO_TimingValueType PeriodPWM; 
static bool_t UpDown = TRUE;


    PeriodPWM = OCPWM_GetPeriodSync(PwmTmr1Ch3_cfg.PWM_Id);
    TonPWM = OCPWM_GetSync(PwmTmr1Ch3_cfg.PWM_Id);

    if (UpDown)
    {
        if (TonPWM <= PeriodPWM)
        {
            /* Set PWM tOn value */
            TonPWM+=DELTA_INC_PWM;
            OCPWM_SetSync(PwmTmr1Ch3_cfg.PWM_Id, TonPWM);                        
        }
        else
        {
            UpDown = FALSE;    
        }                    
    }
    else
    {
        if (TonPWM >= DELTA_INC_PWM )
        {
            /* Set PWM tOn value */
            TonPWM-=DELTA_INC_PWM;
            OCPWM_SetSync(PwmTmr1Ch3_cfg.PWM_Id, TonPWM);                        
        }
        else
        {
            UpDown = TRUE;    
        }                                
    }

    return IO_E_OK;
}
