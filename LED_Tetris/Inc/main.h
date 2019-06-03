/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

// ----------------< DATASHEETS >------------------

/* LED Connection (on Board)
 * +-+-+
 * |X| | 1 2 VCC |
 * +-+-+
 * |X| | 3 4 VCC |
 * +-+-+
 * |X| | 5 6 GND |
 * +-+-+
 * | | | 7 8
 * +-+-+
 * | | | 9 10
 * +-+-+
 * | | | 11 12
 * +-+-+
 * | | | 12 14
 * +-+-+
 * | | | 15 16
 * +-+-+
 * | | | 17 18
 * +-+-+
 * | |X| 19 20  | MOSI
 * +-+-+
 * | | | 21 22
 * +-+-+
 * |X|X| 23 24 ChipSelect | SCK
 * +-+-+
 * | | | 25 26
 * +-+-+
 */

/*  Main table shape
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X| //no usage
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X| | | | | | | | |X|
 *  +-+-+-+-+-+-+-+-+-+-+
 *  |X|X|X|X|X|X|X|X|X|X|
 *  +-+-+-+-+-+-+-+-+-+-+
 */

/*	LEVELING
 *  level 1 < 100   pts
 *  level 2 < 250   pts
 *  level 3 < 500   pts
 *  level 4 < 1000  pts
 *  level 5 < 2150  pts
 *  level 6 < 3500  pts
 *  level 7 < 5000  pts
 *  level 8 >= 5000 pts
 */

/*	SCORE CALCULATION
 *	1 line  $$ +=  2 * level
 *	2 lines $$ += (2 + 3) * level
 *  3 lines $$ += (5 + 4) * level
 *  4 lines $$ += (9 + 5) * level
 *  empty board 	$$ += 70
 *	fast 'go down' 	$$ += 1
 */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
