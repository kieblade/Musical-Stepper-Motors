/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
	* User code by Phelan Hobbs and Seth Jackson
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"	
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f0xx_hal.h"
#include <math.h>

#define MAX 1500

double calcFreq(char ThisNote[]);
void TwoSetup(void);
void FirstExpire(void);
void SecondExpire(void);
void TaChar(char);
void TaCharArray(char[]);
void wait(double del);
void note(int num,int dur);
void pa(int durp);
char letter;
char word[4];
volatile char buffer[MAX];
uint32_t front = 0;
uint32_t rear = 0;
uint32_t itemCount = 0;
int section = 0;
int tone = 0;
int octave = 0;
int nNote = 0;

double loopCounter = 0.0;
uint8_t LRecived = 0;

int coun;
double del;
double C = 16.35;
double Db = 17.32;
double D = 18.35;
double Eb = 19.45;
double E = 20.6;
double F = 21.83;
double Gb = 23.12;
double G = 24.5;
double Ab = 25.96;
double A = 27.5;
double Bb = 29.14;
double B = 30.87;
double freq = 0;
double tempo=120.0;
bool doubleSetup = 0;
int thisOct;

char Note1[3]; 
char Note2[3];
double period1;
double period2;
int TimeRemain1 = 0;
int TimeRemain2 = 0;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable the GPIOA clock in the RCC
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable the GPIOC clock in the RCC
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable the GPIOB clock in the RCC
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable the USART clock in the RCC
	RCC->APB1ENR |= RCC_APB1ENR_DACEN; // Enable the DAC clock in the RCC
	// Initialize pins PC6, PC7, PC8 & PC9
	// Sets the 12th, 14th, 16th and 18th bits in the GPIOC_MODER register
	GPIOC->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);
	// Clears the 13th, 15th, 17th and 19th bits in the GPIOC_MODER register
	GPIOC->MODER &= ~((1 << 13) | (1 << 15) | (1 << 17) | (1 << 19));
	// Clears the 6th, 7th, 8th and 9th bits in the GPIOC_OTYPER register
	GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
	// Clears the 12th, 14th, 16th and 18th bits in the GPIOC_OSPEEDR register
	GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14) | (1 << 16) | (1 << 18));
	// Clears the 12th, 13th, 14th, 15th, 16th, 17th, 18th and 19th bits in the GPIOC_PUPDR register
	GPIOC->PUPDR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 16) | (1 << 17) | (1 << 18) | (1 << 19));
	
	// yellow to PB10 orange to PB11 black to gnd.
	
	GPIOB->MODER |= (1 << 21) | (1 << 23);
	GPIOB->AFR[1] |= (1 << 14) | (1 << 10);
	
	USART3->BRR = HAL_RCC_GetHCLKFreq()/115200;
	USART3->CR1 |= (1 << 5) | (1 << 3) | (1 << 2) | 1;
	
	NVIC_EnableIRQ(USART3_4_IRQn);
	
	NVIC_SetPriority(USART3_4_IRQn, 1);

	// Initialize PA4
	GPIOA->MODER |= ((1<<8)|(1<<9));
	GPIOA->PUPDR &= (~(1<<8)|~(1<<9));
	
	// PA1 and PA2 general output mode, no pull up or pull down.
	GPIOA->MODER |= (1 << 2) | (1 << 0);
	GPIOA->PUPDR &= (~(1<<1)|~(1<<0)|~(1<<3)|~(1<<2));
	GPIOA->ODR &= (~(1 << 1)|~(1 << 0));

	DAC->SWTRIGR |= (1<<0);
	DAC->CR |= ((1<<3)|(1<<4)|(1<<5));
	DAC->CR |= (1<<0);
	
	DAC->SWTRIGR |= (1<<1);
	DAC->CR |= ((1<<21)|(1<<20)|(1<<19));
	DAC->CR |= (1<<16);
	//DAC->DHR8R1 = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		//plink COM < Filename
		
		if (LRecived == 1)
		{
			if (TimeRemain1==0){
				if (TimeRemain2 ==0) {
					doubleSetup = 1;
					TaCharArray("read two notes");
					TaChar('\n');
					TwoSetup(); //if both timers expire, read next four lines
				} else if (!doubleSetup){
					TaCharArray("read note 1");
					TaChar('\n');
					FirstExpire();//if T1 expires, read next 2 lines as it will play on motor 1
				}
			} else if (TimeRemain2 == 0 && !doubleSetup){
				TaCharArray("read note 2");
				TaChar('\n');
				SecondExpire(); //if T2 expires read next 2 lines as it will play on motor 2
			}
			
			if (Note1[0] == 'o')
			{
				TimeRemain1--;
				HAL_Delay(1);
				GPIOA->ODR &= ~(1 << 0); //toggle step
				
			}
			else
			{
					TimeRemain1--;
					GPIOA->ODR ^= (1 << 0); //toggle step
					wait(period1);
					GPIOA->ODR ^= (1 << 0); //toggle step
					wait(period1);
			}
			
			
			if (Note2[0] == 'o')
			{
				TimeRemain2--;
				HAL_Delay(1);
				GPIOA->ODR &= ~(1 << 1); //toggle step
			}
			else
			{
					TimeRemain2--;
					GPIOA->ODR ^= (1 << 1); //toggle step
					wait(period2);
					GPIOA->ODR ^= (1 << 1); //toggle step
					wait(period2);
			}
		}
				

    /* USER CODE END WHILE */
		
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void TwoSetup(void){
	FirstExpire();
	TaCharArray("Current notes are: Note1: ");
	TaCharArray(Note1);
	TaChar('\n');
	SecondExpire();
	doubleSetup = 0;
	TaCharArray("Current notes are: Note1: ");
	TaCharArray(Note1);
	TaCharArray(" Note2: ");
	TaCharArray(Note2);
	TaChar('\n');
}
	void FirstExpire(void){
		
		memset(Note1, 0, sizeof(word));
		TimeRemain1 = 0;
		
		while (section < 2)
		{
			if (front == rear)
			{
				section = 4;
				LRecived = 0;
			}
			else if (LRecived == 1)
			{
				letter = buffer[front];
				front++;
				
				if (section == 0)
				{
					if (letter == '\r')
					{
						// turn on light
						TaCharArray("LINE 444: ");
						TaCharArray(Note1);
						TaChar('\n');
					}
					else if (letter == '\n')
					{
						section++;
					}
					else
					{
						strncat(Note1, &letter, 1);
					}
				}
				else if (section == 1)
				{
					if (letter == '\r')
					{
						TaChar('\n');
					}
					else if (letter == '\n')
					{
						if (strcmp(Note1, "off") == 0)
						{
							GPIOC->ODR &= ~(1 << 9); // green
							DAC->DHR8R1 = 0;
						}
						else
						{
							period1 = calcFreq(Note1);
							int oct1 = thisOct;
							tone= 12*(oct1+1)+nNote;
							DAC->DHR8R1 = tone + 127;
							
							GPIOC->ODR |= (1 << 9); // green
							//GPIOC->ODR |= (1 << 8); // orange
							//GPIOC->ODR |= (1 << 7); // blue
							//GPIOC->ODR |= (1 << 6); // red
						}
						section++;
					}
					else
					{
						TaChar(letter);
						TimeRemain1 = (TimeRemain1 * 10) + (letter - 48);
					}
				}
			}
			else
			{
				section = 4; 
			}
		}
	section = 0;
}
void SecondExpire(void){
	
	memset(Note2, 0, sizeof(word));
	TimeRemain2 = 0;
	
	while (section < 2)
	{
		if (front == rear)
			{
				section = 4;
				LRecived = 0;
			}
			else if (LRecived == 1)
			{
				letter = buffer[front];
				front++;
				
				if (section == 0)
				{
					if (letter == '\r')
					{
						// turn on light
						TaCharArray("LINE 504");
						TaCharArray(Note2);
						TaChar('\n');
					}
					else if (letter == '\n')
					{
						section++;
					}
					else
					{
						strncat(Note2, &letter, 1);
					}
				}
				else if (section == 1)
				{
					if (letter == '\r')
					{
						TaChar('\n');
					}
					else if (letter == '\n')
					{
						TaCharArray("line 543");
						TaChar('\n');
						
						if (strcmp(Note2, "off") == 0)
						{
							GPIOC->ODR &= ~(1 << 6); // red
							DAC->DHR8R2 = 0;
						}
						else
						{
							period2 = calcFreq(Note2);
							int oct2= thisOct;
							tone= 12*(oct2+1)+nNote;
							DAC->DHR8R2 = tone + 127;
							
							GPIOC->ODR |= (1 << 6); // red
						}
						
						// turn off light
						section++;
					}
					else
					{
						TaChar(letter);
						TimeRemain2 = (TimeRemain2 * 10) + (letter - 48);
					}
				}
			}
			else
			{
				section = 4; 
			}
	}
	section = 0;
}

double calcFreq(char ThisNote[]){
	
	TaCharArray("Currently reading: ");
	TaChar(ThisNote[0]);
	TaChar('\n');
	switch (ThisNote[0]){
		case 'C':
			nNote = 0;
			break;
		case 'D':
			nNote = 2;
			break;
		case 'E':
			nNote = 4;
			break;
		case 'F':
			nNote = 5;
			break;
		case 'G':
			nNote = 7;
			break;
		case 'A':
			nNote = 9;
			break;
		case 'B':
			nNote = 11;
			break;
		default:
			TaCharArray("ERROR");
			break;
	}
	if (ThisNote[1] == '#'){
		nNote++;
		thisOct = ThisNote[2] -48;
	} else {
		thisOct = ThisNote[1] -48;
	}
	
	double thisOctDoub = (double)thisOct;
	switch (nNote)
	{
		case 0:
			freq = C*(pow(2.0,thisOctDoub));
			break;
		case 1:
			freq = Db*(pow(2.0,thisOctDoub));
			break;
		case 2:
			freq = D*(pow(2.0,thisOctDoub));
			break;
		case 3:
			freq = Eb*(pow(2.0,thisOctDoub));
			break;
		case 4:
			freq = E*(pow(2.0,thisOctDoub));
			break;
		case 5:
			freq = F*(pow(2.0,thisOctDoub));
			break;
		case 6:
			freq = Gb*(pow(2.0,thisOctDoub));
			break;
		case 7:
			freq = G*(pow(2.0,thisOctDoub));
			break;
		case 8:
			freq = Ab*(pow(2.0,thisOctDoub));
			break;
		case 9:
			freq = A*(pow(2.0,thisOctDoub));
			break;
		case 10:
			freq = Bb*(pow(2.0,thisOctDoub));
			break;
		case 11:
			freq = B*(pow(2.0,thisOctDoub));
			break;
	}
	
	TaCharArray("reads out properly: ");
	TaCharArray(ThisNote);
	TaChar('\n');
	return (1.0/freq);	
	
	
}

/* USER CODE BEGIN 4 */
void TaChar(char c)
{
	while (!(USART3->ISR & (1 << 7)))
	{
		
	}
	USART3->TDR = c;
}

volatile uint32_t count = 0;
void TaCharArray(char L[])
{
	while (L[count] != 0)
	{
		TaChar(L[count]);
		count += 1;
	}
	count = 0;
}

volatile uint32_t waitCount = 0;
void wait(double del)
{
	while (waitCount < del*10000)
	{
		waitCount++;
	}
	waitCount = 0;
}

void USART3_4_IRQHandler(void)
{
	buffer[rear] = USART3->RDR;
	rear++;
	if (LRecived == 0)
	{
		LRecived = 1;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
