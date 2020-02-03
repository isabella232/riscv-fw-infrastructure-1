/* 
* SPDX-License-Identifier: Apache-2.0
* Copyright 2019 Western Digital Corporation or its affiliates.
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
* http:*www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @file   psp_interrupts_swerv_eh1.c
* @author Ronen Haen
* @date   20.05.2019
* @brief  The file supllies interrups services such as  - registration, default handlers and indication
*         whether we are in interrpu context.
*         The file is specific to swerv_eh1 specifications
* 
*/

/**
* include files
*/
#include "psp_types.h"
#include "psp_api.h"

/**
* definitions
*/

/* The stack used by interrupt service routines */
#ifdef D_ISR_STACK_SIZE
	static /*D_PSP_DATA_SECTION*/ D_PSP_16_ALIGNED pspStack_t xISRStack[ D_ISR_STACK_SIZE ] ;
	const pspStack_t xISRStackTop = ( pspStack_t ) &( xISRStack[ ( D_ISR_STACK_SIZE ) - 1 ] );
#else
    #error "ISR Stack size is not defined"
#endif

/**
* macros
*/

/**
* types
*/

/**
* local prototypes
*/
void pspDefaultExceptionIntHandler_isr(void);
void pspDefaultEmptyIntHandler_isr(void);

/**
* external prototypes
*/

/**
* global variables
*/

/* Exception handlers */
D_PSP_DATA_SECTION pspInterruptHandler_t  gExceptions_ints[D_PSP_NUM_OF_INTS_EXCEPTIONS] = {
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr,
                       pspDefaultEmptyIntHandler_isr };

/* Exceptions handler pointer */
//D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntExceptionIntHandler   = pspDefaultExceptionIntHandler_isr;
// [NatiR] Temporarily not in D_PSP_DATA_SECTION. need to further investigate jump to g_fptrIntExceptionIntHandler from ecall
pspInterruptHandler_t g_fptrIntExceptionIntHandler   = pspDefaultExceptionIntHandler_isr;

/* Interrupts handler pointers */
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntSSoftIntHandler       = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntRsrvdSoftIntHandler   = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntMSoftIntHandler       = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntUTimerIntHandler      = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntSTimerIntHandler      = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntRsrvdTimerIntHandler  = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntMTimerIntHandler      = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntUExternIntHandler     = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntSExternIntHandler     = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntRsrvdExternIntHandler = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntMExternIntHandler     = pspDefaultEmptyIntHandler_isr;
D_PSP_DATA_SECTION pspInterruptHandler_t g_fptrIntUSoftIntHandler       = pspDefaultEmptyIntHandler_isr;

/**
* The function installs an interrupt service routine per risc-v cause
*
* @param fptrInterruptHandler     – function pointer to the interrupt service routine
* @param uiInterruptCause           – interrupt source
*
* @return u32_t                   - previously registered ISR
*/
D_PSP_TEXT_SECTION pspInterruptHandler_t pspRegisterInterruptHandler(pspInterruptHandler_t fptrInterruptHandler, u32_t uiInterruptCause)
{
   pspInterruptHandler_t pFptr;

   M_PSP_ASSERT(fptrInterruptHandler == NULL && uiInterruptCause >= E_LAST_COMMON_CAUSE);

   switch (uiInterruptCause)
   {
      case E_USER_SOFTWARE_CAUSE:
    	  pFptr = g_fptrIntUSoftIntHandler;
    	  g_fptrIntUSoftIntHandler = fptrInterruptHandler;
    	  break;
      case E_SUPERVISOR_SOFTWARE_CAUSE:
    	  pFptr = g_fptrIntSSoftIntHandler;
    	  g_fptrIntSSoftIntHandler = fptrInterruptHandler;
          break;
      case E_RESERVED_SOFTWARE_CAUSE:
    	  pFptr = g_fptrIntRsrvdSoftIntHandler;
    	  g_fptrIntRsrvdSoftIntHandler = fptrInterruptHandler;
    	  break;
      case E_MACHINE_SOFTWARE_CAUSE:
    	  pFptr = g_fptrIntMSoftIntHandler;
    	  g_fptrIntMSoftIntHandler = fptrInterruptHandler;
          break;
      case E_USER_TIMER_CAUSE:
    	  pFptr = g_fptrIntUTimerIntHandler;
    	  g_fptrIntUTimerIntHandler = fptrInterruptHandler;
    	  break;
      case E_SUPERVISOR_TIMER_CAUSE:
    	  pFptr = g_fptrIntSTimerIntHandler;
    	  g_fptrIntSTimerIntHandler = fptrInterruptHandler;
    	  break;
      case E_RESERVED_TIMER_CAUSE:
    	  pFptr = g_fptrIntRsrvdTimerIntHandler;
    	  g_fptrIntRsrvdTimerIntHandler = fptrInterruptHandler;
    	  break;
      case E_MACHINE_TIMER_CAUSE:
    	  pFptr = g_fptrIntMTimerIntHandler;
    	  g_fptrIntMTimerIntHandler = fptrInterruptHandler;
    	  break;
      case E_USER_EXTERNAL_CAUSE:
    	  pFptr = g_fptrIntUExternIntHandler;
    	  g_fptrIntUExternIntHandler = fptrInterruptHandler;
          break;
      case E_SUPERVISOR_EXTERNAL_CAUSE:
    	  pFptr = g_fptrIntSExternIntHandler;
    	  g_fptrIntSExternIntHandler = fptrInterruptHandler;
          break;
      case E_RESERVED_EXTERNAL_CAUSE:
    	  pFptr = g_fptrIntRsrvdExternIntHandler;
    	  g_fptrIntRsrvdExternIntHandler = fptrInterruptHandler;
    	  break;
      case E_MACHINE_EXTERNAL_CAUSE:
    	  pFptr = g_fptrIntMExternIntHandler;
    	  g_fptrIntMExternIntHandler = fptrInterruptHandler;
    	  break;
      default:
    	  pFptr = NULL;
    	  break;
   }

   return pFptr;
}

/**
* The function installs an exception handler per exception cause
*
* @param fptrInterruptHandler     – function pointer to the exception handler
* @param uiExceptionCause           – exception cause
*
* @return u32_t                   - previously registered ISR
*/
D_PSP_TEXT_SECTION pspInterruptHandler_t pspRegisterExceptionHandler(pspInterruptHandler_t fptrInterruptHandler, u32_t uiExceptionCause)
{
   pspInterruptHandler_t pFptr;

   M_PSP_ASSERT(fptrInterruptHandler == NULL && uiExceptionCause >= E_EXC_LAST_COMMON);

   pFptr = gExceptions_ints[uiExceptionCause];

   gExceptions_ints[uiExceptionCause] = fptrInterruptHandler;

   return pFptr;
}

/**
* default exception interrupt handler
*
* @param none
*
* @return none
*/
D_PSP_TEXT_SECTION void pspDefaultExceptionIntHandler_isr(void)
{
   /* get the exception cause */
   u32_t cause = M_PSP_READ_CSR(mcause);

   /* is it a valid cause */
   M_PSP_ASSERT(cause < D_PSP_NUM_OF_INTS_EXCEPTIONS);

   /* call the specific exception handler */
   gExceptions_ints[cause]();
}


/**
* default empty interrupt handler
*
* @param none
*
* @return none
*/
D_PSP_TEXT_SECTION void pspDefaultEmptyIntHandler_isr(void)
{
}

