#include "TinyTimber.h"
#include "parallelportTinyTimber.h"

#include <machine/hcs12.h>

#define DATA_OUT 0xFF //write to PortP's DDRP to set PortP as output
#define BACKGROUND_LOOP_RANGE 500

typedef struct{
	Object super;
	unsigned char direction;
	unsigned char flip;
	Time period;
} ToneGenerator;

#define initToneGenerator() {initObject(),DATA_OUT,0x00,0}

ToneGenerator toneGenerate = initToneGenerator();

ParallelPort portDevice = initParallelPort(PORT,&toneGenerate);


void generateTone(ToneGenerator *,int);
void initDevice(ToneGenerator *self,int unused){
	self -> period = USEC(500);
	PARALLEL_PORT_INIT(&portDevice,(int)self -> direction);
	generateTone(self,0);
}

void generateTone(ToneGenerator *self,int unused){
	~(self -> flip);
	PARALLEL_PORT_WRITE(&portDevice,(int)self -> flip);
	SEND(self -> period,0,self,generateTone,0);
}

//TODO: pack the generator as one class

//create background task
typedef struct{
	Object obj;
	float number;
	float result;
	Time period;
} Multiplier;

#define initMultiplier() {initObject(),1.5,1.0,0}

Multiplier backGround = initMultiplier();

void multiply(Multiplier *,int);
void setPeriod(Multiplier *self, int unused){
	self -> period = USEC(1300);
	multiply(self,0);
}
void multiply(Multiplier *self,int unused){
	int count=0;
	float tmp;
	self -> result = 1.0;	
	
	do{
	tmp = self -> result;
	tmp += self -> number;
	self -> result = tmp;
	count++;
	}while(count<BACKGROUND_LOOP_RANGE);
	
	SEND(self -> period,0,self,multiply,0);
}	
	

//kick off two tasks: background task and tone generator
typedef struct{
	Object object;
} StartApp;

StartApp app = {initObject()};

void kickoff (StartApp *obj,int unused){
	ASYNC(&toneGenerate,initDevice,0);
	ASYNC(&backGround,setPeriod,0);
}


int main(){
	TINYTIMBER(&app,kickoff,0);
	return 0;
}

