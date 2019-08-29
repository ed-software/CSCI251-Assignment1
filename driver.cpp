#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>

#include "header.h"
using namespace std;

int main(int argc, char* argv[]) {
    ifstream studentFile;
    ifstream subjectFile;
    ifstream teacherFile;
    ofstream outputFile;

    studentFile.open(argv[1]);
    subjectFile.open(argv[2]);
    teacherFile.open(argv[3]);
    outputFile.open(argv[4]);

    vector<Student> students;
    vector<Subject> subjects;
    vector<Teacher> teachers;
   
    string lineString;

    // SETUP STRUCTS

    while (getline(studentFile, lineString)) {
        students.push_back(getStudents(lineString));
    }

    while (getline(subjectFile, lineString)) {
        subjects.push_back(getSubjects(lineString));
    }

    while (getline(teacherFile, lineString)) {
        teachers.push_back(getTeachers(lineString));
    }

    // PRINT FILE INFORMATION

    cout << "----------STUDENTS----------" << endl;
    for (int i=0;i<students.size();i++) {
        cout << students[i].name << " " << students[i].code << " " << students[i].ability << " " << students[i].consistency << " " << students[i].programName;
        
        for (int j=0;j<students[i].subjectList.size();j++) {
            cout << " " << students[i].subjectList[j];
        }

        cout << endl;
    }

    cout << "----------TEACHERS----------" << endl;
    for (int i=0;i<teachers.size();i++) {
        cout << teachers[i].name << " " << teachers[i].toughness << " " << teachers[i].variability;
        
        for (int j=0;j<teachers[i].subjectList.size();j++) {
            cout << " " << teachers[i].subjectList[j];
        }

        cout << endl;
    }

    cout << "----------SUBJECTS----------" << endl;
    for (int i=0;i<subjects.size();i++) {
        cout << subjects[i].name << " " << subjects[i].difficulty << " " << subjects[i].variability;

        cout << endl;
    }

    // RANDOMLY ALLOCATE TEACHER TO SUBJECT

    uniform_int_distribution<unsigned> uniform(0, teachers.size() - 1);
    default_random_engine randEng;
    randEng.seed(time(0));
        
    for (int i=0; i < subjects.size(); i++) {
        subjects[i].teacher = teachers[uniform(randEng)].name;
    }

    cout << "-------------------" << endl;

    
    // APPLY STUDENT AND TEACHER MODIFIERS TO SUBJECT DISTRIBUTION
    
    for (int i=0; i < students.size(); i++) {
        // For every student
        int studentAbility = stoi(students[i].ability); 
        int studentConsistency = stoi(students[i].consistency);

        for (int j=0; j < students[i].subjectList.size(); j++) {
            // For every subject of the student
            int subjectID = stoi(students[i].subjectList[j]) - 1;
            int subjectDifficulty = stoi(subjects[subjectID].difficulty);
            int subjectVariability = stoi(subjects[subjectID].variability);

            string teacher = subjects[subjectID].teacher;
            int teacherToughness; 
            int teacherVariability;

            for (int k=0; k<teachers.size(); k++) {
                if (teachers[k].name == teacher) {
                    teacherToughness = stoi(teachers[k].toughness);
                    teacherVariability = stoi(teachers[k].variability);
                }
            }

            int mean = studentAbility - subjectDifficulty - teacherToughness;

            int standardDeviation = studentConsistency + subjectVariability + teacherVariability;


            // DETERMINE MARK
            normal_distribution<> normal(mean, standardDeviation);
            default_random_engine randEng;
            randEng.seed(time(0));
            double mark = normal(randEng);

            if (mark > 100) {
                mark = 100;
            } else if (mark < 0) {
                mark = 0;
            }

            string grade;

            if (mark >= 0 && mark <= 44) {
                grade = "F";
            } else if (mark > 44 && mark <= 49) {
                mark = normal(randEng) + 5;
                if (mark < 50) {
                    grade = "F";
                } else if (mark >= 50) {
                    grade = "50-PS";
                }
            } else if (mark > 49 && mark <= 64) {
                grade = "P";
            } else if (mark > 64 && mark <= 74) {
                grade = "C";
            } else if (mark > 74 && mark <= 84) {
                grade = "D";
            } else if (mark > 84) {
                grade = "HD";
            }

            cout << students[i].name;
            cout << ", " << subjects[subjectID].name;
            cout << ", " << teacher;
            cout << ", " << mean;
            cout << ", " << standardDeviation;
            cout << ", " << mark << ", " << grade << endl;

        }
    }

    return 0;
}

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


Subject getSubjects(string lineString) {
    Subject subject;
    subject.name = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    subject.difficulty = lineString.substr(0, lineString.find(","));
    lineString = lineString.substr(lineString.find(",") + 1, lineString.length());

    subject.variability = lineString;

    return subject;
}

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
