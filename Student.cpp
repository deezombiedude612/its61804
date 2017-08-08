#include "Student.h"

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

Student::Student() {
	studentNo = 0;
	fullName = "";
	gender = ' ';
	level = 0;
	english = -1;
	malay = -1;
	math = -1;
	history = -1;
	moral = -1;
}

Student::~Student() {

}

Student::Student(int studentNo, string fullName, char gender, int level) {
	this->studentNo = studentNo;
	this->fullName = fullName;
	this->gender = gender;
	this->level = level;
	english = -1;
	malay = -1;
	math = -1;
	history = -1;
	moral = -1;
}

Student::Student(int studentNo, string fullName, char gender, int level, int english, int malay, int math, int history, int moral) {
	this->studentNo = studentNo;
	this->fullName = fullName;
	this->gender = gender;
	this->level = level;
	this->english = english;
	this->malay = malay;
	this->math = math;
	this->history = history;
	this->moral = moral;
}

int Student::getStudentNo() {
	return studentNo;
}

void Student::setStudentNo(int studentNo) {
	this->studentNo = studentNo;
}

string Student::getFullName() {
	return fullName;
}

void Student::setFullName(string fullName) {
	this->fullName = fullName;
}

char Student::getGender() {
	return gender;
}

void Student::setGender(char gender) {
	this->gender = gender;
}

int Student::getLevel() {
	return level;
}

void Student::setLevel(int level) {
	this->level = level;
}

int Student::getEnglishMarks() {
	return english;
}

void Student::setEnglishMarks(int english) {
	this->english = english;
}

int Student::getMalayMarks() {
	return malay;
}

void Student::setMalayMarks(int malay) {
	this->malay = malay;
}

int Student::getMathMarks() {
	return math;
}

void Student::setMathMarks(int math) {
	this->math = math;
}

int Student::getHistoryMarks() {
	return history;
}

void Student::setHistoryMarks(int history) {
	this->history = history;
}

int Student::getMoralMarks() {
	return moral;
}

void Student::setMoralMarks(int moral) {
	this->moral = moral;
}

/* display student record */
void Student::displayRecord() {
	/* student particulars */
	cout << "\nFull Name" << setw(13) << ": " << fullName;
	cout << "\nStudent Number" << setw(8) << ": " << studentNo;
	cout << "\nGender" << setw(16) << ": ";
	if (gender == 'M')
		cout << "Male";
	else if (gender == 'F')
		cout << "Female";
	cout << "\nLevel" << setw(17) << ": " << level;

	/* marks and respective grades - flagged as "n/a" if marks = -1 for any subject */
	cout << "\n\nResults(%)";
	cout << "\nEnglish";
	if (this->getEnglishMarks() == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << english << "\t(" << marksGrader(english) << ")";
	cout << "\nMalay";
	if (this->getMalayMarks() == -1)
		cout << setw(13) << ": n/a";
	else
		cout << setw(10) << ": " << malay << "\t(" << marksGrader(malay) << ")";
	cout << "\nMath";
	if (this->getMathMarks() == -1)
		cout << setw(14) << ": n/a";
	else
		cout << setw(11) << ": " << math << "\t(" << marksGrader(math) << ")";
	cout << "\nHistory";
	if (this->getHistoryMarks() == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << history << "\t(" << marksGrader(history) << ")";
	cout << "\nMoral";
	if (this->getMoralMarks() == -1)
		cout << setw(13) << ": n/a";
	else
		cout << setw(10) << ": " << moral << "\t(" << marksGrader(moral) << ")";

	char overallGrade;
	int coreMarks[] = { english, malay, math, history, moral };
	int totalMarks = 0;

	cout << "\n\nTotal Score\t: ";

	for (int i = 0; i < 5; i++) {
		if (coreMarks[i] == -1) {
			cout << "n/a";
			cout << "\nAverage Score\t: n/a";
			cout << "\nAverage Grade\t: n/a";
			break;
		}
		else {
			totalMarks += coreMarks[i];
			if (i + 1 == 5) {
				cout << totalMarks << "/500";
				float avgMarks = (float)totalMarks / 5;
				cout << "\nAverage Score\t: " << fixed << setprecision(2) << setfill('0') << avgMarks << "/100";
				overallGrade = marksGrader((int)avgMarks);

				for (int j = 0; j < 5; j++)
					if (marksGrader(coreMarks[j]) == 'F')
						overallGrade = 'F';

				cout << "\nAverage Grade\t: " << overallGrade;
				cout << setfill(' ');
			}
		}
	}
}

/* returns respective grades for marks */
char Student::marksGrader(int marks) {
	if (marks >= 90 && marks <= 100)
		return 'A';
	else if (marks >= 80 && marks <= 89)
		return 'B';
	else if (marks >= 70 && marks <= 79)
		return 'C';
	else if (marks >= 60 && marks <= 69)
		return 'D';

	return 'F';
}
