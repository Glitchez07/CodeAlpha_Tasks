#include <iostream> 
#include <iomanip>

using namespace std;

int main() {
    int numberOfCourses; // Variable to store the number of courses
    const int MAX_COURSES = 100; // Maximum number of courses allowed
    
    double grades[MAX_COURSES]; // Array to store grade points for each course
    int creditHours[MAX_COURSES]; // Array to store credit hours for each course
    
    double totalGradePoints = 0; // Variable to accumulate total grade points
    int totalCredits = 0; // Variable to accumulate total credit hours

    // Display welcome message and prompt for number of courses
    cout << "====================================================\n";
    cout << "              STUDENT CGPA CALCULATOR               \n";
    cout << "====================================================\n\n";

    cout << "Enter the number of courses (1-" << MAX_COURSES << "): ";
    if (!(cin >> numberOfCourses) || numberOfCourses <= 0 || numberOfCourses > MAX_COURSES) {
        cout << "\nError: Invalid number of courses. Exiting program.\n";
        return 1;
    }

    cout << "\nTaking " << numberOfCourses << " course(s) for evaluation.\n";
    cout << "----------------------------------------------------\n";

    // Loop to gather user input for each course's grade point and credit hours
    for (int i = 0; i < numberOfCourses; i++) {
        cout << "\n--- [ Course " << i + 1 << " ] ---\n";

        // User Input & Validation for Grade Point
        cout << "  Enter grade point (0.00 - 4.00): ";
        cin >> grades[i];
        while (cin.fail() || grades[i] < 0.0 || grades[i] > 4.0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  Invalid entry! Enter a value between 0.0 and 4.0: ";
            cin >> grades[i];
        }

        // User Input & Validation for Credit Hours
        cout << "  Enter credit hours (> 0): ";
        cin >> creditHours[i];
        while (cin.fail() || creditHours[i] <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  Invalid entry! Enter a positive integer for credits: ";
            cin >> creditHours[i];
        }

        // Accumulate total grade points and credits for GPA calculation
        totalGradePoints += grades[i] * creditHours[i]; // Calculate total grade points
        totalCredits += creditHours[i]; // Calculate total credit hours
    }

    // Calculate Final  GPA
    double GPA = totalGradePoints / totalCredits;

    // Display summary output
    cout << "\n====================================================\n";
    cout << "                   GRADE REPORT                     \n";
    cout << "====================================================\n";
    
    cout << left << setw(20) << "Course" 
         << setw(16) << "Grade Point" 
         << setw(16) << "Credit Hours" << "\n";
    cout << "----------------------------------------------------\n";

    // Display course details
    for (int i = 0; i < numberOfCourses; i++) {
        cout << left << "Course " << setw(13) << (i + 1)
             << right << setw(8) << fixed << setprecision(2) << grades[i]
             << setw(16) << creditHours[i] << "\n";
    }

    cout << "----------------------------------------------------\n";
    cout << left << setw(27) << "Total Credits:" << right << setw(20) << totalCredits << "\n";
    cout << "====================================================\n";
    cout << left << setw(27) << "CUMULATIVE GPA:" << right << setw(20) << fixed << setprecision(2) << GPA << "\n";
    cout << "====================================================\n";

    // Academic Standing Summary
    cout << " Academic Standing: ";
    if (GPA >= 3.60) {
        cout << "First Class \n";
    } else if (GPA >= 3.00) {
        cout << "Second Class Upper \n";
    } else if (GPA >= 2.50) {
        cout << "Second Class Lower \n";
    } else if (GPA >= 2.00) {
        cout << "Pass \n";
    } else {
        cout << "Academic Probation / Unsatisfactory\n";
    }
    cout << "====================================================\n\n";

    return 0;
}