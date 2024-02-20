#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm> 

enum class StudyField {
    MECHANICAL_ENGINEERING = 1,
    SOFTWARE_ENGINEERING = 2,
    FOOD_TECHNOLOGY = 3,
    URBANISM_ARCHITECTURE = 4,
    VETERINARY_MEDICINE = 5
};

class Student {
private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string enrollmentDate;
    std::string dateOfBirth;

public:
    Student(std::string first_name, std::string last_name, std::string email, std::string enrollment_date, std::string date_of_birth)
        : firstName(first_name), lastName(last_name), email(email), enrollmentDate(enrollment_date), dateOfBirth(date_of_birth) {}

    std::string getEmail() const {
        return email;
    }
};

class Faculty {
private:
    std::string name;
    std::string abbreviation;
    std::vector<Student> students;
    StudyField studyField;

public:
    Faculty(std::string name, std::string abbreviation, StudyField study_field)
        : name(name), abbreviation(abbreviation), studyField(study_field) {}

    void addStudent(const Student& student) {
        students.push_back(student);
    }

    std::string getName() const {
        return name;
    }

    std::vector<Student> getStudents() const {
        return students;
    }

    StudyField getStudyField() const {
        return studyField;
    }

    bool hasStudent(const std::string& studentEmail) const {
        for (const auto& student : students) {
            if (student.getEmail() == studentEmail) {
                return true;
            }
        }
        return false;
    }

    void graduateStudent(const std::string& studentEmail) {
        students.erase(std::remove_if(students.begin(), students.end(),
                                       [&](const Student& student) { return student.getEmail() == studentEmail; }),
                        students.end());
    }

    void displayCurrentEnrolledStudents() const {
        for (const auto& student : students) {
            std::cout << student.getEmail() << std::endl;
        }
    }
};

class University {
private:
    std::vector<Faculty> faculties;

public:
    void createFaculty(std::string name, std::string abbreviation, StudyField study_field) {
        Faculty faculty(name, abbreviation, study_field);
        faculties.push_back(faculty);
    }

    bool assignStudentToFaculty(const std::string& student_email, const std::string& faculty_name) {
        for (auto& faculty : faculties) {
            if (faculty.getName() == faculty_name) {
                Student student(student_email, "", "", "", "");
                faculty.addStudent(student);
                return true;
            }
        }
        return false;
    }

    std::string searchFacultyByStudentEmail(const std::string& student_email) {
        for (const auto& faculty : faculties) {
            if (faculty.hasStudent(student_email)) {
                return faculty.getName();
            }
        }
        return "Student not found";
    }

    void displayUniversityFaculties() {
        for (const auto& faculty : faculties) {
            std::cout << faculty.getName() << std::endl;
        }
    }

    void displayAllFacultiesByField(StudyField study_field) {
        for (const auto& faculty : faculties) {
            if (faculty.getStudyField() == study_field) {
                std::cout << faculty.getName() << std::endl;
            }
        }
    }
void saveState(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& faculty : faculties) {
                file << faculty.getName() << "," << static_cast<int>(faculty.getStudyField()) << ",";
                for (const auto& student : faculty.getStudents()) {
                    file << student.getEmail() << ",";
                }
                file << std::endl;
            }
            file.close();
            std::cout << "State saved successfully." << std::endl;
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }

    void loadState(const std::string& filename) {
        faculties.clear();
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string facultyName, studentEmail;
                int studyFieldInt;
                iss >> facultyName >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                createFaculty(facultyName, "", studyField);
                while (std::getline(iss, studentEmail, ',')) {
                    if (!studentEmail.empty())
                        assignStudentToFaculty(studentEmail, facultyName);
                }
            }
            file.close();
            std::cout << "State loaded successfully." << std::endl;
        } else {
            std::cerr << "Unable to open file." << std::endl;
        }
    }

    void graduateStudent(const std::string& studentEmail) {
        for (auto& faculty : faculties) {
            faculty.graduateStudent(studentEmail);
        }
    }

    void displayGraduates() {
        for (const auto& faculty : faculties) {
            std::cout << "Faculty: " << faculty.getName() << std::endl;
            std::cout << "Graduates:" << std::endl;
        }
    }

    void displayCurrentEnrolledStudents() {
        for (const auto& faculty : faculties) {
            std::cout << "Faculty: " << faculty.getName() << std::endl;
            std::cout << "Current Enrolled Students:" << std::endl;
            faculty.displayCurrentEnrolledStudents();
        }
    }
};

int main() {
    University utmUniversity; 

    while (true) {
        std::cout << "\nFaculty operations:" << std::endl;
        std::cout << "1. Create and assign a student to a faculty" << std::endl;
        std::cout << "2. Graduate a student from a faculty" << std::endl;
        std::cout << "3. Display current enrolled students" << std::endl;
        std::cout << "4. Display graduates" << std::endl;
        std::cout << "5. Tell or not if a student belongs to this faculty" << std::endl;
        std::cout << "\nGeneral operations:" << std::endl;
        std::cout << "6. Create a new faculty" << std::endl;
        std::cout << "7. Search what faculty a student belongs to by a unique identifier" << std::endl;
        std::cout << "8. Display University faculties" << std::endl;
        std::cout << "9. Display all faculties belonging to a field" << std::endl;
        std::cout << "10. Exit" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
switch (choice) {
            case 1: {
                std::string studentEmail, facultyName;
                std::cout << "Enter student email: ";
                std::cin >> studentEmail;
                std::cout << "Enter faculty name to assign the student: ";
                std::cin >> facultyName;
                bool success = utmUniversity.assignStudentToFaculty(studentEmail, facultyName);
                if (success) {
                    std::cout << "Student assigned to faculty successfully." << std::endl;
                } else {
                    std::cout << "Faculty not found." << std::endl;
                }
                break;
            }
            case 2: {
                std::string studentEmail;
                std::cout << "Enter student email to graduate: ";
                std::cin >> studentEmail;
                utmUniversity.graduateStudent(studentEmail);
                std::cout << "Student graduated successfully." << std::endl;
                break;
            }
            case 3: {
                utmUniversity.displayCurrentEnrolledStudents();
                break;
            }
            case 4: {
                utmUniversity.displayGraduates();
                break;
            }
            case 5: {
                std::string studentEmail, facultyName;
                std::cout << "Enter student email: ";
                std::cin >> studentEmail;
                std::cout << "Enter faculty name: ";
                std::cin >> facultyName;
                std::string faculty = utmUniversity.searchFacultyByStudentEmail(studentEmail);
                if (faculty == facultyName) {
                    std::cout << "Yes, the student belongs to this faculty." << std::endl;
                } else {
                    std::cout << "No, the student does not belong to this faculty." << std::endl;
                }
                break;
            }
            case 6: {
                std::string name, abbreviation;
                int studyFieldInt;
                std::cout << "Enter faculty name: ";
                std::cin >> name;
                std::cout << "Enter faculty abbreviation: ";
                std::cin >> abbreviation;
                std::cout << "Enter study field (1 - MECHANICAL_ENGINEERING, 2 - SOFTWARE_ENGINEERING, 3 - FOOD_TECHNOLOGY, 4 - URBANISM_ARCHITECTURE, 5 - VETERINARY_MEDICINE): ";
                std::cin >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                utmUniversity.createFaculty(name, abbreviation, studyField);
                break;
            }
            case 7: {
                std::string studentEmail;
                std::cout << "Enter student email: ";
                std::cin >> studentEmail;
                std::string faculty = utmUniversity.searchFacultyByStudentEmail(studentEmail);
                std::cout << "The student belongs to " << faculty << " faculty." << std::endl;
                break;
            }
            case 8: {
                utmUniversity.displayUniversityFaculties();
                break;
            }
            case 9: {
                int studyFieldInt;
                std::cout << "Enter study field (1 - MECHANICAL_ENGINEERING, 2 - SOFTWARE_ENGINEERING, 3 - FOOD_TECHNOLOGY, 4 - URBANISM_ARCHITECTURE, 5 - VETERINARY_MEDICINE): ";
                std::cin >> studyFieldInt;
                StudyField studyField = static_cast<StudyField>(studyFieldInt);
                std::cout << "Faculties under " << studyFieldInt << ":" << std::endl;
                utmUniversity.displayAllFacultiesByField(studyField);
                break;
            }
            case 10: {
                std::cout << "Exiting..." << std::endl;
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            }
        }
    }

    return 0;
}