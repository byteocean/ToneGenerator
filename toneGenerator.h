#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

#include <machine/hcs12.h>
#include "TinyTimber.h"
#include "parallelportTinyTimber.h"

#define DATA_OUT 0xFF //write to PortP's DDRP to set PortP as output

typedef struct{
	Object super;
	unsigned char direction;
	unsigned char flip;
	Time period;
	//Time deadline;	
} ToneGenerator;

#define initToneGenerator() {initObject(),DATA_OUT,0x00,0}

#define START_TONE_GENERATOR(self) 	ASYNC(self,startToneGenerator,0)
#define SET_TONE(self,newTone)      ASYNC(self,setTone,newTone)

void generateTone(ToneGenerator *self,int unused);
void startToneGenerator(ToneGenerator *self,int unused);
void setTone(ToneGenerator *self,int newTone);

#endif

