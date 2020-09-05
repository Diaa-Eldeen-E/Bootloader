/*
 * flash.h
 *
 *  Created on: Sep 3, 2020
 *      Author: Diaa Eldeen
 */

#ifndef INCLUDE_FLASH_H_
#define INCLUDE_FLASH_H_


#include "handy.h"


#define FLASH_BASE_	0x2000
#define FLASH_TOP_	0xFFFFC

#define NO_OF_BLOCKS	512



typedef enum
{
    FlashReadWrite,                         // Flash can be read and written
    FlashReadOnly,                          // Flash can only be read
    FlashExecuteOnly                        // Flash can only be executed
}
tFlashProtection;


//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
void FlashClearErrors();
int32_t FlashErase(uint32_t ui32Address);
int32_t FlashProgram(uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count);
int32_t FlashProgramBuffering(uint32_t *pui32Data, uint32_t ui32Address, uint32_t ui32Count);


tFlashProtection FlashProtectGet(uint32_t ui32Address);
int32_t FlashProtectSet(uint32_t ui32Address, tFlashProtection eProtect);



#endif /* INCLUDE_FLASH_H_ */
