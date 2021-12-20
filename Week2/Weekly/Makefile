CXX = clang++
CXXFLAGS = -std=c++17 -Wall

all: test

test: bankcli Account_tests.exe
	./Account_tests.exe

bankcli: bankcli.cpp protoc_middleman
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	$(CXX) $(CXXFLAGS) bankcli.cpp bankaccounts.pb.cc -o bankcli `pkg-config --cflags --libs protobuf`

Account_tests.exe: Account_tests.cpp unit_test_framework.cpp protoc_middleman
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp bankaccounts.pb.cc -o $@ `pkg-config --cflags --libs protobuf`

protoc_middleman: bankaccounts.proto
	protoc $$PROTO_PATH --cpp_out=. bankaccounts.proto
	@touch protoc_middleman

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.d *.gch *.dSYM *.stackdump *.out *.pb.cc *.pb.h
	rm -f protoc_middleman bankcli
