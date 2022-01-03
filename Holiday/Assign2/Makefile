CXX = clang++
CXXFLAGS = -std=c++17 -Wall

all: test

test: cal #tests.exe
#	./tests.exe
	./cal 01 2020 > tests/test1.out
	cal 01 2020 > tests/test1.correct.out
	diff -q tests/test1.out tests/test1.correct.out

	./cal 02 2020 > tests/test2.out
	cal 02 2020 > tests/test2.correct.out
	diff -q tests/test2.out tests/test2.correct.out

	./cal 03 2020 > tests/test3.out
	cal 03 2020 > tests/test3.correct.out
	diff -q tests/test3.out tests/test3.correct.out

	./cal 04 2020 > tests/test4.out
	cal 04 2020 > tests/test4.correct.out
	diff -q tests/test4.out tests/test4.correct.out

	./cal 05 2020 > tests/test5.out
	cal 05 2020 > tests/test5.correct.out
	diff -q tests/test5.out tests/test5.correct.out

	./cal 06 2020 > tests/test6.out
	cal 06 2020 > tests/test6.correct.out
	diff -q tests/test6.out tests/test6.correct.out

	./cal 07 2020 > tests/test7.out
	cal 07 2020 > tests/test7.correct.out
	diff -q tests/test7.out tests/test7.correct.out

	./cal 08 2020 > tests/test8.out
	cal 08 2020 > tests/test8.correct.out
	diff -q tests/test8.out tests/test8.correct.out

	./cal 09 2020 > tests/test9.out
	cal 09 2020 > tests/test9.correct.out
	diff -q tests/test9.out tests/test9.correct.out

	./cal 10 2020 > tests/test10.out
	cal 10 2020 > tests/test10.correct.out
	diff -q tests/test10.out tests/test10.correct.out

	./cal 11 2020 > tests/test11.out
	cal 11 2020 > tests/test11.correct.out
	diff -q tests/test11.out tests/test11.correct.out

	./cal 12 2020 > tests/test12.out
	cal 12 2020 > tests/test12.correct.out
	diff -q tests/test12.out tests/test12.correct.out

	./cal 02 1969 > tests/test13.out
	cal 02 1969 > tests/test13.correct.out
	diff -q tests/test13.out tests/test13.correct.out

cal: Assign2.cpp
	$(CXX) $(CXXFLAGS) Assign2.cpp -o $@

#tests.exe: tests.cpp unit_test_framework.cpp
#	$(CXX) $(CXXFLAGS) $< unit_test_framework.cpp -o $@


# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean:
	rm -vrf *.o *.exe *.d *.gch *.dSYM *.stackdump tests/*.out cal
