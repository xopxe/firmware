#include "AdminModule.h"

#include "EmBencode/EmBencode.h"

extern MCC mcc;


static int handle_report(int  pid, int  opcode) {
	//report_status(ok_str, sizeof(ok_str)-1);
	String myID = System.deviceID();
	String version = System.version();
	int freemem = System.freeMemory();

	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)ADMIN_PID);
	mcc.encoder.push(OPCODE_REPORT);
	mcc.encoder.startList();
	//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
	//mcc.encoder.push(MODULE_VERSION);
	mcc.encoder.push(ok_str, sizeof(ok_str)-1);
	mcc.encoder.push(myID.c_str(), myID.length());
	mcc.encoder.push(version.c_str(), version.length());
	mcc.encoder.push(freemem);
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();
	return 1;
}
static int handle_ping(int  pid, int  opcode) {
	//mcc.send_message(pid, OPCODE_PING, data, data_length);

	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)ADMIN_PID);
	mcc.encoder.push(OPCODE_PING);
	mcc.encoder.startList();
	for (int i=0;i<mcc.incomming_params_count;++i) {
		if (mcc.incomming_params_s[i] != NULL) {
			mcc.encoder.push(mcc.incomming_params_s[i], mcc.incomming_params_n[i]);
		} else {
			mcc.encoder.push(mcc.incomming_params_n[i]);
		}
	}
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();
	return 1;
}
static int handle_reset(int  pid, int opcode) {
    System.reset();
    return 1;
}

void AdminModule::report_status(const char * str, int len) {
	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)ADMIN_PID);
	mcc.encoder.push(OPCODE_REPORT);
	mcc.encoder.startList();
	//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
	//mcc.encoder.push(MODULE_VERSION);
	mcc.encoder.push(str, len);
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();
}

AdminModule::AdminModule() {
	for (int i=0; i<ADMIN_OPCODES; ++i) {
		AdminModule::opcode_callbacks[i]=NULL;
	}
	AdminModule::opcode_callbacks[OPCODE_REPORT] = &handle_report;
	AdminModule::opcode_callbacks[OPCODE_PING] = &handle_ping;
	AdminModule::opcode_callbacks[OPCODE_RESET] = &handle_reset;
	AdminModule::pid = mcc.register_opcode_callbacks(AdminModule::opcode_callbacks, ADMIN_OPCODES);
}

