# Fully_operational_scientific_calculator (STM32 Nucleo g031K8Tx)

### OBJECTİVE
Main objective of this project/midterm is to create a fully operational sclentific calculator in C. This calculator will have a keypad connected to enter the numbers and execute basic scientific and trigonometric functions. A 4-digit seven segment display should be used to display these numbers.

### COMPONENT LIST
![image](https://user-images.githubusercontent.com/70964563/152973459-0e60d394-2ec6-438a-a06e-5b22169b077d.png)


#### FLOWCHART

Check .drawio doc


---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------


### TASK 1.
#### 1.1. DISPLAYING THE KEY PRESSED IN THE KEYPAD ON THE SSD
##### 1.1.1. Theoretical Research
Keyboard operating principle investigated and connection diagram learned. SSD working principle investigated and wiring diagram learned. It was learned that LEDs in the SSD must be protected by adding resistance to the circuit. The method of connecting the Keypad and SSD was investigated and how to communicate with each other thanks to STM32. In this way, the first task was successfully carried out.

##### 1.1.2. Keypad Operating Principle

![image](https://user-images.githubusercontent.com/70964563/152973635-1baf5f5d-21b5-420c-8b41-5488f7d2f9ab.png)

The Keypad connection pins are shown in the figure above. Considering that there are 16 different buttons here and there should be 1 input and 1 output for each button, there should be 32 ends, but a different logic has been followed to save mapping and space. According to this logic, when PB4 pin is given 5V, PB8 pin will get 5V. In this way, the PB8 pin will be selected as input and voltage transfer will be provided to the SSD and the number 1 will be read on the SSD. It should be noted that when the PB8 pin receives a value of 5V, it receives a voltage of 5V from which line. To distinguish this, the PB5-PB9 and PA8 pins are activated and checked whether the voltage at this 5V value actually comes from the PB4 pin. The example given here applies to the number 1, and the same logic applies to each button on the keypad.

##### 1.1.3. SSD Operating Principle

![image](https://user-images.githubusercontent.com/70964563/152973724-e8d0df5d-f191-4297-af41-80a565524cce.png)
![image](https://user-images.githubusercontent.com/70964563/152973750-872f9ba5-9369-4a42-b023-9b07adb79140.png)

As can be seen from Figure 2, according to the internal SSD connection diagram, each pin is connected to the same pin of the other digit. In this case, a-B-C-D-e-F-g LEDs must be sent with a value of 0 to light up. As shown in Figure 3, when the “PA0” Switch is turned off, only the “A” LEDi will be active on the common anode SSD.  Other LEDs will work according to the same logic. Again, the desired letters and numbers were obtained by assigning a binary or hex value, as shown in Figure 3.

##### 1.1.4. SSD Communication With Keypad
![image](https://user-images.githubusercontent.com/70964563/152973886-476bac07-e8ca-4902-b531-28de05f4690f.png)

According to the above connection diagram, keypad and SSD communication is provided thanks to the Nucleo Board. An encoding has been made that allows the value printed on the Keypad to be displayed on the SSD. The main logic of our code is that when an interrupt is created via the keypad, it is directed to the function that will activate the LEDs on the SSD in the interrupt_handler() functions. In this way, the value printed on the keypad is shown on the SSD screen. As can be seen from Figure 4, 5V was given to D1-D2-D3-D4 pins at this stage. Then the connections of these pins will be changed to perform the desired operation.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------


### TASK 2

#### 2.1. WRITING THE SCHOOL NUMBER ON THE SSD
##### 2.1.1. Theoretical Research

The keyboard and SSD operating principle was learned, and now that one pin of each digit is connected to the same pin of the other digit according to the internal connection of the SSD, research was conducted and an idea of how this process can be performed.

##### 2.1.2.Keypad Operating Principle
At this stage, the keypad is not used.

##### 2.1.3. SSD Operating Principle
I found that since one pin of each digit is connected to the same pin of the other digit according to the internal SSD connection(bkz:Figure 6) therefore, the following theory has been applied.

##### 2.1.4. School İD On The SSD
![image](https://user-images.githubusercontent.com/70964563/152974381-6caa18d4-1c8f-4aae-88fb-1cc32faca81e.png)
** NOTE: In the remaining cases, SSD connections will not be changed. You can see it most clearly here. Already only D1,D2,D3 and D4 pin connections have been changed for SSD.

I can only do this in a loop and with help from the delay function. In this way, only one digit is activated at a time and the others are inactive and the desired value is written in the form of work. In this way, the first two digits and the last two digits of my school number are displayed on the SSD screen. In addition,now the D1,D2, D3 and D4 pins are designed to activate only a single digit and will no longer receive the 5V voltage directly from the PB6,PB7,PB1 and PB3 pins, respectively. My reason for doing this is to activate only the B pins and make a shorter MODER assignment.

![image](https://user-images.githubusercontent.com/70964563/152974504-a428de52-0fbf-4e46-b1f5-d77d43eb4199.png)


---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------


### TASK 3

#### 3.1. PERFORMING FOUR OPERATIONS ON THE CALCULATOR
##### 3.1.1. İNTRO
So far, now that we can provide SSD connections with keypad, we can move on to the calculator section. For this part, only collection, subseation, multiplication and division operations will be explained.

##### 3.1.2. Theoretical Research
Examples on the calculator working logic and C code have been checked.

##### 3.1.3. Calculator Working Principle
Each number written in the calculator is assigned to a variable and shows us the result as soon as we perform the mathematical operation in the function cycle and press the equals key. Here we should note that for the SSD, each digit must have the same pin connected to another, and there must be a bouncing problem on the keypad. I wanted to get rid of this situation as an abbreviation of the process performed on the SSD for each process. In this case, I could catch it in the case it bounces in the function, and it would improve the quality of our calculator a little bit. 

So I did this procedure. Our main logic here is to write the desired value or letter of that Digit, which only one digit is activated in a loop and other digits are closed and left open. A similar 2. I did it for my school number in the part, but here I will use letters instead of numbers. In other words, the screen output according to the briefly pressed keys is as follows.

A --> add  	B --> Sub	   C --> carP	   D --> boL   

In this case, every operation will be seen and we will have a manual gard in case it goes into an undesirable situation. Undefined operations are specified on the SSD.

##### 3.1.4. Experiment and Notes
During this experiment, each pressed button was assigned to a variable. In this case, each key pressed will go to the corresponding function, write the abbreviation of the function it goes to on the SSD, and thus measures will be taken against any bouncing. In fact, it has a flat logic in the form of make the operation and press the result screen. When pressing the screen, of course, according to the working principle of the SSD described above, it should be written by breaking down into steps. Thanks to the operations performed here, four parts of the operation of our calculator were realized. You can find the necessary descriptions for each process and line in my video and code file.

![image](https://user-images.githubusercontent.com/70964563/152974806-1a0ae384-54b6-4319-91ee-c21ad3bfca1e.png)

![image](https://user-images.githubusercontent.com/70964563/152974853-ffa7088b-132d-4b6f-add8-096479fa45fc.png)

#### ISSUES TASK3
**PROBLEMS ENCOUNTERED
I thought the error was caused by there because the number I assigned to print or process on the screen lost its value when going to another function, and because I first started writing code with TIM1. For example, a variable that takes the value 5 when assigning loses its number when assigning it to another temporary variable, and -214287.... It has a long digit value in its form. To prevent this, with the help of our teacher, this problem was eliminated by defining 
“num = num & 0x7FFFFFFF;      or 	 num = num + pow(2, 31);”.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------


### TASK 4
#### 4.1 SCIENTIFIC CALCULATOR
##### 4.1.1 EXPERİMENT
In this part, we now have information about how each element we use in our circuit works. Each operation performed for the scientific calculator must also be assigned to a variable. But the “#” function used here is very important. If it bounces, it can advance to the trigonometric part or suppress the number pi and finish the function. Bouncing is therefore important for"#". Each operation to remove bouncing in this function and handle it in a controlled manner is written on the SSD screen with an abbreviation code.


EA -->log --> SLog	EB --> ln --> Sln	   EC --> sqrt --> Srot	    ED --> x^2 --> SSqu

As seen above, each time we come to this function, we can get this output on the SSD connected to the calculator and check that we are in the right part. My first goal of doing this is both to improve quality and to take a manual measure against bouncing. Circuit connections have not been changed, but they work as a connection that you can see below. The operations performed in this part were used entirely from the “math.h” library. Undefined operations are specified on the SSD.

![image](https://user-images.githubusercontent.com/70964563/152975257-eb9542bd-6b1c-4994-b2d8-d5050a161c8b.png)

#### ISSUES TASK4
**PROBLEMS ENCOUNTERED
** I working with TIM1 has been discontinued due to the problem I mentioned above. Because it was thought to be a problem caused by Tim1. But another error was encountered that did not exist when there was TIM1. If the “breakpoint” assignment cannot be made for the operation we want to do in this part, no output can be read on the SSD screen. The cause of this remains unsolved. For this reason, as can be seen from my videos, I must constantly continue my code with “resume”. I guess I shouldn't have stopped working with TIM1 or backed up that work. This problem has not been solved.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------


### TASK 5
#### 5.1  TRİGONOMETRİC AND SCIENTIFIC CALCULATOR

##### 5.1.1 EXPERİMENT
The operations performed in this part were used entirely from the “math.h” library.  Again, as above, an abbreviation will be displayed on the screen for each operation, and then it will enter the function. The reason I left the operations in this Section at the end is because I can't print a decimal number even for a more standard division operation. More than the operations performed in this part, it was aimed that the results could be read on the SSD screen and give an accurate result. Unfortunately, the above mentioned problem continues here and these functions cannot be executed without “brakpoint”. For this reason, the mathematical operations performed in this part also give correct results, but do not work correctly. Again, as always, to find out if the output bounces, the abbreviation of the operation is printed on the screen and then the variables to be processed are taken. 

EEA --> sin --> tsin	EEB --> cos --> tcos	 EEC --> tan --> ttan	   EED --> cot --> tcot

The above operations are shown on the screen before each variable is taken and briefly show which operation is performed.
The same circuit diagram was used in this problem. Only a change was made through the code and trigonometric functions were activated. Undefined operations are specified on the SSD.


![image](https://user-images.githubusercontent.com/70964563/152975655-0110f198-2588-44cc-b961-6eb0bd2f44d2.png)


---------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------








