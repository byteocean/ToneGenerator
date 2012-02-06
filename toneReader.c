#include "toneReader.h"

void startToneReader(ToneReader *self,int unused){
	LOAD_TONE(self -> mySong);
}

void readTone(ToneReader *self,int unused){
	self -> currentTone = pop_next_tone(self -> mySong,self -> currentTone);	
}


