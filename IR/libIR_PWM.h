#ifndef LIBIR_PWM_H
#define LIBIR_PWM_H

class LibIR_PWM {
public:
	LibIR_PWM();
	bool setPeriod(int period);
	bool setDuty(float duty);
	bool run();
	bool stop();
};

#endif
