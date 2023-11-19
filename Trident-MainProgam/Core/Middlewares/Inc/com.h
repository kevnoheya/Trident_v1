//################################################
//  �V���A���ʐM
//                         				K.Richardo
// Baudrate : 115200bps
// printf, scanf
//################################################
#ifndef MIDDLEWARES_COM_H_
#define MIDDLEWARES_COM_H_

//************************************************
// Include�錾
//************************************************
#include "main.h"

//************************************************
// Definition�錾
//************************************************
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int f getc(FILE* f)
#endif


//************************************************
// �֐��v���g�^�C�v�錾
//************************************************
void Serial_Init( void );
void __io_putchar(uint8_t ch);
int __io_getchar(void);

#endif /* MIDDLEWARES_COM_H_ */
