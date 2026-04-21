#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node for the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void insertNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                insertNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                insertNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            insertNode(root, course);
        }
    }

    void printCourseList() const {
        inOrder(root);
    }

    Course search(string courseNumber) const {
        Node* currentNode = root;

        while (currentNode != nullptr) {
            if (currentNode->course.courseNumber == courseNumber) {
                return currentNode->course;
            } else if (courseNumber < currentNode->course.courseNumber) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
        }

        return Course();
    }

    bool isEmpty() const {
        return root == nullptr;
    }
};

// Convert text to uppercase
string toUpperCase(string text) {
    for (char& ch : text) {
        ch = toupper(static_cast<unsigned char>(ch));
    }
    return text;
}

// Load course data from file into BST
bool loadCoursesFromFile(const string& fileName, BinarySearchTree& bst) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    string line;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        if (tokens.size() < 2) {
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(toUpperCase(tokens[i]));
        }

        bst.insert(course);
    }

    inputFile.close();
    cout << "Course data loaded successfully." << endl;
    return true;
}

// Print a single course and its prerequisites
void printSingleCourse(const BinarySearchTree& bst, string courseNumber) {
    courseNumber = toUpperCase(courseNumber);
    Course course = bst.search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}

int main() {
    BinarySearchTree courseTree;
    string fileName = "CS 300 ABCU_Advising_Program_Input.csv";
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!dataLoaded) {
                    dataLoaded = loadCoursesFromFile(fileName, courseTree);
                } else {
                    cout << "Course data has already been loaded." << endl;
                }
                break;

            case 2:
                if (!dataLoaded) {
                    cout << "Please load the data file first." << endl;
                } else {
                    cout << endl;
                    cout << "Here is a sample schedule:" << endl;
                    courseTree.printCourseList();
                }
                break;

            case 3:
                if (!dataLoaded) {
                    cout << "Please load the data file first." << endl;
                } else {
                    string courseNumber;
                    cout << "What course do you want to know about? ";
                    cin >> courseNumber;
                    cout << endl;
                    printSingleCourse(courseTree, courseNumber);
                }
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}
