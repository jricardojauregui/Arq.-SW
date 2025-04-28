/*
Programa                  Proyecto Final - Empresa
Nom. materia              Arquitectura de Software
Nom. prof.                Dr. Felipe de Jesús Rdz.
Fecha creac.              22/04/25
Fecha ult. mod.           99/99/99

Bitacora. de mant.
Nom: Santiago López       Fech. Mod.: 4/27/25        Desc: descripcionMod 
Nom: Ricardo Jáuregui     Fech. Mod.: 99/99/99       Desc: descripcionMod 
Nom: Paola Loredo         Fech. Mod.: 99/99/99       Desc: descripcionMod 
Nom: Fabían Orta          Fech. Mod.: 99/99/99       Desc: descripcionMod 
       
Descripción del funcionamiento del programa:
descripcionCorta1
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
leeMovimiento()         Leer datos de movimiento de un archivo        
leePersonal()           Leer datos de personal de un archivo
alta()                  Dar de alta a un empleado
cambio()                Cambiar datos de un empleado
baja()                  Dar de baja a un empleado
copia()                 Copiar datos de un empleado
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

static ifstream finPers;
static ifstream finMov;
static ofstream foutNewPers;
static ofstream foutReporte;

// Control EOF
typedef bool Flag;
static Flag eofPers = false;
static Flag eofMov  = false;

// Registros en memoria
static Personal per;
static Movimiento mov;

typedef istringstream ISS;
string leerCampo(ISS &ss) {
    string f;
    getline(ss, f, ',');
    return f;
}

void abreArchivos() {
    finPers.open("datosExistentesPER.txt");
    finMov.open("datosExistentesMOV.txt");
    foutNewPers.open("nuevo_personal.txt");
    foutReporte.open("reporte.txt");
    
    if (!finPers || !finMov) 
    {
        cerr << "Error al abrir archivos de entrada" << endl;
        exit(EXIT_FAILURE);
    }
}

/*
Objetivo leeMovimiento(): Leer los tipos de movimiento que se pueden realizar dado un archivo
*/

void leeMovimiento(){
    string linea;
    if (getline(finMov, linea)) 
    {
        ISS ss(linea);
        mov.CVE = leerCampo(ss)[0];
        mov.TRAB = leerCampo(ss);
        mov.GPO = leerCampo(ss);
        mov.EMP = leerCampo(ss);
        mov.PTA = leerCampo(ss);
        mov.DEPTO = leerCampo(ss);
        mov.CLAVE = leerCampo(ss)[0];
        mov.NOMB = leerCampo(ss);
        mov.SAL = stod(leerCampo(ss));
        mov.F_ING = stoi(leerCampo(ss));
        eofMov = false;
    } else {
        eofMov = true;
    }
}

void leePersonal(){
    string linea;
    if (getline(finPers, linea)) 
    {
        ISS ss(linea);
        per.TRAB = leerCampo(ss);
        per.GPO = leerCampo(ss);
        per.EMP = leerCampo(ss);
        per.PTA = leerCampo(ss);
        per.DEPTO = leerCampo(ss);
        per.CLAVE = leerCampo(ss)[0];
        per.NOMB = leerCampo(ss);
        per.SAL = stod(leerCampo(ss));
        per.F_ING = stoi(leerCampo(ss));
        eofPers = false;
    } else {
        eofPers = true;
    }
}

void alta() {
    Personal np;
    np.TRAB = mov.TRAB;

    if (!mov.GPO.empty()) 
    {
        np.GPO = mov.GPO;
    } else {
        np.GPO = "G000";
    }

    if (!mov.EMP.empty()) 
    {
        np.EMP = mov.EMP;
    } else {
        np.EMP = "E000";
    }

    if (!mov.PTA.empty()) 
    {
        np.PTA = mov.PTA;
    } else {
        np.PTA = "P000";
    }

    if (!mov.DEPTO.empty()) 
    {
        np.DEPTO = mov.DEPTO;
    } else {
        np.DEPTO = "D000";
    }

    if (mov.CLAVE != 'O') 
    {
        np.CLAVE = mov.CLAVE;
    } else {
        np.CLAVE = 'O';
    }

    if (!mov.NOMB.empty()) {
        np.NOMB = mov.NOMB;
    } else {
        np.NOMB = "N000";
    }

    if (mov.SAL != 0.0) 
    {
        np.SAL = mov.SAL;
    } else {
        np.SAL = 1000.0;
    }

    if (mov.F_ING != 0) 
    {
        np.F_ING = mov.F_ING;
    } else {
        time_t ahora = time(0);
        tm* ltm = localtime(&ahora);

        np.F_ING = (1900 + ltm->tm_year) * 10000 + (1 + ltm->tm_mon) * 100 + ltm->tm_mday;
    }

    foutNewPers << np.TRAB << ',' << np.GPO << ',' << np.EMP << ',' << np.PTA << ',' << np.DEPTO << ',' << np.CLAVE << ',' << np.NOMB << ',' << fixed << setprecision(2) << np.SAL << ',' << np.F_ING << '\n';
    foutReporte << np.TRAB << " ALTA EXITOSA\n";
}


void cambio(){
    if (!mov.GPO.empty()) 
    {
        per.GPO = mov.GPO;
    }
    if (!mov.EMP.empty()) 
    {
        per.EMP = mov.EMP;
    }
    if (!mov.PTA.empty()) 
    {
        per.PTA = mov.PTA;
    }
    if (!mov.DEPTO.empty()) 
    {
        per.DEPTO = mov.DEPTO;
    }
    if (mov.CLAVE != 'O') 
    { 
        per.CLAVE = mov.CLAVE;
    }
    if (!mov.NOMB.empty()) 
    {
        per.NOMB = mov.NOMB;
    }
    if (mov.SAL != 0.0) 
    {
        per.SAL = mov.SAL;
    }
    if (mov.F_ING != 0) 
    {
        per.F_ING = mov.F_ING;
    }

    foutNewPers << per.TRAB << ',' << per.GPO << ',' << per.EMP << ',' << per.PTA << ',' << per.DEPTO << ',' << per.CLAVE << ',' << per.NOMB << ',' << fixed << setprecision(2) << per.SAL << ',' << per.F_ING << '\n';
    foutReporte << per.TRAB << " CAMBIO EXITOSO\n";
       
}

void baja(){
       foutReporte << mov.TRAB << " BAJA EXITOSA\n";
    // No se escribe en nuevo personal (equivale a baja)
}

void copia() {
    foutNewPers << per.TRAB << ',' << per.GPO << ',' << per.EMP << ',' << per.PTA << ',' << per.DEPTO << ',' << per.CLAVE << ',' << per.NOMB << ',' << fixed << setprecision(4) << per.SAL << ',' << per.F_ING << '\n';
}


void movimientoPersonal(){
    switch(mov.CVE) 
    {
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
        foutReporte << "MOVIMIENTO INVALIDO: " << mov.TRAB << '\n';
        break;
    }
}

void termina(){
    finPers.close();
    finMov.close();
    foutNewPers.close();
    foutReporte.close();
}

int main(){
    abreArchivos();
    leeMovimiento();
    leePersonal();

    while (!eofMov || !eofPers) {
        if (!eofMov && !eofPers) 
        {
            if (mov.TRAB == per.TRAB) 
            {
                movimientoPersonal();
                leeMovimiento();
                leePersonal();
            } 
            else if (mov.TRAB < per.TRAB) {
                alta();
                leeMovimiento();
            } 
            else {
                copia();
                leePersonal();
            }
        } 
        else if (!eofMov) {
            alta();
            leeMovimiento();
        } 
        else {
            copia();
            leePersonal();
        }
    }

    termina();
    return 0;
}