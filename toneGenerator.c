#include "toneGenerator.h"


ParallelPort portDevice = initParallelPort(PORT);

void startToneGenerator(ToneGenerator *self,int unused){
	self -> period = USEC(2145);
	//self -> deadline = USEC(100);
	PARALLEL_PORT_INIT(&portDevice,(int)(self -> direction));
	ASYNC(self,generateTone,0);
}  //public function

void generateTone(ToneGenerator *self,int unused){
	(self -> flip)=(self -> flip)^0x01;
	PARALLEL_PORT_WRITE(&portDevice,(int)(self -> flip));
	//SEND(self -> period,self -> deadline,self,generateTone,0);
	AFTER(self -> period,self,generateTone,0);
} //private function

void setTone(ToneGenerator *self,int newTone){
	self -> period = USEC(newTone);
}	//public function



