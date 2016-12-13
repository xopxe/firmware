#include "MotorModule.h"

#include "EmBencode/EmBencode.h"
#include "lib/vector_math.h"

extern MCC mcc;

#define N_SERVOS 3
//#define PI 3.141502654

int ledEnabled = D0;
int servoPorts[N_SERVOS] = { D1, D2, D3 };

Servo servo[N_SERVOS];
int enabled = 0;

static int handle_report(int  pid, int  opcode) {
	//report_status(ok_str, sizeof(ok_str)-1);
	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)MOTOR_PID);
	mcc.encoder.push(OPCODE_REPORT);
	mcc.encoder.startList();
	//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
	//mcc.encoder.push(MODULE_VERSION);
	mcc.encoder.push(ok_str, sizeof(ok_str)-1);
	mcc.encoder.push(enabled);
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();
	return 1;
}

static int handle_servo_write(int  pid, int  opcode) {
	if (mcc.incomming_params_count<2) return -1;
	int motorId = mcc.incomming_params_n[0];
	int motorOut = mcc.incomming_params_n[1];

	if (motorId>0 && motorId<=N_SERVOS) {
		int motorIndex = motorId-1;
		servo[motorIndex].write(motorOut);

		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVO_WRITE);
		mcc.encoder.startList();
		//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
		//mcc.encoder.push(MODULE_VERSION);
		mcc.encoder.push(motorId);
		mcc.encoder.push(motorOut);
		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}

static int setMotorEnable(int motorEnable) {
	enabled=motorEnable;
	digitalWrite(ledEnabled, enabled);
	if (enabled) {
		for (int i=0; i<N_SERVOS; ++i) {
			//uint16_t minPW, uint16_t maxPW, int16_t minAngle, int16_t maxAngle
			servo[i].attach(servoPorts[i],
					SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW,-100, 100);
		}
	} else {
		for (int i=0; i<N_SERVOS; ++i) {
			servo[i].detach();
		}
	}
	return enabled;
}

static int handle_servo_enable(int  pid, int  opcode) {
	if (mcc.incomming_params_count!=1) return -1;
	int motorEnable = mcc.incomming_params_n[0];
	setMotorEnable(motorEnable);
	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)MOTOR_PID);
	mcc.encoder.push(OPCODE_SERVO_ENABLE);
	mcc.encoder.startList();
	mcc.encoder.push(enabled);
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();
	return 1;
}


static int handle_servo_write_us(int  pid, int  opcode) {
	if (mcc.incomming_params_count<2) return -1;
	int motorId = mcc.incomming_params_n[0];
	int motorOut = mcc.incomming_params_n[1];

	if (motorId>0 && motorId<=N_SERVOS) {
		int motorIndex = motorId-1;
		servo[motorIndex].writeMicroseconds(motorOut);

		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVO_WRITE_US);
		mcc.encoder.startList();
		//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
		//mcc.encoder.push(MODULE_VERSION);
		mcc.encoder.push(motorId);
		mcc.encoder.push(motorOut);
		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}


static int handle_servos_write(int  pid, int  opcode) {
	//servoTest.attach(D3);
	//servoTest.writeMicroseconds(2000);

	if (mcc.incomming_params_count<=N_SERVOS) {
		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVOS_WRITE);
		mcc.encoder.startList();

		for (int motorIndex=0; motorIndex<mcc.incomming_params_count; motorIndex++) {
			int motorOut = mcc.incomming_params_n[motorIndex];
			servo[motorIndex].write(motorOut);
			mcc.encoder.push(motorOut);
		}

		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}

static int handle_servos_write_us(int  pid, int  opcode) {
	//servoTest.attach(D3);
	//servoTest.writeMicroseconds(2000);

	if (mcc.incomming_params_count<=N_SERVOS) {
		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVOS_WRITE_US);
		mcc.encoder.startList();

		for (int motorIndex=0; motorIndex<mcc.incomming_params_count; motorIndex++) {
			int motorOut = mcc.incomming_params_n[motorIndex];
			servo[motorIndex].writeMicroseconds(motorOut);
			mcc.encoder.push(motorOut);
		}

		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}


static int handle_servo_read(int  pid, int  opcode) {
	if (mcc.incomming_params_count!=1) return -1;
	int motorId = mcc.incomming_params_n[0];

	if (motorId>0 && motorId<=N_SERVOS) {
		int motorIndex = motorId-1;

		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVO_READ);
		mcc.encoder.startList();
		mcc.encoder.push(motorId);
		mcc.encoder.push(servo[motorIndex].attachedPin());
		mcc.encoder.push(servo[motorIndex].readMicroseconds());
		mcc.encoder.push(servo[motorIndex].read());
		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}

static int handle_servo_trim(int  pid, int  opcode) {
	if (mcc.incomming_params_count<2) return -1;

	int motorId = mcc.incomming_params_n[0];
	int motorTrim = mcc.incomming_params_n[1];

	if (motorId>0 && motorId<=N_SERVOS) {
		int motorIndex = motorId-1;
		servo[motorIndex].setTrim(motorTrim);

		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVO_TRIM);
		mcc.encoder.startList();
		//mcc.encoder.push(admin_str, sizeof(admin_str)-1);
		//mcc.encoder.push(MODULE_VERSION);
		mcc.encoder.push(motorId);
		mcc.encoder.push(motorTrim);
		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}


static int handle_servos_trim(int  pid, int  opcode) {
	if (mcc.incomming_params_count<=N_SERVOS) {
		mcc.startFrame();
		mcc.encoder.startList();
		mcc.encoder.push((long int)MOTOR_PID);
		mcc.encoder.push(OPCODE_SERVOS_TRIM);
		mcc.encoder.startList();

		for (int motorIndex=0; motorIndex<mcc.incomming_params_count; motorIndex++) {
			int motorTrim = mcc.incomming_params_n[motorIndex];
			servo[motorIndex].setTrim(motorTrim);
			mcc.encoder.push(motorTrim);
		}

		mcc.encoder.endList();
		mcc.encoder.endList();
		mcc.endFrame();
		return 1;
	} else {
		return 0;
	}
}

static int handle_drive(int  pid, int  opcode) {
	if (mcc.incomming_params_count!=4) return -1;

	float x_dot = mcc.incomming_params_n[0]; // x_dot
	float y_dot = mcc.incomming_params_n[1]; // y_dot
	float w_dot = mcc.incomming_params_n[2]; // w_dot
	float phi_r = mcc.incomming_params_n[3]; //rotation of the robot respect to x-y axis

	SF3dVector v(x_dot, y_dot, w_dot);

	float d[N_SERVOS] = {5.0, 5.0, 5.0}; //distance from center to wheel #i

	SF3dMatrix M(
		-sin(phi_r),         cos(phi_r),        d[0],
		-sin(PI/3 - phi_r), -cos(PI/3 - phi_r), d[1],
		 sin(PI/3 + phi_r), -cos(PI/3 + phi_r), d[2]
	);

	SF3dVector w = M*v;

	servo[0].write(w.x);
	servo[1].write(w.y);
	servo[2].write(w.z);

	mcc.startFrame();
	mcc.encoder.startList();
	mcc.encoder.push((long int)MOTOR_PID);
	mcc.encoder.push(OPCODE_DRIVE);
	mcc.encoder.startList();
	mcc.encoder.push(w.x);
	mcc.encoder.push(w.y);
	mcc.encoder.push(w.z);
	mcc.encoder.endList();
	mcc.encoder.endList();
	mcc.endFrame();

	return 1;
}


int MotorModule::init() {
	setMotorEnable(1);
	return 1;
}

MotorModule::MotorModule() {
	pinMode(ledEnabled, OUTPUT);
	digitalWrite(ledEnabled, enabled);

	for (int i=0; i<MOTOR_OPCODES; ++i) {
		MotorModule::opcode_callbacks[i]=NULL;
	}
	MotorModule::opcode_callbacks[OPCODE_REPORT] = &handle_report;
	MotorModule::opcode_callbacks[OPCODE_SERVO_ENABLE] = &handle_servo_enable;
	MotorModule::opcode_callbacks[OPCODE_SERVO_WRITE] = &handle_servo_write;
	MotorModule::opcode_callbacks[OPCODE_SERVOS_WRITE] = &handle_servos_write;
	MotorModule::opcode_callbacks[OPCODE_SERVO_WRITE_US] = &handle_servo_write_us;
	MotorModule::opcode_callbacks[OPCODE_SERVOS_WRITE_US] = &handle_servos_write_us;
	MotorModule::opcode_callbacks[OPCODE_SERVO_READ] = &handle_servo_read;
	MotorModule::opcode_callbacks[OPCODE_SERVO_TRIM] = &handle_servo_trim;
	MotorModule::opcode_callbacks[OPCODE_SERVOS_TRIM] = &handle_servos_trim;
	MotorModule::opcode_callbacks[OPCODE_DRIVE] = &handle_drive;
	MotorModule::pid = mcc.register_opcode_callbacks(MotorModule::opcode_callbacks, MOTOR_OPCODES);
}

