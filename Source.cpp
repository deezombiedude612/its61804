#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/* class headers */
#include "Student.h"
#include "ScienceStudent.h"
#include "ArtStudent.h"

/* function prototypes */
void opt1_addRecord(bool* currentEmptyFile);
void opt2_updateOrDisplayRecord();
void opt3_deleteRecord(bool* currentEmptyFile);
void printInterfaceLine(char character, unsigned int repeatTimes, bool newLine);
size_t findCaseInsensitive(string sampleString, string toSearch, size_t pos = 0);

/* main function */
int main() {
	bool emptyFile;		// trigger to see if "studentRecord.txt" is empty or not
	srand(time(0));		// to make way for random student number generator

	/* check if studentRecord.txt exists */
	ifstream stuRecordDetect("studentRecord.txt", ios::ate);		// file stream to check existence of "studentRecord.txt" and if it does, whether it is empty or not
	stuRecordDetect.seekg(0, ios::beg);								// set file stream to seek to beginning of file
	if (!stuRecordDetect) {											// if "studentRecord.txt" does not exist
		string createSignal = "";
		do {
			cout << "Unable to find/open 'studentRecord.txt'. Do you wish to create it from scratch? (Y/N) >> ";
			getline(cin, createSignal);

			if ((createSignal != "Y" && createSignal != "y") && (createSignal != "N" && createSignal != "n")) {
				createSignal.clear();
				cout << "Invalid input detected. Please enter a valid response.\n\n";
			}
		} while ((createSignal != "Y" && createSignal != "y") && (createSignal != "N" && createSignal != "n"));

		if (createSignal == "N" || createSignal == "n")			// if user does not want to create record file from scratch
			return EXIT_FAILURE;								// quit program immediately
		else {													// otherwise..
			ofstream stuRecordMake("studentRecord.txt", ios::out);		// filestream to create new student record file
			if (!stuRecordMake) {															// if unable to create new student record file
				cerr << "Unfortunately, \"studentRecord.txt\" could not be created.\n";		// display error message
				return EXIT_FAILURE;														// quit program immediately
			}
			cout << "\"studentRecord.txt\" has been created.\n";	// notify user that a blank record is created
			emptyFile = true;										// flag file as empty (for obvious reasons)
			stuRecordMake.close();									// close file stream
		}
	}
	else {	// if "studentRecord.txt" exists
		string *check;
		try {
			check = new string();        // string pointer to cross check existence of file (and if it does, check whether all records are void)
		}
		catch (bad_alloc ex) {
			cerr << "\nClear up some memory before using this program.\n";	// display warning message if not enough memory
			return EXIT_FAILURE;											// quit program immediately
		}
		getline(stuRecordDetect, *check);	// attempt to get line
		if ((*check).length() == 0)			// if "studentRecord.txt" is empty
			emptyFile = true;				// consider file as empty
		else {									// otherwise..
			emptyFile = false;					// assume content does exist
			stuRecordDetect.seekg(0, ios::beg);	// set file stream to seek to beginning of file
			int numberOfLines = 0;				// number of records present (line counter)
			int numberOfVoidLines = 0;			// number of records to be voided present (void counter)
			while (!stuRecordDetect.eof()) {
				numberOfLines++;											// increment line counter
				getline(stuRecordDetect, *check);							// get record line
				if ((*check).substr((*check).length() - 4, 4) == "void")	// if record line is to be voided
					numberOfVoidLines++;									// increment void counter
			}
			if (numberOfLines == numberOfVoidLines)	// if all records are to be voided
				emptyFile = true;					// consider file as empty
		}
		delete check;			// delete string pointer content
		check = NULL;			// set string pointer to null
	}
	stuRecordDetect.close();		// close file stream

	/* main menu interface */
	int menu_opt = 0;
	do {
		if (menu_opt == 0) {
			cout << endl;
			printInterfaceLine('-', 100, false);
			cout << "\nWHAM Student Management System\n";
			printInterfaceLine('-', 100, false);
			cout << "\n\t1. Add New Student Record";
			cout << "\n\t2. Display or Update Student Record";
			cout << "\n\t3. Delete Student Record";
			cout << "\n\t4. Exit Program";
			cout << endl;
			printInterfaceLine('-', 75, false);
		}
		cout << endl;

		cout << "Enter '0' to display main menu options.";		// prompt for option choice
		cout << "\nEnter option >> ";
		cin >> menu_opt;

		if (cin.fail()) {											// bad input received (wrong data type, etc.)
			cout << "Invalid input detected. ";
			cin.clear();											// clear cin object
			cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
			menu_opt = -1;											// proceed to warning message from switch-case (under default)
		}
		switch (menu_opt) {
		case 1:				// add student record
			opt1_addRecord(&emptyFile);
			break;

		case 2:				// display student record only if it is not empty
			if (emptyFile)
				cout << "There is absolutely no record to display or update at the moment.\n";
			else
				opt2_updateOrDisplayRecord();
			break;

		case 3:				// delete student record only if it is not empty
			if (emptyFile)
				cout << "There is absolutely no record to delete at the moment.\n";
			else
				opt3_deleteRecord(&emptyFile);
			break;

		case 0: case 4:		// redisplay options and exit, respectively
			break;

		default:			// if invalid option or input has been entered
			cout << "Please enter an integer within the given range!\n";	// display warning message
		}
	} while (menu_opt != 4);		// exit if option chosen is 6
	/* end main menu interface */

	/* Notification: Successful Program Termination */
	cout << "\nProgram status: Terminated.";
	cout << "\nThank you for using this program. :)";
	cout << endl;

	return 0;
}

/* adds student records to the system */
void opt1_addRecord(bool* currentEmptyFile) {
	cin.get();	// prevents skip

	/* start interface */
	cout << endl;
	printInterfaceLine('-', 100, false);
	cout << "\nAdd Student Record\n";
	printInterfaceLine('-', 100, true);

	string proceedFlag = "";
	do {
		cout << "\nDo you wish to continue? (Y/N) >> ";
		getline(cin, proceedFlag);

		if (proceedFlag == "Y" || proceedFlag == "y") {		// start with adding student record only if desired
			proceedFlag.clear();

			int studentType = 0;
			do {
				if (studentType == 0) {
					cout << "\nStudent Types:";
					cout << "\n\t1. Lower Secondary Student";
					cout << "\n\t2. Upper Secondary Student (Science Stream)";
					cout << "\n\t3. Upper Secondary Student (Art Stream)\n";
				}
				cout << "\nEnter '0' to display student types.";		// get student type
				cout << "\nEnter student type >> ";
				cin >> studentType;

				if (cin.fail()) {											// bad input received (wrong data type, etc.)
					cout << "Invalid input detected. ";
					cin.clear();											// clear cin object
					cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
					studentType = -1;										// proceed to warning message
				}
				if (studentType >= 1 && studentType <= 3) {
					cin.get();	// prevents skip

					/* temporary input holders */
					string inputFullName, inputGender;
					int inputLevel = 0;
					int inputMarks[8];	// in order (0-7): english, malay, math, history, moral, physics/economics, chemistry/commerce, biology/art

					/* list of subjects meant for display */
					string coreSubjects[] = { "English", "Malay", "Math", "History", "Moral" };
					string scSubjects[] = { "Physics", "Chemistry", "Biology" };
					string artSubjects[] = { "Economics", "Commerce", "Art" };

					/* prompt for full name*/
					cout << endl;
					while (inputFullName.length() < 1) {		// exit only if valid input is received
						cout << "Full Name >> ";
						getline(cin, inputFullName);

						if (inputFullName.length() > 0)												// if input has been entered
							for (int i = 0; i < inputFullName.length(); i++)                        // check input for invalid characters
								if ((inputFullName[i] == ';' || inputFullName[i] == ':')) {			// if ';' or ';' has been entered
									cout << "Do not include " << inputFullName[i] << " in the student's full name.\n\n"; // display warning message
									inputFullName.clear();											// clear input
									break;
								}
					}

					/* prompt for gender */
					do {
						cout << "Gender (M/F) >> ";
						getline(cin, inputGender);

						if (!((inputGender == "M" || inputGender == "m") || (inputGender == "F" || inputGender == "f")))	// if invalid gender has been input
							cout << "Invalid input detected. Please enter a valid response.\n\n";	// display warning message
					} while (inputGender != "M" && inputGender != "F");	// exit only if valid input is received

					/* prompt for grade level */
					if (studentType == 1) {							// lower secondary students only, grade level is between 1 and 3
						while (inputLevel < 1 || inputLevel > 3) {
							cout << "Secondary (1-3) >> ";
							cin >> inputLevel;

							if (cin.fail()) {											// bad intput received (wrong data type, etc.)
								cout << "Invalid input detected. ";
								cin.clear();											// clear cin object
								cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
								inputLevel = 0;											// proceed to warning message
							}
							if (inputLevel < 1 || inputLevel > 3)								// if input is not within valid range or bad input was detected
								cout << "Please enter an integer within the given range!\n\n";	// display warning message
						}
					}
					else if (studentType == 2 || studentType == 3) {	// upper secondary students only, grade level is either 4 or 5
						while (inputLevel != 4 && inputLevel != 5) {
							cout << "Secondary (4-5) >> ";
							cin >> inputLevel;

							if (cin.fail()) {											// bad input received (wrong data type, etc.)
								cout << "Invalid input detected. ";
								cin.clear();											// clear cin object
								cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
								inputLevel = 0;											// proceed to warning message
							}
							if (inputLevel != 4 && inputLevel != 5)								// if input is not within valid range or bad input was detected
								cout << "Please enter an integer within the given range!\n\n";	// display warning message
						}
					}

					cin.get();				// prevents skip
					cout << endl;
					string getMarksFlag;	// to contain decision whether user wants to key in student's marks
					do {
						cout << "Would you like to enter the student's marks? (Y/N) >> ";
						getline(cin, getMarksFlag);

						if ((getMarksFlag != "Y" && getMarksFlag != "y") && (getMarksFlag != "N" && getMarksFlag != "n"))	// if invalid response
							cout << "Invalid input detected. Please enter a valid response.\n\n";							// display warning message
					} while ((getMarksFlag != "Y" && getMarksFlag != "y") && (getMarksFlag != "N" && getMarksFlag != "n"));	// exit only if valid response is received

					if (getMarksFlag == "Y" || getMarksFlag == "y") {	// if user wants to key in student's marks
						string unavailableFlag = "";					// contain decision to keep any one of the marks unavailable, see instructions

						/* instructions to user */
						cout << "\nEnter '-1' for any unavailable marks. They will be regarded as N/A when read later on.";
						cout << "\nOtherwise, marks range from 0 - 100. Fill up the following marks as required.\n\n";

						/* enter marks for core subjects */
						for (int i = 0; i < 5; i++)
							do {
								cout << coreSubjects[i] << " >> ";	// prompt for marks
								cin >> inputMarks[i];

								if (cin.fail()) {											// bad input received (wrong data type, etc.)
									cout << "Invalid input detected. ";
									cin.clear();											// clear cin object
									cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
									inputMarks[i] = -2;										// proceed to warning message
								}
								if (inputMarks[i] == -1) {			// if user enters 'unavailable'
									cin.get();						// prevents skip
									unavailableFlag.clear();		// clear for new decision flag
									do {
										cout << "Are you sure to leave this unavailable? (Y/N) >> ";	// prompt user to confirm 'unavailable' marks
										cin >> unavailableFlag;
									} while ((unavailableFlag != "Y" && unavailableFlag != "y") && (unavailableFlag != "N" && unavailableFlag != "n"));	// exit only if valid input was entered

									if (unavailableFlag == "Y" || unavailableFlag == "y")						// if user wants to make marks unavailable
										break;																	// exit from loop
									else if (unavailableFlag != "N" && unavailableFlag != "n")					// otherwise,
										cout << "Invalid input detected. Please enter a valid response.\n\n";	// display warning message
								}
								else if (inputMarks[i] < 0 || inputMarks[i] > 100)					// if input is not within valid range or bad input was detected
									cout << "Please enter an integer within the given range!\n\n";	// display warning message
							} while (inputMarks[i] < 0 || inputMarks[i] > 100);						// exit loop only when marks entered are of valid range

							if (studentType == 2 || studentType == 3)	// if record is for an upper secondary student
								for (int i = 5; i < 8; i++)				// enter marks for respective stream subjects
									do {
										if (studentType == 2)			// for science stream students only
											cout << scSubjects[i - 5] << " >> ";
										else if (studentType == 3)		// for art stream students only
											cout << artSubjects[i - 5] << " >> ";
										cin >> inputMarks[i];

										if (cin.fail()) {											// bad input received (wrong data type, etc.
											cout << "Invalid input detected. ";
											cin.clear();											// clear cin object
											cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
											inputMarks[i] = -2;										// proceed to warning message
										}
										if (inputMarks[i] == -1) {		// if user enters 'unavailable
											cin.get();					// prevents skip
											unavailableFlag.clear();	// clear for new decision flag
											do {
												cout << "Are you sure to leave this unavailable? (Y/N) >> ";	// prompt user to confirm 'unavailable' marks
												cin >> unavailableFlag;
											} while ((unavailableFlag != "Y" && unavailableFlag != "y") && (unavailableFlag != "N" && unavailableFlag != "n"));	// exit only if valid input was entered

											if (unavailableFlag == "Y" || unavailableFlag == "y")						// if user wants to make marks unavailable
												break;																	// exit from loop
											else if (unavailableFlag != "N" && unavailableFlag != "n")					// otherwise,
												cout << "Invalid input detected. Please enter a valid response.\n\n";	// display warning message
										}
										else if (inputMarks[i] < 0 || inputMarks[i] > 100)					// if input is not within range or bad input was detected
											cout << "Please enter an integer within the given range!\n\n";	// display warning message
									} while (inputMarks[i] < 0 || inputMarks[i] > 100);						// exit loop only when marks entered are of valid range

									cin.get();	// prevents skip
					}

					/* print student details */
					cout << "\nSTUDENT DETAILS";
					cout << "\nFull Name\t: " << inputFullName;
					cout << "\nGender\t\t: " << (char)toupper(inputGender[0]);
					cout << "\nSecondary\t: " << inputLevel;
					if (studentType == 2)
						cout << " Science";
					if (studentType == 3)
						cout << " Arts";

					/* print available marks only if user had input them */
					if (getMarksFlag == "Y" || getMarksFlag == "y") {
						cout << endl;
						for (int i = 0; i < 5; i++)		// display marks for core subjects
							if (inputMarks[i] != -1)		// display only available marks
								cout << endl << coreSubjects[i] << setw(5) << left << "\t\t: " << inputMarks[i] << "%";

						if (studentType == 2 || studentType == 3)	// if upper secondary student record is being created
							for (int i = 5; i < 8; i++)				// display marks for respective stream subjects
								if (inputMarks[i] != -1) {			// display only available marks
									if (studentType == 2) {						// for science stream students
										if (scSubjects[i - 5].length() > 8)
											cout << endl << scSubjects[i - 5] << setw(4) << "\t:" << inputMarks[i] << "%";
										else
											cout << endl << scSubjects[i - 5] << setw(5) << "\t\t: " << inputMarks[i] << "%";
									}
									else if (studentType == 3) {                    // for art stream students
										if (artSubjects[i - 5].length() >= 8)
											cout << endl << artSubjects[i - 5] << setw(4) << "\t:" << inputMarks[i] << "%";
										else
											cout << endl << artSubjects[i - 5] << setw(6) << "\t\t\t: " << inputMarks[i] << "%";
									}
								}
					}

					/* generate random student number (10000 - 99999) */
					int randomStudentNumber = rand() % 90000 + 10000;

					/* check for any used student numbers */
					if (!(*currentEmptyFile)) {
						vector<int> usedStudentNumbers;						// contain list of used student numbers
						string lineContent;									// used to retrieve each line from "studentRecord.txt"
						stringstream recordContainer;						// stringstream container used to extract student number from "studentRecord.txt" through lineContent
						int compStuNo;										// to contain used student numbers to be put into the usedStudentNumbers vector

						try {
							ifstream checkStuNo("studentRecord.txt", ios::in);	// file stream to read "studentRecord.txt"

							while (!checkStuNo.eof()) {							// exits only when the whole file has been read
								getline(checkStuNo, lineContent);            	// retrieve current record entry
								recordContainer << lineContent;					// put entry into the stringstream container
								recordContainer >> compStuNo;					// extract student number from entry
								usedStudentNumbers.push_back(compStuNo);		// place student number into vector
								recordContainer.str(string());					// clears stringstream for next record entry
							}

							checkStuNo.close();        // close file stream
						}
						catch (bad_alloc ex) {
							cout << "\nOops, something broke! Try clearing some memory before running this program again.\n";
							exit(EXIT_FAILURE);
						}

						if (usedStudentNumbers.size() == 90000) {			// maximum number of students numbers is 90000. if reached,
							cout << "\n\nError: Add student operation has been aborted. All student numbers have been used up."
								<< "\nPlease prepare a new student number format as soon as possible.";	// display warning message

							cout << "\n\nReturning to main menu..\n";		// abort add student operation and return to main menu
							printInterfaceLine('-', 125, true);
							return;
						}

						/* check if generated student number is equal to any belonging to an existing entry or not */
						for (int i = 0; i < usedStudentNumbers.size(); i++)			// exits from for loop if a non-matching student number was generated
							if (randomStudentNumber == usedStudentNumbers.at(i)) {	// if a match is found
								randomStudentNumber = rand() % 90000 + 10000;		// generate another random student number
								i = 0;												// start comparison from top again in the vectro
							}
					}
					cout << "\n\nGiven student number: " << randomStudentNumber;	// display student number generated

					cout << endl;
					string saveFlag = "";	// contain decision to keep or discard completed record
					do {
						cout << "\nSave student details? (Y/N) >> ";	// prompt for decision to keep or discard record
						getline(cin, saveFlag);
						if ((saveFlag != "Y" && saveFlag != "y") && (saveFlag != "N" && saveFlag != "n"))	// if invalid response was entered
							cout << "Invalid input detected. Please enter a valid response.\n";				// display warning message
					} while ((saveFlag != "Y" && saveFlag != "y") && (saveFlag != "N" && saveFlag != "n"));	// exit only if valid response was entered
					if (saveFlag == "N" || saveFlag == "n")			// discard record
						cout << "Student record discarded.\n";
					else if (saveFlag == "Y" || saveFlag == "y") {	// keep record
						try {
							ofstream recordStudent("studentRecord.txt", ios::app);    // file stream to keep student record in "studentRecord.txt"

							recordStudent.seekp(0, ios::end);
							if (!*currentEmptyFile)            // if this is not the first record
								recordStudent << endl;        // add new line
							recordStudent << randomStudentNumber << ";" << inputFullName << ";" << (char)toupper(inputGender[0]) << ";" << inputLevel;

							if (studentType == 2)
								recordStudent << "S";
							else if (studentType == 3)
								recordStudent << "A";

							if (getMarksFlag == "Y" || getMarksFlag == "y") {
								if (studentType == 1)
									for (int i = 0; i < 5; i++)
										recordStudent << ";" << inputMarks[i];
								else if (studentType == 2 || studentType == 3)
									for (int i = 0; i < 8; i++)
										recordStudent << ";" << inputMarks[i];
							}
							else {
								if (studentType == 1)
									for (int i = 0; i < 5; i++)
										recordStudent << ";" << -1;
								else if (studentType == 2 || studentType == 3)
									for (int i = 0; i < 8; i++)
										recordStudent << ";" << -1;
							}
							recordStudent << ";active";            // active students will be detected for record

							recordStudent.flush();
							recordStudent.close();                // close file stream
							cout << "Student record saved!\n";    // notification: successful entry of student record
							*currentEmptyFile = false;            // changes empty file trigger if previously true; otherwise, this does nothing.
						}
						catch (bad_alloc ex) {
							cout << "\nOops, something broke! Try clearing some memory before running this program again.\n";
							exit(EXIT_FAILURE);
						}
					}
				}
				else if (studentType != 0) {										// else if display student type options is not chosen or bad input was received
					cout << "Please enter an integer within the given range!\n";	// display warning message
				}
			} while (studentType < 1 || studentType > 3);		// exit only if valid student type choice was entered
		}
		else if (proceedFlag != "N" && proceedFlag != "n") {					// if invalid response is detected
			proceedFlag.clear();												// clear response flag
			cout << "Invalid input detected. Please enter a valid response.\n";	// display warning message
		}
	} while (proceedFlag != "N" && proceedFlag != "n");			// exit only if user does not wish to proceed with creating student record(s) anymore

	/* notification: return to main menu */
	cout << "\nReturning to main menu...\n";
	printInterfaceLine('-', 100, true);
}

/* displays (and updates) a student record available in the system */
void opt2_updateOrDisplayRecord() {
	cin.get(); // prevents skip

	/* used to contain all the student records sharing any possible similarity with the search keyword */
	vector<Student> studentLVector;			// for lower secondary students only
	vector<ScienceStudent> scStudentVector;	// for upper secondary students - science stream only
	vector<ArtStudent> artStudentVector;	// for upper secondary students - art stream only

	/* start interface */
	cout << endl;
	printInterfaceLine('-', 100, false);
	cout << "\nDisplay or Update Student Record\n";
	printInterfaceLine('-', 100, true);

	string searchKeyword = "";	// contains search term used to search for the desired record
	do {						// keep repeating until an input has been entered
		cout << "\nEnter \"-1\" to return to the main menu.";
		cout << "\nEnter student ID or name (partial or full) >> ";		// prompt user for search keyword
		getline(cin, searchKeyword);

		/* check if keyword entered is equal to "exit" */
		if (searchKeyword == "-1")					// if equal,
			break;									// exit from loop, return to main menu
		else if (searchKeyword.length() >= 1) {		// else compare from student log files
			cout << endl;

			try {
				ifstream searchRecord("studentRecord.txt", ios::in);	// file stream used to read student record file
				string lineContent;										// used to carry each line from the student record file
				stringstream recordContainer;							// contains 'lineContent', later to be used to split data separated by semicolons (;)

				searchRecord.seekg(0, ios::beg);	// set to beginning of the student record file

				int numberOfRecords;			// number of student records available in the student record file
				int notMatchingRecords = 0;		// number of non-matching student records in the student record file

				/* clear vectors for clean searches */
				studentLVector.clear();
				scStudentVector.clear();
				artStudentVector.clear();

				/* search for matching records*/
				for (numberOfRecords = 0; !searchRecord.eof(); numberOfRecords++) {
					getline(searchRecord, lineContent);
					recordContainer << lineContent;

					/* temporary containers to hold student details */
					int compStudentNumber;
					char tempCharHolder;	// to contain the ';' punctuation derived from the student record file
					string compFullName;
					char gender;
					string compLevel;

					recordContainer >> compStudentNumber >> tempCharHolder;
					getline(recordContainer, compFullName, ';');

					/* see if searchKeyword matches a student number or part of a full name, and provided it is not 'void' */
					if ((to_string(compStudentNumber) == searchKeyword || findCaseInsensitive(compFullName, searchKeyword, 0) != string::npos) && (lineContent.substr(lineContent.size() - 4, 4) != "void")) {
						int marks[5];	// contains core subject marks - (in order) English, Malay, Math, History, Moral

						recordContainer >> gender >> tempCharHolder;
						getline(recordContainer, compLevel, ';');

						for (int i = 0; i < 5; i++)
							recordContainer >> marks[i] >> tempCharHolder;

						/* add relevant student record(s) */
						int gradeLevel = stoi(compLevel);				// retreive grade level (regardless of stream)
						if (gradeLevel >= 1 && gradeLevel <= 3)			// lower secondary level
							studentLVector.push_back(Student(compStudentNumber, compFullName, gender, gradeLevel,
								marks[0], marks[1], marks[2], marks[3], marks[4]));
						else if (gradeLevel == 4 || gradeLevel == 5) {	// upper secondary level
							int streamMarks[3];			// contains stream subject marks - (in order) Physics/Economics, Chemistry/Commerce, Biology/Art
							for (int i = 0; i < 3; i++)
								recordContainer >> streamMarks[i] >> tempCharHolder;
							if (compLevel[1] == 'S')			// science stream only
								scStudentVector.push_back(
									ScienceStudent(compStudentNumber, compFullName, gender, gradeLevel,
										marks[0], marks[1], marks[2], marks[3], marks[4],
										streamMarks[0], streamMarks[1], streamMarks[2]));
							else if (compLevel[1] == 'A')		// art stream only
								artStudentVector.push_back(
									ArtStudent(compStudentNumber, compFullName, gender, gradeLevel,
										marks[0], marks[1], marks[2], marks[3], marks[4],
										streamMarks[0], streamMarks[1], streamMarks[2]));
						}
					}
					else
						notMatchingRecords++;			// increment number of non-matching records
					recordContainer.str(string());		// clear stringstream for next record line
				}
				searchRecord.close();		// close ifstream

				if (numberOfRecords == notMatchingRecords)		// if no records match search keyword
					cout << "No record matches!!\n";			// notify user about it
				else {
					int matchStudentNo;			// student number is set to be unique, so tracking down the one record to be displayed can be done by finding it
					bool match = false;
					if ((studentLVector.size() + scStudentVector.size() + artStudentVector.size()) > 1) {	// if more than one record matches search keyword
						cout << "Search keyword matches more than one record.";

						/* prompt user to choose record to display */
						int recordChoice = 0;
						do {
							/* display matching record(s) */
							cout << "\nChoose the record you wish to display.\n";
							if (recordChoice == 0) {
								printInterfaceLine('=', 100, false);
								cout << "\n#\tStudent No.\t\tStudent Name";
								cout << setw(40) << right << "Gender" << setw(10) << right << "Level";
								cout << endl;
								printInterfaceLine('=', 100, false);
								int count = 0;
								int spaceWidth;
								while (count < numberOfRecords) {
									spaceWidth = 14;
									if (count < studentLVector.size())
										for (Student &stu : studentLVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << stu.getStudentNo() << "\t\t\t" << stu.getFullName();
											cout << setw(50 - stu.getFullName().length()) << right << stu.getGender();
											cout << setw(10) << right << stu.getLevel();
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10)
												if ((count + 1) == i)
													spaceWidth--;
										}
									else if (count < studentLVector.size() + scStudentVector.size())
										for (ScienceStudent &scStu : scStudentVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << scStu.getStudentNo() << "\t\t\t" << scStu.getFullName();
											cout << setw(50 - scStu.getFullName().length()) << right << scStu.getGender();
											cout << setw(10) << right << scStu.getLevel() << "S";
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10) 
												if ((count + 1) == i) 
													spaceWidth--;
										}
									else if (count < studentLVector.size() + scStudentVector.size() + artStudentVector.size())
										for (ArtStudent &artStu : artStudentVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << artStu.getStudentNo() << "\t\t\t" << artStu.getFullName();
											cout << setw(50 - artStu.getFullName().length()) << right << artStu.getGender();
											cout << setw(10) << right << artStu.getLevel() << "A";
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10)
												if ((count + 1) == i)
													spaceWidth--;
										}
									else
										count++;
								}
								cout << endl << endl;
							}

							cout << "Enter '0' to view your choices again.\nChoice (#) >> ";	// prompt user to key in choice from range given
							cin >> recordChoice;

							if (cin.fail()) {											// bad input received (wrong data type, etc.)
								cout << "Invalid input detected. ";
								cin.clear();											// clear cin object
								cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
								recordChoice = -1;										// proceed to warning message from switch-case (under default)
							}
							for (int i = 0; i < recordChoice; i++) {
								if (i < studentLVector.size())
									for (int j = 0; j < studentLVector.size(); j++) {
										if (j == i) { 												// if choice matches a lower secondary student record
											match = true;											// flag as match found
											matchStudentNo = studentLVector.at(j).getStudentNo();	// transfer student number of matching record
										}
									}
								else if (i < studentLVector.size() + scStudentVector.size())
									for (int j = studentLVector.size(); j < studentLVector.size() + scStudentVector.size(); j++) {
										if (j == i) {												// if choice matches an upper secondary student record (science)
											match = true;											// flag as match found
											matchStudentNo = scStudentVector.at(j - studentLVector.size()).getStudentNo();	// transfer student number of matching record
										}
									}
								else if (i < studentLVector.size() + scStudentVector.size() + artStudentVector.size())
									for (int j = studentLVector.size() + scStudentVector.size(); j < studentLVector.size() + scStudentVector.size() + artStudentVector.size(); j++) {
										if (j == i) {												// if choice matches an upper secondary student record (art)
											match = true;											// flag as match found
											matchStudentNo = artStudentVector.at(j - studentLVector.size() - scStudentVector.size()).getStudentNo();	// transfer student number of matching record
										}
									}
							}
							if (!match || (recordChoice < 0 || recordChoice >= numberOfRecords))	// if choice input does not point to any student record
								cout << "Please enter an integer within the given range!\n";		// display warning message
						} while (recordChoice < 1 || recordChoice > studentLVector.size() + scStudentVector.size() + artStudentVector.size());

						cin.get();		// prevents skip
					}
					else {			// if only one record has a match with the search keyword, transfer student number of matching record from..
						if (studentLVector.size() == 1)					// lower secondary student
							matchStudentNo = studentLVector.at(0).getStudentNo();
						else if (scStudentVector.size() == 1) 			// upper secondary student - science stream
							matchStudentNo = scStudentVector.at(0).getStudentNo();
						else if (artStudentVector.size() == 1)			// upper secondary student - art stream
							matchStudentNo = artStudentVector.at(0).getStudentNo();
						cout << "Only one record match was found!!" << endl;
						match = true;		// flag as match found
					}

					/* display selected record, prompt whether update is required */
					string updateFlag = "0";
					do {
						if (updateFlag == "0")
							if (match) {
								cout << endl;
								printInterfaceLine('=', 90, false);
								/* display only if selected record belongs to a Lower Secondary student */
								for (Student &thisStudent : studentLVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								/* display only if selected record belongs to a Upper Secondary student - science stream */
								for (ScienceStudent &thisStudent : scStudentVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								/* display only if selected record belongs to a Upper Secondary student - art stream */
								for (ArtStudent &thisStudent : artStudentVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								cout << endl << endl;
								printInterfaceLine('=', 90, true);
							}

						cout << "\nEnter '0' to redisplay student record.";
						cout << "\nDo you wish to update this record? (Y/N) >> ";	// prompt for decision whether to update
						getline(cin, updateFlag);

						if (updateFlag == "Y" || updateFlag == "y") {		// if user wishes to update record
							int updateChoice = 0;							// update option of choice by user
							do {
								if (updateChoice == 0) {					// display what can be updated
									cout << "\nYou may update any one of the following:";
									cout << "\n\t1. Full Name";
									cout << "\n\t2. Gender";
									cout << "\n\t3. Level";
									cout << "\n\t4. Subject Marks";
									cout << "\n\t5. Exit Update Mode";
									cout << endl;
								}
								cout << "\nWhat would you like to update?";
								cout << "\nEnter '0' to redisplay options. Choice >> ";		// prompt for update option
								cin >> updateChoice;

								if (cin.fail()) {											// bad input received (wrong data type, etc.)
									cout << "Invalid input detected. ";
									cin.clear();											// clear cin object
									cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
									updateChoice = -1;										// proceed to warning message from switch-case (under default)
								}

								string updateConfirm;		// for various confirmation processes
								string newFullName;			// to store input new name
								string newGender;			// to store input new gender
								int newLevel = 0;			// to store input new level
								string newStreamType;		// only applicable if input new level is 4 or 5 (upper secondary students)

								switch (updateChoice) {
								case 1:				// update full name
									cout << endl;
									cin.get();		// prevents skip;

									/* get new full name */
									do {
										cout << "Enter new full name >> ";			// prompt for new full name
										getline(cin, newFullName);

										if (newFullName.length() > 0)										// if input has been entered
											for (int i = 0; i < newFullName.length(); i++)					// check input for invalid characters
												if ((newFullName[i] == ';' || newFullName[i] == ':')) {		// if ';' or ';' has been entered
													cout << "Do not include " << newFullName[i]
														<< " in the student's full name.\n\n";				// display warning message
													newFullName.clear();									// clear input
													break;
												}
									} while (newFullName.length() < 1);		// carry this out until a valid full name has been entered

									/* prompt for confirmation */
									do {
										/* display current name searched from the vectors */
										cout << "\nCurrent Full Name\t\t: ";
										for (Student &thisStudent : studentLVector)
											if (matchStudentNo == thisStudent.getStudentNo())
												cout << thisStudent.getFullName();

										for (ScienceStudent &thisStudent : scStudentVector)
											if (matchStudentNo == thisStudent.getStudentNo())
												cout << thisStudent.getFullName();

										for (ArtStudent &thisStudent : artStudentVector)
											if (matchStudentNo == thisStudent.getStudentNo())
												cout << thisStudent.getFullName();

										cout << "\nNew Full Name\t\t\t: " << newFullName;	// display new name
										cout << "\n\nConfirm changes? (Y/N) >> ";			// prompt for confirmation
										getline(cin, updateConfirm);

										/* if user wishes to change the full name */
										if (updateConfirm == "Y" || updateConfirm == "y") {
											/* temporary changes made only to the record copied into the vector */
											for (Student &thisStudent : studentLVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setFullName(newFullName);

											for (ScienceStudent &thisStudent : scStudentVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setFullName(newFullName);

											for (ArtStudent &thisStudent : artStudentVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setFullName(newFullName);

											/* make changes in "studentRecord.txt" */
											ifstream reader1("studentRecord.txt", ios::in);		// open "studentRecord.txt" to extract file lines

											vector<string> fileCopy;			// to retrieve lines from file
											vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
											string lineRecord;					// to keep each line from the file into fileCopy
											stringstream lineContainer;			// used when line modification is needed
											int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

											reader1.seekg(0, ios::beg);				// set to read from the beginning of the file
											while (!reader1.eof()) {				// while there is still content to read,
												getline(reader1, lineRecord);		// retrieve each line,
												fileCopy.push_back(lineRecord);		// and then store it into the fileCopy vector
												numberOfRecords++;					// increment number of records
											}
											reader1.close();					// close file input stream

											string ignoreString;				// usually to carry over any part of the line that need not be modified
											string restOfLine;					// same as ignoreString, except to keep anything after the modified part of the line
											string newFileLine;					// to keep file line with modification

											for (string &fileLine : fileCopy) {								// check each line in fileCopy vector
												if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
													lineContainer << fileLine;								// store line into stringstream container
													getline(lineContainer, ignoreString, ';');				// student number
													getline(lineContainer, ignoreString, ';');				// old full name, to be ignored
													getline(lineContainer, restOfLine);						// contains whatever comes after the full name

													newFileLine = to_string(matchStudentNo) + ";" + newFullName + ";" + restOfLine;		// stores new full name along with student number and restOfLine
													truncateFileCopy.push_back(newFileLine);				// push the modified line into truncateFileCopy vector
												}
												else											// otherwise
													truncateFileCopy.push_back(fileLine);		// push unmodified line into truncateFileCopy vector
											}

											ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
											for (string &truncLine : truncateFileCopy) {			// for every line in truncateFileCopy,
												writer1 << truncLine;							// write each line
												numberOfRecords--;								// number of records counter to see if there are anymore records left
												if (numberOfRecords != 0)						// if some lines still remain,
													writer1 << endl;							// add a new line
											}
											writer1.close();			// close file output stream
											fileCopy.clear();			// clear fileCopy vector
											truncateFileCopy.clear();	// clear truncateCopy vector

											cout << "Successfully changed full name!\n";
										}
										else if (updateConfirm == "N" || updateConfirm == "n")	// otherwise if user chooses to discard update operation
											cout << "Discarded input name.\n";
										else			// for any other invalid input, display warning message
											cout << "Invalid input detected. Please enter a valid response.\n";
									} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

									break;

								case 2:				// update gender
									cin.get();		// prevents skip

									do {
										cout << "Enter new gender (M/F) >> ";        // prompt for new gender
										getline(cin, newGender);

										if ((newGender != "M" && newGender != "m") && (newGender != "F" && newGender != "f"))
											cout << "Invalid input detected. Please enter a valid response.\n";				// for any other invalid input, display warning message
									} while ((newGender != "M" && newGender != "m") && (newGender != "F" && newGender != "f"));

									/* prompt for confirmation */
									do {
										/* display current gender as recorded in the vector */
										cout << "\nCurrent Gender\t\t: ";
										for (Student &thisStudent : studentLVector)
											if (matchStudentNo == thisStudent.getStudentNo()) {
												if (thisStudent.getGender() == 'M')
													cout << "Male";
												else if (thisStudent.getGender() == 'F')
													cout << "Female";
											}

										for (ScienceStudent &thisStudent : scStudentVector)
											if (matchStudentNo == thisStudent.getStudentNo()) {
												if (thisStudent.getGender() == 'M')
													cout << "Male";
												else if (thisStudent.getGender() == 'F')
													cout << "Female";
											}

										for (ArtStudent &thisStudent : artStudentVector)
											if (matchStudentNo == thisStudent.getStudentNo()) {
												if (thisStudent.getGender() == 'M')
													cout << "Male";
												else if (thisStudent.getGender() == 'F')
													cout << "Female";
											}

										/* display new gender */
										cout << "\nNew Gender\t\t\t: ";
										if (toupper(newGender[0]) == 'M')
											cout << "Male";
										else if (toupper(newGender[0]) == 'F')
											cout << "Female";

										cout << "\n\nConfirm changes? (Y/N) >> ";		// prompt for confirmation
										getline(cin, updateConfirm);

										/* if user wishes to change student's gender */
										if (updateConfirm == "Y" || updateConfirm == "y") {
											/* temporary changes made only to the record copied into the vector */
											for (Student &thisStudent : studentLVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setGender(toupper(newGender[0]));

											for (ScienceStudent &thisStudent : scStudentVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setGender(toupper(newGender[0]));

											for (ArtStudent &thisStudent : artStudentVector)
												if (matchStudentNo == thisStudent.getStudentNo())
													thisStudent.setGender(toupper(newGender[0]));

											/* make changes in "studentRecord.txt" */
											ifstream reader1("studentRecord.txt", ios::in);        // open "studentRecord.txt" to extract file lines

											vector<string> fileCopy;			// to retrieve lines from file
											vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
											string lineRecord;					// to keep each line from the file into fileCopy
											stringstream lineContainer;			// used when line modification is needed
											int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

											reader1.seekg(0, ios::beg);			// set to read from the beginning of the file
											while (!reader1.eof()) {			// while there is still content to read,
												getline(reader1, lineRecord);	// retrieve each line,
												fileCopy.push_back(lineRecord);	// and then store it into the fileCopy vector
												numberOfRecords++;				// increment number of records
											}
											reader1.close();					// close file input stream

											char tempChar;						// temporary container for char
											string ignoreString;				// usually to carry over any part of the line that need not be modified
											string restOfLine;					// same as ignoreString, except to keep anything after the modified part of the line
											string newFileLine;					// to keep file line with modification

											for (string &fileLine : fileCopy) {								// check each line in fileCopy vector
												if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
													lineContainer << fileLine;								// store line into stringstream container
													getline(lineContainer, ignoreString, ';');				// student number
													getline(lineContainer, ignoreString, ';');				// student full name
													newFileLine = to_string(matchStudentNo) + ";" + ignoreString + ";";	// store student number and full name first
													lineContainer.get(tempChar);							// old gender
													getline(lineContainer, restOfLine);						// rest of the line coming after the gender
													char newCharGender = toupper(newGender[0]);				// get new gender
													newFileLine += (newCharGender + restOfLine);			// add new gender and whatever that is left of the line to the modified line
													truncateFileCopy.push_back(newFileLine);				// push the modified line into truncateFileCopy vector
												}
												else										// otherwise
													truncateFileCopy.push_back(fileLine);	// push unmodified line into truncateFileCopy vector
											}

											ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
											for (string &truncLine : truncateFileCopy) {			// for every line in truncateFileCopy,
												writer1 << truncLine;							// write each line
												numberOfRecords--;								// number of records counter to see if there are anymore records left
												if (numberOfRecords != 0)						// if some lines still remain,
													writer1 << endl;							// add a new line
											}
											writer1.close();			// close file output stream
											fileCopy.clear();			// clear fileCopy vector
											truncateFileCopy.clear();	// clear truncateCopy vector

											cout << "Successfully changed gender!\n";
										}
										else if (updateConfirm == "N" || updateConfirm == "n") 	// otherwise if user chooses to discard update operation
											cout << "Discarded input gender.\n";
										else		// for any other invalid input, display warning message
											cout << "Invalid input detected. Please enter a valid response.\n";
									} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

									break;

								case 3:			// update level
									do {
										cout << "Enter new level (1 - 5) >> ";		// prompt for new level
										cin >> newLevel;

										if (cin.fail()) {											// bad input received (wrong data type, etc.)
											cout << "Invalid input detected. ";
											cin.clear();											// clear cin object
											cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
											newLevel = 0;											// proceed to warning message from switch-case (under default)
										}

										if (newLevel < 1 || newLevel > 5)	// warning message for any input outside valid range
											cout << "Enter an integer within the given range!\n\n";
										else {
											cin.get();		// prevents skip
											if (newLevel == 4 || newLevel == 5)		// if level belongs to upper secondary category
												do {
													cout << "Enter stream type (Science - S, Arts - A) >> ";	// prompt for new stream type
													getline(cin, newStreamType);

													if ((newStreamType != "S" && newStreamType != "s") && (newStreamType != "A" && newStreamType != "a"))
														cout << "Invalid input detected. Please enter a valid response.\n\n";	// display warning message for any invalid input
												} while ((newStreamType != "S" && newStreamType != "s") && (newStreamType != "A" && newStreamType != "a"));

												do {
													/* display current level as recorded in the vector, and stream based on which vector the record is coming from if applicable */
													cout << "\nCurrent Level\t\t: ";
													for (Student &thisStudent : studentLVector)
														if (matchStudentNo == thisStudent.getStudentNo())
															cout << thisStudent.getLevel();

													for (ScienceStudent &thisStudent : scStudentVector)
														if (matchStudentNo == thisStudent.getStudentNo())
															cout << thisStudent.getLevel() << " Science";

													for (ArtStudent &thisStudent : artStudentVector)
														if (matchStudentNo == thisStudent.getStudentNo())
															cout << thisStudent.getLevel() << " Arts";

													/* display new level and stream, if applicable */
													cout << "\nNew Level\t\t\t: " << newLevel;
													if (newLevel == 4 || newLevel == 5) {
														if (toupper(newStreamType[0]) == 'S')
															cout << " Science";
														else if (toupper(newStreamType[0]) == 'A')
															cout << " Arts";
													}

													cout << "\n\nConfirm changes? (Y/N) >> ";	// prompt for confirmation
													getline(cin, updateConfirm);

													/* if user wishes to change student's level */
													if (updateConfirm == "Y" || updateConfirm == "y") {
														/* prompt user if he/she wishes to transfer any compatible marks */
														do {
															cout << "\nNOTE: Any incompatible stream marks will be deprecated.";
															cout << "\nDo you wish to keep the currently stored marks? (Y/N) >> ";
															getline(cin, updateConfirm);

															if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
																cout << "Invalid input detected. Please enter a valid response.\n";		// for any other invalid input, display warning message
														} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

														/* add new record */
														if (newLevel == 4 || newLevel == 5) {
															/* if new student level is a science stream level, recreate record in the Science Student record vector */
															if (toupper(newStreamType[0]) == 'S') {
																ScienceStudent newScStudentRecord;				// empty science stream student record
																for (Student &thisStudent : studentLVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {			// if old record is from the lower secondary student record vector
																		newScStudentRecord = newScStudentRecord + thisStudent;	// transfer basic details, not inclusive of level
																		newScStudentRecord.setLevel(newLevel);					// state new level in new record
																		if (updateConfirm == "Y" || updateConfirm == "Y") {		// transfer marks if desired by user
																			newScStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																			newScStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																			newScStudentRecord.setMathMarks(thisStudent.getMathMarks());
																			newScStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																			newScStudentRecord.setMoralMarks(thisStudent.getMathMarks());
																		}
																		scStudentVector.push_back(newScStudentRecord);		// add new record into vector
																	}
																}
																for (ArtStudent &thisStudent : artStudentVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {		// if old record is from the art stream student record vector
																		Student *temp = new Student(thisStudent.getStudentNo(), thisStudent.getFullName(),
																			thisStudent.getGender(), 0);				// transfer basic details to a temporary lower secondary student record
																		newScStudentRecord = newScStudentRecord + *temp;	// transfer it again to a science stream student record
																		delete temp;										// delete temporary record
																		temp = NULL;										// set pointer of temporary record to NULL
																		newScStudentRecord.setLevel(newLevel);				// state new level in new record
																		if (updateConfirm == "Y" || updateConfirm == "Y") {	// transfer marks if desired by user
																			newScStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																			newScStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																			newScStudentRecord.setMathMarks(thisStudent.getMathMarks());
																			newScStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																			newScStudentRecord.setMoralMarks(thisStudent.getMathMarks());
																		}
																		scStudentVector.push_back(newScStudentRecord);		// add new record into vector
																	}
																}
																for (ScienceStudent &thisStudent : scStudentVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {		// if old record resides in science stream student record vector,
																		thisStudent.setLevel(newLevel);						// don't make a new record, just update the level
																		if (updateConfirm == "N" || updateConfirm == "n") {	// clear marks if desired by user
																			thisStudent.setEnglishMarks(-1);
																			thisStudent.setMalayMarks(-1);
																			thisStudent.setMathMarks(-1);
																			thisStudent.setHistoryMarks(-1);
																			thisStudent.setMoralMarks(-1);
																			thisStudent.setPhysicsMarks(-1);
																			thisStudent.setChemistryMarks(-1);
																			thisStudent.setBiologyMarks(-1);
																		}
																	}
																}
															}
															/* if new student level is an arts stream level, recreate record in the Arts Student record vector */
															else if (toupper(newStreamType[0]) == 'A') {
																ArtStudent newArtStudentRecord;			// empty art stream student record
																for (Student &thisStudent : studentLVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {				// if old record is from the lower secondary student record vector
																		newArtStudentRecord = newArtStudentRecord + thisStudent;    // transfer basic details, not inclusive of level
																		newArtStudentRecord.setLevel(newLevel);						// state new level in new record
																		if (updateConfirm == "Y" || updateConfirm == "y") {			// transfer marks if desired by user
																			newArtStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																			newArtStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																			newArtStudentRecord.setMathMarks(thisStudent.getMathMarks());
																			newArtStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																			newArtStudentRecord.setMoralMarks(thisStudent.getMoralMarks());
																		}
																		artStudentVector.push_back(newArtStudentRecord);			// add new record into vector
																	}
																}
																for (ScienceStudent &thisStudent : scStudentVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {				// if old record is from the science stream student record vector
																		Student *temp = new Student(thisStudent.getStudentNo(),
																			thisStudent.getFullName(), thisStudent.getGender(), 0);	// transfer basic details to a temporary lower secondary student record
																		newArtStudentRecord = newArtStudentRecord + *temp;			// transfer it again to an art stream student record
																		delete temp;												// delete temporary record
																		temp = NULL;												// set pointer of temporary record to NULL
																		newArtStudentRecord.setLevel(newLevel);						// state new level in rew record
																		if (updateConfirm == "Y" || updateConfirm == "y") {			// transfer marks if desired by user
																			newArtStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																			newArtStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																			newArtStudentRecord.setMathMarks(thisStudent.getMathMarks());
																			newArtStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																			newArtStudentRecord.setMoralMarks(thisStudent.getMoralMarks());
																		}
																		artStudentVector.push_back(newArtStudentRecord);			// add new record into vector
																	}
																}
																for (ArtStudent &thisStudent : artStudentVector) {
																	if (thisStudent.getStudentNo() == matchStudentNo) {		// if old record resides in art stream student record vector,
																		thisStudent.setLevel(newLevel);						// don't make a new record, just update the level
																		if (updateConfirm == "N" || updateConfirm == "n") {	// clear marks if desired by user
																			thisStudent.setEnglishMarks(-1);
																			thisStudent.setMalayMarks(-1);
																			thisStudent.setMathMarks(-1);
																			thisStudent.setHistoryMarks(-1);
																			thisStudent.setMoralMarks(-1);
																			thisStudent.setEconomicsMarks(-1);
																			thisStudent.setCommerceMarks(-1);
																			thisStudent.setArtMarks(-1);
																		}
																	}
																}
															}
														}
														/* if new student level is a lower secondary level, recreate record in the Lower Secondary Student record vector */
														else if (newLevel >= 1 && newLevel <= 3) {
															Student newLStudentRecord;			// empty lower secondary student record
															for (ScienceStudent &thisStudent : scStudentVector) {
																if (thisStudent.getStudentNo() == matchStudentNo) {		// if old record is from the science stream student record vector
																														/* transfer basic details */
																	newLStudentRecord.setStudentNo(thisStudent.getStudentNo());
																	newLStudentRecord.setFullName(thisStudent.getFullName());
																	newLStudentRecord.setGender(thisStudent.getGender());
																	newLStudentRecord.setLevel(newLevel);

																	if (updateConfirm == "Y" || updateConfirm == "y") {	// transfer marks if desired by user
																		newLStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																		newLStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																		newLStudentRecord.setMathMarks(thisStudent.getMathMarks());
																		newLStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																		newLStudentRecord.setMoralMarks(thisStudent.getMoralMarks());
																	}
																	studentLVector.push_back(newLStudentRecord);		// add new record into student record
																}
															}
															for (ArtStudent &thisStudent : artStudentVector) {
																if (thisStudent.getStudentNo() == matchStudentNo) {		// if old record is from the art stream student record vector
																														/* transfer basic details */
																	newLStudentRecord.setStudentNo(thisStudent.getStudentNo());
																	newLStudentRecord.setFullName(thisStudent.getFullName());
																	newLStudentRecord.setGender(thisStudent.getGender());
																	newLStudentRecord.setLevel(newLevel);

																	if (updateConfirm == "Y" || updateConfirm == "y") {	// transfer marks if desired by user
																		newLStudentRecord.setEnglishMarks(thisStudent.getEnglishMarks());
																		newLStudentRecord.setMalayMarks(thisStudent.getMalayMarks());
																		newLStudentRecord.setMathMarks(thisStudent.getMathMarks());
																		newLStudentRecord.setHistoryMarks(thisStudent.getHistoryMarks());
																		newLStudentRecord.setMoralMarks(thisStudent.getMoralMarks());
																	}
																	studentLVector.push_back(newLStudentRecord);		// add new record into student record
																}
															}
															for (Student &thisStudent : studentLVector) {
																if (thisStudent.getStudentNo() == matchStudentNo) {		// if old student record resides in lower secondary student record vector,
																	thisStudent.setLevel(newLevel);						// don't make a new record, just update the level
																	if (updateConfirm == "N" || updateConfirm == "n") {	// clear marks if desired by user
																		thisStudent.setEnglishMarks(-1);
																		thisStudent.setMalayMarks(-1);
																		thisStudent.setMathMarks(-1);
																		thisStudent.setHistoryMarks(-1);
																		thisStudent.setMoralMarks(-1);
																	}
																}
															}
														}

														/* delete old copy of the record if not modified */
														if (newLevel == 4 || newLevel == 5) {
															if (toupper(newStreamType[0]) == 'S') {
																/* temporary vectors to swap with current vector if old record is found */
																vector<Student> temp1;
																vector<ArtStudent> temp2;

																/*
																* if line in current vector does not match old record, add to temporary vector.
																* swap only happens when one of the records in the current vector match old record
																*/
																for (Student &thisStudent : studentLVector) {
																	if (thisStudent.getStudentNo() != matchStudentNo)
																		temp1.push_back(thisStudent);
																}
																if (temp1.size() != studentLVector.size()) {
																	temp1.swap(studentLVector);
																	cout << "Old record deleted from Student vector.\n\n";
																}
																for (ArtStudent &thisStudent : artStudentVector) {
																	if (thisStudent.getStudentNo() != matchStudentNo)
																		temp2.push_back(thisStudent);
																}
																if (temp2.size() != artStudentVector.size()) {
																	temp2.swap(artStudentVector);
																	cout << "Old record deleted from Art Student vector.\n\n";
																}
															}
															else if (toupper(newStreamType[0]) == 'A') {
																/* temporary vectors to swap with current vector if old record is found */
																vector<Student> temp1;
																vector<ScienceStudent> temp2;

																/*
																* if line in current vector does not match old record, add to temporary vector.
																* swap only happens when one of the records in the current vector match old record
																*/
																for (Student &thisStudent : studentLVector) {
																	if (thisStudent.getStudentNo() != matchStudentNo)
																		temp1.push_back(thisStudent);
																}
																if (temp1.size() != studentLVector.size()) {
																	temp1.swap(studentLVector);
																	cout << "Old record deleted from Student vector.\n\n";
																}
																for (ScienceStudent &thisStudent : scStudentVector) {
																	if (thisStudent.getStudentNo() != matchStudentNo)
																		temp2.push_back(thisStudent);
																}
																if (temp2.size() != scStudentVector.size()) {
																	temp2.swap(scStudentVector);
																	cout << "Old record deleted from Science Student vector.\n\n";
																}
															}
														}
														else if (newLevel >= 1 && newLevel <= 3) {
															/* temporary vectors to swap with current vector if old record is found */
															vector<ScienceStudent> temp1;
															vector<ArtStudent> temp2;

															/*
															* if line in current vector does not match old record, add to temporary vector.
															* swap only happens when one of the records in the current vector match old record
															*/
															for (ScienceStudent &thisStudent : scStudentVector) {
																if (thisStudent.getStudentNo() != matchStudentNo)
																	temp1.push_back(thisStudent);
															}
															if (temp1.size() != scStudentVector.size()) {
																temp1.swap(scStudentVector);
																cout << "Old record deleted from Science Student vector.\n\n";
															}
															for (ArtStudent &thisStudent : artStudentVector) {
																if (thisStudent.getStudentNo() != matchStudentNo)
																	temp2.push_back(thisStudent);
															}
															if (temp2.size() != artStudentVector.size()) {
																temp2.swap(artStudentVector);
																cout << "Old record deleted from Art Student vector.\n\n";
															}
														}

														/* make changes in "studentRecord.txt" */
														ifstream reader1("studentRecord.txt", ios::in);		// open "studentRecord.txt" to extract file lines

														vector<string> fileCopy;			// to retrieve lines from file
														vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
														string lineRecord;					// to keep each line from the file into fileCopy
														stringstream lineContainer;			// used when line modification is needed
														int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

														reader1.seekg(0, ios::beg);			// set to read from the beginning of the file
														while (!reader1.eof()) {			// while there is still content to read,
															getline(reader1, lineRecord);	// retrieve each line,
															fileCopy.push_back(lineRecord);	// and then store it into the fileCopy vector
															numberOfRecords++;				// increment number of records
														}
														reader1.close();					// close file input stream

														char tempChar;			// temporary container for char
														string ignoreString;	// usually to carry over any part of the line that need not be modified
														string restOfLine;		// same as ignoreString, except to keep anything after the modified part of the line
														string newFileLine;		// to keep file line with modification

														for (string &fileLine : fileCopy) {								// check each line in fileCopy vector
															if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
																lineContainer << fileLine;								// store line into stringstream container
																getline(lineContainer, ignoreString, ';');				// student number
																getline(lineContainer, ignoreString, ';');				// student name
																lineContainer.get(tempChar);                            // student gender
																newFileLine = to_string(matchStudentNo) + ";" + ignoreString +
																	";" + tempChar + ";";								// store student number, name and gender into modified line
																lineContainer.get(tempChar);							// ';'
																getline(lineContainer, ignoreString, ';');				// old level
																newFileLine += (to_string(newLevel));					// add new level to modified line

																/* modify marks - will do nothing if no changes are to be made to them, and add them with the rest of the line to the modified line */
																if (newLevel == 4 || newLevel == 5) {
																	if (toupper(newStreamType[0]) == 'S') {
																		newFileLine += "S;";
																		for (ScienceStudent &thisStudent : scStudentVector) {
																			if (thisStudent.getStudentNo() == matchStudentNo) {
																				newFileLine += (to_string(thisStudent.getEnglishMarks()) + ";"
																					+ to_string(thisStudent.getMalayMarks()) + ";"
																					+ to_string(thisStudent.getMathMarks()) + ";"
																					+ to_string(thisStudent.getHistoryMarks()) + ";"
																					+ to_string(thisStudent.getMoralMarks()) + ";"
																					+ to_string(thisStudent.getPhysicsMarks()) + ";"
																					+ to_string(thisStudent.getChemistryMarks()) + ";"
																					+ to_string(thisStudent.getBiologyMarks()) + ";active");
																			}
																		}
																	}
																	else if (toupper(newStreamType[0]) == 'A') {
																		newFileLine += "A;";
																		for (ArtStudent &thisStudent : artStudentVector) {
																			if (thisStudent.getStudentNo() == matchStudentNo) {
																				newFileLine += (to_string(thisStudent.getEnglishMarks()) + ";"
																					+ to_string(thisStudent.getMalayMarks()) + ";"
																					+ to_string(thisStudent.getMathMarks()) + ";"
																					+ to_string(thisStudent.getHistoryMarks()) + ";"
																					+ to_string(thisStudent.getMoralMarks()) + ";"
																					+ to_string(thisStudent.getEconomicsMarks()) + ";"
																					+ to_string(thisStudent.getCommerceMarks()) + ";"
																					+ to_string(thisStudent.getArtMarks()) + ";active");
																			}
																		}
																	}
																}
																else if (newLevel >= 1 && newLevel <= 3) {
																	for (Student &thisStudent : studentLVector) {
																		if (thisStudent.getStudentNo() == matchStudentNo) {
																			newFileLine += (to_string(thisStudent.getEnglishMarks()) + ";"
																				+ to_string(thisStudent.getMalayMarks()) + ";"
																				+ to_string(thisStudent.getMathMarks()) + ";"
																				+ to_string(thisStudent.getHistoryMarks()) + ";"
																				+ to_string(thisStudent.getMoralMarks()) + ";active");
																		}
																	}
																}
																truncateFileCopy.push_back(newFileLine);	// push the modified line into truncateFileCopy vector
															}
															else											// otherwise
																truncateFileCopy.push_back(fileLine);		// push unmodified line into truncateFileCopy vector
														}

														ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
														for (string &truncLine : truncateFileCopy) {		// for every line in truncateFileCopy,
															writer1 << truncLine;							// write each line
															numberOfRecords--;								// number of records counter to see if there are anymore records left
															if (numberOfRecords != 0)						// if some lines still remain,
																writer1 << endl;							// add a new line
														}
														writer1.close();			// close file output stream
														fileCopy.clear();			// clear fileCopy vector
														truncateFileCopy.clear();	// clear truncateCopy vector

														cout << "Successfully changed level!\n";
													}
													else if (updateConfirm == "N" || updateConfirm == "n") 	// otherwise if user chooses to discard update operation
														cout << "Discarded input level.\n";
													else	// for any other invalid input, display warning message
														cout << "Invalid input detected. Please enter a valid response.\n";
												} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));
										}
									} while (newLevel < 1 || newLevel > 5);
									break;

								case 4:			// update subject marks
									for (Student &thisStudent : studentLVector) {
										if (thisStudent.getStudentNo() == matchStudentNo) {
											int markUpdateChoice = 0;		// choice for what marks to modify
											string subjects[] = { "English", "Malay", "Math", "History", "Moral" };
											int currentMarks[] = { thisStudent.getEnglishMarks(), thisStudent.getMalayMarks(),
												thisStudent.getMathMarks(), thisStudent.getHistoryMarks(),thisStudent.getMoralMarks() };
											do {
												if (markUpdateChoice == 0) {	// display marks that can be modified
													cout << "\nYou may choose to update any of the following marks:";
													for (int i = 0; i < 5; i++)
														cout << "\n\t" << i + 1 << ". " << subjects[i];
													cout << endl;
												}
												cout << "\nEnter '0' to redisplay mark update options. Enter '6' to return to the previous menu.";
												cout << "\nChoice >> ";			// prompt user for what marks to modify
												cin >> markUpdateChoice;

												if (cin.fail()) {											// bad input received (wrong data type, etc.)
													cout << "Invalid input detected. ";
													cin.clear();											// clear cin object
													cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
													markUpdateChoice = -1;									// proceed to warning message from switch-case (under default)
												}

												if (markUpdateChoice < 0 || markUpdateChoice > 6)			// if input is not in given range or is just of a wrong type
													cout << "Enter an integer within the given range!\n";	// display warning message
												else if (markUpdateChoice != 6 && markUpdateChoice != 0) {
													int newMark = -1;	// does not specifically belong to one subject mark only, changes depending on user choice
													do {
														cout << "Enter new " << subjects[markUpdateChoice - 1]
															<< " marks (enter '-1' if unavailable) >> ";	// prompt for new marks for the selected subject
														cin >> newMark;

														if (cin.fail()) {											// bad input received (wrong data type, etc.)
															cout << "Invalid input detected. ";
															cin.clear();											// clear cin object
															cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
															newMark = -2;											// proceed to warning message from switch-case (under default)
														}
														if (newMark == -1) {	// if user enters marks as unavailable (-1)
															cin.get();		// prevents skip
															do {
																cout << "Are you sure you wish to keep this subject's marks unavailable? (Y/N) >> ";	// prompt for confirmation on the matter
																cin >> updateConfirm;

																/* if an invalid response is received, display warning message */
																if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
																	cout << "Invalid input detected. Please enter a valid response.\n";
															} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

															if (updateConfirm == "Y" || updateConfirm == "y")	// if user wishes to make marks for the selected subject unavailable
																break;
														}
														if (newMark < 0 || newMark > 100)							// if marks are out of range
															cout << "Enter an integer within the given range!\n\n";	// display warning message
													} while (newMark < 0 || newMark > 100);

													cin.get();        // prevents skip
													do {
														/* display current marks as recorded in vector */
														cout << "\nCurrent " << subjects[markUpdateChoice - 1] << " marks: ";
														if (currentMarks[markUpdateChoice - 1] == -1)
															cout << "n/a";
														else
															cout << currentMarks[markUpdateChoice - 1];

														/* display new marks */
														cout << "\nNew " << subjects[markUpdateChoice - 1] << " marks: ";
														if (newMark == -1)
															cout << "n/a";
														else
															cout << newMark;

														cout << "\n\nConfirm marks change? (Y/N) >> ";	// prompt user for confirmation
														getline(cin, updateConfirm);

														if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
															cout << "Invalid input detected. Please enter a valid response.\n";		// for any other invalid input, display warning message
													} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

													if (updateConfirm == "Y" || updateConfirm == "y") {
														/* update record in vector */
														if (markUpdateChoice == 1)
															thisStudent.setEnglishMarks(newMark);
														else if (markUpdateChoice == 2)
															thisStudent.setMalayMarks(newMark);
														else if (markUpdateChoice == 3)
															thisStudent.setMathMarks(newMark);
														else if (markUpdateChoice == 4)
															thisStudent.setHistoryMarks(newMark);
														else if (markUpdateChoice == 5)
															thisStudent.setMoralMarks(newMark);

														/* make changes in "studentRecord.txt" */
														ifstream reader1("studentRecord.txt", ios::in);	// open "studentRecord.txt" to extract file lines

														vector<string> fileCopy;			// to retrieve lines from file
														vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
														string lineRecord;					// to keep each line from the file into fileCopy
														stringstream lineContainer;			// used when line modification is needed
														int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

														reader1.seekg(0, ios::beg);			// set to read from the beginning of the file
														while (!reader1.eof()) {			// while there is still content to read,
															getline(reader1, lineRecord);	// retrieve each line,
															fileCopy.push_back(lineRecord);	// and then store it into the fileCopy vector
															numberOfRecords++;				// increment number of records
														}
														reader1.close();					// close file input stream

														char tempChar;			// temporary container for char
														string ignoreString;	// usually to carry over any part of the line that need not be modified
														string restOfLine;		// same as ignoreString, except to keep anything after the modified part of the line
														string newFileLine; 	// to keep file line with modification

														for (string &fileLine : fileCopy) {								// check each line in fileCopy vector
															if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
																lineContainer << fileLine;								// store line into stringstream container
																getline(lineContainer, ignoreString, ';');				// student number
																getline(lineContainer, ignoreString, ';');				// student full name
																lineContainer.get(tempChar);							// student gender
																newFileLine = to_string(matchStudentNo) + ";" +
																	ignoreString + ";" + tempChar + ";";		// store student number, full name and gender in modified line
																lineContainer.get(tempChar);							// ';'
																getline(lineContainer, ignoreString, ';');				// student level
																newFileLine += (ignoreString + ";");					// store student level in modified line
																for (int i = 1; i < markUpdateChoice; i++) {			// store marks not meant to be modified
																	getline(lineContainer, ignoreString, ';');
																	newFileLine += (ignoreString + ";");
																}
																newFileLine += (to_string(newMark) + ";");				// store new marks for selected subject
																getline(lineContainer, ignoreString, ';');				// get rid of previous marks for the selected subject
																getline(lineContainer, restOfLine);						// get the rest of the line
																newFileLine += restOfLine;								// add the rest of the line
																truncateFileCopy.push_back(newFileLine);				// push the modified line into truncateFileCopy vector
															}
															else                                                        // otherwise
																truncateFileCopy.push_back(fileLine);					// push unmodified line into truncateFileCopy vector
														}

														ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
														for (string &truncLine : truncateFileCopy) {			// for every line in truncateFileCopy,
															writer1 << truncLine;							// write each line
															numberOfRecords--;								// number of records counter to see if there are anymore records left
															if (numberOfRecords != 0)						// if some lines still remain,
																writer1 << endl;							// add a new line
														}
														writer1.close();			// close file output stream
														fileCopy.clear();			// clear fileCopy vector
														truncateFileCopy.clear();	// clear truncateCopy vector

														cout << "\nSuccessfully changed marks!\n";
													}
													else if (updateConfirm == "N" || updateConfirm == "n") 	// otherwise if user chooses to discard update operation
														cout << "\nDiscarded input marks.\n";

													markUpdateChoice = 0;            // redisplay choices of marks that can be modified upon completion
												}
												else if (markUpdateChoice == 6)
													updateChoice = 0;				// redisplay update options upon exiting that update mode

											} while (markUpdateChoice != 6);
										}
									}
									for (ScienceStudent &thisStudent : scStudentVector) {
										if (thisStudent.getStudentNo() == matchStudentNo) {
											int markUpdateChoice = 0;			// choice for what marks to modify
											string subjects[] = { "English", "Malay", "Math", "History", "Moral", "Physics", "Chemistry", "Biology" };
											int currentMarks[] = { thisStudent.getEnglishMarks(), thisStudent.getMalayMarks(),
												thisStudent.getMathMarks(), thisStudent.getHistoryMarks(),
												thisStudent.getMoralMarks(), thisStudent.getPhysicsMarks(),
												thisStudent.getChemistryMarks(), thisStudent.getBiologyMarks() };
											do {
												if (markUpdateChoice == 0) {		// display marks that can be modified
													cout << "\nYou may choose to update any of the following marks:";
													for (int i = 0; i < 8; i++)
														cout << "\n\t" << i + 1 << ". " << subjects[i];
													cout << endl;
												}
												cout << "\nEnter '0' to redisplay mark update options. Enter '9' to return to the previous menu.";
												cout << "\nChoice >> ";			// prompt user for what marks to modify
												cin >> markUpdateChoice;

												if (cin.fail()) {											// bad input received (wrong data type, etc.)
													cout << "Invalid input detected. ";
													cin.clear();											// clear cin object
													cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
													markUpdateChoice = -1;									// proceed to warning message from switch-case (under default)
												}

												if (markUpdateChoice < 0 || markUpdateChoice > 9)
													cout << "Enter an integer within the given range!\n";	// display warning message
												else if (markUpdateChoice != 9 && markUpdateChoice != 0) {
													int newMark = -1;	// does not specifically belong to one subject mark only, changes depending on user choice
													do {
														cout << "Enter new " << subjects[markUpdateChoice - 1]
															<< " marks (enter '-1' if unavailable) >> ";	// prompt for new marks for the selected subject
														cin >> newMark;

														if (cin.fail()) {											// bad input received (wrong data type, etc.)
															cout << "Invalid input detected. ";
															cin.clear();											// clear cin object
															cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
															newMark = -2;											// proceed to warning message from switch-case (under default)
														}
														if (newMark == -1) {	// if user enters marks as unavailable (-1)
															cin.get();		// prevents skip
															do {
																cout << "Are you sure you wish to keep this subject's marks unavailable? (Y/N) >> ";	// prompt for confirmation on the matter
																cin >> updateConfirm;

																/* if an invalid response is entered, display warning message */
																if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
																	cout << "Invalid input detected. Please enter a valid response.\n";
															} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

															/* if user wishes to make marks unavailable */
															if (updateConfirm == "Y" || updateConfirm == "y")
																break;
														}
														if (newMark < 0 || newMark > 100)							// if marks are out of range
															cout << "Enter an integer within the given range!\n\n";	// display warning message
													} while (newMark < 0 || newMark > 100);

													cin.get();		// prevents skip
													do {
														/* display current marks as recorded in vector */
														cout << "\nCurrent " << subjects[markUpdateChoice - 1] << " marks: ";
														if (currentMarks[markUpdateChoice - 1] == -1)
															cout << "n/a";
														else
															cout << currentMarks[markUpdateChoice - 1];

														/* display new marks */
														cout << "\nNew " << subjects[markUpdateChoice - 1] << " marks: ";
														if (newMark == -1)
															cout << "n/a";
														else
															cout << newMark;

														cout << "\n\nConfirm marks change? (Y/N) >> ";		// prompt user for confirmation
														getline(cin, updateConfirm);

														if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
															cout << "Invalid input detected. Please enter a valid response.\n";	// for any other invalid input, display warning message
													} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

													if (updateConfirm == "Y" || updateConfirm == "y") {
														/* update record in vector */
														if (markUpdateChoice == 1)
															thisStudent.setEnglishMarks(newMark);
														else if (markUpdateChoice == 2)
															thisStudent.setMalayMarks(newMark);
														else if (markUpdateChoice == 3)
															thisStudent.setMathMarks(newMark);
														else if (markUpdateChoice == 4)
															thisStudent.setHistoryMarks(newMark);
														else if (markUpdateChoice == 5)
															thisStudent.setMoralMarks(newMark);
														else if (markUpdateChoice == 6)
															thisStudent.setPhysicsMarks(newMark);
														else if (markUpdateChoice == 7)
															thisStudent.setChemistryMarks(newMark);
														else if (markUpdateChoice == 8)
															thisStudent.setBiologyMarks(newMark);

														/* make changes in "studentRecord.txt" */
														ifstream reader1("studentRecord.txt", ios::in);		// open "studentRecord.txt" to extract file lines

														vector<string> fileCopy;			// to retrieve lines from file
														vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
														string lineRecord;					// to keep each line from the file into fileCopy
														stringstream lineContainer;			// used when line modification is needed
														int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

														reader1.seekg(0, ios::beg);			// set to read from the beginning of the file
														while (!reader1.eof()) { 			// while there is still content to read,
															getline(reader1, lineRecord);	// retrieve each line,
															fileCopy.push_back(lineRecord);	// and then store it into the fileCopy vector
															numberOfRecords++;				// increment number of records
														}
														reader1.close();                    // close file input stream

														char tempChar;			// temporary container for char
														string ignoreString;	// usually to carry over any part of the line that need not be modified
														string restOfLine;		// same as ignoreString, except to keep anything after the modified part of the line
														string newFileLine;		// to keep file line with modification

														for (string &fileLine : fileCopy) {		// check each line in fileCopy vector
															if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
																lineContainer << fileLine;								// store line into stringstream container
																getline(lineContainer, ignoreString, ';');				// student number
																getline(lineContainer, ignoreString, ';');				// student full name
																lineContainer.get(tempChar);                            // student gender
																newFileLine = to_string(matchStudentNo) + ";" +
																	ignoreString + ";" + tempChar + ";";		// store student number, full name and gender in modified line
																lineContainer.get(tempChar);							// ';'
																getline(lineContainer, ignoreString, ';');				// student level
																newFileLine += (ignoreString + ";");					// store student level in modified line
																for (int i = 1; i < markUpdateChoice; i++) {			// store marks not meant to be modified
																	getline(lineContainer, ignoreString, ';');
																	newFileLine += (ignoreString + ";");
																}
																newFileLine += (to_string(newMark) + ";");				// store new marks for selected subject
																getline(lineContainer, ignoreString, ';');				// get rid of previous marks for the selected subject
																getline(lineContainer, restOfLine);						// get the rest of the line
																newFileLine += restOfLine;								// add the rest of the line
																truncateFileCopy.push_back(newFileLine);				// push the modified line into truncateFileCopy vector
															}
															else										// otherwise
																truncateFileCopy.push_back(fileLine);	// push unmodified line into truncateFileCopy vector
														}

														ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
														for (string &truncLine : truncateFileCopy) {			// for every line in truncateFileCopy,
															writer1 << truncLine;							// write each line
															numberOfRecords--;								// number of records counter to see if there are anymore records left
															if (numberOfRecords != 0)						// if some lines still remain,
																writer1 << endl;							// add a new line
														}
														writer1.close();			// close file output stream
														fileCopy.clear();			// clear fileCopy vector
														truncateFileCopy.clear();	// clear truncateCopy vector

														cout << "\nSuccessfully changed marks!\n";
													}
													else if (updateConfirm == "N" || updateConfirm == "n") 	// otherwise if user chooses to discard update operation
														cout << "\nDiscarded input marks.\n";

													markUpdateChoice = 0;		// redisplay choices of marks that can be modified upon completion
												}
												else if (markUpdateChoice == 9)
													updateChoice = 0;			// redisplay update options upon exiting that update mode

											} while (markUpdateChoice != 9);
										}
									}
									for (ArtStudent &thisStudent : artStudentVector) {
										if (thisStudent.getStudentNo() == matchStudentNo) {
											int markUpdateChoice = 0;		// choice for what marks to modify
											string subjects[] = { "English", "Malay", "Math", "History", "Moral", "Economics", "Commerce", "Art" };
											int currentMarks[] = { thisStudent.getEnglishMarks(), thisStudent.getMalayMarks(),
												thisStudent.getMathMarks(), thisStudent.getHistoryMarks(),
												thisStudent.getMoralMarks(), thisStudent.getEconomicsMarks(),
												thisStudent.getCommerceMarks(), thisStudent.getArtMarks() };
											do {
												if (markUpdateChoice == 0) {		// display marks that can be modified
													cout << "\nYou may choose to update any of the following marks:";
													for (int i = 0; i < 8; i++)
														cout << "\n\t" << i + 1 << ". " << subjects[i];
													cout << endl;
												}
												cout << "\nEnter '0' to redisplay mark update options. Enter '9' to return to the previous menu.";
												cout << "\nChoice >> ";			// prompt user for what marks to modify
												cin >> markUpdateChoice;

												if (cin.fail()) {											// bad input received (wrong data type, etc.)
													cout << "Invalid input detected. ";
													cin.clear();											// clear cin object
													cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
													markUpdateChoice = -1;									// proceed to warning message from switch-case (under default)
												}

												if (markUpdateChoice < 0 || markUpdateChoice > 9)			// if input is out of range, or simply invalid
													cout << "Enter an integer within the given range!\n";	// display warning message
												else if (markUpdateChoice != 9 && markUpdateChoice != 0) {
													int newMark = -1;		// does not specifically belong to one subject mark only, changes depending on user choice
													do {
														cout << "Enter new " << subjects[markUpdateChoice - 1]
															<< " marks (enter '-1' if unavailable) >> ";	// prompt for new marks for the selected subject
														cin >> newMark;

														if (cin.fail()) {											// bad input received (wrong data type, etc.)
															cout << "Invalid input detected. ";
															cin.clear();											// clear cin object
															cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
															newMark = -2;											// proceed to warning message from switch-case (under default)
														}
														if (newMark == -1) {	// if user enters marks as unavailable (-1)
															cin.get();			// prevents skip
															do {
																cout << "Are you sure you wish to keep this subject's marks unavailable? (Y/N) >> ";	// prompt for confirmation on the matter
																cin >> updateConfirm;

																/* if invalid input is entered, display warning message */
																if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
																	cout << "Invalid input detected. Please enter a valid response.\n";
															} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

															/* if user wishes to make marks unavailable */
															if (updateConfirm == "Y" || updateConfirm == "y")
																break;
														}
														if (newMark < 0 || newMark > 100)			// if marks are out of range
															cout << "Enter an integer within the given range!\n\n";	// display warning message
													} while (newMark < 0 || newMark > 100);

													cin.get();		// prevents skip
													do {
														/* display current marks as recorded in vector */
														cout << "\nCurrent " << subjects[markUpdateChoice - 1] << " marks: ";
														if (currentMarks[markUpdateChoice - 1] == -1)
															cout << "n/a";
														else
															cout << currentMarks[markUpdateChoice - 1];

														/* display new marks */
														cout << "\nNew " << subjects[markUpdateChoice - 1] << " marks: ";
														if (newMark == -1)
															cout << "n/a";
														else
															cout << newMark;

														cout << "\n\nConfirm marks change? (Y/N) >> ";		// prompt user for confirmation
														getline(cin, updateConfirm);

														if ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"))
															cout << "Invalid input detected. Please enter a valid response.\n";	// for any invalid input, display warning message
													} while ((updateConfirm != "Y" && updateConfirm != "y") && (updateConfirm != "N" && updateConfirm != "n"));

													if (updateConfirm == "Y" || updateConfirm == "y") {
														/* temporary changes made only to the record copied into the vector */
														if (markUpdateChoice == 1)
															thisStudent.setEnglishMarks(newMark);
														else if (markUpdateChoice == 2)
															thisStudent.setMalayMarks(newMark);
														else if (markUpdateChoice == 3)
															thisStudent.setMathMarks(newMark);
														else if (markUpdateChoice == 4)
															thisStudent.setHistoryMarks(newMark);
														else if (markUpdateChoice == 5)
															thisStudent.setMoralMarks(newMark);
														else if (markUpdateChoice == 6)
															thisStudent.setEconomicsMarks(newMark);
														else if (markUpdateChoice == 7)
															thisStudent.setCommerceMarks(newMark);
														else if (markUpdateChoice == 8)
															thisStudent.setArtMarks(newMark);

														/* make changes in "studentRecord.txt" */
														ifstream reader1("studentRecord.txt", ios::in);		// open "studentRecord.txt" to extract file lines

														vector<string> fileCopy;			// to retrieve lines from file
														vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
														string lineRecord;					// to keep each line from the file into fileCopy
														stringstream lineContainer;			// used when line modification is needed
														int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

														reader1.seekg(0, ios::beg);			// set to read from the beginning of the file
														while (!reader1.eof()) {			// while there is still content to read,
															getline(reader1, lineRecord);	// retrieve each line,
															fileCopy.push_back(lineRecord);	// and then store it into the fileCopy vector
															numberOfRecords++;				// increment number of records
														}
														reader1.close();					// close file input stream

														char tempChar;			// temporary container for char
														string ignoreString;	// usually to carry over any part of the line that need not be modified
														string restOfLine;		// same as ignoreString, except to keep anything after the modified part of the line
														string newFileLine;		// to keep file line with modification

														for (string &fileLine : fileCopy) {			// check each line in fileCopy vector
															if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
																lineContainer << fileLine;							// store line into stringstream container
																getline(lineContainer, ignoreString, ';');			// student number
																getline(lineContainer, ignoreString, ';');			// student full name
																lineContainer.get(tempChar);						// student gender
																newFileLine = to_string(matchStudentNo) + ";" +
																	ignoreString + ";" + tempChar + ";";	// store student number, full name and gender in modified line
																lineContainer.get(tempChar);						// ';'
																getline(lineContainer, ignoreString, ';');			// student level
																newFileLine += (ignoreString + ";");				// store student level in modified line
																for (int i = 1; i < markUpdateChoice; i++) {		// store marks not meant to be modified
																	getline(lineContainer, ignoreString, ';');
																	newFileLine += (ignoreString + ";");
																}
																newFileLine += (to_string(newMark) + ";");			// store new marks for selected subject
																getline(lineContainer, ignoreString, ';');			// get rid of previous marks for the selected subject
																getline(lineContainer, restOfLine);					// get the rest of the line
																newFileLine += restOfLine;							// add the rest of the line
																truncateFileCopy.push_back(newFileLine);			// push the modified line into truncateFileCopy vector
															}
															else											// otherwise,
																truncateFileCopy.push_back(fileLine);		// push unmodified line into truncateFileCopy vector
														}

														ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
														for (string &truncLine : truncateFileCopy) {		// for every line in truncateFileCopy,
															writer1 << truncLine;						// write each line
															numberOfRecords--;							// number of records counter to see if there are anymore records left
															if (numberOfRecords != 0)					// if some lines still remain,
																writer1 << endl;						// add a new line
														}
														writer1.close();			// close file output stream
														fileCopy.clear();			// clear fileCopy vector
														truncateFileCopy.clear();	// clear truncateCopy vector

														cout << "\nSuccessfully changed marks!\n";
													}
													else if (updateConfirm == "N" || updateConfirm == "n") 	// otherwise if user chooses to discard update operation
														cout << "\nDiscarded input marks.\n";

													markUpdateChoice = 0;		// redisplay choices of marks that can be modified upon completion
												}
												else if (markUpdateChoice == 9)
													updateChoice = 0;			// redisplay update options upon exiting that update mode
											} while (markUpdateChoice != 9);
										}
									}
									break;

								case 0:case 5:		// redisplay options and exit respectively
									cin.get();		// prevents skip
									break;

								default:		// if invalid option or input has been entered
									cout << "Enter an integer within the given range!\n";
								}
							} while (updateChoice != 5);
						}
						else if (updateFlag != "0" && (updateFlag != "N" && updateFlag != "n"))		// if invalid response was entered instead
							cout << "Invalid input detected. Please enter a valid response.\n";		// display warning message
					} while (updateFlag != "N" && updateFlag != "n");
				}
			}
			catch (bad_alloc ex) {
				cout << "\nOops, something broke! Try clearing some memory before running this program again.\n";
				exit(EXIT_FAILURE);
			}
		}
	} while (searchKeyword != "-1");		// if user enters "-1", return to main menu

											/* notification: return to main menu */
	cout << "\nReturning to main menu...\n";
	printInterfaceLine('-', 100, true);
}

/* deletes student records available in the system */
void opt3_deleteRecord(bool* currentEmptyFile) {
	cin.get();	// prevents skip

	/* used to contain all the student records sharing any possible similarity with the search keyword */
	vector<Student> studentLVector;			// for lower secondary students only
	vector<ScienceStudent> scStudentVector;	// for upper secondary students - science stream only
	vector<ArtStudent> artStudentVector;	// for upper secondary students - art stream only

	/* start interface */
	cout << endl;
	printInterfaceLine('-', 100, false);
	cout << "\nDelete Student Record\n";
	printInterfaceLine('-', 100, true);

	string searchKeyword = "";	// contains search term used to search for the desired record
	do {						// keep repeating until an input has been entered
		cout << "\nEnter \"-1\" to return to the main menu.";
		cout << "\nEnter student ID or name (partial or full) >> ";		// prompt user for search keyword
		getline(cin, searchKeyword);

		/* check if keyword entered is equal to "exit" */
		if (searchKeyword == "-1")					// if equal,
			break;									// exit from loop, return to main menu
		else if (searchKeyword.length() >= 1) {		// else compare from student log files
			try {
				cout << endl;
				ifstream searchRecord("studentRecord.txt", ios::in);	// file stream used to read student record file
				string lineContent;										// used to carry each line from the student record file
				stringstream recordContainer;							// contains 'lineContent', later to be used to split data separated by semicolons (;)

				searchRecord.seekg(0, ios::beg);	// set to beginning of the student record file

				int numberOfRecords;				// number of student records available in the student record file
				int notMatchingRecords = 0;			// number of non-matching student records in the student record file

				/* clear vectors for clean searches */
				studentLVector.clear();
				scStudentVector.clear();
				artStudentVector.clear();

				/* search for matching records*/
				for (numberOfRecords = 0; !searchRecord.eof(); numberOfRecords++) {
					getline(searchRecord, lineContent);
					recordContainer << lineContent;

					/* temporary containers to hold student details */
					int compStudentNumber;
					char tempCharHolder;	// to contain the ';' punctuation derived from the student record file
					string compFullName;
					char gender;
					string compLevel;

					recordContainer >> compStudentNumber >> tempCharHolder;
					getline(recordContainer, compFullName, ';');

					/* see if searchKeyword matches a student number or part of a full name, and provided it is not 'void' */
					if ((to_string(compStudentNumber) == searchKeyword || findCaseInsensitive(compFullName, searchKeyword, 0) != string::npos) && (lineContent.substr(lineContent.size() - 4, 4) != "void")) {
						int marks[5];		// contains core subject marks - (in order) English, Malay, Math, History, Moral

						recordContainer >> gender >> tempCharHolder;
						getline(recordContainer, compLevel, ';');

						for (int i = 0; i < 5; i++)
							recordContainer >> marks[i] >> tempCharHolder;

						/* add relevant student record(s) */
						int gradeLevel = stoi(compLevel);				// retreive grade level (regardless of stream)
						if (gradeLevel >= 1 && gradeLevel <= 3)			// lower secondary level
							studentLVector.push_back(Student(compStudentNumber, compFullName, gender, gradeLevel, marks[0], marks[1], marks[2], marks[3], marks[4]));
						else if (gradeLevel == 4 || gradeLevel == 5) {	// upper secondary level
							int streamMarks[3];							// contains stream subject marks - (in order) Physics/Economics, Chemistry/Commerce, Biology/Art
							for (int i = 0; i < 3; i++)
								recordContainer >> streamMarks[i] >> tempCharHolder;
							if (compLevel[1] == 'S')		// science stream only
								scStudentVector.push_back(ScienceStudent(compStudentNumber, compFullName, gender, gradeLevel,
									marks[0], marks[1], marks[2], marks[3], marks[4],
									streamMarks[0], streamMarks[1], streamMarks[2]));
							else if (compLevel[1] == 'A')	// art stream only
								artStudentVector.push_back(ArtStudent(compStudentNumber, compFullName, gender, gradeLevel,
									marks[0], marks[1], marks[2], marks[3], marks[4],
									streamMarks[0], streamMarks[1], streamMarks[2]));
						}
					}
					else
						notMatchingRecords++;		// increment number of non-matching records
					recordContainer.str(string());	// clear stringstream for next record line
				}
				searchRecord.close();		// close ifstream

				if (numberOfRecords == notMatchingRecords)		// if no records match search keyword
					cout << "No record matches!!\n";			// notify user about it
				else {
					int matchStudentNo;			// student number is set to be unique, so tracking down the one record to be displayed can be done by finding it
					bool match = false;
					if ((studentLVector.size() + scStudentVector.size() + artStudentVector.size()) > 1) {	// if more than one record matches search keyword
						cout << "Search keyword matches more than one record.";

						/* prompt user to choose record to display */
						int recordChoice = 0;
						do {
							/* display matching record(s) */
							cout << "\nChoose the record you wish to delete.\n";
							if (recordChoice == 0) {
								printInterfaceLine('=', 100, false);
								cout << "\n#\tStudent No.\t\tStudent Name";
								cout << setw(40) << right << "Gender" << setw(10) << right << "Level";
								cout << endl;
								printInterfaceLine('=', 100, false);
								int count = 0;
								int spaceWidth;
								while (count < numberOfRecords) {
									spaceWidth = 14;
									if (count < studentLVector.size())
										for (Student &stu : studentLVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << stu.getStudentNo() << "\t\t\t" << stu.getFullName();
											cout << setw(50 - stu.getFullName().length()) << right << stu.getGender();
											cout << setw(10) << right << stu.getLevel();
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10)
												if ((count + 1) == i)
													spaceWidth--;
										}
									else if (count < studentLVector.size() + scStudentVector.size())
										for (ScienceStudent &scStu : scStudentVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << scStu.getStudentNo() << "\t\t\t" << scStu.getFullName();
											cout << setw(50 - scStu.getFullName().length()) << right << scStu.getGender();
											cout << setw(10) << right << scStu.getLevel() << "S";
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10)
												if ((count + 1) == i)
													spaceWidth--;
										}
									else if (count < studentLVector.size() + scStudentVector.size() + artStudentVector.size())
										for (ArtStudent &artStu : artStudentVector) {
											cout << endl << count + 1 << setw(spaceWidth) << right << artStu.getStudentNo() << "\t\t\t" << artStu.getFullName();
											cout << setw(50 - artStu.getFullName().length()) << right << artStu.getGender();
											cout << setw(10) << right << artStu.getLevel() << "A";
											count++;

											for (int i = 10; (count + 1) >= i; i *= 10)
												if ((count + 1) == i)
													spaceWidth--;
										}
									else
										count++;
								}
								cout << endl << endl;
							}

							cout << "Enter '0' to view your choices again.\nChoice (#) >> ";	// prompt user to key in choice from range given
							cin >> recordChoice;

							if (cin.fail()) {											// bad input received (wrong data type, etc.)
								cout << "Invalid input detected. ";
								cin.clear();											// clear cin object
								cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ignore the rest of the wrong input
								recordChoice = -1;										// proceed to warning message from switch-case (under default)
							}
							for (int i = 0; i < recordChoice; i++) {
								if (i < studentLVector.size())
									for (int j = 0; j < studentLVector.size(); j++) {
										if (j == i) {												// if choice matches a lower secondary student record
											match = true;											// flag as match found
											matchStudentNo = studentLVector.at(j).getStudentNo();	// transfer student number of matching record
										}
									}
								else if (i < studentLVector.size() + scStudentVector.size())
									for (int j = studentLVector.size(); j < studentLVector.size() + scStudentVector.size(); j++) {
										if (j == i) {																		// if choice matches an upper secondary student record (science)
											match = true;																	// flag as match found
											matchStudentNo = scStudentVector.at(j - studentLVector.size()).getStudentNo();	// transfer student number of matching record
										}
									}
								else if (i < studentLVector.size() + scStudentVector.size() + artStudentVector.size())
									for (int j = studentLVector.size() + scStudentVector.size(); j < studentLVector.size() + scStudentVector.size() + artStudentVector.size(); j++) {
										if (j == i) {																									// if choice matches an upper secondary student record (art)
											match = true;																								// flag as match found
											matchStudentNo = artStudentVector.at(j - studentLVector.size() - scStudentVector.size()).getStudentNo();	// transfer student number of matching record
										}
									}
							}
							if (!match || (recordChoice < 0 || recordChoice >= numberOfRecords))		// if choice input does not point to any student record
								cout << "Please enter an integer within the given range!\n";			// display warning message
						} while (recordChoice < 1 || recordChoice > studentLVector.size() + scStudentVector.size() + artStudentVector.size());

						cin.get();	// prevents skip
					}
					else {		// if only one record has a match with the search keyword, transfer student number of matching record from..
						if (studentLVector.size() == 1)				// lower secondary student
							matchStudentNo = studentLVector.at(0).getStudentNo();
						else if (scStudentVector.size() == 1)		// upper secondary student - science stream
							matchStudentNo = scStudentVector.at(0).getStudentNo();
						else if (artStudentVector.size() == 1)		// upper secondary student - art stream
							matchStudentNo = artStudentVector.at(0).getStudentNo();
						cout << "Only one record match was found!!" << endl;
						match = true;		// flag as match found
					}

					/* display selected record if found, prompt whether to delete record or not */
					string deleteFlag = "0";
					do {
						if (deleteFlag == "0")
							if (match) {
								cout << endl;
								printInterfaceLine('=', 90, false);
								/* display only if selected record belongs to a Lower Secondary student */
								for (Student &thisStudent : studentLVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								/* display only if selected record belongs to a Upper Secondary student - science stream */
								for (ScienceStudent &thisStudent : scStudentVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								/* display only if selected record belongs to a Upper Secondary student - art stream */
								for (ArtStudent &thisStudent : artStudentVector)
									if (matchStudentNo == thisStudent.getStudentNo())
										thisStudent.displayRecord();

								cout << endl << endl;
								printInterfaceLine('=', 90, true);
							}

						cout << "\nEnter '0' to redisplay student record.";
						cout << "\nWARNING: Deletion of record is permanent and cannot be undone!";
						cout << "\nDo you wish to delete this record? (Y/N) >> ";	// prompt for decision to delete record
						getline(cin, deleteFlag);

						if (deleteFlag == "Y" || deleteFlag == "y") {			// if user wishes to delete record
							ifstream reader1("studentRecord.txt", ios::in);		// open "studentRecord.txt" to extract file lines

							/* modify file to void record */
							vector<string> fileCopy;			// to retrieve lines from file
							vector<string> truncateFileCopy;	// to store modified lines straight from fileCopy
							string lineRecord;					// to keep each line from the file into fileCopy
							stringstream lineContainer;			// used when line modification is needed
							int numberOfRecords = 0;			// used when rewriting file - acts as a trigger for when a next line is needed

							reader1.seekg(0, ios::beg);				// set to read from the beginning of the file
							while (!reader1.eof()) {				// while there is still content to read,
								getline(reader1, lineRecord);		// retrieve each line,
								fileCopy.push_back(lineRecord);		// and then store it into the fileCopy vector
								numberOfRecords++;					// increment number of records
							}
							reader1.close();						// close file input stream

							string ignoreString;		// usually to carry over any part of the line that need not be modified
							string newFileLine;			// to keep file line with modification
							char tempChar;				// temporary container for char

														/* change status from "active" to "void" in studentRecord.txt */
							for (string &fileLine : fileCopy) {
								if (fileLine.substr(0, 5) == to_string(matchStudentNo)) {	// if the line contains record to be modified
									lineContainer << fileLine;								// store line into stringstream container
									getline(lineContainer, ignoreString, ';');				// student number
									getline(lineContainer, ignoreString, ';');				// student full name

									newFileLine = to_string(matchStudentNo) + ";" + ignoreString + ";";	// store student number and full name into modified line
									getline(lineContainer, ignoreString, ';');				// student gender
									newFileLine += (ignoreString + ";");                    // store gender into modified line
									getline(lineContainer, ignoreString, ';');				// student level
									newFileLine += (ignoreString + ";");					// store level into modified line
									int numberOfSubjects = 5;								// default for core subjects, lower secondary student record
									if (ignoreString[0] == '4' || ignoreString[0] == '5')	// if record belongs to upper secondary student record
										numberOfSubjects = 8;								// set number of subjects to include stream subjects
									for (int i = 0; i < numberOfSubjects; i++) {			// store subject marks into modified line
										getline(lineContainer, ignoreString, ';');
										newFileLine += (ignoreString + ";");
									}
									newFileLine += "void";									// set status of record to void, so that the system no longer recognises it
									cout << newFileLine << endl;
									truncateFileCopy.push_back(newFileLine);				// push the modified line into truncateFileCopy vector
								}
								else											// otherwise,
									truncateFileCopy.push_back(fileLine);		// push unmodified line into truncateFileCopy vector
							}

							ofstream writer1("studentRecord.txt", ios::out);	// open "studentRecord.txt" to update file lines (clear file first)
							for (string &truncLine : truncateFileCopy) {		// for every line in truncateFileCopy,
								writer1 << truncLine;							// write each line
								numberOfRecords--;								// number of records counter to see if there are anymore records left
								if (numberOfRecords != 0)						// if some lines still remain,
									writer1 << endl;							// add a new line
							}
							writer1.close();			// close file output stream
							fileCopy.clear();			// clear fileCopy vector
							truncateFileCopy.clear();	// clear truncateCopy vector

							cout << "\nRecord deleted.\n";
						}
						else if (deleteFlag == "N" || deleteFlag == "n")	// if user does not want to delete record
							cout << "\nRecord was not deleted.\n";			// display notification message
						else if (deleteFlag != "0")		// if invalid response was entered instead
							cout << "Invalid input detected. Please enter a valid response.\n";	// display warning message

					} while ((deleteFlag != "Y" && deleteFlag != "y") && (deleteFlag != "N" && deleteFlag != "n"));
				}

			}
			catch (bad_alloc ex) {
				cout << "\nOops, something broke! Try clearing some memory before running this program again.\n";
				exit(EXIT_FAILURE);
			}
		}

		/* check if no records exist in studentRecord.txt */
		try {
			ifstream stuRecordDetect("studentRecord.txt", ios::ate);		// file stream to check existence of "studentRecord.txt" and if it does, whether it is empty or not
			stuRecordDetect.seekg(0, ios::beg);		// set file stream to seek to beginning of file
			string *check = new string();			// string pointer to cross check existence of file (and if it does, check whether all records are void)
			getline(stuRecordDetect, *check);		// attempt to get line
			if ((*check).length() == 0)				// if "studentRecord.txt" is empty
				*currentEmptyFile = true;			// consider file as empty
			else {										// otherwise..
				*currentEmptyFile = false;				// assume content does exist
				stuRecordDetect.seekg(0, ios::beg);		// set file stream to seek to beginning of file
				int numberOfLines = 0;					// number of records present (line counter)
				int numberOfVoidLines = 0;				// number of records to be voided present (void counter)
				while (!stuRecordDetect.eof()) {
					numberOfLines++;										// increment line counter
					getline(stuRecordDetect, *check);						// get record line
					if ((*check).substr((*check).length() - 4, 4) == "void")	// if record line is to be voided
						numberOfVoidLines++;								// increment void counter
				}
				if (numberOfLines == numberOfVoidLines)		// if all lines are void
					*currentEmptyFile = true;				// consider file as empty
			}
			delete check;			// delete string pointer content
			check = NULL;			// set string pointer to null

			stuRecordDetect.close();		// close file stream

			if (*currentEmptyFile)		// if file is considered empty
				break;					// quit function to main menu immediately
		}
		catch (bad_alloc ex) {
			cout << "\nOops, something broke! Try clearing some memory before running this program again.\n";
			exit(EXIT_FAILURE);
		}

	} while (searchKeyword != "-1");			// if user enters "-1", return to main menu

												/* notification: return to main menu */
	cout << "\nReturning to main menu...\n";
	printInterfaceLine('-', 100, true);
}

/* line drawing for interface aesthetic */
void printInterfaceLine(char character, unsigned int repeatTimes, bool newLine) {
	for (unsigned int i = 0; i < repeatTimes; i++)
		cout << character;
	if (newLine)
		cout << endl;
}

/* algorithm alternative to search source (sampleString) for existence of target (toSearch) in case-insensitive mode */
size_t findCaseInsensitive(string sampleString, string toSearch, size_t pos) {
	transform(sampleString.begin(), sampleString.end(), sampleString.begin(), ::tolower);	// convert source to lower-case
	transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);				// convert source to upper-case
	return sampleString.find(toSearch, pos);
}
