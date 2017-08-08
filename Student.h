#pragma once
#ifndef ITS61804_ASGN_STUDENT_H
#define ITS61804_ASGN_STUDENT_H

#include <string>
using namespace std;

class Student {
private:
	int studentNo;
	string fullName;
	char gender;
	int level;
	int english;
	int malay;
	int math;
	int history;
	int moral;

public:
	/* default constructors */
	Student();
	~Student();
	Student(int studentNo, string fullName, char gender, int level);
	Student(int studentNo, string fullName, char gender, int level, int english, int malay, int math,
		int history, int moral);

	/* get and set functions */
	int getStudentNo();
	void setStudentNo(int studentNo);
	string getFullName();
	void setFullName(string fullName);
	char getGender();
	void setGender(char gender);
	int getLevel();
	void setLevel(int level);
	int getEnglishMarks();
	void setEnglishMarks(int english);
	int getMalayMarks();
	void setMalayMarks(int malay);
	int getMathMarks();
	void setMathMarks(int math);
	int getHistoryMarks();
	void setHistoryMarks(int history);
	int getMoralMarks();
	void setMoralMarks(int moral);

	/* other functions */
	virtual void displayRecord();
	char marksGrader(int marks);
};


#endif //ITS61804_ASGN_STUDENT_H
