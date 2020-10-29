/*
 * BL.h
 *
 *  Created on: Oct 29, 2020
 *      Author: Diaa Eldeen
 */

#ifndef INCLUDE_BL_H_
#define INCLUDE_BL_H_


#include "handy.h"

#include "flash.h"


typedef enum
{
	Success = 0,
	Error,
    SectorEraseError,
    FlashProgError,
}
tBLError;


typedef enum
{
	BLRejoin,
    App0Rejoin,
    App1Rejoin,
}
tBLFlashRejoin;



int32_t BLDownloadImage(uint32_t pui32Buffer[], uint32_t ui32Addr, uint32_t ui32Size);
int32_t BLJump(uint32_t ui32Addr);


#endif /* INCLUDE_BL_H_ */
