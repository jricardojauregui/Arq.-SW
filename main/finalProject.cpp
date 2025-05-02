/*
Program                   Proyecto Final - companyresa
Class                     Arquitectura de Software
Professor                 Dr. Felipe de Jesús Rdz.
Fecha creac.              22/04/25
Fecha ult. mod.           28/04/25

Bitacora. de mant.
Nom: Santiago López       Fech. Mod.: 04/27/25       Desc: descripcionMod 
Nom: Ricardo Jáuregui     Fech. Mod.: 04/28/25       Desc: descripcionMod 
Nom: Paola Loredo         Fech. Mod.: 99/99/99       Desc: descripcionMod 
Nom: Fabían Orta          Fech. Mod.: 99/99/99       Desc: descripcionMod 
       
Descripción del funcionamiento del programa:
Actualizar el archivo de Personal de una companyresa con Movimientos de companyleados y generar un reporte de los movimientos realizados.
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
newRegister()             Function to handle new personnel registration
deleteRegister()          Function to handle personnel deletion
changeRegister()          Function to handle personnel data changes
personnelMovements()      Function to process personnel movements
end()                     Function to close file streams
main()                    Main function to control the flow of the program
---------------------------------------------------------------
*/

struct Personnel {
    string worker, group, company, plant, department, name;
    char key;
    double baseSalary;
    int startDate;
};

struct Movements {
    char ky;
    string worker, group, company, plant, department, name;
    char key;
    double baseSalary;
    int startDate;
};

static ifstream filePer;
static ifstream fileMov;
static ofstream fileNewPer;
static ofstream fileReport;

static bool eofPer = false;
static bool eofMov = false;

static Personnel per;
static Movements mov;

static bool exists = false;

string readField(istringstream &ss) {
    string f;
    getline(ss, f, ',');
    return f;
}

void start() {
    filePer.open("existingPersonnel.txt");
    fileMov.open("movements.txt");
    fileNewPer.open("newPersonnel.txt");
    fileReport.open("report.txt");
    if (!filePer || !fileMov) {
        cerr << "Error - Failure in initializing input files" << endl;
        exit(EXIT_FAILURE);
    }
}

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
        string tmp = readField(ss);
        if (tmp.empty()) {
            mov.key = 'O';
        } else {
            mov.key = tmp[0];
        }
        mov.name = readField(ss);
        mov.baseSalary = stod(readField(ss));
        mov.startDate = stoi(readField(ss));
        eofMov = false;
    } else {
        eofMov = true;
    }
}

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
        string tmpkey = readField(ss);
        if (tmpkey.empty()) {
            per.key = 'O';
        } else {
            per.key = tmpkey[0];
        }
        per.name = readField(ss);
        per.baseSalary = stod(readField(ss));
        per.startDate = stoi(readField(ss));
        eofPer = false;
    } else {
        eofPer = true;
    }
}

void copy() {
    fileNewPer << per.worker << ',' << per.group << ',' << per.company << ',' << per.plant << ',' << per.department << ',' << per.key << ',' << per.name << ',' << fixed << setprecision(2) << per.baseSalary << ',' << per.startDate << '\n';
}

void newRegister() {
    if (exists) {
        fileReport << mov.worker << " UNSUCCESSFUL REGISTRATION" << '\n';
        copy();
    } else {
        Personnel np;
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
            time_t t = time(nullptr);
            tm* lt = localtime(&t);
            np.startDate = (1900 + lt->tm_year) * 10000 + (1 + lt->tm_mon) * 100 + lt->tm_mday;
        }
        fileNewPer << np.worker << ',' << np.group << ',' << np.company << ',' << np.plant << ',' << np.department << ',' << np.key << ',' << np.name << ',' << fixed << setprecision(2) << np.baseSalary << ',' << np.startDate << '\n';
        fileReport << mov.worker << " SUCCESSFUL REGISTRATION" << '\n';
    }
}

void deleteRegister() {
    if (exists) {
        fileReport << mov.worker << " SUCCESSFUL DELETION" << '\n';
    } else {
        fileReport << mov.worker << " UNSUCCESSFUL DELETION" << '\n';
    }
}

void changeRegister() {
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

void personnelMovements() {
    switch (mov.ky) {
        case 'A': 
            newRegister();   
            break;
        case 'B': 
            deleteRegister();   
            break;
        case 'C': 
            changeRegister(); 
            break;
        default:
            fileReport << mov.worker << " INVALID MOVEMENT ARGUMENT" << '\n';
    }
}

void end() {
    filePer.close();
    fileMov.close();
    fileNewPer.close();
    fileReport.close();
}

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