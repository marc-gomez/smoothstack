#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

struct Student {
	string studentId;
	int score;
	bool comparator(Student x, Student y) {
		return x.score < y.score;
	}
};

class MergeSort {
	// Recursively merge sorts a vector of students based on their score
	//
	// "right" refers to one past the end, similar to iterators
	void merge_sort(vector<Student>& students, int left, int right) {
		if (right < left + 2) {
			return;
		}
		int middle = left + (right - left)/2;
		merge_sort(students, left, middle);
		merge_sort(students, middle, right);
		do_merge(students, left, middle, right);
	}

	void do_merge(vector<Student>& students, int left, int mid, int right) {
		int size = right - left;
		vector<Student> c(size);
		Student s;
		bool (Student::*lessthan)(Student, Student) = &Student::comparator;

		for (int i = left, j = mid, k = 0; k < size; ++k) {
			if (i == mid) {
				c[k] = students[j++];
			}
			else if (j == right) {
				c[k] = students[i++];
			}
			else {
				c[k] = ( (s.*lessthan)(students[i], students[j]) ) ? students[i++] : students[j++];
			}
		}

		copy(c.begin(), c.end(), &students[left]);
	}
	
	public:
		void sort(vector<Student>& students) {
			merge_sort(students, 0, students.size());
		};
};

int main() {
	vector<Student> students;
	
	Student s1;
	s1.studentId = "1";
	s1.score = 10;
		
	Student s2;
	s2.studentId = "2";
	s2.score = 20;

	Student s3;
	s3.studentId = "3";
	s3.score = 30;
	
	Student s4;
	s4.studentId = "4";
	s4.score = 40;

	// Input student into vector in "random" order
	students.push_back(s3);
	students.push_back(s1);
	students.push_back(s4);
	students.push_back(s2);

	// Sort vector
	MergeSort m;
	m.sort(students);

	// Print vector
	for(int i = 0; i < students.size(); ++i) {
		cout << "Student: " << students[i].studentId << ", Score: " << students[i].score << endl;
	}

	// Check results
	assert(students[0].score == 10);
	assert(students[1].score == 20);
	assert(students[2].score == 30);
	assert(students[3].score == 40);
}
