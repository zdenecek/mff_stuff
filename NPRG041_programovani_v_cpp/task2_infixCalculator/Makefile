
all: calculator

.PHONE: test clean all

test: calculator
	./run_tests.sh calculator

calculator: calculator.cpp
	g++ -g  -o calculator calculator.cpp 

clean:
	rm -f calculator tests/*.my.out tests/*.err
