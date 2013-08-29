#include "libIR.h"

const unsigned int LibIR::IRsend::SEND_PIN_1 = Beagle_GPIO::P8_12;
const unsigned int LibIR::IRsend::SEND_PIN_2 = Beagle_GPIO::P8_14;
const unsigned int LibIR::IRsend::SEND_PIN_3 = Beagle_GPIO::P8_16;
const unsigned int LibIR::IRsend::RECV_PIN   = Beagle_GPIO::P8_18;

using namespace std;

LibIR::IRsend::IRsend()
{
    if (!this->gpio.isActive())
    {
	GPIO_ERROR("Invalid or Inactive GPIO Module");
	return;
    }

    this->gpio.configurePin( SEND_PIN_1, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( SEND_PIN_2, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( SEND_PIN_3, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( RECV_PIN,   Beagle_GPIO::kINPUT );
    
    this->gpio.writePin( SEND_PIN_1, 0);
    this->gpio.writePin( SEND_PIN_2, 0);
    this->gpio.writePin( SEND_PIN_3, 0);
}

// ------------------------
// Private
// ------------------------

void LibIR::IRsend::mark(int time, unsigned int pin)
{
    this->gpio.writePin( pin, 1 );
    if (time > 0) usleep(time - GPIO_OFFSET);
}

void LibIR::IRsend::space(int time, unsigned int pin)
{
    //std::cout << "SPACE" << std::endl;
    this->gpio.writePin( pin, 0 );
    if (time > 0) usleep(time - GPIO_OFFSET);
}

bool LibIR::IRsend::enableIROut(int khz)
{
    pwm.stop();

    if (!pwm.setPeriod(khz*1000))
    {
		return false;
    }

    if (!pwm.setDuty(0.5))
    {
		return false;
    }

    pwm.run();
    return true;
}

// ------------------------
// Public
// ------------------------

void LibIR::IRsend::sendNEC(unsigned long data, int nbits, unsigned int pin)
{
    if (!this->enableIROut(38))
    {
        return;
    }
    this->mark(NEC_HDR_MARK, pin);
    self->space(NEC_HDR_SPACE, pin);
    for (int i = 0; i < nbits; i++)
    {
        if (data & TOPBIT)
        {
            self->mark(NEC_BIT_MARK, pin);
            self->space(NEC_ONE_SPACE, pin);
        }
        else
        {
            self->mark(NEC_BIT_MARK, pin);
            self->space(NEC_ZERO_SPACE, pin);
        }
        data <<= 1;
    }
    self->mark(NEC_BIT_MARK, pin);
    self->space(0, pin);
}

void LibIR::IRsend::sendSony(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRsend::sendRaw(vector<unsigned int> buf, int hz, unsigned int pin)
{
	if (!self->enableIROut(hz))
    {
        return;
    }
    
    for (vector<unsigned int>::size_type i = 0; i != buf.size(); i++)
    {
        if (i & 1)
        {
            self->space(buf[i], pin);
        }
        else
        {
            self->mark(buf[i], pin);
        }
    }
    self->space(0, pin);
}

void LibIR::IRsend::sendRC5(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRsend::sendRC6(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRsend::sendDISH(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRsend::sendSharp(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRsend::sendPanasonic(unsigned int address, unsigned long data, unsigned int pin)
{

}

void LibIR::IRsend::sendJVC(unsigned long data, int nbits, int repeat, unsigned int pin)
{

}
