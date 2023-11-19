//################################################
// Flash������
//                             			K.Richardo
//################################################
#ifndef MIDDLEWARES_INC_FLASH_H_
#define MIDDLEWARES_INC_FLASH_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

extern const uint32_t course_start_address;
//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void eraseFlash( void );
void writeFlash(uint32_t address, uint8_t *data, uint32_t size  );
void loadFlash(uint32_t address, uint8_t *data, uint32_t size );

#endif /* MIDDLEWARES_INC_FLASH_H_ */
