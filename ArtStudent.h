#pragma once
#ifndef ITS61804_ASGN_ARTSTUDENT_H
#define ITS61804_ASGN_ARTSTUDENT_H

#include <string>
#include "Student.h"

using namespace std;

class ArtStudent : public Student {
private:
	int economics;
	int commerce;
	int art;

public:
	typedef Student super;	// used to call the base class (Student) functions

	/* default constructors */
	ArtStudent();
	~ArtStudent();
	ArtStudent(int studentNo, string fullName, char gender, int level);
	ArtStudent(int studentNo, string fullName, char gender, int level, int english, int malay, int math,
		int history, int moral, int economics, int commerce, int art);

	/* get and set functions */
	int getEconomicsMarks();
	void setEconomicsMarks(int economics);
	int getCommerceMarks();
	void setCommerceMarks(int commerce);
	int getArtMarks();
	void setArtMarks(int art);

	/* other functions */
	void displayRecord() override;
	friend ArtStudent operator+(ArtStudent &toRecord, Student &oldRecord);
};

#endif //ITS61804_ASGN_ARTSTUDENT_H