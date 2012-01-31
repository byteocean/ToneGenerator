#include "TinyTimber.h"
#include "parallelportTinyTimber.h"

void parallel_port_init(ParallelPort* port,int dataDirection){
	port -> devicePort -> ddrp =(unsigned char) dataDirection; //set Port P as output
}

void parallel_port_write(ParallelPort* port,int ch){
	port -> devicePort -> ptp = (unsigned char)ch; 
}
