struct Subject {
    std::string name;
    std::string difficulty;
    std::string variability;
    std::string teacher;
};

struct Teacher {
    std::string name;
    std::string toughness;
    std::string variability;
    std::vector<std::string> subjectList;
};

struct Student {
    std::string name;
    std::string code;
    std::string ability;
    std::string consistency;
    std::string programName;
    std::vector<std::string> subjectList;
};

Student getStudents(std::string);
Subject getSubjects(std::string);
Teacher getTeachers(std::string);