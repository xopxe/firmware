#ifndef __task_admin_h
#define __task_admin_h

#include "mcc.h"

//#define MODULE_VERSION 1
//char admin_str[] = "admin";


#define ADMIN_OPCODES 3
//#define OPCODE_REPORT 0
#define OPCODE_PING 1
#define OPCODE_RESET 2


class AdminModule
{
	int pid;
	OpcodeCallback opcode_callbacks[ADMIN_OPCODES];
  public:
	static void report_status(const char * str, int len);
	AdminModule ();
};


#endif
