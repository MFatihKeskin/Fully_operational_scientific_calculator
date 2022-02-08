/*
* bsp.c
*
* author: MUHAMMET FATIH KESKIN 
*
*/

//EVERY COMMENT YOU SEE HERE AND SIMILAR POSSIBLE SITUATIONS ARE IN THE ONLY CODING VIDEO I PUT IN THE LINKS.


#include "stm32g0xx.h"
#include "bsp.h"
#include "math.h"

volatile int add_calc; //if we press A go addition calculation
volatile int sub_calc; //if we press B go subtraction calculation
volatile int mul_calc; //if we press C go multiplication calculation
volatile int div_calc; //if we press D go division calculation

volatile int sci_mode; //scientific mode as counter but sometimes(Ex) x={A,B,C,D}
volatile int sci_mode_log10; // scientific mode log10 base calculation(EA press)
volatile int sci_mode_ln; // scientific mode ln calculation (EB press)
volatile int sci_mode_root; // scientific mode square_root calculation(EC press)
volatile int sci_mode_square; // scientific mode square calculation(ED press)

volatile int trigonometric_calc; // double press for trigonometric_calc(EEx) x={A,B,C,D}
volatile int trigonometric_calc_sin;//trigonometric mode sinus calculation (EEA)
volatile int trigonometric_calc_cos;//trigonometric mode cosinus calculation(EEB)
volatile int trigonometric_calc_tan;//trigonometric mode tanjant calculation(EEC)
volatile int trigonometric_calc_cot;//trigonometric mode cotanjant calculation(EEC)
volatile double trigonometric_calc_pi;//trigonometric mode pi calculation(EED)
static int pi_ctr;//pi counter for if three times press E (EEE)
volatile float trigonometric_result; //it will store the value I will send to my function

volatile int num; //in the first case, it will store the first number, then it will assign it and store the second number
volatile int num_temp; //a variable that the "num" variable above will assign

volatile int int_temp; //transient variable for decimal numbers for int
volatile float float_temp;//transient variable for decimal numbers for post comma1
volatile float post_comma; //transient variable for decimal numbers for post comma2

volatile int result;// I'll keep all the results and send them to suppress them
static int ctr; //ctr for all function its independent any function


void delay(volatile uint32_t s)
{
	for (; s > 0; s--) //standard delay func
		;
}
void init_variable(void) // all the above variables are defined
{
	add_calc = 0;
	sub_calc = 0;
	mul_calc = 0;
	div_calc = 0;

	sci_mode = 0;
	sci_mode_log10 = 0;
	sci_mode_ln = 0;
	sci_mode_root = 0;
	sci_mode_square = 0;

	trigonometric_calc = -1; //for double press
	trigonometric_calc_sin = 0;
	trigonometric_calc_cos = 0;
	trigonometric_calc_tan = 0;
	trigonometric_calc_cot = 0;
	trigonometric_calc_pi = 3.141;
	pi_ctr = 0;
	trigonometric_result = 0.0;

	num = 0;
	num_temp = 0;

	int_temp = 0;
	float_temp = 0.0;
	post_comma=0.0;

	result = 0;
	ctr = 0;

}

void init_GPIO(void)
{
	// Enable GPIOA clock
	RCC->IOPENR |= (1U << 0);

	/* Setup PA0 as output */
	GPIOA->MODER &= ~(3U << 2 * 0);
	GPIOA->MODER |= (1U << 2 * 0);
	/* Setup PA1 as output */
	GPIOA->MODER &= ~(3U << 2 * 1);
	GPIOA->MODER |= (1U << 2 * 1);
	/* Setup PA4 as output */
	GPIOA->MODER &= ~(3U << 2 * 4);
	GPIOA->MODER |= (1U << 2 * 4);
	/* Setup PA5 as output */
	GPIOA->MODER &= ~(3U << 2 * 5);
	GPIOA->MODER |= (1U << 2 * 5);
	/* Setup PA6 as output */
	GPIOA->MODER &= ~(3U << 2 * 6);
	GPIOA->MODER |= (1U << 2 * 6);
	/* Setup PA11 as output */
	GPIOA->MODER &= ~(3U << 2 * 11);
	GPIOA->MODER |= (1U << 2 * 11);
	/* Setup PA12 as output */
	GPIOA->MODER &= ~(3U << 2 * 12);
	GPIOA->MODER |= (1U << 2 * 12);

	// Enable GPIOB clock
	RCC->IOPENR |= (1U << 1);

	GPIOB->MODER &= ~(3U << 2 * 1);//PB1
	GPIOB->MODER |= (1U << 2 * 1);
	GPIOB->MODER &= ~(3U << 2 * 3);//PB3
	GPIOB->MODER |= (1U << 2 * 3);
	GPIOB->MODER &= ~(3U << 2 * 6);//PB6
	GPIOB->MODER |= (1U << 2 * 6);
	GPIOB->MODER &= ~(3U << 2 * 7);//PB7
	GPIOB->MODER |= (1U << 2 * 7);

	GPIOA->ODR |= (1U << 8); //PA0 A
	GPIOB->ODR |= (1U << 9); //PA1 B
	GPIOB->ODR |= (1U << 5); //PA4 C
	GPIOB->ODR |= (1U << 4); //PA5 D
}

void keypad_enable(void)
{		

	init_GPIO(); //the above variables are opened

	// Enable GPIOB clock
	RCC->IOPENR |= (1U << 1);

	// Setup pa8, pb9, pb5, pb4 as output rows
	GPIOA->MODER &= ~(3U << 2 * 8);
	GPIOA->MODER |= (1U << 2 * 8);

	GPIOB->MODER &= ~(3U << 2 * 9);
	GPIOB->MODER |= (1U << 2 * 9);

	GPIOB->MODER &= ~(3U << 2 * 5);
	GPIOB->MODER |= (1U << 2 * 5);

	GPIOB->MODER &= ~(3U << 2 * 4);
	GPIOB->MODER |= (1U << 2 * 4);

	//Setup pa9, pb0, pb2, pb8 as input columns
	GPIOA->MODER &= ~(3U << 2 * 9);
	GPIOA->PUPDR |= (2U << 2 * 9);

	GPIOB->MODER &= ~(3U << 0);
	GPIOB->PUPDR |= (2U << 0);

	GPIOB->MODER &= ~(3U << 2 * 2);
	GPIOB->PUPDR |= (2U << 2 * 2);

	GPIOB->MODER &= ~(3U << 2 * 8);
	GPIOB->PUPDR |= (2U << 2 * 8);

	// Setup interrupts for pa9, pb0,pb2,pb8
	EXTI->EXTICR[2] |= (0U << 8 * 1); //pa9
	EXTI->EXTICR[0] |= (1U << 0);	  //pb0
	EXTI->EXTICR[0] |= (1U << 8 * 2); //pb2
	EXTI->EXTICR[2] |= (1U << 0);	  //pb8

	//rising edge selected for pa9, pb0,pb2,pb8
	EXTI->RTSR1 |= (1U << 9);
	EXTI->RTSR1 |= (1U << 0);
	EXTI->RTSR1 |= (1U << 2);
	EXTI->RTSR1 |= (1U << 8);

	//mask register selected for pa9, pb0,pb2,pb8
	EXTI->IMR1 |= (1U << 9);
	EXTI->IMR1 |= (1U << 0);
	EXTI->IMR1 |= (1U << 2);
	EXTI->IMR1 |= (1U << 8);

	// enable NVIC and set interrupt priority
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	clearSSD();
}

void EXTI0_1_IRQHandler(void)
{ //interrupt from pb0
	clear_Rows_Keypad();

	GPIOA->ODR ^= (1U << 8);
	if ((GPIOB->IDR >> 0) & 1)
	{
		setSSD(15); //#
	}
	GPIOA->ODR ^= (1U << 8); //pa8

	GPIOB->ODR ^= (1U << 9); //pb9
	if ((GPIOB->IDR >> 0) & 1)
	{
		setSSD(9);
		num = (int)num;
		num = num * pow(10, ctr); //for multi digit number but I dont saw that on SSD but this number can =>10
		num += 9;				  //assign value to constant
		ctr++;					  //counter
	}

	GPIOB->ODR ^= (1U << 9); //pb9

	GPIOB->ODR ^= (1U << 5); //pb5
	if ((GPIOB->IDR >> 0) & 1)
	{
		setSSD(6);
		num = num * pow(10, ctr); //for multi digit number but I dont saw that on SSD but this number can =>10
		num += 6;
		ctr++; //counter
	}

	GPIOB->ODR ^= (1U << 5); //pb5

	GPIOB->ODR ^= (1U << 4); //pb4
	if ((GPIOB->IDR >> 0) & 1)
	{
		setSSD(3);
		num = num * pow(10, ctr); //for multi digit number but I dont saw that on SSD but this number can =>10
		num += 3;
		ctr++; //counter
	}
	GPIOB->ODR ^= (1U << 4); //pb4

	EXTI->RPR1 |= (1U << 0); //clear interrupt flag
	set_Rows_Keypad();
}

void EXTI2_3_IRQHandler(void)
{ //interrupt from PB2
	clear_Rows_Keypad();

	GPIOA->ODR ^= (1U << 8);
	if ((GPIOB->IDR >> 2) & 1)
	{
		setSSD(0);
		num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
		num += 0;
		ctr++;
	}
	GPIOA->ODR ^= (1U << 8); //pa8

	GPIOB->ODR ^= (1U << 9); //pb9
	if ((GPIOB->IDR >> 2) & 1)
	{
		setSSD(8);
		num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
		num += 8;
		ctr++;
	}
	GPIOB->ODR ^= (1U << 9); //pb9

	GPIOB->ODR ^= (1U << 5); //pb5
	if ((GPIOB->IDR >> 2) & 1)
	{
		setSSD(5);
		num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
		num += 5;
		ctr++;
	}
	GPIOB->ODR ^= (1U << 5); //pb5

	GPIOB->ODR ^= (1U << 4); //pb4
	if ((GPIOB->IDR >> 2) & 1)
	{
		setSSD(2);
		num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
		num += 2;
		ctr++;
	}
	GPIOB->ODR ^= (1U << 4); //pb4

	EXTI->RPR1 |= (1U << 2); //clear interrupt flag
	set_Rows_Keypad();
}

void EXTI4_15_IRQHandler(void)
{ //pb8 and pa9
	if ((GPIOB->IDR >> 8) & 1)
	{

		clear_Rows_Keypad();
		GPIOA->ODR ^= (1U << 8);
		if ((GPIOB->IDR >> 8) & 1)
		{
			setSSD(14);
		}
		GPIOA->ODR ^= (1U << 8); //pa8

		GPIOB->ODR ^= (1U << 9); //pb9
		if ((GPIOB->IDR >> 8) & 1)
		{
			setSSD(7);
			num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
			num += 7;
			ctr++;
		}
		GPIOB->ODR ^= (1U << 9); //pb9

		GPIOB->ODR ^= (1U << 5); //pb5
		if ((GPIOB->IDR >> 8) & 1)
		{
			setSSD(4);
			num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
			num += 4;
			ctr++;
		}
		GPIOB->ODR ^= (1U << 5); //pb5

		GPIOB->ODR ^= (1U << 4); //pb4
		if ((GPIOB->IDR >> 8) & 1)
		{
			setSSD(1);
			num = num * pow(10, ctr);//for multi digit number but I dont saw that on SSD but this number can =>10
			num += 1;
			ctr++;
		}
		GPIOB->ODR ^= (1U << 4); //pb4
		EXTI->RPR1 |= (1U << 8); //clear interrupt flag
		set_Rows_Keypad();
	}

	if ((GPIOA->IDR >> 9) & 1)
	{
		clear_Rows_Keypad();
		GPIOA->ODR ^= (1U << 8);
		if ((GPIOA->IDR >> 9) & 1)
		{
			setSSD(13); //div
		}
		GPIOA->ODR ^= (1U << 8); //pa8

		GPIOB->ODR ^= (1U << 9); //pb9
		if ((GPIOA->IDR >> 9) & 1)
		{
			setSSD(12); //mul
		}
		GPIOB->ODR ^= (1U << 9); //pb9

		GPIOB->ODR ^= (1U << 5); //pb5
		if ((GPIOA->IDR >> 9) & 1)
		{
			setSSD(11); //sub
		}
		GPIOB->ODR ^= (1U << 5); //pb5

		GPIOB->ODR ^= (1U << 4); //pb4
		if ((GPIOA->IDR >> 9) & 1)
		{
			setSSD(10); //add
		}
		GPIOB->ODR ^= (1U << 4); //pb4
		EXTI->RPR1 |= (1U << 9); //clear interrupt flag
		set_Rows_Keypad();
	}
}

void D1_digit() //only D1 digit is active, for negative symbol
{
	GPIOB->ODR |= (1U << 6);  // D1  digit to PB6
	GPIOB->ODR &= ~(1U << 7); // D2  digit to PB7
	GPIOB->ODR &= ~(1U << 1); // D3  digit to PB1
	GPIOB->ODR &= ~(1U << 3); // D4  digit to PB3
}

void D1_digit_with_delay(uint32_t x) // I'll send the number D1 to the SSD
{
	GPIOB->ODR |= (1U << 6);
	GPIOB->ODR &= ~(1U << 7);
	GPIOB->ODR &= ~(1U << 1);
	GPIOB->ODR &= ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void D2_digit(){//only D2 digit is active
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR |= (1U << 7);
	GPIOB ->ODR &= ~(1U << 1);
	GPIOB ->ODR &= ~(1U << 3);
}

void D2_digit_with_delay(uint32_t x)// I'll send the number D2 to the SSD
{
	GPIOA->MODER &= ~(3U << 2 * 7); //for dot
	GPIOB->ODR &= ~(1U << 6);
	GPIOB->ODR |= (1U << 7);
	GPIOB->ODR &= ~(1U << 1);
	GPIOB->ODR &= ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void D3_digit(){//only D3 digit is active
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR &= ~(1U << 7);
	GPIOB ->ODR |= (1U << 1);
	GPIOB ->ODR &= ~(1U << 3);
}

void D3_digit_with_delay(uint32_t x)// I'll send the number D3 to the SSD
{
	GPIOA->MODER &= ~(3U << 2 * 7); //for dot
	GPIOB->ODR &= ~(1U << 6);
	GPIOB->ODR &= ~(1U << 7);
	GPIOB->ODR |= (1U << 1);
	GPIOB->ODR &= ~(1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void D4_digit(){//only D1 digit is active
	GPIOB ->ODR &= ~(1U << 6);
	GPIOB ->ODR &= ~(1U << 7);
	GPIOB ->ODR &= ~(1U << 1);
	GPIOB ->ODR |= (1U << 3);
}

void D4_digit_with_delay(uint32_t x)// I'll send the number D4 to the SSD
{
	GPIOA->MODER &= ~(3U << 2 * 7); //for dot
	GPIOB->ODR &= ~(1U << 6);
	GPIOB->ODR &= ~(1U << 7);
	GPIOB->ODR &= ~(1U << 1);
	GPIOB->ODR |= (1U << 3);
	setSSD(x);
	delay(500);
	clearSSD();
}

void school_id(void) // my ıdle state = 1793
{//if I didn't use such a loop and delay function,
 //all the digits would have the same value as the SSD connection
	for (int i = 0; i < 6000; i++)
	{
		D1_digit_with_delay(1); // only D1
		D2_digit_with_delay(7); // only D2
		D3_digit_with_delay(9); // only D3
		D4_digit_with_delay(3); // only D4
	}
	clearSSD();
}

void clearSSD(void) //the common anode was cleaned by giving SSD 1.
{
	GPIOA->ODR |= (1U << 0);  //PA0 A
	GPIOA->ODR |= (1U << 1);  //PA1 B
	GPIOA->ODR |= (1U << 4);  //PA4 C
	GPIOA->ODR |= (1U << 5);  //PA5 D
	GPIOA->ODR |= (1U << 12); //PA12 E
	GPIOA->ODR |= (1U << 11); //PA11 F
	GPIOA->ODR |= (1U << 6);  //PA6 G
}

void setSSD(int print) // we have to give 0 to light the LEDs
{
	clearSSD(); //clean it up if there's any residue.
	switch (print)
	{
	case 0://0
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		break;

	case 1://1
		GPIOA->ODR &= ~(1U << 1); //PA1 B
		GPIOA->ODR &= ~(1U << 4); //PA4 C
		break;

	case 2://2
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	case 3://3
		GPIOA->ODR &= ~(1U << 0); //PA0 A
		GPIOA->ODR &= ~(1U << 1); //PA1 B
		GPIOA->ODR &= ~(1U << 4); //PA4 C
		GPIOA->ODR &= ~(1U << 5); //PA5 D
		GPIOA->ODR &= ~(1U << 6); //PA6 G
		break;

	case 4://4
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	case 5://5
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	case 6://6
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	case 7://7
		GPIOA->ODR &= ~(1U << 0); //PA0 A
		GPIOA->ODR &= ~(1U << 1); //PA1 B
		GPIOA->ODR &= ~(1U << 4); //PA4 C
		break;

	case 8://8
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	case 9://9
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;

	/* A -> addition or  log10 and sin with mode*/
	case 10:
		for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
		{
			D2_digit_with_delay(29);//a
			D3_digit_with_delay(25);//d
			D4_digit_with_delay(25);//d
		}

		if (num != 0 && sci_mode == 0 && trigonometric_calc == -1)
		{//if I didn't define sci_mode and trigonometric_calc,
		//it would increase these variables below and enter this function again.
		//I bought a guard to prevent it (in if clause)

			num = num & 0x7FFFFFFF;

			num_temp = num; //if the first number is received, save it by typing in num_temp.
			//because I'll get this variable to take the second number. num-temp store a first number
			add_calc++;		//I used it as a counter here because I wanted to make a transaction when pressed equally
			num = 0;		//reset as it will go to get the second number
		}
		if (sci_mode == 1) //scientific mode EA
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(30);//s
				D2_digit_with_delay(19);//l
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(33);//g
			}
			num = num & 0x7FFFFFFF;
			sci_mode_log10 = 1;// if EA is 1,increase this variable as a counter and save the result until it is equal
		}
		if (trigonometric_calc == 1)//EEA func because trigonometric_calc = -1 for first state
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(34);//t
				D2_digit_with_delay(30);//s
				D3_digit_with_delay(22);//i
				D4_digit_with_delay(23);//n
			}
			num = num & 0x7FFFFFFF;
			trigonometric_calc_sin = 1; // if EEA is 1,increase this variable as a counter and save the result until it is equal
		}

		else
		{
			ctr = 0; // because catch other interrupt and if ctr==0 => we dont know num1 and num2
		}
		break;

	/* B -> sub or ln and cos with mode*/
	case 11:
		for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
		{
			D2_digit_with_delay(30);//s
			D3_digit_with_delay(17);//u
			D4_digit_with_delay(36);//b
		}
		if (num != 0 && sci_mode == 0 && trigonometric_calc == -1)
		{//if I didn't define sci_mode and trigonometric_calc,
		//it would increase these variables below and enter this function again.
		//I bought a guard to prevent it (in if clause)
			num = num & 0x7FFFFFFF;
			num_temp = num;
			sub_calc = 1;//sub_Calc as a counter because we dont press the equal
			num = 0;
		}
		if (sci_mode == 1) //EB scientific mode
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(30);//s
				D3_digit_with_delay(19);//l
				D4_digit_with_delay(23);//n
			}
			num = num & 0x7FFFFFFF;
			sci_mode_ln = 1; //ln calc
		}
		if (trigonometric_calc == 1)  //EEB is trigonometric mode
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(34);//t
				D2_digit_with_delay(31);//c
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(30);//s
			}
			num = num & 0x7FFFFFFF;
			trigonometric_calc_cos = 1; //cos calc
		}
		else
		{
			ctr = 0;// because catch other interrupt and if ctr==0 => we dont know num1 and num2
		}
		break;

	/* C -> multiplication or root and tan with mode */
	case 12:
		for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
		{
			D1_digit_with_delay(31);//c
			D2_digit_with_delay(29);//a
			D3_digit_with_delay(21);//r
			D4_digit_with_delay(32);//p
		}
		if (num != 0 &&sci_mode == 0 && trigonometric_calc == -1)
		{//if I didn't define sci_mode and trigonometric_calc,
		//it would increase these variables below and enter this function again.
		//I bought a guard to prevent it (in if clause)

			num = num + pow(2, 31); // its the same num = num & 0x7FFFFFFF
			num_temp = num; //num temp store the first value
			mul_calc = 1; //multiplication calc active but we dont pressed to equal
			num = 0;
		}
		if (sci_mode == 1) //EC
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(30);//s
				D2_digit_with_delay(21);//r
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(34);//t
			}
			num = num & 0x7FFFFFFF;
			sci_mode_root = 1; //square_root calc but we dont pressed to equal
		}
		if (trigonometric_calc == 1) //EEC
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(34);//t
				D2_digit_with_delay(34);//t
				D3_digit_with_delay(29);//a
				D4_digit_with_delay(23);//n
			}
			num = num & 0x7FFFFFFF;
			trigonometric_calc_tan = 1; //tanjant calculation is active
		}
		else
		{
			ctr = 0;// because catch other interrupt and if ctr==0 => we dont know num1 and num2
		}

		break;

	/* D -> division or square and cot with scientrific mode */
	case 13:

		if (num != 0 && sci_mode == 0)
		{//if I didn't define sci_mode, it would increase these variables below and enter this function again.
		//I bought a guard to prevent it (in if clause)

			num = num + pow(2, 31);// its the same num = num & 0x7FFFFFFF
			num_temp = num;
			div_calc = 1; // division mode is active
			num = 0;
		}
		if (sci_mode == 1) // ED
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(30);//s
				D2_digit_with_delay(30);//s
				D3_digit_with_delay(35);//q
				D4_digit_with_delay(17);//u
			}
			num = num & 0x7FFFFFFF;
			sci_mode_square = 1; // pow func active
		}
		if (trigonometric_calc == 1)
		{
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D1_digit_with_delay(34);//t
				D2_digit_with_delay(31);//c
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(34);//t
			}
			num = num & 0x7FFFFFFF;
			trigonometric_calc_cot = 1; // cotanjant calc active
		}
		else
		{
			ctr = 0;// because catch other interrupt and if ctr==0 => we dont know num1 and num2
		}
		break;


		//All operations were done here and the result was tried to be seen on the SSD.
		//The reason for this "=" the result was that I was able to see without pressing.
		//In this way, I made calculations of all functions in this function
		// The variables carrying the result were different for each function,
		//and I reduced it to one so that there could be no confusion.
	case 14:/* * ->for results or equal */
		if (add_calc == 1)
		{
			if (num != 0)
			{
				num = num & 0x7FFFFFFF;
			}
			result = (num_temp + num);//add and print result
			send_result_SSD(result);
		}
		if (sub_calc == 1 && num != 0)
		{
			num = num + pow(2, 31);//same as num = num & 0x7FFFFFFF
			result = num_temp - num; //sub and print result
			send_result_SSD(result);
		}
		if (mul_calc == 1 && num != 0)
		{
			num = num & 0x7FFFFFFF;
			result = num_temp * num; //mul and print result
			if (result > 9999)
			{//if we see a number too large to see on the SSD, print ouFL.
				for (int i = 0; i < 3000; i++)
				{ //ouFL
					D1_digit_with_delay(16);
					D2_digit_with_delay(17);
					D3_digit_with_delay(18);
					D4_digit_with_delay(19);
				}
				clearSSD();
			}
			send_result_SSD(result); // if result<9999 go print func
		}
		if (div_calc == 1 && num != 0)
		{
			num = num & 0x7FFFFFFF;
			float result_temp=0.0;
			result_temp = num_temp / num;
			for (int i = 0; i < 2000; i++) // press the screen to see if it's not bounce.
			{
				D2_digit_with_delay(36);//b
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(19);//L
			}
			if (num == 0)
			{//print error if undefined operation is entered
				for (int i = 0; i < 3000; i++)
				{ // print eror
					D1_digit_with_delay(20);//e
					D2_digit_with_delay(21);//r
					D3_digit_with_delay(16);//0
					D4_digit_with_delay(21);//r
				}
				clearSSD();
			}
			else
			{
				dot_calc(result);//go print func
			}
		}
		else if (num == 0)
		{//I wrote it because I felt I had to define it again. He could miss it sometimes,
		//but he doesn't miss it anymore. It could be Bounce.
			for (int i = 0; i < 3000; i++)
			{ //eror
				D1_digit_with_delay(20);//e
				D2_digit_with_delay(21);//r
				D3_digit_with_delay(16);//o
				D4_digit_with_delay(21);//r
			}
			clearSSD();
		}

		if (sci_mode_log10 == 1) // log10 base calc
		{
			trigonometric_result = log10(num); //
			result = trigonometric_result * 1000;
			dot_calc(result); //Send to SSD and make point calculation screen print
		}
		if (sci_mode_ln == 1) //ln calc
		{
			//num = num & 0x7FFFFFFF;
			trigonometric_result = log(num);
			result = trigonometric_result * 1000;
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		if (sci_mode_root == 1) //square_root calc
		{
			trigonometric_result = sqrt(num);
			result = trigonometric_result * 1000;
			if (num < 0)
			{//we dont have this complex number therefore incalid
				for (int i = 0; i < 6000; i++)
				{ //invalid
					D1_digit_with_delay(22);//ı
					D2_digit_with_delay(23);//v
					D3_digit_with_delay(24);//l
					D4_digit_with_delay(25);//d
				}
				clearSSD();
			}
			else
			{
				dot_calc(result);//Send to SSD and make point calculation screen print
			}
		}
		if (sci_mode_square == 1) //pow func
		{
			result = pow(num, 2);
			send_result_SSD(result); //not a decimal number
		}
		if (trigonometric_calc_sin == 1)//sinus calc
		{
			trigonometric_result = sin(num);
			result = trigonometric_result * 1000;
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		if (trigonometric_calc_cos == 1)//cosinus calc
		{
			trigonometric_result = cos(num);
			result = trigonometric_result * 1000;
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		if (trigonometric_calc_tan == 1)//tanjant calc
		{
			trigonometric_result = tan(num);
			result = trigonometric_result * 1000;
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		if (trigonometric_calc_cot == 1) // "math.h" not have a cot(num) we can use 1/tan
		{
			trigonometric_result = 1 / tan(num);
			result = trigonometric_result * 1000;
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		break;

	/* # -> scientific - if pressed three times it will show the num of pi */
	case 15:
		if (trigonometric_calc == 1) //trigonometric calc, default value is -1
		{
			result = 3141; // pi=3.141
			dot_calc(result);//Send to SSD and make point calculation screen print
		}
		sci_mode++;
		trigonometric_calc ++;
		break;

	case 16: //o
		GPIOA->ODR &= ~(1U << 4);
		GPIOA->ODR &= ~(1U << 5);
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 6);
		break;

	case 17: //u
		GPIOA->ODR &= ~(1U << 4);
		GPIOA->ODR &= ~(1U << 5);
		GPIOA->ODR &= ~(1U << 12);
		break;

	case 18: //F
		GPIOA->ODR &= ~(1U << 0);
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 11);
		GPIOA->ODR &= ~(1U << 6);
		break;

	case 19: //L
		GPIOA->ODR &= ~(1U << 5);
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 11);
		break;
	case 20: //E
		GPIOA->ODR &= ~(1U << 0);
		GPIOA->ODR &= ~(1U << 5);
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 11);
		GPIOA->ODR &= ~(1U << 6);
		break;

	case 21: //r
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 6);
		break;
	case 22:// I
		GPIOA->ODR &= ~(1U << 1); //PA1 B
		GPIOA->ODR &= ~(1U << 4); //PA4 C
		break;

	case 23:// n
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		break;

	case 24:// v
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA3 D
		break;

	case 25:// d
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA3 D
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		break;

	case 26: /* - negative */
		D1_digit(); //only first digit
		GPIOA->ODR &= ~(1U << 6); //PA6 G
		delay(500);
		break;

	case 27: //dot
		GPIOA->ODR &= ~(1U << 7); //PA7 dot
		delay(500);
		break;

	case 28: //dot enable
		GPIOA->MODER &= ~(3U << 2 * 7);
		GPIOA->MODER |= (1U << 2 * 7);
		break;

	case 29: //a
		GPIOA->ODR &= ~(1U << 0);
		GPIOA->ODR &= ~(1U << 1);
		GPIOA->ODR &= ~(1U << 4);
		GPIOA->ODR &= ~(1U << 5);
		GPIOA->ODR &= ~(1U << 12);
		GPIOA->ODR &= ~(1U << 6);
		break;
	case 30://s
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 31://c
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 32://p
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 33://g
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 34://t
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 35://q
		GPIOA->ODR &= ~(1U << 0);  //PA0 A
		GPIOA->ODR &= ~(1U << 1);  //PA1 B
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
		break;
	case 36: //b
		GPIOA->ODR &= ~(1U << 4);  //PA4 C
		GPIOA->ODR &= ~(1U << 5);  //PA5 D
		GPIOA->ODR &= ~(1U << 12); //PA12 E
		GPIOA->ODR &= ~(1U << 11); //PA11 F
		GPIOA->ODR &= ~(1U << 6);  //PA6 G
	}
}

void send_result_SSD(uint32_t temp) //DIVIDE IT INTO DIGITS AND SEND IT TO THE FUNCTION ABOVE AND PRINT IT ON THE SCREEN
{
	if (temp < 0)
	{
		temp = temp * (-1); //for negative result
		int D1 = temp / 1000;
		int D2 = (temp / 100) % 10;
		int D3 = (temp / 10) % 10;
		int D4 = temp % 10;
		for (int i = 0; i < 6400; i++)
		{
			setSSD(26);
			D2_digit_with_delay(D1);
			D3_digit_with_delay(D2);
			D4_digit_with_delay(D3);
		}
		school_id(); //IDLE STATE
	}
	else
	{
		int D1 = temp / 1000;
		int D2 = (temp / 100) % 10;
		int D3 = (temp / 10) % 10;
		int D4 = temp % 10;
		for (int i = 0; i < 6400; i++)
		{
			D1_digit_with_delay(D1);
			D2_digit_with_delay(D2);
			D3_digit_with_delay(D3);
			D4_digit_with_delay(D4);
		}
		school_id();
	}
}

void dot_calc(int temp)// divide it into digits by incoming number and decide which Digit the point will be
{//and click this number on the screen
	if(temp<0)
	{

		temp = temp * (-1);//for negative result
		int D1 = temp / 1000;
		int D2 = (temp / 100) % 10;
		int D3 = (temp / 10) % 10;
		int D4 = temp % 10;
		if(temp>0 && temp<10000)
		{
			for(int i=0; i<6000; ++i)
			{
				D2_digit();
				setSSD(28);
				setSSD(26);
				setSSD(24);
				D2_digit_with_delay(D1);
				D3_digit_with_delay(D2);
				D4_digit_with_delay(D3);
			}
		}
		else if(temp >= 10000 && temp <= 100000)
		{
			for(int i=0; i<6000; ++i)
			{
				D3_digit();
				setSSD(28);
				setSSD(25);
				setSSD(26);
				D2_digit_with_delay(D1);
				D3_digit_with_delay(D2);
				D4_digit_with_delay(D3);
			}
		}
		school_id();
	}
	 else{
		 int D1 = temp / 1000;
		 int D2 = (temp / 100) % 10;
		 int D3 = (temp / 10) % 10;
		 int D4 = temp % 10;
		 if(temp > 0 && temp < 10000){
			 for(int i=0; i<6000; ++i){
				 D1_digit();
				 setSSD(28);
				 setSSD(25);
				 D1_digit_with_delay(D1);
				 D2_digit_with_delay(D2);
				 D3_digit_with_delay(D3);
				 D4_digit_with_delay(D4);
			 }
		 }
		 else if(temp >= 10000 && temp <= 100000){
			 for(int i=0; i<6000; ++i){
				 D2_digit();
				 setSSD(28);
				 setSSD(25);
				 D1_digit_with_delay(D1);
				 D2_digit_with_delay(D2);
				 D3_digit_with_delay(D3);
				 D4_digit_with_delay(D4);
			 }
		 }
		 else if(temp >= 10000 && temp <= 100000)
		 {
			 for(int i=0; i<6500; ++i)
			 {
				 D3_digit();
				 setSSD(28);
				 setSSD(25);
				 D1_digit_with_delay(D1);
				 D2_digit_with_delay(D2);
				 D3_digit_with_delay(D3);
				 D4_digit_with_delay(D4);
			 }
		 }
		 school_id();
	}
}

void clear_Rows_Keypad(void)
{
	GPIOA->ODR &= ~(1U << 8); //PA8
	GPIOB->ODR &= ~(1U << 9); //PB9
	GPIOB->ODR &= ~(1U << 5); //PB5
	GPIOB->ODR &= ~(1U << 4); //PB4
}

void set_Rows_Keypad(void)
{
	GPIOA->ODR |= (1U << 8); //PA8
	GPIOB->ODR |= (1U << 9); //PB9
	GPIOB->ODR |= (1U << 5); //PB5
	GPIOB->ODR |= (1U << 4); //PB4
}

