/*
Programa                  Proyecto Final - Empresa
Nom. materia              Arquitectura de Software
Nom. prof.                Dr. Felipe de Jesús Rdz.
Fecha creac.              22/04/25
Fecha ult. mod.           28/04/25

Bitacora. de mant.
Nom: Santiago López       Fech. Mod.: 04/27/25       Desc: descripcionMod 
Nom: Ricardo Jáuregui     Fech. Mod.: 04/28/25       Desc: descripcionMod 
Nom: Paola Loredo         Fech. Mod.: 99/99/99       Desc: descripcionMod 
Nom: Fabían Orta          Fech. Mod.: 99/99/99       Desc: descripcionMod 
       
Descripción del funcionamiento del programa:
Actualizar el archivo de Personal de una empresa con Movimientos de empleados y generar un reporte de los movimientos realizados.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

/*
Contenido               Propósito
---------------------------------------------------------------
leerCampo()             Leer un campo de una línea
inicio()                Inicializar archivos de entrada y salida
leeMovimiento()         Leer datos de movimiento de un archivo        
leePersonal()           Leer datos de personal de un archivo
copia()                 Copiar datos de un empleado
alta()                  Dar de alta a un empleado
cambio()                Cambiar datos de un empleado
baja()                  Dar de baja a un empleado
movimientoPersonal()    Switch dependiendo de la opción elegida de los movimientos
termina()               Terminar el programa
main()                  Implementar funciones existentes
*/

struct Personal {
    string TRAB, GPO, EMP, PTA, DEPTO, NOMB;
    char CLAVE;
    double SAL;
    int F_ING;
};

struct Movimiento {
    char CVE;
    string TRAB, GPO, EMP, PTA, DEPTO, NOMB;
    char CLAVE;
    double SAL;
    int F_ING;
};

static ifstream archPer;
static ifstream archMov;
static ofstream archNuevoPer;
static ofstream archReporte;

static bool finPer = false;
static bool finMov = false;

static Personal per;
static Movimiento mov;

static bool existe = false;

string leerCampo(istringstream &ss) {
    string f;
    getline(ss, f, ',');
    return f;
}

void inicio() {
    archPer.open("datosExistentesPER.txt");
    archMov.open("datosExistentesMOV.txt");
    archNuevoPer.open("nuevoPersonal.txt");
    archReporte.open("reporte.txt");
    if (!archPer || !archMov) {
        cerr << "Error al abrir archivos de entrada" << endl;
        exit(EXIT_FAILURE);
    }
}

void leeMovimiento() {
    string linea;
    if (getline(archMov, linea)) {
        istringstream ss(linea);
        mov.CVE = leerCampo(ss)[0];
        mov.TRAB = leerCampo(ss);
        mov.GPO = leerCampo(ss);
        mov.EMP = leerCampo(ss);
        mov.PTA = leerCampo(ss);
        mov.DEPTO = leerCampo(ss);
        string tmp = leerCampo(ss);
        if (tmp.empty()) {
            mov.CLAVE = 'O';
        } else {
            mov.CLAVE = tmp[0];
        }
        mov.NOMB = leerCampo(ss);
        mov.SAL = stod(leerCampo(ss));
        mov.F_ING = stoi(leerCampo(ss));
        finMov = false;
    } else {
        finMov = true;
    }
}

void leePersonal() {
    string linea;
    if (getline(archPer, linea)) {
        istringstream ss(linea);
        per.TRAB = leerCampo(ss);
        per.GPO = leerCampo(ss);
        per.EMP = leerCampo(ss);
        per.PTA = leerCampo(ss);
        per.DEPTO = leerCampo(ss);
        string tmpClave = leerCampo(ss);
        if (tmpClave.empty()) {
            per.CLAVE = 'O';
        } else {
            per.CLAVE = tmpClave[0];
        }
        per.NOMB = leerCampo(ss);
        per.SAL = stod(leerCampo(ss));
        per.F_ING = stoi(leerCampo(ss));
        finPer = false;
    } else {
        finPer = true;
    }
}

void copia() {
    archNuevoPer << per.TRAB << ',' << per.GPO << ',' << per.EMP << ',' << per.PTA << ',' << per.DEPTO << ',' << per.CLAVE << ',' << per.NOMB << ',' << fixed << setprecision(2) << per.SAL << ',' << per.F_ING << '\n';
}

void alta() {
    if (existe) {
        archReporte << mov.TRAB << " ALTA NO EXITOSA" << '\n';
        copia();
    } else {
        Personal np;
        np.TRAB = mov.TRAB;
        if (mov.GPO.empty()) {
            np.GPO = "G000";
        } else {
            np.GPO = mov.GPO;
        }
        if (mov.EMP.empty()) {
            np.EMP = "E000";
        } else {
            np.EMP = mov.EMP;
        }
        if (mov.PTA.empty()) {
            np.PTA = "P000";
        } else {
            np.PTA = mov.PTA;
        }
        if (mov.DEPTO.empty()) {
            np.DEPTO = "D000";
        } else {
            np.DEPTO = mov.DEPTO;
        }
        if (mov.CLAVE != 'O') {
            np.CLAVE = mov.CLAVE;
        } else {
            np.CLAVE = 'O';
        }
        if (mov.NOMB.empty()) {
            np.NOMB = "N000";
        } else {
            np.NOMB = mov.NOMB;
        }
        if (mov.SAL != 0.0) {
            np.SAL = mov.SAL;
        } else {
            np.SAL = 1000.0;
        }
        if (mov.F_ING != 0) {
            np.F_ING = mov.F_ING;
        } else {
            time_t t = time(nullptr);
            tm* tl = localtime(&t);
            np.F_ING = (1900 + tl->tm_year) * 10000 + (1 + tl->tm_mon) * 100 + tl->tm_mday;
        }
        archNuevoPer << np.TRAB << ',' << np.GPO << ',' << np.EMP << ',' << np.PTA << ',' << np.DEPTO << ',' << np.CLAVE << ',' << np.NOMB << ',' << fixed << setprecision(2) << np.SAL << ',' << np.F_ING << '\n';
        archReporte << mov.TRAB << " ALTA EXITOSA" << '\n';
    }
}

void baja() {
    if (existe) {
        archReporte << mov.TRAB << " BAJA EXITOSA" << '\n';
    } else {
        archReporte << mov.TRAB << " BAJA NO EXITOSA" << '\n';
    }
}

void cambio() {
    if (existe) {
        if (!mov.GPO.empty()) {
            per.GPO = mov.GPO;
        }
        if (!mov.EMP.empty()) {
            per.EMP = mov.EMP;
        }
        if (!mov.PTA.empty()) {
            per.PTA = mov.PTA;
        }
        if (!mov.DEPTO.empty()) {
            per.DEPTO = mov.DEPTO;
        }
        if (mov.CLAVE != 'O') {
            per.CLAVE = mov.CLAVE;
        }
        if (!mov.NOMB.empty()) {
            per.NOMB = mov.NOMB;
        }
        if (mov.SAL != 0.0) {
            per.SAL = mov.SAL;
        }
        if (mov.F_ING != 0) {
            per.F_ING = mov.F_ING;
        }
        archNuevoPer << per.TRAB << ',' << per.GPO << ',' << per.EMP << ',' << per.PTA << ',' << per.DEPTO << ',' << per.CLAVE << ',' << per.NOMB << ',' << fixed << setprecision(2) << per.SAL << ',' << per.F_ING << '\n';
        archReporte << mov.TRAB << " CAMBIO EXITOSO" << '\n';
    } else {
        archReporte << mov.TRAB << " CAMBIO NO EXITOSO" << '\n';
    }
}

void movimientoPersonal() {
    switch (mov.CVE) {
        case 'A': 
            alta();   
            break;
        case 'B': 
            baja();   
            break;
        case 'C': 
            cambio(); 
            break;
        default:
            archReporte << mov.TRAB << " MOVIMIENTO INVALIDO" << '\n';
    }
}

void termina() {
    archPer.close();
    archMov.close();
    archNuevoPer.close();
    archReporte.close();
}

int main() {
    inicio();
    leeMovimiento();
    leePersonal();

    while (!finMov || !finPer) {
        if (!finMov && !finPer) {
            if (mov.TRAB == per.TRAB) {
                existe = true;
                movimientoPersonal();
                leeMovimiento();
                leePersonal();
            } else if (mov.TRAB < per.TRAB) {
                existe = false;
                movimientoPersonal();
                leeMovimiento();
            } else {
                copia();
                leePersonal();
            }
        } else if (!finMov) {
            existe = false;
            movimientoPersonal();
            leeMovimiento();
        } else {
            copia();
            leePersonal();
        }
    }

    termina();
    return 0;
}