#ifndef SONG_H
#define SONG_H

#include "TinyTimber.h"


#define INIT_PITCHSET  {4050,3822,3608,3405,3214,3034, \
            2863,2703,2551,2408,2273,2145,	\
			2025,1911,1804,1703,1607,1517,	\
			1432,1351,1276,1204,1136,1073,	\
			1012}  //us
				  
#define INIT_LENGTHSET {500,1000,250} //a=500ms,b=1000ms,c=250ms

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
  int pitchSet[25];  
  int lengthSet[3]; 
  int brotherJohnPitch[SONG_LENGTH];
  int brotherJohnLength[SONG_LENGTH];
}SongInfo,*PSongInfo;

SongInfo thissong = {INIT_PITCHSET,INIT_LENGTHSET,
			INIT_BROTHERJOHN_PITCH,
			INIT_BROTHERJOHN_LENGTH};
PSongInfo  brotherJohn = &thissong;

typedef struct{ 
	struct Tone *nextTone;
	int pitch;
	int length;	
} Tone,*PTone;

#define LOAD_TONE(song,key)       SYNC(song,load_tone,key)
#define POP_TONE(song,preTone)        SYNC(song,pop_tone,preTone)

void load_tone(Tone* ,int);
PTone display_tone(Tone*,PTone);

#endif

