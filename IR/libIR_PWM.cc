#include "libIR_PWM.h"

#include <string>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>

using namespace std;

string _capemgr;
string _pwm_test;
int _period;
int _duty;

/* +------------------
   | Private
   +------------------
*/

string file_find(string path, string prefix)
{
	DIR *dir;
	struct dirent *ent;

	string result;

	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (string(ent->d_name).find(prefix) != string::npos) {
				result = string(path) + "/" + ent->d_name;
				return result;
			}
		}
	}
	return result;
}

string capemgr()
{
	if (_capemgr.empty())
	{	
		_capemgr = file_find("/sys/devices", "bone_capemgr.");
	}
	return _capemgr;
}

bool file_contains(string fname, string search)
{
	ifstream file (fname.c_str());
	if (file.is_open())
	{
		char s[256];
		while (file.getline(s,256)) {
			if (string(s).find(search) != string::npos) {
				file.close();
				return true;
			}
		}
	}
	
	file.close();
	return false;
}

void write_file(string fname, string content)
{
	ofstream file (fname.c_str(), fstream::trunc);
	if (file.is_open())
	{
		file << content;
		file.close();
	}
}

bool load_dt(string name)
{
	string capeMgrDir;
	if ((capeMgrDir = capemgr()).empty()) { return false; }

	string slotsFileName = capeMgrDir + "/slots";

	bool found;

	found = file_contains(slotsFileName.c_str(), name);

	if (!found) {
		write_file(slotsFileName, name);

		for (int i = 0; i < 10000; i++) {
			found = file_contains(slotsFileName, name);
			if (found) {
				return true;
			}
		}
		return false;
	}

	return true;
}

string convertInt(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

/* +------------------
   | Public
   +------------------
*/

LibIR_PWM::LibIR_PWM()
{
	//if (load_dt("am33xx_pwm") && load_dt("bone_pwm_P8_13"))
	if (load_dt("BB-BONE-IR"))
	{
		string ocp = file_find("/sys/devices", "ocp.");
		_pwm_test = file_find(ocp, "pwm_test_P8_13.");
		
		write_file(_pwm_test+"/polarity", "0");
		write_file(_pwm_test+"/run", "0");
	}
}

bool LibIR_PWM::setPeriod(int period)
{
	_period = round(1.0e9 / period);
	write_file(_pwm_test+"/period", convertInt(_period));
}

bool LibIR_PWM::setDuty(float duty)
{
	_duty = round(_period * duty);
	write_file(_pwm_test+"/duty", convertInt(_duty));
}

bool LibIR_PWM::run()
{
	write_file(_pwm_test+"/run", "1");
}

bool LibIR_PWM::stop()
{
	write_file(_pwm_test+"/run", "0");
}


