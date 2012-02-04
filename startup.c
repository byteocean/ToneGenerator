/*
	startup.c
	startup for applications in Flash memory
	2011-11-11 RoJ
	Application is stand alone
*/
/* CONTROLLER SPECIFIC DEFINITIONS (DG256) */
#define	CORE_BASE	0
#define	CRG_BASE	0x34
/* PLL 24 MHz bus */
#define	REFDVVal	1
#define	SYNRVal		5

#define	TOP_OF_STACK	0x4000
#define	STACK_SIZE	256
#define	BOTTOM_OF_STACK	TOP_OF_STACK-STACK_SIZE

#include	<machine/hcs12/core.h>
#include	<machine/hcs12/crg.h>

/* **********************************************************************
 *	EXIT PROCEDURE
 *	Target for C-library functions that terminates a program
 * ******************************************************************** */
 void _lowexit( void )
 {
	/* Turn off timer hardware */
	( ((CRG*) (CRG_BASE))->crgint ) = 0;
	( ((CRG*) (CRG_BASE))->rtictl ) = 0;
	__asm( "	STOP" );	
 }

/* **********************************************************************
 *	CHIP INIT
 * ******************************************************************** */
void	__hwinit()
{
    /* Set default page for banked memory */
	( ((CORE*) (CORE_BASE))->ppage ) = 0x30;

	/* Set Register base, ram base and eeprom base */
	/* initrm = 0x39 means that RAM is aligned to 0x3FFF and downwards */
	( ((CORE*) (CORE_BASE))->initrg ) = 0;
	( ((CORE*) (CORE_BASE))->initrm ) = 0x39;
	( ((CORE*) (CORE_BASE))->initee ) = 0;

	/* Controller mode 
	* 11100011 (=0xE3)
	* ||||||||
	* |||||||+-EME-----PORTE and DDRE are removed from memory
	* ||||||+--EMK-----PORTK and DDRK are removed from memory
	* |||||+---Reserved
	* ||||+----IVIS----No visibility of internal ops on ext. bus
	* |||+-----Reserved
	* ||+------MODA--\
	* |+-------MODB----Normal Expanded Wide
	* +--------MODC--/
	*/
	( ((CORE*) (CORE_BASE))->mode ) = 0xE3;
	
	/* Pear register
	* 10100100 (=0xA4)
	* ||||||||
	* |||||||+-Reserved
	* ||||||+--Reserved
	* |||||+---RDWE----Read / Write (R/W) pin enabled
	* ||||+----LSTRE---Low Strobe (LSTRB) pin disabled
	* |||+-----NECLK---External E clock is enabled
	* ||+------PIPOE---IPIP0,1 indicate the instruction queue
	* |+-------Reserved
	* +--------NOACCE--No CPU free cycle visibility.
	*/
	( ((CORE*) (CORE_BASE))->pear ) = 0xA4;
	
	/* Misc register
	* 00001101 (=0x0D)
	* ||||||||
	* |||||||+-ROMON------- Turn on internal flash
	* ||||||+--ROMHM------- Enable $4000-$7FFF internal flash
	* |||||+---EXSTR0----\_ Set Stretch to three cycles
	* ||||+----EXSTR1----/
	* |||+-----Reserved
	* ||+------Reserved
	* |+-------Reserved
	* +--------Reserved
	*/
	( ((CORE*) (CORE_BASE))->misc ) = 0x0D;

	/* EBI Control
	* 00000001 (=0x01)
	* ||||||||
	* |||||||+-ESTR----E stretches for external access.
	*/
	( ((CORE*) (CORE_BASE))->ebictl ) = 0x01;
	
	/* PLL */
	( ((CRG*) (CRG_BASE))->refdv ) = REFDVVal;
	( ((CRG*) (CRG_BASE))->synr ) = SYNRVal;
	/* wait here till the PLL is locked. */
	while( (( ((CRG*) (CRG_BASE))->crgflg ) & 8 )== 0);	/* LOCK bit 00001000 */
	/* 	switch the bus clock to the PLL. */
	( ((CRG*) (CRG_BASE))->clksel ) = 0x80;		/* PLLSEL bit 10000000 */
	
}
 
  
/* **********************************************************************
	STANDARD STARTUP ROUTINE
*********************************************************************** */
/* Force following code to named segment "init" (see linker script) */
#pragma	TEXT	init
__interrupt void _start( void )
{
	__asm("	LDS	#%a", TOP_OF_STACK );	/* Get a valid stackpointer */
	__hwinit();			/* Chip iniz */
	/* BOTTOM_OF_STACK is the upper memory limit available for malloc */
	__crtInit( BOTTOM_OF_STACK );	/* Initialize C-library */
	main();				/* Call user application */
	exit(0);			/* if returning from 'main' */
}


#pragma	DATA	resetvector
// segment start address should be FF8C
__interrupt	void	(*resetvec[])() = 
{
	_start
};
#pragma	DATA	security_bits
static char security_bit_array[] = {0xFF,0xCF,0xFF,0xFE};
#pragma DATA	default



