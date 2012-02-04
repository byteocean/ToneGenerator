#include "TinyTimber.h"
#include "parallelportTinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"

#include <machine/hcs12.h>

#define DATA_OUT 0xFF //write to PortP's DDRP to set PortP as output

typedef struct{
	Object super;
	unsigned char direction;
	unsigned char flip;
	Time period;
	Time deadline;	
} ToneGenerator;

#define initToneGenerator() {initObject(),DATA_OUT,0x00,0,0}

ToneGenerator toneGenerate = initToneGenerator();

ParallelPort portDevice = initParallelPort(PORT);


void generateTone(ToneGenerator *,int);
void initDevice(ToneGenerator *self,int unused){
	self -> period = USEC(500);
	self -> deadline = USEC(100);
	PARALLEL_PORT_INIT(&portDevice,(int)(self -> direction));
	ASYNC(self,generateTone,0);
}

void generateTone(ToneGenerator *self,int unused){
	(self -> flip)=(self -> flip)^0x01;;
	PARALLEL_PORT_WRITE(&portDevice,(int)(self -> flip));
	SEND(self -> period,self -> deadline,self,generateTone,0);
	//AFTER(self -> period,self,generateTone,0);
}

//TODO: pack the generator as one class

//create background task
typedef struct{
	Object obj;
	int number;
	int result;
      //int loopRange;
	Time period;
	Time deadline;
} Multiplier;

#define initMultiplier() {initObject(),2,1,0,0}

Multiplier backGround = initMultiplier();

void multiply(Multiplier *,int);
void setPeriod(Multiplier *self, int unused){
	self -> period = USEC(1300);
	self -> deadline = USEC(1300);
	ASYNC(self,multiply,0);
}
void multiply(Multiplier *self,int unused){
	int count=0;
	int tmp=2;
	//self -> result = 1.0;	
	
	while(count<loopRange){
	//tmp = self -> result;
	//tmp += self -> number;
	//(self -> result) = tmp;
	count++;
	}
	
	SEND(self -> period,self -> deadline,self,multiply,0);
	//AFTER(self -> period,self,multiply,0);
}	
	

//kick off two tasks: background task and tone generator
typedef struct{
	Object object;
	int loopRange;
} StartApp;

StartApp app = {initObject(),0};

void reader(StartApp*, int);
void receiver(StartApp*, int);

Can can0 = initCan(CAN0BASE, &app, receiver);
Serial sci0 = initSerial(SCI_PORT0, &app, reader);

void receiver(StartApp *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
 
}

void reader(StartApp *self, int c) {
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");
    if(c=='i'){
	app.loopRange += 50;
    }else if(c=='d'){
	if (app.loopRange > 0){
		app.loopRange -=50;
	}
    }
}

void kickoff (StartApp *obj,int unused){
	SCI_INIT(&sci0);
	CAN_INIT(&can0);
	ASYNC(&toneGenerate,initDevice,0);
	ASYNC(&backGround,setPeriod,0);
}


int main(){
	INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
	INSTALL(&can0, can_interrupt, CAN_IRQ0);
	TINYTIMBER(&app,kickoff,0);
	return 0;
}

