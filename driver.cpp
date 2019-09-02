#include "header.h"
using namespace std;

int main(int argc, char* argv[]) {
    // Create file stream variables
    ifstream studentFile;
    ifstream subjectFile;
    ifstream teacherFile;
    ofstream outputFile;

    // Open file stream variables using command line arguments
    studentFile.open(argv[1]);
    subjectFile.open(argv[2]);
    teacherFile.open(argv[3]);
    outputFile.open(argv[4]);

    // Check that the correct number of arguments have been run
    if (argc != 5) {
        cerr << "Incorrect number of arguments" << endl;
        exit(EXIT_FAILURE);
    }

    // Check that files have all opened successfully
    if (!studentFile.is_open()) {
        cerr << "Student file failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if (!subjectFile.is_open()) {
        cerr << "Subject file failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    if (!teacherFile.is_open()) {
        cerr << "Teacher file failed to open" << endl;
        exit(EXIT_FAILURE);
    }

    // Create vectors of structs
    vector<Student> students;
    vector<Subject> subjects;
    vector<Teacher> teachers;
   
    // SETUP STRUCTS
    string lineOfData; // string that is equal to each line of data being converted into a struct

    while (getline(studentFile, lineOfData)) {
        students.push_back(getStudents(lineOfData));
    }

    while (getline(subjectFile, lineOfData)) {
        subjects.push_back(getSubjects(lineOfData));
    }

    while (getline(teacherFile, lineOfData)) {
        teachers.push_back(getTeachers(lineOfData));
    }
    
    // Functions that check that all the data is valid, and reports detailed errors if it isn't
    validateStudents(students);
    validateTeachers(teachers);
    validateSubjects(subjects);

    // PRINT FILE INFORMATION
    cout << "----------STUDENTS----------" << endl; // Add lines to separate each section of data
    printStudents(students);
    cout << "----------TEACHERS----------" << endl;
    printTeachers(teachers);
    cout << "----------SUBJECTS----------" << endl;
    printSubjects(subjects);
    cout << "----------------------------" << endl; 

    // RANDOMLY ALLOCATE TEACHER TO SUBJECT
    vector<Teacher> allocatedTeachers = allocateTeachers(&teachers, &subjects);

    for (int i=0; i < students.size(); i++) { // Run this code for each student

        // Add student name to output file
        outputFile << "Student: " + students[i].name << endl;

        // Add student name to standard out
        cout << "Next student: " + students[i].name << ", " << students[i].programName  << endl;

        // Assume that the student has failed the subject until they have met the criteria to pass
        bool failed = false;

        for (int j=0; j < students[i].subjectList.size(); j++) { // Run this code for every subject of the student
            if (failed == false) { // If they have not yet passed the subject

                // Create and initialize variables for calculating distribution
                int studentAbility = stoi(students[i].ability); 
                int studentConsistency = stoi(students[i].consistency);
                int subjectID = stoi(students[i].subjectList[j]) - 1;
                int subjectDifficulty = stoi(subjects[subjectID].difficulty);
                int subjectVariability = stoi(subjects[subjectID].variability);
                int teacherToughness = stoi(allocatedTeachers[j].toughness); 
                int teacherVariability = stoi(allocatedTeachers[j].variability);
                string teacher = allocatedTeachers[j].name;

                // Calculate mean
                int mean = studentAbility - subjectDifficulty - teacherToughness;

                // Calcualte standard deviation
                int standardDeviation = studentConsistency + subjectVariability + teacherVariability;

                // Create a vector of Teachers that holds a newly generated list of allocated teachers when a subject is failed
                vector<Teacher> newAllocatedTeachers;

                // Determine Mark and Grade
                string grade = "F";
                int attempts = 0;
                while (grade == "F" && attempts < 3) { // While the subject is unpassed and attempts still under 3

                    if (attempts > 0) { // If this is no longer the first attempt, assign a new teacher to the subject
                        newAllocatedTeachers = allocateTeachers(&teachers, &subjects);
                        teacherToughness = stoi(newAllocatedTeachers[j].toughness); 
                        teacherVariability = stoi(newAllocatedTeachers[j].variability);
                        teacher = newAllocatedTeachers[j].name;
                    }
                    
                    // Determine mark
                    double mark = determineMark(mean, standardDeviation);
                    mark+= attempts*5; // Add 5 to the mark on the 2nd attempt, and 10 on the final attempt

                    // Determine grade from mark. This does not include recalculated supplementary grades.
                    grade = determineGrade(mark);

                    // Print this subject attempt to standard out
                    if (attempts>0) {
                        cout << "    Attempting subject again: " << subjects[subjectID].name << endl;
                    } else {
                        cout << "    Next student subject: " << subjects[subjectID].name << endl;
                    }

                    // Increment subject attempts
                    attempts++;

                    // Add data to output file 
                    outputFile << "    Subject: " << subjects[subjectID].name << endl;
                    outputFile << "    Teacher: " << teacher << endl;
                    outputFile << "    Performance: " << round(mark) << "-" << grade << endl;

                    // In case the mark is between 45 and 50 a supplementary grade will need to be calculated
                    if (grade == "WS") {
                        mark = determineMark(mean, standardDeviation) + 5;
                        if (mark >= 50) {
                            outputFile << "    Supplementary Performance: 50-PS" << endl;
                        } else {
                            outputFile << "    Supplementary Performance: " << mark << "-F" << endl;
                        }
                    }

                    // Output distribution parameters to the output file
                    outputFile << "    Mean: " << mean << endl;
                    outputFile << "    Standard Deviation: " << standardDeviation << endl;
                    outputFile << endl;

                    // If the student has failed 3 times print the following
                    if (attempts > 2 && grade == "F") {
                        outputFile << "    Subject failed too many times. Student did not graduate." << endl << endl;
                        cout << "    Subject failed too many times. Student did not graduate." << endl << endl;
                        failed = true;
                    }
                }
            }
        }

        // If the subject has been passed
        if (failed == false) {
            outputFile << "    Successfully graduated." << endl;
            outputFile << endl;

            cout << "    Successfully graduated." << endl;
            cout << endl;
        }
    }
    return 0;
}
