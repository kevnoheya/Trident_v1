//################################################
//  �V���A���ʐM
//                         				K.Richardo
// Baudrate : 115200bps
// printf, scanf
//################################################
#include "com.h"

//===============================================
// UART1 : �o�b�t�@�[�̏�����
//===============================================
void Serial_Init( void )
{
	setbuf( stdout, NULL );
	setbuf( stdin, NULL );
}
//===============================================
// UART1 : printf(syscalls.c�̊֐���u��������)
//===============================================
void __io_putchar(uint8_t ch)
{
	HAL_UART_Transmit(&huart1, &ch, 1, 1);
}

//===============================================
// UART1 : scanf(syscalls.c�̊֐���u��������)
//===============================================
int __io_getchar(void)
{
	HAL_StatusTypeDef Status = HAL_BUSY;
	uint8_t Data;

	while(Status != HAL_OK)
	Status = HAL_UART_Receive(&huart1, &Data, 1, 10);

	return(Data);
}
