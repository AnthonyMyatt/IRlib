lib:
	-mkdir ./build
	g++ -L../IR_PWM/build -lIR_PWM -luv -I../IR_PWM -c -o ./build/libIR.o ./libIR.cc
	g++ -c -o ./build/Beagle_GPIO.o ./Beagle_GPIO.cc
	ar rcs ./build/libIR.a ./build/libIR.o ./build/Beagle_GPIO.o

all: lib

clean:
	-rm -rf build/*.o build/*.a
