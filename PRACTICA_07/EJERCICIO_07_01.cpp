// Materia: Programación I, Paralelo 4
// Autor: Leonardo Delgado Medrano
// Fecha creación: 21/11/2023
// Fecha modificación: 21/11/2023
// Número de ejericio: 1
// Problema planteado: Escribe un programa que tenga un menú de tres opciones:
#include <iostream>
#include <fstream>
#include<cstdlib>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;

const char* Nombre_Archivo = "EquiposLPFB.bin";
const char* Nombre_Archivo_Resultado = "ResultadosLPFB.bin";

struct sEquiposLPFB
{
    char nombreEquipo[50];
    char colorCamiseta[50];
    char departamento[30];
    int anioCreacion;
};

struct sResultadosLPFB
{
    char nombreEquipoLocal[50];
    char nombreEquipoVisitante[50];
    int golesEquipoLocal;
    int golesEquipoVisitante;
    char fechaPartido[10];
};

struct sTablaDePosiciones
{
    char nombreEquipo[50];
    int cantidadpartidos;
    int ganados;
    int perdidos;
    int golesFavor;
    int golesContra;
};
int menu ();
// Funcion para ingresar nuevos equipos
sEquiposLPFB IngresarEquipo ();
// Funcion para ingresar resultados
sResultadosLPFB IngresarResultado ();
//Funcion para añadir equipos al archivo binario
void EscribirArchivoBinarioEquipo (sEquiposLPFB);
//Funcion para añadir resultados al archivo binario
void EscribirArchivoBinarioResultado (sResultadosLPFB);
//Busca que los equipos que se enfrenten si se encuentren en la liga
bool buscarDatoArchivoBinario(char [50]);
//copia los datos de los equipos a la tabla de posiciones
vector<sTablaDePosiciones> LeerArchivo ();
//Llena los datos de la tabla
void LlenarTabla(vector<sTablaDePosiciones>&);
//Imprime la tabla de posiciones
void Imprimir (vector<sTablaDePosiciones>);
//ordenar por puntos
void OrdenarPorPuntos (vector<sTablaDePosiciones>&);
//ordenar por diferencia de goles
void OrdenarPorDG (vector<sTablaDePosiciones>&);
int main ()
{
    int opcion;
    do
    {
        opcion = menu();
        switch (opcion)
	    {
	    case 1:
	    	while(true)
	    	{
	    		sEquiposLPFB nuevo = IngresarEquipo();
		    	if (buscarDatoArchivoBinario(nuevo.nombreEquipo))
		    	{
		        	EscribirArchivoBinarioEquipo(nuevo);
		        	break;
				}
				else
					cout<<"Datos no validos"<<endl;
			}
	        break;
	    case 2:
	    	{
	    		bool nt=true;
		        while(nt)
		    	{
		    		sResultadosLPFB nuevoresultado = IngresarResultado();
			    	if (buscarDatoArchivoBinario(nuevoresultado.nombreEquipoLocal)==false && buscarDatoArchivoBinario(nuevoresultado.nombreEquipoVisitante)==false && strcmp(nuevoresultado.nombreEquipoLocal,nuevoresultado.nombreEquipoVisitante)!=0)
			    	{
			        	EscribirArchivoBinarioResultado(nuevoresultado);
			        	nt = false;
					}
					else
						cout<<"Uno o ambos equipos no forma(n) parte de la liga, o se ingreso el mismo equipo de local y visitante"<<endl;
				}
		        break;
			}
	    case 3:
	        {
	            vector<sTablaDePosiciones> Tablapos;
                Tablapos = LeerArchivo();
                LlenarTabla(Tablapos);
                OrdenarPorPuntos(Tablapos);
                OrdenarPorDG(Tablapos);
                Imprimir(Tablapos);
	        }
	        break;
	    case 4:
	        cout<<"Salio del sistema"<<endl;
	        break;
	    default:
	        cout<<"Opcion no valida"<<endl;
	        break;
	    }

    }while (opcion!=4);
    return 0;
}
void OrdenarPorDG (vector<sTablaDePosiciones> &tab)
{
    for(int i=0;i<tab.size();i++)
    {
        for(int j=0;j<tab.size();j++)
        {
            if((tab[i].golesFavor-tab[i].golesContra>tab[j].golesFavor-tab[j].golesContra) && ((tab[i].ganados*3)+(tab[i].cantidadpartidos-tab[i].ganados-tab[i].perdidos)==(tab[j].ganados*3)+(tab[j].cantidadpartidos-tab[j].ganados-tab[j].perdidos)))
            {
                sTablaDePosiciones aux=tab[i];
                tab[i]=tab[j];
                tab[j]=aux;
            }
        }
    }
}
void OrdenarPorPuntos (vector<sTablaDePosiciones> &tab)
{
    for(int i=0;i<tab.size();i++)
    {
        for(int j=0;j<tab.size();j++)
        {
            if((tab[i].ganados*3)+(tab[i].cantidadpartidos-tab[i].ganados-tab[i].perdidos)>(tab[j].ganados*3)+(tab[j].cantidadpartidos-tab[j].ganados-tab[j].perdidos))
            {
                sTablaDePosiciones aux=tab[i];
                tab[i]=tab[j];
                tab[j]=aux;
            }
        }
    }
}
void Imprimir (vector<sTablaDePosiciones> tab)
{
    cout<<"\t\tTABLA DE POSICIONES"<<endl;
    cout<<"================================================================="<<endl;
    cout<<"Club"<<"\t\t\t"<<"PJ\tG\tE\tP\tGF\t\GC\tDG\tPts"<<endl;
    for(int i=0;i<tab.size();i++)
    {
        cout<<tab[i].nombreEquipo<<"\t\t";
        if(strlen(tab[i].nombreEquipo)<10)
            cout<<"\t";
        cout<<tab[i].cantidadpartidos<<"\t"<<tab[i].ganados<<"\t"<<tab[i].cantidadpartidos-tab[i].ganados-tab[i].perdidos<<"\t"<<tab[i].perdidos<<"\t";
        cout<<tab[i].golesFavor<<"\t"<<tab[i].golesContra<<"\t"<<tab[i].golesFavor-tab[i].golesContra<<"\t"<<(tab[i].ganados*3)+(tab[i].cantidadpartidos-tab[i].ganados-tab[i].perdidos)<<endl;
    }
    cout<<endl<<endl;
}
void LlenarTabla (vector<sTablaDePosiciones> &Tabla)
{
    ifstream ArchivoLectura;
    sResultadosLPFB resul;
    for(int i=0;i<Tabla.size();i++)
    {
        ArchivoLectura.open(Nombre_Archivo_Resultado, ios::in | ios::binary);
        while(ArchivoLectura.read((char*)&resul, sizeof(sResultadosLPFB)))
        {
            if(strcmp(Tabla[i].nombreEquipo,resul.nombreEquipoLocal)==0)
            {
                Tabla[i].cantidadpartidos++;
                Tabla[i].golesFavor+=resul.golesEquipoLocal;
                Tabla[i].golesContra+=resul.golesEquipoVisitante;
                if(resul.golesEquipoLocal>resul.golesEquipoVisitante)
                    Tabla[i].ganados++;
                else if (resul.golesEquipoLocal<resul.golesEquipoVisitante)
                    Tabla[i].perdidos++;
            }
            else if (strcmp(Tabla[i].nombreEquipo,resul.nombreEquipoVisitante)==0)
            {
                Tabla[i].cantidadpartidos++;
                Tabla[i].golesFavor+=resul.golesEquipoVisitante;
                Tabla[i].golesContra+=resul.golesEquipoLocal;
                if(resul.golesEquipoLocal<resul.golesEquipoVisitante)
                    Tabla[i].ganados++;
                else if (resul.golesEquipoLocal>resul.golesEquipoVisitante)
                    Tabla[i].perdidos++;
            }
        }
        ArchivoLectura.close();
    }
}
vector<sTablaDePosiciones> LeerArchivo ()
{
	sEquiposLPFB equipo;
	vector<sTablaDePosiciones> Tabla;
	ifstream archivoLectura;
    archivoLectura.open(Nombre_Archivo,ios::in | ios::binary);
    while (archivoLectura.read((char*)&equipo, sizeof(sEquiposLPFB)))
    {
    	sTablaDePosiciones nuevo;
    	strcpy(nuevo.nombreEquipo,equipo.nombreEquipo);
    	nuevo.cantidadpartidos = 0;
    	nuevo.ganados = 0;
    	nuevo.perdidos = 0;
    	nuevo.golesFavor = 0;
    	nuevo.golesContra = 0;
    	Tabla.push_back(nuevo);
    }
    archivoLectura.close();
    return Tabla;
}
bool buscarDatoArchivoBinario(char buscar[50])
{
    ifstream archivoLectura;
    sEquiposLPFB equipo;
    archivoLectura.open(Nombre_Archivo,ios::in | ios::binary);
    while (archivoLectura.read((char*)&equipo, sizeof(sEquiposLPFB)))
    {
        if (strcmp(buscar, equipo.nombreEquipo) == 0)
        {
        	archivoLectura.close();
            return false;
		}
    }
    archivoLectura.close();
    return true;
}
void EscribirArchivoBinarioResultado (sResultadosLPFB re)
{
	ofstream archivoEscritura;
    archivoEscritura.open(Nombre_Archivo_Resultado,ios::app | ios::binary);
    archivoEscritura.write((char*)&re, sizeof(sResultadosLPFB));
    archivoEscritura.close();
}
void EscribirArchivoBinarioEquipo (sEquiposLPFB eq)
{
	ofstream archivoEscritura;
    archivoEscritura.open(Nombre_Archivo,ios::app | ios::binary);
    archivoEscritura.write((char*)&eq, sizeof(sEquiposLPFB));
    archivoEscritura.close();
}
sResultadosLPFB IngresarResultado ()
{
    fflush(stdin);
	sResultadosLPFB e;
	cout<<"Ingrese el nombre del equipo local: ";
	cin.getline(e.nombreEquipoLocal, 50);
	cout<<"Ingrese el nombre del equipo visitante: ";
	cin.getline(e.nombreEquipoVisitante, 50);
	cout<<"Ingrese la cantidad del goles del equipo local: ";
	cin>>e.golesEquipoLocal;
	cout<<"Ingrese la cantidad del goles del equipo visitante: ";
	cin>>e.golesEquipoVisitante;
	cout<<"Ingrese la fecha del partido: ";
	cin>>e.fechaPartido;
	return e;
}
sEquiposLPFB IngresarEquipo ()
{
	fflush(stdin);
	sEquiposLPFB e;
	cout<<"Ingrese el nombre del equipo: ";
	cin.getline(e.nombreEquipo, 50);
	cout<<"Ingrese el color de la camiseta del equipo: ";
	cin.getline(e.colorCamiseta, 50);
	cout<<"Ingrese el departamento del equipo: ";
	cin.getline(e.departamento, 30);
	cout<<"Ingrese el anio de creacion del equipo: ";
	cin>>e.anioCreacion;
	return e;
}
int menu ()
{
    int opcion;
    cout<<"SISTEMA DE EQUIPOS DE LA LPFB"<<endl;
    cout<<"\t========================="<<endl;
    cout<<"\t1. Ingresar datos de un equipo"<<endl;
    cout<<"\t2. Ingresar resultado de los partidos"<<endl;
    cout<<"\t3. Reporte de tabla de posiciones"<<endl;
    cout<<"\t4. Salir"<<endl;
    cout<<"Opcion a realizar: ";
    cin>>opcion;
    return opcion;
}

