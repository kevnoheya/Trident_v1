//################################################
// FlashÉÅÉÇÉä
//                             			K.Richardo
//################################################
#include "flash.h"
//************************************************
// ïœêîêÈåæ
//************************************************
const uint32_t course_start_address = 0x80E0000; //sector11 start address
//const uint32_t end_adress = 0x80FFFFF; // sector11 end address

//===============================================
// Flash : Erase
//===============================================
void eraseFlash( void )
{
	FLASH_EraseInitTypeDef erase;
	erase.TypeErase = FLASH_TYPEERASE_SECTORS;	// select sector
	erase.Sector = FLASH_SECTOR_11;		       // set selector11
	erase.NbSectors = 1;		// set to erase one sector
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;	// set voltage range (2.7 to 3.6V)

	uint32_t pageError = 0;

	HAL_FLASHEx_Erase(&erase, &pageError);	// erase sector
}

//===============================================
// Flash : Write
//===============================================
void writeFlash(uint32_t address, uint8_t *data, uint32_t size  )
{
	HAL_FLASH_Unlock();		// unlock flash
	eraseFlash();			// erase sector11

  for ( uint32_t add = address; add < (address + size); add++ ){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, add, *data); // write byte
    data++;  // add data pointer
	}

	HAL_FLASH_Lock();		// lock flash
}

//===============================================
// Flash : Load
//===============================================
void loadFlash(uint32_t address, uint8_t *data, uint32_t size )
{
	memcpy(data, (uint8_t*)address, size); // copy data
}
