/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "ui.h"
#include "system.h"
#include "battery.h"
#include "myStruct.h"
#include "tick.h"
#include "com.h"
#include "sensor.h"
#include "mpu9250.h"
#include "motor.h"
#include "control.h"
#include "move.h"
#include "log.h"
#include "parameter.h"
#include "map.h"
#include "search.h"
#include "flash.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim11;

extern UART_HandleTypeDef huart1;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_L_Pin GPIO_PIN_13
#define SW_L_GPIO_Port GPIOC
#define SEN_WALL_FR_Pin GPIO_PIN_0
#define SEN_WALL_FR_GPIO_Port GPIOC
#define SEN_WALL_R_Pin GPIO_PIN_1
#define SEN_WALL_R_GPIO_Port GPIOC
#define SEN_WALL_L_Pin GPIO_PIN_2
#define SEN_WALL_L_GPIO_Port GPIOC
#define SEN_WALL_FL_Pin GPIO_PIN_3
#define SEN_WALL_FL_GPIO_Port GPIOC
#define MOT_PWMA_Pin GPIO_PIN_0
#define MOT_PWMA_GPIO_Port GPIOA
#define MOT_PWMB_Pin GPIO_PIN_2
#define MOT_PWMB_GPIO_Port GPIOA
#define GYRO_NCS_Pin GPIO_PIN_4
#define GYRO_NCS_GPIO_Port GPIOA
#define GYRO_SCK_Pin GPIO_PIN_5
#define GYRO_SCK_GPIO_Port GPIOA
#define GYRO_MISO_Pin GPIO_PIN_6
#define GYRO_MISO_GPIO_Port GPIOA
#define GYRO_MOSI_Pin GPIO_PIN_7
#define GYRO_MOSI_GPIO_Port GPIOA
#define BATT_V_Pin GPIO_PIN_1
#define BATT_V_GPIO_Port GPIOB
#define LED_WALL_FL_Pin GPIO_PIN_12
#define LED_WALL_FL_GPIO_Port GPIOB
#define LED_WALL_L_Pin GPIO_PIN_13
#define LED_WALL_L_GPIO_Port GPIOB
#define LED_WALL_R_Pin GPIO_PIN_14
#define LED_WALL_R_GPIO_Port GPIOB
#define LED_WALL_FR_Pin GPIO_PIN_15
#define LED_WALL_FR_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_9
#define LED4_GPIO_Port GPIOC
#define MOT_AIN2_Pin GPIO_PIN_10
#define MOT_AIN2_GPIO_Port GPIOC
#define MOT_AIN1_Pin GPIO_PIN_11
#define MOT_AIN1_GPIO_Port GPIOC
#define MOT_STBY_Pin GPIO_PIN_12
#define MOT_STBY_GPIO_Port GPIOC
#define MOT_BIN1_Pin GPIO_PIN_2
#define MOT_BIN1_GPIO_Port GPIOD
#define MOT_BIN2_Pin GPIO_PIN_3
#define MOT_BIN2_GPIO_Port GPIOB
#define ENC_L_A_Pin GPIO_PIN_4
#define ENC_L_A_GPIO_Port GPIOB
#define ENC_L_B_Pin GPIO_PIN_5
#define ENC_L_B_GPIO_Port GPIOB
#define ENC_R_A_Pin GPIO_PIN_6
#define ENC_R_A_GPIO_Port GPIOB
#define ENC_R_B_Pin GPIO_PIN_7
#define ENC_R_B_GPIO_Port GPIOB
#define SW_R_Pin GPIO_PIN_8
#define SW_R_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_9
#define BUZZER_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
