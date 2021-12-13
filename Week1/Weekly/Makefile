CXX = clang++
CXXFLAGS = -std=c++17 -Wall

all: test

test: Account_tests.exe bankcli.exe
	./Account_tests.exe

	./bankcli.exe < bank_tests/bank_test00.in > bank_test00.out
	diff -q bank_test00.out bank_tests/bank_test00.out.correct

	./bankcli.exe < bank_tests/bank_test01.in > bank_test01.out
	diff -q bank_test01.out bank_tests/bank_test01.out.correct
	
	./bankcli.exe < bank_tests/bank_test02.in > bank_test02.out
	diff -q bank_test02.out bank_tests/bank_test02.out.correct
	
	./bankcli.exe < bank_tests/bank_test03.in > bank_test03.out
	diff -q bank_test03.out bank_tests/bank_test03.out.correct
	
	./bankcli.exe < bank_tests/bank_test04.in > bank_test04.out
	diff -q bank_test04.out bank_tests/bank_test04.out.correct
	
	./bankcli.exe < bank_tests/bank_test05.in > bank_test05.out
	diff -q bank_test05.out bank_tests/bank_test05.out.correct
	
	./bankcli.exe < bank_tests/bank_test06.in > bank_test06.out
	diff -q bank_test06.out bank_tests/bank_test06.out.correct
	
	./bankcli.exe < bank_tests/bank_test07.in > bank_test07.out
	diff -q bank_test07.out bank_tests/bank_test07.out.correct
	
	./bankcli.exe < bank_tests/bank_test08.in > bank_test08.out
	diff -q bank_test08.out bank_tests/bank_test08.out.correct
	
	./bankcli.exe < bank_tests/bank_test09.in > bank_test09.out
	diff -q bank_test09.out bank_tests/bank_test09.out.correct
	
	./bankcli.exe < bank_tests/bank_test10.in > bank_test10.out
	diff -q bank_test10.out bank_tests/bank_test10.out.correct
	
	./bankcli.exe < bank_tests/bank_test11.in > bank_test11.out
	diff -q bank_test11.out bank_tests/bank_test11.out.correct
	
	./bankcli.exe < bank_tests/bank_test12.in > bank_test12.out
	diff -q bank_test12.out bank_tests/bank_test12.out.correct
	
	./bankcli.exe < bank_tests/bank_test13.in > bank_test13.out
	diff -q bank_test13.out bank_tests/bank_test13.out.correct

bankcli.exe: bankcli.cpp
	$(CXX) $(CXXFLAGS) bankcli.cpp -o $@

Account_tests.exe: Account_tests.cpp unit_test_framework.cpp
	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.d *.gch *.dSYM *.stackdump *.out
