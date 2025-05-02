/*
Program                   Final Project - Personnel Management System
Course name               Arquitectura de Software
Professor's name          Dr. Felipe de Jesús Rdz.
Creation date             22/04/25
Last mod. date            28/04/25

Maintenance log
Name: Santiago López      Mod. date: 04/27/25       Desc: Created main structs 
Name: Ricardo Jáuregui    Mod. date: 04/27/25       Desc: Created functions to read and write files, and process movements
Name: Fabian Orta         Mod. date: 04/27/25       Desc: Created txt files
Name: Paola Loredo        Mod. date: 04/27/25       Desc: Modified Personnel and Movements structs 
Name: Santiago López      Mod. date: 04/27/25       Desc: Modified readMovement function to handle empty fields 
Name: Paola Loredo        Mod. date: 04/27/25       Desc: Modified newEmployee and changeEmployee functions to handle empty fields
Name: Ricardo Jáuregui    Mod. date: 04/27/25       Desc: Modified switch case in personnelMovements function to handle invalid movements 
Name: Santiago López      Mod. date: 04/27/25       Desc: Modified readPersonnel function because of an error in an empty char
Name: Ricardo Jáuregui    Mod. date: 04/27/25       Desc: Added comments to the code
Name: Paola Loredo        Mod. date: 04/27/25       Desc: Finalized the program and cleaned up code
       
Program Description
Update a company's Personnel file with employee transactions and generate a report of the transactions made.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

/*
Content                   Purpose
---------------------------------------------------------------
readField()               Function to read a field from a string stream
start()                   Function to initialize file streams
readMovement()            Function to read a movement from the movements file
readPersonnel()           Function to read a personnel record from the personnel file
copy()                    Function to copy personnel data to the new personnel file
addEmployee()             Function to handle new personnel registration
deleteEmployee()          Function to handle personnel deletion
changeEmployee()          Function to handle personnel data changes
personnelMovements()      Function to process personnel movements
end()                     Function to close file streams
main()                    Main function to control the flow of the program
---------------------------------------------------------------
*/

/*
Purpose of struct Personnel: Define a personnel record with all relevant fields.
*/
struct Personnel {
    /* Variable: worker - Unique identifier for the employee */
    string worker;

    /* Variable: group - Group code within the company */
    string group;

    /* Variable: company - Company code */
    string company;

    /* Variable: plant - Plant code where the employee works */
    string plant;

    /* Variable: department - Department code */
    string department;

    /* Variable: name - Full name of the employee */
    string name;

    /* Variable: key - Status key (e.g., 'O' for old) */
    char key;

    /* Variable: baseSalary - Base salary of the employee */
    double baseSalary;

    /* Variable: startDate - Start date in YYYYMMDD format */
    int startDate;
};

/*
Purpose of struct Movements: Define a movement record to apply to personnel data.
*/
struct Movements {
    /* Variable: ky - Movement type key (A,B,C, etc.) */
    char ky;

    /* Variable: worker - Target employee identifier */
    string worker;

    /* Variable: group - New group code */
    string group;

    /* Variable: company - New company code */
    string company;

    /* Variable: plant - New plant code */
    string plant;

    /* Variable: department - New department code */
    string department;

    /* Variable: name - New employee name */
    string name;

    /* Variable: key - New status key */
    char key;

    /* Variable: baseSalary - New base salary */
    double baseSalary;

    /* Variable: startDate - New start date */
    int startDate;
};

/*
Purpose of file stream flags: Manage input/output across functions.
*/
static ifstream filePer;
static ifstream fileMov;
static ofstream fileNewPer;
static ofstream fileReport;

/* Variable: eofPer - Indicates end of personnel file */
static bool eofPer = false;

/* Variable: eofMov - Indicates end of movements file */
static bool eofMov = false;

/* Variable: per - Current personnel record buffer */
static Personnel per;

/* Variable: mov - Current movement record buffer */
static Movements mov;

/* Variable: exists - Flag if personnel record exists matching movement */
static bool exists = false;

/*
Purpose of readField(): Extracts next comma-separated field from string stream.
*/
string readField(istringstream &ss) {
    string f;
    getline(ss, f, ',');
    return f;
}

/*
Purpose of start(): Open input and output files, verify successful opening.
*/
void start() {
    filePer.open("existingPersonnel.txt");
    fileMov.open("movements.txt");
    fileNewPer.open("newPersonnel.txt");
    fileReport.open("report.txt");
    if (!filePer || !fileMov) {  
        /* Check if files opened successfully */
        cerr << "Error - Failure in initializing input files" << endl;
        exit(EXIT_FAILURE);
    }
}

/*
Purpose of readMovement(): Read and parse next movement record from movements file.
*/
void readMovement() {
    string line;
    if (getline(fileMov, line)) {
        istringstream ss(line);
        mov.ky = readField(ss)[0];
        mov.worker = readField(ss);
        mov.group = readField(ss);
        mov.company = readField(ss);
        mov.plant = readField(ss);
        mov.department = readField(ss);
        string tempKey = readField(ss);
        /* Check if key is empty and assign default */
        if (tempKey.empty()) {
            mov.key = 'O';
        } else {
            mov.key = tempKey[0];
        }
        mov.name = readField(ss);
        mov.baseSalary = stod(readField(ss));
        mov.startDate = stoi(readField(ss));
        eofMov = false;
    } else {
        eofMov = true;
    }
}

/*
Purpose of readPersonnel(): Read and validate next personnel record from personnel file.
*/
void readPersonnel() {
    static string lastWorker = "";
    string line;
    if (getline(filePer, line)) {
        istringstream ss(line);
        per.worker = readField(ss);

        if (!lastWorker.empty() && per.worker < lastWorker) {
            cerr << "Error - Personnel file is not sorted by 'worker' in ascending form." << endl;
            exit(EXIT_FAILURE);
        }
        lastWorker = per.worker;

        per.group = readField(ss);
        per.company = readField(ss);
        per.plant = readField(ss);
        per.department = readField(ss);
        string tempKey = readField(ss);
        /* Check if key is empty and assign default */
        if (tempKey.empty()) {
            per.key = 'O';
        } else {
            per.key = tempKey[0];
        }
        per.name = readField(ss);
        per.baseSalary = stod(readField(ss));
        per.startDate = stoi(readField(ss));
        eofPer = false;
    } else {
        eofPer = true;
    }
}

/*
Purpose of copy(): Write current personnel record to new personnel file.
*/
void copy() {
    fileNewPer << per.worker << ',' << per.group << ',' << per.company << ',' << per.plant << ',' << per.department << ',' << per.key << ',' << per.name << ',' << fixed << setprecision(2) << per.baseSalary << ',' << per.startDate << '\n';
}

/*
Purpose of addEmployee(): Process an 'A' movement to add new employee or log failure.
*/
void addEmployee() {
    if (exists) {
        fileReport << mov.worker << " UNSUCCESSFUL REGISTRATION" << '\n';
        copy();
    } else {
        Personnel np;
        /* Initialize np fields with movement data or defaults */
        np.worker = mov.worker;
        
        if (mov.group.empty()) {
            np.group = "000";
        } else {
            np.group = mov.group;
        }
        
        if (mov.company.empty()) {
            np.company = "000";
        } else {
            np.company = mov.company;
        }
        
        if (mov.plant.empty()) {
            np.plant = "000";
        } else {
            np.plant = mov.plant;
        }
        
        if (mov.department.empty()) {
            np.department = "000000";
        } else {
            np.department = mov.department;
        }
        
        if (mov.key != 'O') {
            np.key = mov.key;
        } else {
            np.key = 'O';
        }
        
        if (mov.name.empty()) {
            np.name = "         ";
        } else {
            np.name = mov.name;
        }
        
        if (mov.baseSalary != 0.0) {
            np.baseSalary = mov.baseSalary;
        } else {
            np.baseSalary = 0.00;
        }
        
        if (mov.startDate != 0) {
            np.startDate = mov.startDate;
        } else {
            /* Default to current date if no startDate provided */
            time_t t = time(nullptr);
            tm* lt = localtime(&t);
            np.startDate = (1900 + lt->tm_year) * 10000 + (1 + lt->tm_mon) * 100 + lt->tm_mday;
        }
        
        fileNewPer << np.worker << ',' << np.group << ',' << np.company << ',' << np.plant << ',' << np.department << ',' << np.key << ',' << np.name << ',' << fixed << setprecision(2) << np.baseSalary << ',' << np.startDate << '\n';
        fileReport << mov.worker << " SUCCESSFUL REGISTRATION" << '\n';
    }
}

/*
Purpose of deleteEmployee(): Process a 'B' movement to delete employee or log failure.
*/
void deleteEmployee() {
    if (exists) {
        fileReport << mov.worker << " SUCCESSFUL DELETION" << '\n';
    } else {
        fileReport << mov.worker << " UNSUCCESSFUL DELETION" << '\n';
    }
}

/*
Purpose of changeEmployee(): Process a 'C' movement to update existing employee data.
*/
void changeEmployee() {
    if (exists) {
        if (!mov.group.empty()) {
            per.group = mov.group;
        }
        if (!mov.company.empty()) {
            per.company = mov.company;
        }
        if (!mov.plant.empty()) {
            per.plant = mov.plant;
        }
        if (!mov.department.empty()) {
            per.department = mov.department;
        }
        if (mov.key != 'O') {
            per.key = mov.key;
        }
        if (!mov.name.empty()) {
            per.name = mov.name;
        }
        if (mov.baseSalary != 0.0) {
            per.baseSalary = mov.baseSalary;
        }
        if (mov.startDate != 0) {
            per.startDate = mov.startDate;
        }
        fileNewPer << per.worker << ',' << per.group << ',' << per.company << ',' << per.plant << ',' << per.department << ',' << per.key << ',' << per.name << ',' << fixed << setprecision(2) << per.baseSalary << ',' << per.startDate << '\n';
        fileReport << mov.worker << " SUCCESSFUL CHANGE" << '\n';
    } else {
        fileReport << mov.worker << " UNSUCCESSFUL CHANGE" << '\n';
    }
}

/*
Purpose of personnelMovements(): Direct movement processing based on movement key.
*/
void personnelMovements() {
    switch (mov.ky) {
        case 'A': 
            addEmployee();   
            break;
        case 'B': 
            deleteEmployee();   
            break;
        case 'C': 
            changeEmployee(); 
            break;
        default:
            /* Invalid movement type */
            fileReport << mov.worker << " INVALID MOVEMENT ARGUMENT" << '\n';
    }
}

/*
Purpose of end(): Close all open file streams.
*/
void end() {
    filePer.close();
    fileMov.close();
    fileNewPer.close();
    fileReport.close();
}

/*
Purpose of main(): Initialize system, process all movements and personnel records, then clean up.
*/
int main() {
    start();
    readMovement();
    readPersonnel();

    while (!eofMov || !eofPer) {
        if (!eofMov && !eofPer) {
            if (mov.worker == per.worker) {
                exists = true;
                personnelMovements();
                readMovement();
                readPersonnel();
            } else if (mov.worker < per.worker) {
                exists = false;
                personnelMovements();
                readMovement();
            } else {
                copy();
                readPersonnel();
            }
        } else if (!eofMov) {
            exists = false;
            readPersonnel();
            readMovement();
        } else {
            copy();
            readPersonnel();
        }
    }

    end();
    return 0;
}