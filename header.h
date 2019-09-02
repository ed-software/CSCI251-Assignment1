#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>

// Struct that stores subject data
struct Subject {
    std::string name;
    std::string difficulty;
    std::string variability;
    std::string teacher;
};

// Struct that stores teacher data
struct Teacher {
    std::string name;
    std::string toughness;
    std::string variability;
    std::vector<std::string> subjectList;
};

// Struct that stores student data
struct Student {
    std::string name;
    std::string code;
    std::string ability;
    std::string consistency;
    std::string programName;
    std::vector<std::string> subjectList;
};

// Function declarations
Student getStudents(std::string);
Subject getSubjects(std::string);
Teacher getTeachers(std::string);
double determineMark(double, double);
std::string determineGrade(double);
std::vector<Teacher> allocateTeachers(std::vector<Teacher> *, std::vector<Subject> *);
void printStudents(std::vector<Student>);
void printTeachers(std::vector<Teacher>);
void printSubjects(std::vector<Subject>);
void validateStudents(std::vector<Student>);
void validateTeachers(std::vector<Teacher>);
void validateSubjects(std::vector<Subject>);
