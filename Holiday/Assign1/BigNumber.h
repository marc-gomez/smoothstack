#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <limits.h>

typedef std::vector< std::vector<unsigned char> > numvec;

class BigNumber {
	private:
		bool overflow;
		union {
			long lnum;
			unsigned char cnum[64];
		} actualUnion;

	public:
		BigNumber() {
			long zero = 0;
			set(zero);
		}

		BigNumber(long input) {
			set(input);
		}

		BigNumber(std::string str_in) {
			long num;
			std::istringstream(str_in) >> num;

			// If input number cannot fit in a long
			if (num == LONG_MAX || num == LONG_MIN) {
				set(str_in);
			}
			else {
				set(stol(str_in));
			}
		}
		
		void multiply(const BigNumber &input, BigNumber &output) {
			long a = stol(this->number());
			long b = stol(input.number());

			// Use "*" if no predicted overflow
			if (b == 0 || a <= LONG_MAX / b) {
				output.set(a*b);
			}

			// Otherwise use long multiplication
			else {
				std::vector<long> additions;
				while(b != 0) {
					long temp = b % 10;
					b /= 10;
					additions.push_back(a * temp);
				}
				long c = 0;
				long zeros = 0;
				for (int i = 0; i < additions.size(); ++i) {
					c += additions[i] * pow(10, zeros);
					zeros++;
				}
				output.set(std::to_string(c));
			}
		}

		numvec simulate_multiply(const BigNumber &input, BigNumber &output) {
			numvec lines;
			std::vector<unsigned char> line;
			long a = stol(this->number());
			long b = stol(input.number());

			bool over = false;
			if (b != 0 && a > LONG_MAX / b) {
				over = true;
			}

			// output A to vector
			std::string first = this->number();
			for (int i = 0; i < first.length(); ++i) {
				line.push_back(first[i]);
			}
			lines.push_back(line);
			line.clear();

			// output B to vector
			std::string second = input.number();
			for (int i = 0; i < second.length(); ++i) {
				line.push_back(second[i]);
			}
			lines.push_back(line);
			line.clear();

			// output addition lines to vector
			std::vector<long> additions;
			while(b != 0) {
				long temp = b % 10;
				b /= 10;
				long r = a * temp;
				additions.push_back(r);
				std::string middle = std::to_string(r);
				for (int i = 0; i < middle.length(); ++i) {
					line.push_back(middle[i]);
				}
				lines.push_back(line);
				line.clear();
			}
			long c = 0;
			long zeros = 0;
			for (int i = 0; i < additions.size(); ++i) {
				c += additions[i] * pow(10, zeros);
				zeros++;
			}
			
			// set BigNumber output
			if (!over) {
				output.set(c);
			}
			else {
				output.set(std::to_string(c));
			}

			// output sum to vector
			std::string end = std::to_string(c);
			for (int i = 0; i < end.length(); ++i) {
				line.push_back(end[i]);
			}
			lines.push_back(line);
			line.clear();

			return lines;
		}

		void print(const numvec &result) const {
			int width = std::max(result[result.size()-1].size()+1,
								result[result.size()-2].size()+result.size()-4);
			for(int i = 0; i < result.size(); ++i) {
				std::string line(result[i].begin(), result[i].end());
				
				// print A
				if (i == 0) {
					std::cout << std::setw(width);
					std::cout << std::right << line << std::endl;
				}

				// print B
				else if (i == 1) {
					std::cout << std::setw(width);
					std::cout << std::right << "x " + line << std::endl;

					std::cout << std::setw(width+1);
					std::cout << std::setfill('-') << "\n";
					std::cout << std::setfill(' ');
				}

				// print sum/last line
				else if (i == result.size() - 1) {
					std::cout << std::setw(width+1);
					std::cout << std::setfill('-') << "\n";
					std::cout << std::setfill(' ');

					std::cout << std::setw(width);
					std::cout << std::right << line << std::endl;
				}

				// print addition lines
				else {
					int zeros = i - 2;
					std::string zero_str = "";
					for(int i = 0; i < zeros; ++i) {
						zero_str += "0";
					}

					std::cout << std::setw(width);
					std::cout << std::right << line + zero_str << std::endl;
				}
			}
		}
		
		std::string number() const {
			if (!overflow) {
				return std::to_string(actualUnion.lnum);
			}
			return std::string((char*)actualUnion.cnum);
		}

		void set(long input) {
			overflow = false;
			actualUnion.lnum = input;
		}

		void set(std::string str_in) {
			overflow = true;
			strcpy((char*)actualUnion.cnum, str_in.c_str());
		}
};

#endif
