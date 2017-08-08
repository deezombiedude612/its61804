#pragma once
#ifndef ITS61804_ASGN_SCIENCESTUDENT_H
#define ITS61804_ASGN_SCIENCESTUDENT_H

#include <string>
#include "Student.h"
#include "ArtStudent.h"
#include <iostream>

using namespace std;

class ScienceStudent : public Student {
private:
	int physics;
	int chemistry;
	int biology;

public:
	typedef Student super;	// used to call the base class (Student) functions

	/* default constructors */
	ScienceStudent();
	~ScienceStudent();
	ScienceStudent(int studentNo, string fullName, char gender, int level);
	ScienceStudent(int studentNo, string fullName, char gender, int level, int english, int malay,
		int math, int history, int moral, int physics, int chemistry, int biology);

	/* get and set functions */
	int getPhysicsMarks();
	void setPhysicsMarks(int physics);
	int getChemistryMarks();
	void setChemistryMarks(int chemistry);
	int getBiologyMarks();
	void setBiologyMarks(int biology);

	/* other functions */
	void displayRecord() override;
	friend ScienceStudent operator+(ScienceStudent &toRecord, Student &oldRecord);
};

#endif //ITS61804_ASGN_SCIENCESTUDENT_H
