#include "ArtStudent.h"

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

ArtStudent::ArtStudent() : Student() {
	economics = -1;
	commerce = -1;
	art = -1;
}

ArtStudent::~ArtStudent() {

}

ArtStudent::ArtStudent(int studentNo, string fullName, char gender, int level)
	: Student(studentNo, fullName, gender, level) {
	economics = -1;
	commerce = -1;
	art = -1;
}

ArtStudent::ArtStudent(int studentNo, string fullName, char gender, int level, int english, int malay, int math,
	int history, int moral, int economics, int commerce, int art)
	:Student(studentNo, fullName, gender, level, english, malay, math, history, moral) {
	this->economics = economics;
	this->commerce = commerce;
	this->art = art;
}

int ArtStudent::getEconomicsMarks() {
	return economics;
}

void ArtStudent::setEconomicsMarks(int economics) {
	this->economics = economics;
}

int ArtStudent::getCommerceMarks() {
	return commerce;
}

void ArtStudent::setCommerceMarks(int commerce) {
	this->commerce = commerce;
}

int ArtStudent::getArtMarks() {
	return art;
}

void ArtStudent::setArtMarks(int art) {
	this->art = art;
}

/* display student record */
void ArtStudent::displayRecord() {
	/* student particulars*/
	cout << "\nFull Name" << setw(13) << ": " << super::getFullName();
	cout << "\nStudent Number" << setw(8) << ": " << super::getStudentNo();
	cout << "\nGender" << setw(16) << ": ";
	if (super::getGender() == 'M')
		cout << "Male";
	else if (super::getGender() == 'F')
		cout << "Female";
	cout << "\nLevel" << setw(17) << ": " << super::getLevel() << " Arts";

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
	cout << "\nEconomics";
	if (economics == -1)
		cout << setw(9) << ": n/a";
	else
		cout << setw(6) << ": " << economics << "\t(" << super::marksGrader(economics) << ")";
	cout << "\nCommerce";
	if (commerce == -1)
		cout << setw(10) << ": n/a";
	else
		cout << setw(7) << ": " << commerce << "\t(" << super::marksGrader(commerce) << ")";
	cout << "\nArt";
	if (art == -1)
		cout << setw(15) << ": n/a";
	else
		cout << setw(12) << ": " << art << "\t(" << super::marksGrader(art) << ")";

	char overallGrade;
	int coreMarks[] = { super::getEnglishMarks(), super::getMalayMarks(), super::getMathMarks(), super::getHistoryMarks(), super::getMoralMarks() };
	int streamMarks[] = { economics, commerce, art };
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
							cout << "\nAverage Score\t: " << fixed << setprecision(2) << setfill('0') << avgMarks
								<< "/100";
							overallGrade = super::marksGrader((int)avgMarks);

							for (int k = 0; k < 5; k++)
								if (super::marksGrader(coreMarks[k]) == 'F')
									overallGrade = 'F';
							if (super::marksGrader(streamMarks[0]) == 'F')
								if (super::marksGrader(streamMarks[1]) == 'F' &&
									super::marksGrader(streamMarks[2]) == 'F')
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
ArtStudent operator+(ArtStudent &toRecord, Student &oldRecord) {
	toRecord.setStudentNo(oldRecord.getStudentNo());
	toRecord.setFullName(oldRecord.getFullName());
	toRecord.setGender(oldRecord.getGender());
	toRecord.setLevel(0);
	return toRecord;
}
