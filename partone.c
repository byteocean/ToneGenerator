#ifndef _MAIN_FUNCTION_
#define _MAIN_FUNCTION_

#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"

#include "instrumentInfo.h"
#include "toneReader.h"
#include "toneGenerator.h"

typedef struct{
	Object object;
} MusicPlayer;

MusicPlayer app = {initObject()};

ToneGenerator toneGenerate = initToneGenerator();
InstrumentInfo basicInfo = initInstrumentInfo();
ToneReader toneReader = initToneReader();


void reader(MusicPlayer*, int);
void receiver(MusicPlayer*, int);
Can can0 = initCan(CAN0BASE, &app, receiver);
Serial sci0 = initSerial(SCI_PORT0, &app, reader);

void executeMusicPlayer(MusicPlayer *,int );
void startMusicPlayer(MusicPlayer *self,int unused){
	SCI_INIT(&sci0);
	CAN_INIT(&can0);

	START_TONE_READER(&toneReader);
	ASYNC(self,executeMusicPlayer,0);
	START_TONE_GENERATOR(&toneGenerate);	
}

void executeMusicPlayer(MusicPlayer *self,int unused){
	 int pitch,pithchLength;
	 int pitchID,pitchLengthID;
	 
	 READ_TONE(&toneReader);
	 
	 pitchID = toneReader.mySong->pitchID;
	 
	 pitch = basicInfo.pitchSet[pitchID+10+toneReader.key];
	 toneGenerate.period = USEC(pitch);
	 
	 pitchLengthID = toneReader.mySong->lengthID;
	 pithchLength = basicInfo.lengthSet[pitchLengthID];
	 
	 AFTER(pithchLength,self,executeMusicPlayer,0);
}


void receiver(MusicPlayer *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
 
}

void reader(MusicPlayer *self, int c) {
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");
    if(c=='i'){
	if(toneReader.key<5)
	toneReader.key += 1;
    }else if(c=='d'){
	if (toneReader.key > (-1)){
		toneReader.key -=1;
	}
    }
}

#endif


int main(){
	INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
	INSTALL(&can0, can_interrupt, CAN_IRQ0);
	TINYTIMBER(&app,startMusicPlayer,0);
	return 0;
}




