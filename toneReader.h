#ifndef TONE_READER_H
#define TONE_READER_H

#include "song.h"

typedef struct{
	Object object;
	Tone *mySong;
	Tone *currentTone;
	int key;
} ToneReader;

#define initToneReader() {initObject(),NULL,NULL,0}

#define START_TONE_READER(self) ASYNC(self,startToneReader,0)
#define READ_TONE(self)         ASYNC(self,readTone,0)

void startToneReader(ToneReader *self,int unused);
void readTone(ToneReader *self,int unused);

#endif


