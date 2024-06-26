/******************************************************************************/
/* File   : SwcApplEcuM.c                                                     */
/* Author : Nagaraja HULIYAPURADA-MATA                                        */
/* Date   : 01.02.1982                                                        */
/******************************************************************************/

/******************************************************************************/
/* #INCLUDES                                                                  */
/******************************************************************************/
#include "Std_Types.hpp"

#include "Types_SwcServiceEcuM.hpp"
#include "CfgSwcServiceEcuM.hpp"

#include "EcuM_Prv.hpp"
#include "infMcalCanSwcApplEcuM.hpp"
#include "infMcalMcuSwcApplEcuM.hpp"
#include "infSwcServiceDcmSwcApplEcuM.hpp"
#include "infSwcServiceDetSwcApplEcuM.hpp"

#ifdef BUILD_RAJA
#include "infSwcServiceFiMSwcApplEcuM.hpp"
#endif

#include "infSwcServiceOsSwcApplEcuM.hpp"

/******************************************************************************/
/* #DEFINES                                                                   */
/******************************************************************************/

/******************************************************************************/
/* MACROS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* TYPEDEFS                                                                   */
/******************************************************************************/

/******************************************************************************/
/* CONSTS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* PARAMS                                                                     */
/******************************************************************************/

/******************************************************************************/
/* OBJECTS                                                                    */
/******************************************************************************/

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/
extern void EcuM_ErrorHook(uint16 reason);

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vSetInterruptsProgrammable(void){
   Os_InitializeVectorTable();
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vInitDriverZero(void){
#if(ECUM_INCLUDE_DET == STD_ON)
   (void)infSwcServiceDetSwcApplEcuM_vInitFunction(0);
#endif
   (void)Dem_PreInit(0);
}

FUNC(const Type_CfgSwcServiceEcuM_st*, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_stDetermineConfigurationPb(void){
   return(CfgSwcServiceEcuM_capst[0]);
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vCheckConsistency(void){
   P2CONST(uint8, AUTOMATIC, ECUM_APPL_CONST) adrTempPBHashPtr_u8;
   VAR(uint8,     AUTOMATIC)                  cntrLoopCtr_u8;
   VAR(boolean,   AUTOMATIC)                  flgupdateValid = FALSE;
   if(
         NULL_PTR
      == SwcServiceEcuM_pcstCfg
   ){
      EcuM_ErrorHook(ECUM_E_NULL_PTR_RETURNED);
   }
   for(
      cntrLoopCtr_u8 = 0;
      cntrLoopCtr_u8 < CfgSwcServiceEcuM_dNumConfigSet;
      cntrLoopCtr_u8 ++
   ){
      if(
            SwcServiceEcuM_pcstCfg
         == CfgSwcServiceEcuM_capst[cntrLoopCtr_u8]
      ){
         flgupdateValid = TRUE;
         break;
      }
   }
   if(
         TRUE
      != flgupdateValid
   ){
      EcuM_ErrorHook(ECUM_E_INVALID_PTR_RETURNED);
   }
   adrTempPBHashPtr_u8 = &(SwcServiceEcuM_pcstCfg->au8HashPostBuild[0]);
   for(
      cntrLoopCtr_u8 = 0;
      cntrLoopCtr_u8 < CfgSwcServiceEcuM_dLenHash;
      cntrLoopCtr_u8 ++
   ){
      if(
            CfgSwcServiceEcuM_au8HashPreCompile[cntrLoopCtr_u8]
         != adrTempPBHashPtr_u8[cntrLoopCtr_u8]
      ){
         EcuM_ErrorHook(ECUM_E_CONFIGURATION_DATA_INCONSISTENT);
      }
   }
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vInitDriverOne(
   P2CONST(Type_CfgSwcServiceEcuM_st, AUTOMATIC, ECUM_APPL_CONST) ConfigPtr
){
   (void)infMcalCanSwcApplEcuM_vInitFunction();
   (void)CanIf_Init(ConfigPtr->stRefCfgModules.pcstCfgEcuabCanIf);
   (void)ComM_Init(ConfigPtr->stRefCfgModules.pcstCfgSwcServiceComM);
   (void)CanSM_Init(ConfigPtr->stRefCfgModules.pcstCfgSwcServiceSmCan);
   (void)PduR_Init(ConfigPtr->stRefCfgModules.pcstCfgSwcServicePduR);
   (void)Com_Init(ConfigPtr->stRefCfgModules.pcstCfgSwcServiceCom);
   (void)infSwcServiceDcmSwcApplEcuM_vInitFunction(0);
   (void)CanTp_Init(ConfigPtr->stRefCfgModules.pcstCfgSwcServiceCanTp);
   (void)Dem_Init();
#ifdef BUILD_RAJA
   (void)infSwcServiceFiMSwcApplEcuM_vInitFunction(ConfigPtr->stRefCfgModules.pcstCfgSwcServiceFiM);
#endif
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vSetSourceWakeup(void){
   VAR(Type_McalMcu_eReasonReset, AUTOMATIC) leReasonReset = McalMcu_eGetReasonReset();
   VAR(uint8, AUTOMATIC) cntrLoopCtr_u8;

   if(
         McalMcu_eReasonReset_Undefined
      != leReasonReset
   ){
      for(
         cntrLoopCtr_u8 = 0;
         cntrLoopCtr_u8 < CfgSwcServiceEcuM_dNumSourceWakeup;
         cntrLoopCtr_u8 ++
      ){
         if(
               leReasonReset
            == EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].ResetReason
         ){
            EcuM_SetWakeupEvent(
               EcuM_Cfg_idxWakeupSourcesPC_au32[cntrLoopCtr_u8].WakeupSourceId
            );
         }
      }
   }
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vSwitchOsModeApp(void){
}

FUNC(void, SWCAPPLECUM_CODE) infSwcApplEcuMSwcServiceEcuM_vStartOS(void){
   StartOS(SwcServiceEcuM_pcstCfg->tModeAppDefault)
}

/******************************************************************************/
/* EOF                                                                        */
/******************************************************************************/
