#ifndef CAN_TINYT_H
#define CAN_TINYT_H

#include "TinyTimber.h"

typedef unsigned char uchar;

typedef struct {
  uchar msgId;  //Valid values: 0-127
  uchar nodeId; //Valid values: 0-15
  uchar length;
  uchar buff[8];
} CANMsg;

extern struct tagCAN;

#define CAN_BUFSIZE 8

typedef struct {
  Object  super; 
  struct tagCAN *port;
  Object  *obj;
  Method  *meth;
  char    tail;
  char    head;
  char    count;
  CANMsg  iBuff[CAN_BUFSIZE];
} Can;

#define initCan(port, obj, meth)  { initObject(), port, (Object*)obj, (Method)meth, 0, 0, 0}

#define CAN_PORT0   (struct tagCAN *)CAN0BASE
#define CAN_PORT1   (struct tagCAN *)CAN1BASE

#define CAN_IRQ0    IRQ_MSCAN0Rx
#define CAN_IRQ1    IRQ_MSCAN1Rx

#define CAN_INIT(can)               SYNC(can, can_init, 0)
#define CAN_RECEIVE(can, msgptr)    SYNC(can, can_receive, msgptr)
#define CAN_SEND(can, msgptr)       SYNC(can, can_send, msgptr)

void can_init(Can *obj, int unused);
int can_receive(Can *self, CANMsg *msg);
int can_send(Can *self, CANMsg *msg);

void can_interrupt(Can *self, int unused);

#endif
