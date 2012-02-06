#ifndef INSTRUMENT_INFO_H
#define INSTRUMENT_INFO_H

#define INIT_PITCHSET  {4050,3822,3608,3405,3214,3034, \
            2863,2703,2551,2408,2273,2145,	\
			2025,1911,1804,1703,1607,1517,	\
			1432,1351,1276,1204,1136,1073,	\
			1012}  //us
				  
#define INIT_LENGTHSET {500,1000,250} //a=500ms,b=1000ms,c=250ms

typedef struct{
  int pitchSet[25];  
  int lengthSet[3]; 
} InstrumentInfo,*PInstrumentInfo;

#define initInstrumentInfo() {INIT_PITCHSET,INIT_LENGTHSET}

#endif


