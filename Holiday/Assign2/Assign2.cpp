#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

static const char * months[] = {"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December"};

static int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class Calendar{
	private:
		int month;
		int year;

		int** calendar;

		int zeller(int month, int year){
			if(month <= 2) {
				month += 12;
				year--;
			}

			int century = year / 100;
			year %= 100;
			int weekday = (1 + floor((13*(month+1))/5) + year +
					floor(year/4) + floor(century/4) + (5*century));
			weekday = (weekday-1) % 7;
			return weekday;
		}

	public:
		Calendar(int month, int year) {
			set(month, year);
		}

		~Calendar() {
			for (int i = 0; i < 7; ++ i) {
				delete[] calendar[i];
			}
			delete[] calendar;
		}

		int ** get() const;

		void print() const {
			int len = string(months[month-1]).length();
			if (len == 3) {
				cout << setw(9) << right;
				cout << months[month-1] << " " << year << "      " << endl;
			}
			else if (len == 4 || len == 5) {
				cout << setw(10) << right;
				cout << months[month-1] << " " << year << "     " << endl;
			}
			else if (len == 6 || len == 7) {
				cout << setw(11) << right;
				cout << months[month-1] << " " << year << "    " << endl;
			}
			else if (len == 8 || len == 9) {
				cout << setw(12) << right;
				cout << months[month-1] << " " << year << "   " << endl;
			}
			cout << "Su Mo Tu We Th Fr Sa" << endl;
			for(int w = 0; w < 7; ++w) {
				bool validWeek = false;
				if (w == 5 && calendar[5][0] == 0) {
					cout << "                    ";
				}
				for(int d = 0; d < 7; ++d) {
					if (calendar[w][d] == 0 && w == 0) {
						cout << "   ";
					}
					else if (calendar[w][d] > 0){
						cout << setw(2);
						cout << calendar[w][d];
						validWeek = true;
					}
					else if (calendar[w][d] == 0 && w > 0 && validWeek) {
						cout << "  ";
					}
					if (d < 6 && validWeek) {
						cout << " ";
					}
					if (d == 6 && w < 6) {
						cout << endl;
					}
				}
			}
		}

		void set(int month, int year) {
			this->month = month;
			this->year = year;

			int numdays = days[month-1];
			if (month == 2 && year % 4 == 0) {
				numdays++;
			}

			int day = zeller(month, year);

			int ** cal { new int*[7]};
			for (int i = 0; i < 7; ++i) {
				cal[i] = new int[7];
			}

			int count = 1;
			for (int w = 0; w < 7; ++w) {
				for (int d = 0; d < 7; ++d) {
					if (count > numdays) {
						cal[w][d] = 0;
					}
					else if (w == 0 && d < day) {
						cal[w][d] = 0;
					}
					else {
						cal[w][d] = count;
						++count;
					}
				}
			}

			calendar = cal;
		}
};

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Usage: cal MM YYYY" << endl;
		return -1;
	}

	string monthStr;
	string yearStr;

	monthStr = argv[1];
	yearStr = argv[2];

	if (monthStr.size() != 2 || isdigit(monthStr[0]) == false || isdigit(monthStr[1] == false)) {
		cout << "Month input is invalid" << endl;
		return -1;
	}

	if (yearStr.size() != 4 || isdigit(yearStr[0]) == false || isdigit(yearStr[1] == false) ||
				isdigit(yearStr[2]) == false || isdigit(yearStr[3]) == false) {
		cout << "Year input is invalid" << endl;
		return -1;
	}

	int month = stoi(monthStr);
	int year = stoi(yearStr);

	if (month < 1 || month > 12) {
		cout << "Month input is invalid" << endl;
		return -1;
	}

	Calendar c(month, year);
	c.print();

	return 0;
}
