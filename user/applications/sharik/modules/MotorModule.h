#ifndef __motor_module_h
#define __motor_module_h

#include "mcc.h"

//#define MODULE_VERSION 1
//char admin_str[] = "admin";


#define MOTOR_OPCODES 10
//#define OPCODE_REPORT 0
#define OPCODE_SERVO_ENABLE 1
#define OPCODE_SERVO_WRITE 2
#define OPCODE_SERVOS_WRITE 3
#define OPCODE_SERVO_WRITE_US 4
#define OPCODE_SERVOS_WRITE_US 5
#define OPCODE_SERVO_READ 6
#define OPCODE_SERVO_TRIM 7
#define OPCODE_SERVOS_TRIM 8
#define OPCODE_SERVOS_TRIM 8
#define OPCODE_DRIVE 9

class MotorModule
{
	int pid;
	OpcodeCallback opcode_callbacks[MOTOR_OPCODES];
  public:
	MotorModule ();
	int init();
};


#endif
