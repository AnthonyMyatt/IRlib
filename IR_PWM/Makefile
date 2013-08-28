lib:
	-mkdir ./build
	g++ -c -o ./build/libIR_PWM.o ./libIR_PWM.cc
	ar rcs libIR_PWM.a libIR_PWM.o

all: lib

clean:
	-rm -rf build/*.o build/*.a
