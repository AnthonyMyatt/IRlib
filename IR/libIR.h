#ifndef LIBIR_H
#define LIBIR_H

#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>

#include <uv.h>

#include "libIR_PWM.h"
#include "Beagle_GPIO.hh"

#include "libIRInt.h"

//#define DEBUG

//#include "debug.h"

#define GPIO_OFFSET 100 // The GPIO set method takes 100us to execute, therefore Mark/Space methods must accomodate for this delay

using namespace std;

namespace LibIR
{
	class IRSend {
	public:
		IRSend();
		~IRSend();
	
		static const unsigned int SEND_PIN_1;
		static const unsigned int SEND_PIN_2;
		static const unsigned int SEND_PIN_3;
		static const unsigned int RECV_PIN;

		void sendNEC(unsigned long data, int nbits, unsigned int pin);
  		void sendSony(unsigned long data, int nbits, unsigned int pin);
  		// Neither Sanyo nor Mitsubishi send is implemented yet
  		//  void sendSanyo(unsigned long data, int nbits);
  		//  void sendMitsubishi(unsigned long data, int nbits);
  		void sendRaw(vector<unsigned int> buf, int len, int hz, unsigned int pin);
  		void sendRC5(unsigned long data, int nbits, unsigned int pin);
  		void sendRC6(unsigned long data, int nbits, unsigned int pin);
  		void sendDISH(unsigned long data, int nbits, unsigned int pin);
  		void sendSharp(unsigned long data, int nbits, unsigned int pin);
  		void sendPanasonic(unsigned int address, unsigned long data, unsigned int pin);
  		void sendJVC(unsigned long data, int nbits, int repeat, unsigned int pin); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.

	protected:
		// private
		bool enableIROut(int khz);
		void mark(int usec, unsigned int pin);
		void space(int usec, unsigned int pin);

	private:
		Beagle_GPIO gpio;     // Low-Level (mmap) GPIO Control
		LibIR_PWM   pwm;      // PWM Library
		uv_loop_t   *loop;    // LibUV Loop

		static void sendNEC(uv_work_t* req);
    		static void sendSony(uv_work_t* req);
    		static void sendRaw(uv_work_t* req);
    		static void sendRC5(uv_work_t* req);
    		static void sendRC6(uv_work_t* req);
    		static void sendDISH(uv_work_t* req);
    		static void sendSharp(uv_work_t* req);
    		static void sendPanasonic(uv_work_t* req);
    		static void sendJVC(uv_work_t* req);

    		static void finished(uv_work_t* req, int status);
	};
}
#endif
