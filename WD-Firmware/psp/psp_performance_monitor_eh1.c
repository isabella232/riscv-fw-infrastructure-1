/* 
* Copyright (c) 2010-2016 Western Digital, Inc.
*
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
* @file    psp_performance_monitor.c
* @author  Nidal Faour
* @date    15.03.2020
* @brief   performance monitor service provider
* 
*/

/**
* include files
*/
#include "psp_api.h"

/**
* definitions
*/


/**
* macros
*/


/**
* types
*/

/**
* local prototypes
*/

/**
* external prototypes
*/

/**
* global variables
*/


/**
* @brief The function enable/disable the group performance monitor
*
* @param uiMonitorEnBit           – monitor enable/disable bit
*
* @return No return value
*/
void pspEnableAllPerformanceMonitor(u32_t uiMonitorEnBit)
{
	M_PSP_SET_CSR(D_PSP_MGPMC_NUM, uiMonitorEnBit & D_PSP_MGMPC_MASK);
}


/**
* @brief The function enables/disable the performance counters
*
* @param uiCountersEn     – counters to enable disable
*
* @return No return value
*/
void pspEnablePerformanceCounters(u32_t uiCountersEn)
{
	M_PSP_SET_CSR(D_PSP_MCOUNTEREN_NUM, uiCountersEn & D_PSP_MCOUNTEREN_MASK);
}

/**
* @brief The function pair a counter to an event
*
* @param uiCounter     – counter to be set
* @param eEvent       – event to be paired to the selected counter
*
* @return No return value
*/
void pspPerformanceCounterSet(u32_t uiCounter, ePspPerformanceMonitorEvents_t eEvent)
{
	switch (uiCounter)
	{
		case D_PSP_COUNTER0:
				M_PSP_WRITE_CSR(D_PSP_MHPMEVENT3_NUM, eEvent);
				break;
		case D_PSP_COUNTER0:
				M_PSP_WRITE_CSR(D_PSP_MHPMEVENT4_NUM, eEvent);
				break;
		case D_PSP_COUNTER0:
				M_PSP_WRITE_CSR(D_PSP_MHPMEVENT5_NUM, eEvent);
				break;
		case D_PSP_COUNTER0:
				M_PSP_WRITE_CSR(D_PSP_MHPMEVENT6_NUM, eEvent);
				break;
		default:
		  M_PSP_ASSERT(1);
			break;
	}
}
/**
* @brief The function gets the counter value
*
* @param uiCounter    – counter index
*
* @return u64_t      – Counter value
*/
u64_t pspPerformanceCounterGet(u32_t uiCounter)
{
	u64_t udCounterVal = 0;
	switch (uiCounter)
	{
		case D_PSP_CYCLE_COUNTER:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MCYCLE_NUM);
			break;
		case D_PSP_TIME_COUNTER:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_TIME_NUM);
			break;
		case D_PSP_INSTRET_COUNTER:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MINSTRET_NUM);
			break;
		case D_PSP_COUNTER0:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MHPMCOUNTER3_NUM);
			break;
		case D_PSP_COUNTER1:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MHPMCOUNTER4_NUM);
			break;
		case D_PSP_COUNTER2:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MHPMCOUNTER5_NUM);
			break;
		case D_PSP_COUNTER3:
		  udCounterVal = M_PSP_READ_CSR(D_PSP_MHPMCOUNTER6_NUM);
			break;
		default:
		  M_PSP_ASSERT(1);
		  break;
	}
	return udCounterVal;
}

