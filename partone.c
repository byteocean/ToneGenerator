#include "TinyTimber.h"
#include "parallelportTinyTimber.h"

#include <machine/hcs12.h>

#define DATA_OUT 0xFF //write to PortP's DDRP to set PortP as output

typedef struct{
	Object super;
	char direction;
	char flip;
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
	SEND(self->period,0,self,generateTone,0);
}

int main(){
	TINYTIMBER(&toneGenerate,initDevice,0);
	return 0;
}

