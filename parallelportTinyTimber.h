#ifndef PARALLEL_PORT_TINYT_H
#define PARALLEL_PORT_TINYT_H

#include "TinyTimber.h"
#include <machine/hcs12.h>
#include <machine/hcs12/pim.h>

typedef struct{
	Object super;
	PPIM devicePort;
	//Object *obj;
	//Method *meth;   is it necessary here? no? 
} ParallelPort;

#define initParallelPort(port)  {initObject(),port};
	   
#define PORT (PPIM)(PTT)  //PTT=0x0240;

	   
#define PARALLEL_PORT_INIT(port,data) SYNC(port,parallel_port_init,data)
#define PARALLEL_PORT_WRITE(port,output)	SYNC(port,parallel_port_write,output)

void parallel_port_init(ParallelPort* port,int dataDirection);
void parallel_port_write(ParallelPort* port,int ch);

#endif
