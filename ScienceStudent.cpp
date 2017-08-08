#include "ScienceStudent.h"

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

ScienceStudent::ScienceStudent() : Student() {
	physics = -1;
	chemistry = -1;
	biology = -1;
}

ScienceStudent::~ScienceStudent() {

}

ScienceStudent::ScienceStudent(int studentNo, string fullName, char gender, int level)
	: Student(studentNo, fullName, gender, level) {
	physics = -1;
	chemistry = -1;
	biology = -1;
}

ScienceStudent::ScienceStudent(int studentNo, string fullName, char gender, int level, int english, int malay,
	int math, int history, int moral, int physics, int chemistry, int biology)
	:Student(studentNo, fullName, gender, level, english, malay, math, history, moral) {
	this->physics = physics;
	this->chemistry = chemistry;
	this->biology = biology;
}

int ScienceStudent::getPhysicsMarks() {
	return physics;
}

void ScienceStudent::setPhysicsMarks(int physics) {
	this->physics = physics;
}

int ScienceStudent::getChemistryMarks() {
	return chemistry;
}

void ScienceStudent::setChemistryMarks(int chemistry) {
	this->chemistry = chemistry;
}

int ScienceStudent::getBiologyMarks() {
	return biology;
}

void ScienceStudent::setBiologyMarks(int biology) {
	this->biology = biology;
}

/* display student record */
void ScienceStudent::displayRecord() {
	/* student particulars*/
	cout << "\nFull Name" << setw(13) << ": " << super::getFullName();
	cout << "\nStudent Number" << setw(8) << ": " << super::getStudentNo();
	cout << "\nGender" << setw(16) << ": ";
	if (super::getGender() == 'M')
		cout << "Male";
	else if (super::getGender() == 'F')
		cout << "Female";
	cout << "\nLevel" << setw(17) << ": " << super::getLevel() << " Science";

	/* marks and respective grades - flagged as "n/a" if marks = -1 for any subject */
	cout << "\n\nResults(%)";
	cout << "\nEnglish";
	if (super::getEnglishMarks() == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << super::getEnglishMarks() << "\t(" << super::marksGrader(super::getEnglishMarks()) << ")";
	cout << "\nMalay";
	if (super::getMalayMarks() == -1)
		cout << setw(13) << ": n/a";
	else
		cout << setw(10) << ": " << super::getMalayMarks() << "\t(" << super::marksGrader(super::getMalayMarks()) << ")";
	cout << "\nMath";
	if (super::getMathMarks() == -1)
		cout << setw(14) << ": n/a";
	else
		cout << setw(11) << ": " << super::getMathMarks() << "\t(" << super::marksGrader(super::getMathMarks()) << ")";
	cout << "\nHistory";
	if (super::getHistoryMarks() == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << super::getHistoryMarks() << "\t(" << super::marksGrader(super::getHistoryMarks()) << ")";
	cout << "\nMoral";
	if (super::getMoralMarks() == -1)
		cout << setw(13) << ": n/a";
	else
		cout << setw(10) << ": " << super::getMoralMarks() << "\t(" << super::marksGrader(super::getMoralMarks()) << ")";
	cout << "\nPhysics";
	if (physics == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << physics << "\t(" << super::marksGrader(physics) << ")";
	cout << "\nChemistry";
	if (chemistry == -1)
		cout << setw(9) << ": n/a";
	else
		cout << setw(6) << ": " << chemistry << "\t(" << super::marksGrader(chemistry) << ")";
	cout << "\nBiology";
	if (biology == -1)
		cout << setw(11) << ": n/a";
	else
		cout << setw(8) << ": " << biology << "\t(" << super::marksGrader(biology) << ")";

	char overallGrade;
	int coreMarks[] = { super::getEnglishMarks(), super::getMalayMarks(), super::getMathMarks(), super::getHistoryMarks(), super::getMoralMarks() };
	int streamMarks[] = { physics, chemistry, biology };
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
				for (int j = 0; j < 5; j++) {
					if (streamMarks[j] == -1) {
						cout << "n/a";
						cout << "\nAverage Score\t: n/a";
						cout << "\nAverage Grade\t: n/a";
						break;
					}
					else {
						totalMarks += streamMarks[j];
						if (j + 1 == 3) {
							cout << totalMarks << "/800";
							float avgMarks = (float)totalMarks / 8;
							cout << "\nAverage Score\t: " << fixed << setprecision(2) << setfill('0') << avgMarks << "/100";
							overallGrade = super::marksGrader((int)avgMarks);

							for (int k = 0; k < 5; k++)
								if (super::marksGrader(coreMarks[k]) == 'F')
									overallGrade = 'F';
							if (super::marksGrader(streamMarks[0]) == 'F') {
								if (super::marksGrader(streamMarks[1]) == 'F' || super::marksGrader(streamMarks[2]) == 'F')
									overallGrade = 'F';
							}
							else if (super::marksGrader(streamMarks[1]) == 'F' && super::marksGrader(streamMarks[2]) == 'F')
								overallGrade = 'F';

							cout << "\nAverage Grade\t: " << overallGrade;
							cout << setfill(' ');
						}

					}
				}
			}
		}
	}
}

/* operator overloading - to transfer student's basic details */
ScienceStudent operator+(ScienceStudent &toRecord, Student& oldRecord) {
	toRecord.setStudentNo(oldRecord.getStudentNo());
	toRecord.setFullName(oldRecord.getFullName());
	toRecord.setGender(oldRecord.getGender());
	toRecord.setLevel(0);
	return toRecord;
}