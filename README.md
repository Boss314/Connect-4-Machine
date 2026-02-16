# Connect-4-Machine
## About the Project
The Connect 4 Machine is a device capable of playing the popular board game Connect 4 against a human player. It was designed to provide a fun challenge by leveraging the MSP432's hardware and software's capabilities.

The Connect 4 Machine is able to detect where a player places a piece with sensors located at the top of every column. It calculates an optimal move using an algorithm. And it communicates with the player via the MSP BoosterPack's liquid crystal display. 

## Requirements
### Hardware Requirements
- Texas Instruments MSP432P401
- Educational BoosterPack MKII
- x7 [TCRT5000 Infrared Reflective Sensor](https://www.vishay.com/docs/83760/tcrt5000.pdf)
- Connect 4 board (10x20cm)
- Plastic structure around the board
- Jumper wires
- Breadboard Power rail

### Software Requirements
- Code Composer Studio IDE
- Texas Instruments MSP432 DriverLib Library 

## Building Instructions
### Assembling
- Affix the sensors on the top of the board's columns using the plastic structure
- Connect the VCC and GND pins of the sensors to the power rail and connect the power rail to the 5V and GND pins of the MSP432
- Connect the digital output pin of each sensor to the pins designated in source/sensorsdriver.c

### Compilation and Burning
 - Copy the include/ and source/ directories in a CCS project for the MSP432P401R
 - Compile and flash the code onto the microcontroller

## User's Guide
Upon startup, the connect 4 board should be empty. When it is the player's turn to play, they may place a piece in any column on the board. When it is the computer's turn to play, the program will calculate its move and the player will have to physically place a piece in the corresponding column. The system will prompt the player to make the computer's move via the LCD. 

The system will alert the player when they have won or lost the game, as well as if the game has entered a tie, with the LCD.

## Project Layout
	Connect-4-Machine
	├── include/						# C header files
	|   ├── board.h
	|   ├── connect4algorithm.h
	|   ├── displaycontroller.h
	|   ├── sensorsdriver.h
	|   └── types.h
	├── source/							# C source files
	|   ├── board.c
	|   ├── connect4algorithm.c
	|   ├── displaycontroller.c
	|   ├── globals.c
	|   ├── main.c
	|   └── sensorsdriver.c
	└── README.md
	
## Video and Presentation
- [Pitch Video](https://www.youtube.com/watch?v=mvRkMqnd31U)
- [Presentation](https://docs.google.com/presentation/d/1qTY8_llUPCq2M-b6enmNlHhAITYZlf7NaWkmQ0FVLDs/edit?usp=sharing)

## Team
- Enrico Faa: sensors control code, data structures
- Temuulen Javkhlanbaatar: connect 4 algorithm code, data structures
- Munkhdul Dorjderem: LCD display control code

