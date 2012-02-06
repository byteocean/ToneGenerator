#ifndef SONG_H
#define SONG_H

#include "TinyTimber.h"

#define INIT_BROTHERJOHN_PITCH {0,2,4,0,0,2, \
			4,0,4,5,7,4,	\
			5,7,7,9,7,5,	\
			4,0,7,9,7,5,	\
			4,0,0,-5,0,0,	\
			-5,0}
						  
#define INIT_BROTHERJOHN_LENGTH {0,0,0,0,0,0,	\
			0,0,0,0,1,0,	\
			0,1,2,2,2,2,	\
			0,0,2,2,2,2,	\
			0,0,0,0,1,0,	\
			0,1}
						   
#define SONG_LENGTH 31

typedef struct{
  int brotherJohnPitch[SONG_LENGTH];
  int brotherJohnLength[SONG_LENGTH];
}SongInfo,*PSongInfo;



typedef struct{
	struct Tone *nextTone;
	int pitchID;
	int lengthID;	
} Tone,*PTone;

#define initTone() {NULL,0,0}

#define LOAD_TONE(song)       SYNC(song,load_tone,0)
//#define POP_NEXT_TONE(song,preTone)        SYNC(song,pop_next_tone,preTone)

void load_tone(Tone* ,int unused);
Tone *pop_next_tone(Tone*,Tone*);

#endif



