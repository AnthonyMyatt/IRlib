lib:
	-mkdir ./build
	g++ -L../IR_PWM/build -lIR_PWM -luv -I../IR_PWM -c -o ./build/libIR.o ./libIR.cc
	ar rcs libIR.a libIR.o

all: lib

clean:
	-rm -rf build/*.o build/*.a
