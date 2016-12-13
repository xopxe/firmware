/*
 * mcc.h
 *
 *  Created on: Oct 4, 2013
 *      Author: jvisca
 */

#ifndef MCC_H_
#define MCC_H_

#ifndef NULL
#define NULL 0
#endif

#include "application.h"
#include "EmBencode/EmBencode.h"

#define MAX_POLL_CALLBACKS 50
#define MAX_PIDS 50
#define MAX_PARAMS 10

#define ADMIN_PID (long int) 0
#define MOTOR_PID (long int) 1

#define OPCODE_REPORT (long int)0

#define MCC_PORT (unsigned int)55555

static const char ok_str[] = "OK";
static const char error_str[] = "ERROR";
static const char parseerror_str[] = "PARSEERROR";
static const char exeerror_str[] = "EXEERROR";

extern UDP Udp;

typedef void (*PollCallback) ();
typedef int (*OpcodeCallback) (int pid, int opcode);
typedef OpcodeCallback* OpcodeCallbacks;
struct OpcodesCallbackReg {
	OpcodeCallbacks callbacks;
	byte n_callbacks;
};
typedef OpcodesCallbackReg OpcodesCalbbackReg;


class MCC {
	PollCallback poll_callbacks[MAX_POLL_CALLBACKS];
	int n_poll_callbacks;

	OpcodesCallbackReg opcode_callbacks[MAX_PIDS];
	//uint8_t opcode_callbacks_count[MAX_PIDS]; //FIXME merge with previous as an array of structs.
	int n_opcode_callbacks;

    void process_incomming(char ch);
    void send_parse_error_message();
    void send_execution_error_message(int tpid, int opcode, int errcode);

	char packetBuffer[800];
	IPAddress bcastIP = IPAddress(255,255,255,255);

public:
	MCC();

	void startFrame();
	void endFrame();
	EmBencode encoder;

	char* incomming_params_s[MAX_PARAMS];
	int incomming_params_n[MAX_PARAMS];
	int incomming_params_count = 0;

	int register_poll_callback(PollCallback cb);
	void unregister_poll_callback(int);

	int register_opcode_callbacks(OpcodeCallback* opcode_callbacks, int opcodes_count);

    void send_message(int spid, int opcode, char const *data, int data_length);

	void tick();
};

#endif /* MCC_H_ */
