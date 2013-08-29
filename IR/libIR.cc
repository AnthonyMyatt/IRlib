#include "libIR.h"

const unsigned int LibIR::IRSend::SEND_PIN_1 = Beagle_GPIO::P8_12;
const unsigned int LibIR::IRSend::SEND_PIN_2 = Beagle_GPIO::P8_14;
const unsigned int LibIR::IRSend::SEND_PIN_3 = Beagle_GPIO::P8_16;
const unsigned int LibIR::IRSend::RECV_PIN   = Beagle_GPIO::P8_18;

// This defines the base structure passed to all uv_work threads
struct req_data
{
    LibIR::IRSend* self;
    unsigned int pin;
};

// These extend the base structure for each function
struct Generic_req : req_data	// NEC, Sony, RC5, RC6, DISH, Sharp
{
    unsigned long data;
    int nbits;
};

struct Raw_req : req_data
{
    vector<unsigned int>  buf;
    int len;
    int hz;
};

struct Panasonic_req : req_data
{
    unsigned int address;
    unsigned long data;
};

struct JVC_req : req_data
{
    unsigned long data;
    int nbits;
    int repeat;
};

// ---------------------------

using namespace std;

LibIR::IRSend::IRSend()
{
    if (!this->gpio.isActive())
    {
	GPIO_ERROR("Invalid or Inactive GPIO Module");
	return;
    }

    this->loop = uv_default_loop();

    this->gpio.configurePin( LibIR::IRSend::SEND_PIN_1, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( LibIR::IRSend::SEND_PIN_2, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( LibIR::IRSend::SEND_PIN_3, Beagle_GPIO::kOUTPUT );
    this->gpio.configurePin( LibIR::IRSend::RECV_PIN,   Beagle_GPIO::kINPUT );
    
    this->gpio.writePin( LibIR::IRSend::SEND_PIN_1, 0);
    this->gpio.writePin( LibIR::IRSend::SEND_PIN_2, 0);
    this->gpio.writePin( LibIR::IRSend::SEND_PIN_3, 0);
}

LibIR::IRSend::~IRSend()
{

}

// ------------------------
// Private
// ------------------------

void LibIR::IRSend::mark(int time, unsigned int pin)
{
    this->gpio.writePin( pin, 1 );
    if (time > 0) usleep(time - GPIO_OFFSET);
}

void LibIR::IRSend::space(int time, unsigned int pin)
{
    this->gpio.writePin( pin, 0 );
    if (time > 0) usleep(time - GPIO_OFFSET);
}

bool LibIR::IRSend::enableIROut(int khz)
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
// Private - This is where the fun stuff happens
// ------------------------

void LibIR::IRSend::sendNEC(uv_work_t* req)
{
    Generic_req* request = (Generic_req*)req->data;

    LibIR::IRSend* self = request->self;

    unsigned long data = request->data;
    int nbits = request->nbits;
    unsigned int pin = request->pin;

    if (!self->enableIROut(38))
    {
        return;
    }
    self->mark(NEC_HDR_MARK, pin);
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

void LibIR::IRSend::sendRaw(uv_work_t* req)
{    
    Raw_req* request = (Raw_req*)req->data;
    
    LibIR::IRSend* self = request->self;
    
    vector<unsigned int> buf = request->buf;
    int len = request->len;
    unsigned int hz = request->hz;
    unsigned int pin = request->pin;
    
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

void LibIR::IRSend::finished(uv_work_t* req, int status)
{
	
}

// ------------------------
// Public
// ------------------------

void LibIR::IRSend::sendNEC(unsigned long data, int nbits, unsigned int pin)
{
	Generic_req* request = new Generic_req;
    request->self = this;
    request->data = data;
    request->nbits = nbits;
    request->pin = pin;

    uv_work_t req;
    req.data = request;

    uv_queue_work(uv_default_loop(), &req, sendNEC, finished);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void LibIR::IRSend::sendSony(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRSend::sendRaw(vector<unsigned int> buf, int len, int hz, unsigned int pin)
{
	Raw_req* request = new Raw_req;
    request->self = this;
    request->buf = buf;
    request->len = len;
    request->hz = hz;
    request->pin = pin;
    
    uv_work_t req;
    req.data = request;
    
    uv_queue_work(this->loop, &req, sendRaw, finished);
    uv_run(this->loop, UV_RUN_DEFAULT);
}

void LibIR::IRSend::sendRC5(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRSend::sendRC6(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRSend::sendDISH(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRSend::sendSharp(unsigned long data, int nbits, unsigned int pin)
{

}

void LibIR::IRSend::sendPanasonic(unsigned int address, unsigned long data, unsigned int pin)
{

}

void LibIR::IRSend::sendJVC(unsigned long data, int nbits, int repeat, unsigned int pin)
{

}
