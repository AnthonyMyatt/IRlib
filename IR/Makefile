TOP = $(shell pwd)
BuildDir = $(TOP)/build

lib:
	-mkdir $(BuildDir)
	g++ -c libIR_PWM.cc -o $(BuildDir)/libIR_PWM.o
	g++ -c libIR.cc -luv -o $(BuildDir)/libIR.o
	g++ -c Beagle_GPIO.cc -o $(BuildDir)/Beagle_GPIO.o
	ar rcs $(BuildDir)/libIR.a $(BuildDir)/*.o

all: lib

clean:
	-rm -rf build/*.o build/*.a
