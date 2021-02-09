/**********************************************************************
* © James D. Broesch, 2011, 2021
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/
#include <math.h> // This is important!
#include <xc.h>

#include "WM8510CodecDrv.h"
#include "sask.h"

// FRAME_SIZE - Size of each audio frame 	
				
#define FRAME_SIZE 				128
#define N_FRAMES				1
#define SIGNAL_SIZE 			FRAME_SIZE * N_FRAMES

// Allocate memory for buffers and drivers
// codecBuffer - Buffer used by the codec driver

// Used by the Codec Driver.
int 	codecBuffer[WM8510DRV_DRV_BUFFER_SIZE];

// This is our signal.
int 	signal[SIGNAL_SIZE];

// Not really used in this code. It is hear for later use.
int 	frameIndex;

// Points to the signal we wish to transmit.
int 	*pSignal;

//Instantiate the drivers structures
//and create handles.

WM8510Handle codec;
WM8510Handle *codecHandle = &codec; 

int main(void)
{
	// Define the variables for generating the output signal.
	float f = (float)12; // Number of cycles per frame.
	float T = (float)1/(float)SIGNAL_SIZE;
	float PI =  3.14159265;
	int n;		

	// Initialize the processor.
	DSCInit();					

	// Initialize the board and the drivers
	SASKInit();
	WM8510Init(codecHandle,codecBuffer);

	// Start Audio input and output function
	WM8510Start(codecHandle);
		
	// Configure codec for 8K operation	
	WM8510SampleRate8KConfig(codecHandle);

	// Generate the output waveform.
	for(n=0; n < SIGNAL_SIZE; n++)
		{
		signal[n] = sin(2*PI*f*(float)n*T) * 0x7FFF;
		}

	// Main processing loop. Executed for every input and
	// output frame. For this example, input frames are ignored.

	frameIndex = 0;
	pSignal = signal; // Point to the signal buffer.

	// For approximately 1 second output the signal.
	while(frameIndex++ < 62)
	{
			// Write the frame to the output
			WM8510Write(codecHandle, pSignal, FRAME_SIZE);

			// Wait till the codec is available for a new frame
			while(WM8510IsWriteBusy(codecHandle));														
	}
return(0); // Go home.
}
