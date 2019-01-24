#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>


using namespace std;

#define UNASSIGNED 0
#define N 9


bool BuscarNoAsignado(int matriz[N][N], int &fila, int &columna);
bool EsSeguro(int matriz[N][N], int fila, int columna, int numero);

/* asigna valores a todas las ubicaciones no asignadas para resolver el Sudoku */
bool ResolverSudoku(int matriz[N][N])
{
    int fila, columna;
    if (!BuscarNoAsignado(matriz, fila, columna))
       return true;
    for (int numero = 1; numero <= 9; numero++)
    {
        if (EsSeguro(matriz, fila, columna, numero))
        {
            matriz[fila][columna] = numero;
            if (ResolverSudoku(matriz))
                return true;
            matriz[fila][columna] = UNASSIGNED;
        }
    }
    return false;
}

/* Busca en la matriz para encontrar una entrada que aún no esté asignada */
bool BuscarNoAsignado(int matriz[N][N], int &fila, int &columna)
{
    for (fila = 0; fila < N; fila++)
        for (columna = 0; columna < N; columna++)
            if (matriz[fila][columna] == UNASSIGNED)
                return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la fila especificada coincide con el
número dado */
bool VefiricarFila(int matriz[N][N], int fila, int numero)
{
    for (int columna = 0; columna < N; columna++)
        if (matriz[fila][columna] == numero)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la columna especificada coincide
   el numero dado */
bool VerificarColumna(int matriz[N][N], int columna, int numero)
{
    for (int fila = 0; fila < N; fila++)
        if (matriz[fila][columna] == numero)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada dentro del cuadro 3x3 especificado coincide
   el numero dado */
bool VefiricarSubmatriz(int matriz[N][N], int boxStartfila, int boxStartcolumna, int numero)
{
    for (int fila = 0; fila < 3; fila++)
        for (int columna = 0; columna < 3; columna++)
            if (matriz[fila+boxStartfila][columna+boxStartcolumna] == numero)
                return true;
    return false;
}

/* Devuelve si se puede asignar un número a la fila dada, ubicación de columna */
bool EsSeguro(int matriz[N][N], int fila, int columna, int numero)
{
    return !VefiricarFila(matriz, fila, numero) && !VerificarColumna(matriz, columna, numero) &&
           !VefiricarSubmatriz(matriz, fila - fila % 3 , columna - columna % 3, numero);
}

/* Imprime la matriz */
void Mostrarmatriz(int matriz[N][N])
{
    for (int fila = 0; fila < N; fila++)
    {
        for (int columna = 0; columna < N; columna++)
            cout<<matriz[fila][columna]<<"  ";
        cout<<endl;
    }
}

/* Aqui se debe hacer el envio al archivo csv */
void csvmatriz(int matriz[N][N])
{
  ofstream archivo;
  archivo.open("solucionsecuencial.csv");
  for(int g=0;g<N;g++)
    for(int h=0;h<N;h++)
      if(h<8)
        archivo<<matriz[g][h]<<",";
      else
        archivo<<matriz[g][h]<<"\n";
  archivo.close();
}


/* Convierte numero del argv[] que es string a integer */
int ConvertirNumero(string valor)
{
	int n = atoi(valor.c_str());
    return n;
}

/* Verifica si el valor del argv[] es un numero */
bool EsNumero(string valor)
{
	if(ConvertirNumero(valor)==0)
  {
		return true;
	}
	else
  {
		int aux = atoi(valor.c_str());
		if(aux==0)
    {
			return false;
	  }
		else
    {
			return true;
		}
	}
}

void VerEntrada(int matriz[N][N], string argumento, string pos_i, string pos_j, string numero)
{
  int fila,columna,numero1;
  if (argumento.substr(0,1)=="[")//Valida la primera entrada
  {
    cout<<endl;
    while(argumento!="")
    {
      pos_i=argumento.substr(1,1);
      pos_j=argumento.substr(3,1);
      numero=argumento.substr(5,1);
      if(EsNumero(pos_i) && EsNumero(pos_j) && EsNumero(numero) &&
        ConvertirNumero(pos_i)>=0 && ConvertirNumero(pos_i)<9 && ConvertirNumero(pos_j)>=0 &&
        ConvertirNumero(pos_j)<9 && ConvertirNumero(numero)>=1 && ConvertirNumero(numero)<=9)
      {
        fila=ConvertirNumero(pos_i);
        columna=ConvertirNumero(pos_j);
        numero1 = ConvertirNumero(numero);
        matriz[fila][columna]=numero1;
      }
      else
      {
        cout<<"Error de formato, intentalo nuevamente"<<endl<<endl;
        exit(1);
      }
      argumento=argumento.substr(7);
    }
    cout<<endl;
  }
  else
  {
    cout<<"Error de formato, intentalo nuevamente"<<endl<<endl;
    exit(1);
  }
}


/* Main */
int main(int argc, char *argv[])
{
  unsigned t0, t1;
  int matriz[N][N];
  string argumento=argv[1];
	string pos_i,pos_j,numero;
  t0=clock();
  for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		  matriz[i][j]=0;

      VerEntrada(matriz,argumento,pos_i,pos_j,numero);
      if (ResolverSudoku(matriz) == true)
      {
        //Mostrarmatriz(matriz);
        csvmatriz(matriz);
        cout<<"Resultado generado con exito"<<endl;
      }
      else
          cout<<"No existe solucion"<<endl;
      t1 = clock();
      double time = (double(t1-t0)/CLOCKS_PER_SEC);
      cout << "Tiempo de Ejecucion: " << time << endl;
      return 0;
}
