
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "display_lib.h"
#include <time.h>

#define false 0
#define true 1
#define STEPDOWNMAX 8


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
// ADC1 --> buttons for sterring
// DAC  --> music through the speaker
// SPI1 --> LED matrix
// TIM2 --> 7-segment display [400 Hz]
// TIM3 --> play music [8000 Hz]
// TIM4 --> frequency of game [10Hz]

volatile _Bool mainTable[18][10] = { false }; //18 rows, 10 columns

_Bool gameOn = false; // is game paused or not
_Bool state = false; //state of pressed button
volatile uint16_t gameScore = 0; //score of game

uint16_t ADCvalue; //value of pressed button

uint8_t scoreDisplayNum = 1; //num of 7-segment display (1,2,3 or 4)

volatile uint8_t currX = 1; //current position of the piece
volatile uint8_t currY = 5;
volatile uint8_t currShape = 0; //num of current shape (numbers shown below)
volatile uint8_t currShapePhase = 0; // which rotation phase

uint8_t stepDownVar; // variable used for going down with shape
volatile uint8_t stepDownVarMax = STEPDOWNMAX;

extern const uint8_t rawData[669362];
int32_t musicIndex = 0;

extern _Bool shapeT[4][4][4];
extern _Bool shapeO[4][4][4];
extern _Bool shapeI[4][4][4];
extern _Bool shapeL[4][4][4];
extern _Bool shapeJ[4][4][4];
extern _Bool shapeS[4][4][4];
extern _Bool shapeZ[4][4][4];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

// ---------------< declarations of functions >-----------------
void rotate();
void goLeft(); //Przesuniecie w lewo
void goRight(); //Przesuniecie w prawo
void goDown(); //Zjedz w dol
void Play_Pause(); //play/pause
void stepDown();
void finish();
void writeLedMatrix();
_Bool fullRow(uint8_t row);
void removeShape(_Bool shape[4][4][4], int8_t row, int8_t col, uint8_t position);
void putShape(_Bool shape[4][4][4], int8_t row, int8_t col, uint8_t position);
void placeNew();


// -------------< Timers, Spi, DAC, ADC functions >--------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM2)
    {
		DISP_VAL_NULL;
		switch(scoreDisplayNum)
		{
		case 1:
		{
			displayValue(1,(gameScore/1000));
		}break;
		case 2:
		{
			displayValue(2,(gameScore/100)%10);
		}break;
		case 3:
		{
			displayValue(3,(gameScore/10)%10);
		}break;
		case 4:
		{
			displayValue(4,gameScore%10);
			scoreDisplayNum = 0;
		};break;
	  };
		++scoreDisplayNum;
	}

	if(htim->Instance == TIM3)
	{
		/*
		if(gameOn == true)
		{
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,rawData[musicIndex]);
			musicIndex++;
			if(musicIndex >= 669361) musicIndex = 0;
		}
		*/
	}

	if(htim->Instance == TIM4)
	{
		if(gameOn == true)
		{
			writeLedMatrix(); //WAZNE ABY ODKOMENTOWAC PO TESTACH writeGG()
			++stepDownVar;
			if(stepDownVar >= stepDownVarMax) //new value 8 !!
			{
				stepDown();
				stepDownVar = 0;
			}
		}
	}
}

// ----------------------<Game functions>------------------------
void initMainTable()
{
	for(int i=0; i<18; i++)
	{
		for(int j=1;j<9;j++)
		{
			mainTable[i][0] = true;
			mainTable[i][j] = false;
			mainTable[i][9] = true;
			mainTable[17][j] = true;
		}
	}
}

uint8_t valueOfColumn(uint8_t col, uint8_t shift) //shift: 8 or 0 (screen 1 or 2)
{
	uint8_t suma = 0x00;
	int tmp = 0x01;
	for(int i=8;i>0;i--)
	{
		if(mainTable[i+shift][col] == true)
		{
			suma += tmp;
		}
		tmp = tmp << 1;
	}
	return suma;
}

void writeLedByte(uint8_t addr1, uint8_t data1, uint8_t addr2, uint8_t data2)
{
	uint8_t data[] = {addr1,data1,addr2,data2};

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,(uint8_t*)data,4,HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
}

void initLED()
{

	writeLedByte(0x09,0x00,0x09,0x00); // no Decode-Mode
	writeLedByte(0x0a,0x00,0x0a,0x00); // Intensity of light: here 1/4
	writeLedByte(0x0b,0x07,0x0b,0x07); // Scan-Limit: all digits
	writeLedByte(0x0c,0x01,0x0c,0x01); // Shutdown: Normal Operation
	writeLedByte(0x0f,0x00,0x0f,0x00); // Display text: nothing

}

_Bool ANDMatrix(_Bool shape[4][4][4], int8_t row, int8_t col,uint8_t position) //return if can change position or rotate shape
{
	for(int8_t i = row;i < row + 4; i++)
	{
		for(int8_t j = col; j < col + 4;j++)
		{
			if(i < 0 || i > 17) continue;
			if(j < 0 || j > 9) continue;
			if(mainTable[i][j] == 1 && shape[position][i-row][j-col] == 1) return true;
		}
	}
	return false;
}

void writeLedMatrix()
{
	for(int i=1;i<9;i++)
	{
		writeLedByte(9-i,valueOfColumn(i,8),9-i,valueOfColumn(i,0));
	}
}

//Obrot ksztaltu
void rotate()
{
	_Bool*** tmpShape;
	switch(currShape)
	{
		case 0:tmpShape = shapeT;break;
		case 1:tmpShape = shapeO;break;
		case 2:tmpShape = shapeI;break;
		case 3:tmpShape = shapeL;break;
		case 4:tmpShape = shapeJ;break;
		case 5:tmpShape = shapeS;break;
		case 6:tmpShape = shapeZ;break;
	}
	removeShape(tmpShape,currX,currY,currShapePhase);
	if(ANDMatrix(tmpShape,currX,currY,(currShapePhase + 1) % 4) == false)
	{
		currShapePhase = (currShapePhase + 1) % 4;
		putShape(tmpShape,currX,currY,currShapePhase);
	}
	else
	{
		putShape(tmpShape,currX,currY,currShapePhase);
	}
}

//Przesuniecie w lewo
void goLeft()
{
	_Bool*** tmpShape;
	switch(currShape)
	{
		case 0:tmpShape = shapeT;break;
		case 1:tmpShape = shapeO;break;
		case 2:tmpShape = shapeI;break;
		case 3:tmpShape = shapeL;break;
		case 4:tmpShape = shapeJ;break;
		case 5:tmpShape = shapeS;break;
		case 6:tmpShape = shapeZ;break;
	}
	removeShape(tmpShape,currX,currY,currShapePhase);
	if(ANDMatrix(tmpShape,currX,currY-1,currShapePhase) == false)
	{
		putShape(tmpShape,currX,--currY,currShapePhase);
	}
	else
	{
		putShape(tmpShape,currX,currY,currShapePhase);
	}
}

//Przesuniecie w prawo
void goRight()
{
	_Bool*** tmpShape;
	switch(currShape)
	{
		case 0:tmpShape = shapeT;break;
		case 1:tmpShape = shapeO;break;
		case 2:tmpShape = shapeI;break;
		case 3:tmpShape = shapeL;break;
		case 4:tmpShape = shapeJ;break;
		case 5:tmpShape = shapeS;break;
		case 6:tmpShape = shapeZ;break;
	}
	removeShape(tmpShape,currX,currY,currShapePhase);
	if(ANDMatrix(tmpShape,currX,currY+1,currShapePhase) == false)
	{
		putShape(tmpShape,currX,++currY,currShapePhase);
	}
	else
	{
		putShape(tmpShape,currX,currY,currShapePhase);
	}
}

//Zjedz w dol
void goDown()
{
	stepDownVarMax = 2;
}

//play/pause
void Play_Pause()
{
	gameOn ^= 1;
}

//maybe not use, remove it after prapare new solution
/*
void movePiece(int direction)
{
	switch(direction)
	{
	case 1: goLeft();break;
	case 2: rotate();break;
	case 3: goDown();break;
	case 4: goRight();break;
	case 5: Play_Pause();break;
	default:break;
	}
}
*/

// no usage, delete after testing
_Bool firstRowZero(_Bool shape[4][4][4], uint8_t position)
{
	for (int k = 0; k < 4; k++)
	{
		if (shape[position][0][k] == true) return false;
	}
	return true;
}

void removeShape(_Bool shape[4][4][4], int8_t row, int8_t col, uint8_t position)
{
	for(int8_t i = row;i < row + 4;i++)
		{
			for(int8_t j = col; j < col + 4; j++)
			{
				if (j < 9 && i < 17 && j >= 0)
				{
					 if(shape[position][i - row][j - col] == true) mainTable[i][j] = false;
				}
			}
		}
}

void putShape(_Bool shape[4][4][4], int8_t row, int8_t col, uint8_t position) // row and col means left top corner
{
	for(int8_t i = row;i < row + 4;i++)
	{
		for(int8_t j = col; j < col + 4; j++)
		{
			if (j < 9 && i < 17 && j >= 0)
			{
				 if(shape[position][i - row][j - col] == true) mainTable[i][j] = true;
			}
		}
	}
}

void placeNew()
{

	uint8_t shapeNr = rand()%7;
	currX = 1;
	currY = 3;
	currShape = shapeNr;
	currShapePhase = 0;
	switch(shapeNr)
	{
	case 0:
		{
			if(ANDMatrix(shapeT,currX,currY,currShapePhase) == false) putShape(shapeT,currX,currY,currShapePhase);
			else finish();
		}break;
	case 1:
		{
			if(ANDMatrix(shapeO,--currX,currY,currShapePhase) == false) putShape(shapeO,currX,currY,currShapePhase);
			else finish();
		}break;
	case 2:
		{
			if(ANDMatrix(shapeI,--currX,currY,currShapePhase) == false) putShape(shapeI,currX,currY,currShapePhase);
			else finish();
		}break;
	case 3:
		{
			if(ANDMatrix(shapeL,currX,currY,currShapePhase) == false) putShape(shapeL,currX,currY,currShapePhase);
			else finish();
		}break;
	case 4:
		{
			if(ANDMatrix(shapeJ,currX,currY,currShapePhase) == false) putShape(shapeJ,currX,currY,currShapePhase);
			else finish();
		}break;
	case 5:
		{
			if(ANDMatrix(shapeS,--currX,currY,currShapePhase) == false) putShape(shapeS,currX,currY,currShapePhase);
			else finish();
		}break;
	case 6:
		{
			if(ANDMatrix(shapeZ,--currX,currY,currShapePhase) == false) putShape(shapeZ,currX,currY,currShapePhase);
			else finish();
		}break;
	}
	//'curr-1' is in some shapes cause shape is starting not from first row
}

void stepDown()
{
	_Bool*** tmpShape;
		switch(currShape)
		{
		case 0:tmpShape = shapeT;break;
		case 1:tmpShape = shapeO;break;
		case 2:tmpShape = shapeI;break;
		case 3:tmpShape = shapeL;break;
		case 4:tmpShape = shapeJ;break;
		case 5:tmpShape = shapeS;break;
		case 6:tmpShape = shapeZ;break;
		}
		removeShape(tmpShape,currX,currY,currShapePhase);
		if(ANDMatrix(tmpShape,currX+1,currY,currShapePhase) == false)
		{
			putShape(tmpShape,++currX,currY,currShapePhase);
		}
		else
		{
			putShape(tmpShape,currX,currY,currShapePhase);
			uint8_t countLines = 0;
			for(uint8_t i = currX; i < currX + 4; i++)
			{
				if(fullRow(i)==true && i < 17)
				{
					deleteRow(i);
					countLines++;
					pushDownTable(i);
				}
			}
			if(countLines == 4) countLines+=6;
			gameScore = gameScore + countLines;
			stepDownVarMax = STEPDOWNMAX;
			placeNew();
		}
}

void pushDownTable(uint8_t row)
{
	for(uint8_t i=row-1;i>1;i--)
	{
		for(uint8_t k=0;k<9;k++)
		{
			mainTable[i+1][k] = mainTable[i][k];
		}
	}
	for(int i=0;i<9;i++)
	{
		mainTable[0][i] = false;
	}
}

_Bool fullRow(uint8_t row)
{
	for(uint8_t i = 1; i<9; i++)
	{
		if(mainTable[row][i] == 0) return false;
	}
	return true;
}

void deleteRow(uint8_t row)
{
	for(uint8_t i=1;i<9;i++)
	{
		mainTable[row][i] = false;
	}
}

void writePlay()
{
	writeLedByte(0x01,0x00,0x01,0x16); // Line 1
	writeLedByte(0x02,0x82,0x02,0x13); // Line 2
	writeLedByte(0x03,0x03,0x03,0xf6); // Line 3
	writeLedByte(0x04,0x11,0x04,0x00); // Line 4
	writeLedByte(0x05,0x38,0x05,0x00); // Line 5
	writeLedByte(0x06,0x83,0x06,0x43); // Line 6
	writeLedByte(0x07,0x01,0x07,0xa5); // Line 7
	writeLedByte(0x08,0x81,0x08,0xf3); // Line 8
}

void writeGG()
{
	writeLedByte(0x01,0x00,0x01,0x00); // Line 1
	writeLedByte(0x02,0x08,0x02,0x08); // Line 2
	writeLedByte(0x03,0x4c,0x03,0x4c); // Line 3
	writeLedByte(0x04,0x4a,0x04,0x4a); // Line 4
	writeLedByte(0x05,0x42,0x05,0x42); // Line 5
	writeLedByte(0x06,0x42,0x06,0x42); // Line 6
	writeLedByte(0x07,0x3c,0x07,0x3c); // Line 7
	writeLedByte(0x08,0x00,0x08,0x00); // Line 8
}

void finish()
{
	gameOn = false;
	writeGG();
	HAL_Delay(3000);
	initMainTable();
	writePlay();
}


//Must be in while loop
void buttonPressedAction()
{
	HAL_ADC_Start(&hadc1);

	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		ADCvalue = HAL_ADC_GetValue(&hadc1);
	}

	if (ADCvalue < 4000)
	{
		HAL_Delay(1);

		HAL_ADC_Start(&hadc1);

		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
		{
			ADCvalue = HAL_ADC_GetValue(&hadc1);
		}

		if (ADCvalue < 1500 && ADCvalue > 1300)
		{
			if (state == false)
			{
				//pressedKey = 5
				Play_Pause();
				state = true;
			}
		}

		else if (ADCvalue < 750 && ADCvalue >= 600)
		{
			if (state == false)
			{
				//pressedKey = 4
				goRight();
				state = true;
			}
		}

		else if (ADCvalue < 400 && ADCvalue >300)
		{
			if (state == false)
			{
				//pressedKey = 3
				goDown();
				state = true;
			}
		}

		else if (ADCvalue < 200 && ADCvalue > 100)
		{
			if (state == false)
			{
				//pressedKey = 2
				rotate();
				state = true;
			}
		}

		else if (ADCvalue < 50)
		{
			if (state == false)
			{
				//pressedKey = 1
				goLeft();
				state = true;
			}
		}
		// Czy ten else pod spodem jest w ogole potrzebny??????????
		}
		else
		{
			if (state == true)
			{
				//pressedKey = 0
				state = false;
			}
			HAL_Delay(1);
		}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_DAC_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */


  HAL_TIM_Base_Start_IT(&htim2); //scoreboard
  HAL_TIM_Base_Start_IT(&htim3); //muzyka
  HAL_TIM_Base_Start_IT(&htim4); //kroki gry

  HAL_DAC_Start(&hdac,DAC_CHANNEL_1);

  srand((uint8_t)TIM2->ARR);
  initLED();
  initMainTable();
  gameOn = true;
  placeNew();
  writePlay();
  /* MAKRO DO TESTOW  */

//  HAL_Delay(1100);
//  goDown();
//  HAL_Delay(11000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(10000);
//  rotate();
//  HAL_Delay(900);
//  goLeft();
//  HAL_Delay(8000);
//  goRight();
//  HAL_Delay(1000);
//  goRight();
//  HAL_Delay(1000);
//  goRight();
//  HAL_Delay(1000);
//  goRight();
//  HAL_Delay(9000);
//  rotate();
//  HAL_Delay(9000);
//  rotate();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(8000);
//  rotate();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(1000);
//  goLeft();
//  HAL_Delay(15000);
//  rotate();
//  HAL_Delay(1000);
//  goRight();
//  HAL_Delay(1000);
//  goRight();
//  HAL_Delay(1000);
//  goRight();
//

  /*END OF MACRO*/



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
	  buttonPressedAction();
  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* DAC init function */
static void MX_DAC_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization 
    */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT1 config 
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 209;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 104;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 839;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
                           PE6 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
