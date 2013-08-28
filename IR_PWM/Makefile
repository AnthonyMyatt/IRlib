lib:
	-mkdir ./build
	g++ -c -o ./build/libIR_PWM.o ./libIR_PWM.cc
	ar rcs ./build/libIR_PWM.a ./build/libIR_PWM.o

all: lib

clean:
	-rm -rf build/*.o build/*.a
