#include "header.h"
using namespace std;

// Function that returns a single Student struct from an individual line from the input file
Student getStudents(string lineString) {
    Student student;

    student.name = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    student.code = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    student.ability = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    student.consistency = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    student.programName = lineString.substr(0, lineString.find(":"));
    lineString = lineString.substr(lineString.find(":") + 1, lineString.length());

    while (lineString.find(",") != string::npos) {
        student.subjectList.push_back(lineString.substr(0, lineString.find(",")));
        lineString = lineString.substr(lineString.find(",") + 1, lineString.length());
    }
    student.subjectList.push_back(lineString);

    return student;
}

// Function that returns a single Subject struct from an individual line from the input file
Subject getSubjects(string lineString) {
    Subject subject;
    subject.name = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    subject.difficulty = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    subject.variability = lineString;

    return subject;
}

// Function that returns a single Teacher struct from an individual line from the input file
Teacher getTeachers(string lineString) {
    Teacher teacher;

    teacher.name = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    teacher.toughness = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    teacher.variability = lineString.substr(0, lineString.find(":"));
    lineString = lineString.substr(lineString.find(":") + 1, lineString.length());

    while (lineString.find(",") != string::npos) {
        teacher.subjectList.push_back(lineString.substr(0, lineString.find(",")));
        lineString = lineString.substr(lineString.find(",") + 1, lineString.length());
    }
    teacher.subjectList.push_back(lineString);

    return teacher;
}

// Function that returns a vector list of Teacher structs, randomized and to the size of the subject list
// Each teacher can only be assigned to a subject they are able to teach
vector<Teacher> allocateTeachers(vector<Teacher> *teachers, vector<Subject> *subjects) {

    vector<Teacher> allocatedTeacherList; // The vector that will be returned by the function at the end
    vector<int> rejectedTeacherList; // When a teacher cannot teach a subject they have been randomly assigned to they are put in this list

    uniform_int_distribution<unsigned> uniform(0, (*teachers).size() - 1); // Create a uniform distribution the size of the teacher list
    default_random_engine randEng; 
    randEng.seed(random_device{}()); // Seed the random number generator

    for (int i=0; i < (*subjects).size(); i++) { // Loop through each subject
        bool allocated = false; // Initially set allocated to false

        rejectedTeacherList.clear(); // Start with a fresh list for each subject

        while (allocated == false) { // Loop until we have found and allocated a teacher
            int randomTeacher = uniform(randEng); // Pick a random teacher

            // If no teachers can teach this subject then abort
            if (rejectedTeacherList.size() == (*teachers).size()) { 
                cerr << "No teacher assigned to subject " << i+1 << ", " << (*subjects)[i].name << endl;
                cerr << "Terminating program " << endl;
                exit(EXIT_FAILURE);
            }

            // Check if a teacher is able to teach a subject before assigning them to it
            for (int j=0; j<(*teachers)[randomTeacher].subjectList.size(); j++) { // For each subject in this teacher's list
                if (stoi((*teachers)[randomTeacher].subjectList[j]) == i + 1) { // If this subject is equal to one in their list
                    allocatedTeacherList.push_back((*teachers)[randomTeacher]); // Allocate this teacher
                    allocated = true; // Now we can set allocated to true
                } else {
                    // Add teacher to list of teachers that cannot teach this subject 
                    // Do this uniquely so that the same teacher is not duplicated in the list
                    bool found = false;
                    for (int i=0; i<rejectedTeacherList.size();i++) {
                        if (rejectedTeacherList[i] == randomTeacher) {
                            found = true;
                        }
                    }

                    if (found == false) {
                        rejectedTeacherList.push_back(randomTeacher); // Add this teacher to the rejected list
                    }
                }
            }
        }
    }
    return allocatedTeacherList;
}

void printStudents(vector<Student> students) { // Print all student data stored in each struct
    for (int i=0;i<students.size();i++) {
        cout << students[i].name << " " << students[i].code << " " << students[i].ability << " " << students[i].consistency << " " << students[i].programName;
        
        for (int j=0;j<students[i].subjectList.size();j++) {
            cout << " " << students[i].subjectList[j];
        }

        cout << endl;
    }
}

void printTeachers(vector<Teacher> teachers) { // Print all teacher data stored in each struct
    for (int i=0;i<teachers.size();i++) {
        cout << teachers[i].name << " " << teachers[i].toughness << " " << teachers[i].variability;
        
        for (int j=0;j<teachers[i].subjectList.size();j++) {
            cout << " " << teachers[i].subjectList[j];
        }

        cout << endl;
    }
}

void printSubjects(vector<Subject> subjects) { // Print all subject data stored in each struct
    for (int i=0;i<subjects.size();i++) {
        cout << subjects[i].name << " " << subjects[i].difficulty << " " << subjects[i].variability;

        cout << endl;
    }
}

double determineMark(double mean, double standardDeviation) { // Generate a random mark based on distribution parameters
    normal_distribution<> normal(mean, standardDeviation);
    default_random_engine randEng;
    randEng.seed(random_device{}());
    double mark = normal(randEng);

    // Ensure mark is between 0 and 100
    if (mark > 100) {
        mark = 100;
    } else if (mark < 0) {
        mark = 0;
    }

    return mark;
}

string determineGrade(double mark) { // Determine grade based on mark, not including the recalculation for supplementary exams
    string grade = "F"; // Assume the students are going to fail

    if (mark >= 0 && mark <= 44) {
        grade = "F";
    } else if (mark > 44 && mark <= 49) {
        grade = "WS";
    } else if (mark > 49 && mark <= 64) {
        grade = "P";
    } else if (mark > 64 && mark <= 74) {
        grade = "C";
    } else if (mark > 74 && mark <= 84) {
        grade = "D";
    } else if (mark > 84) {
        grade = "HD";
    }
    
    return grade;
}

// A function that validates the student struct
// Prints an error and aborts if a problem is noticed
void validateStudents(vector<Student> students) {
    if (students.size() > 10) {
        cerr << "Error in Student file" << endl;
        cerr << "Maximum 10 students" << endl;
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<students.size();i++) {

        if (students[i].subjectList.size() > 10) {
            cerr << "Error on Student file, line " << i+1 << " in the subject list column" << endl;
            cerr << "Too many subjects assigned to this student";
            cerr << endl;
            exit(EXIT_FAILURE);
        }

        if (students[i].name.empty()) {
            cerr << "Error on Student file, line " << i+1 << " in the name column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (students[i].code.empty()) {
            cerr << "Error on Student file, line " << i+1 << " in the code column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (students[i].code.length() != 6) {
            cerr << "Error on Student file, line " << i+1 << " in the code column" << endl;
            cerr << "Student code must be exactly 6 characters long";
            cerr << endl;
            exit(EXIT_FAILURE);
        }

        if (students[i].ability.empty()) {
            cerr << "Error on Student file, line " << i+1 << " in the ability column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(students[i].ability) < 0 || stoi(students[i].ability) > 100) {
            cerr << "Error on Student file, line " << i+1 << " in the ability column" << endl;
            cerr << "Ability must be a value between 0 and 100";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (students[i].consistency.empty()) {
            cerr << "Error on Student file, line " << i+1 << " in the consistency column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(students[i].consistency) < 0 || stoi(students[i].consistency) > 15) {
            cerr << "Error on Student file, line " << i+1 << " in the consistency column" << endl;
            cerr << "Consistency must be a value between 0 and 15";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (students[i].programName.empty()) {
            cerr << "Error on Student file, line " << i+1 << " in the program name column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        for (int j=0;j<students[i].subjectList.size();j++) {
            if (students[i].subjectList[j].empty()) {
                cerr << "Error on Student file, line " << i+1 << " in the subject list, subject number " << j << endl;
                cerr << "Field cannot be empty";
                cerr << endl;
                exit(EXIT_FAILURE);
            }

            if (students[i].subjectList[j].find_first_not_of("0123456789") != string::npos) {
                cerr << "Error on Student file, line " << i+1 << " in the subject list, subject number " << j << endl;
                cerr << "Subject must be an integer";
                cerr << endl;
                exit(EXIT_FAILURE);
                
            }
        }
    }
}

void validateTeachers(vector<Teacher> teachers) {
if (teachers.size() > 10) {
        cerr << "Error in Teacher file" << endl;
        cerr << "Maximum 10 teachers" << endl;
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<teachers.size();i++) {

        if (teachers[i].subjectList.size() > 10) {
            cerr << "Error on Teacher file, line " << i+1 << " in the subject list column" << endl;
            cerr << "Too many subjects assigned to this teacher";
            cerr << endl;
            exit(EXIT_FAILURE);
        }

        if (teachers[i].name.empty()) {
            cerr << "Error on Teacher file, line " << i+1 << " in the name column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (teachers[i].toughness.empty()) {
            cerr << "Error on Teacher file, line " << i+1 << " in the toughness column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(teachers[i].toughness) < -15 || stoi(teachers[i].toughness) > 15) {
            cerr << "Error on Teacher file, line " << i+1 << " in the toughness column" << endl;
            cerr << "Toughness must be a value between -15 and 15";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (teachers[i].variability.empty()) {
            cerr << "Error on Teacher file, line " << i+1 << " in the variability column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(teachers[i].variability) < -3 || stoi(teachers[i].variability) > 3) {
            cerr << "Error on Teacher file, line " << i+1 << " in the variability column" << endl;
            cerr << "Variability must be a value between -3 and 3";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        for (int j=0;j<teachers[i].subjectList.size();j++) {
            if (teachers[i].subjectList[j].empty()) {
                cerr << "Error on Teacher file, line " << i+1 << " in the subject list, subject number " << j << endl;
                cerr << "Field cannot be empty";
                cerr << endl;
                exit(EXIT_FAILURE);
            }

            if (teachers[i].subjectList[j].find_first_not_of("0123456789") != string::npos) {
                cerr << "Error on Teacher file, line " << i+1 << " in the subject list, subject number " << j << endl;
                cerr << "Subject must be an integer";
                cerr << endl;
                exit(EXIT_FAILURE);
                
            }
        }
    }
}

void validateSubjects(vector<Subject> subjects) {
    if (subjects.size() > 10) {
        cerr << "Error in Subject file" << endl;
        cerr << "Maximum 10 subjects" << endl;
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<subjects.size();i++) {
        if (subjects[i].name.empty()) {
            cerr << "Error on Subject file, line " << i+1 << " in the name column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (subjects[i].difficulty.empty()) {
            cerr << "Error on Subject file, line " << i+1 << " in the difficulty column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(subjects[i].difficulty) < -15 || stoi(subjects[i].difficulty) > 15) {
            cerr << "Error on Subject file, line " << i+1 << " in the difficulty column" << endl;
            cerr << "Difficulty must be a value between -15 and 15";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (subjects[i].variability.empty()) {
            cerr << "Error on Subject file, line " << i+1 << " in the variability column" << endl;
            cerr << "Field cannot be empty";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 

        if (stoi(subjects[i].variability) < -3 || stoi(subjects[i].variability) > 3) {
            cerr << "Error on Subject file, line " << i+1 << " in the variability column" << endl;
            cerr << "Variability must be a value between -3 and 3";
            cerr << endl;
            exit(EXIT_FAILURE);
        } 
    }
}
