CXX=clang++

.PHONY: all clean simple

all: simple

simple: simple.cc
	gclang++ -O0 -g simple.cc -o simple
	get-bc simple
	opt -enable-new-pm=0 -load ../lib/libprintfunc.so --PrintFunc simple.bc -o simple.function-sequence.bc
	$(CXX) simple.function-sequence.bc -L ../lib -lpf-rt -o simple.function-sequence
	OUT_FILE=simple.out RECORD_FILE=simple.record ./simple.function-sequence a b c
	diff simple.out simple.out.expected || (echo "Test failed"; exit 1)
	diff simple.record simple.record.expected || (echo "Test failed"; exit 1)

clean:
	rm -f simple simple.bc simple.out simple.record simple.func simple.function-sequence.bc simple.function-sequence