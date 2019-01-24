#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <mpi.h>

/*
mpirun MpiSudokuParalelo "[0;3;4][0;5;2][0;8;3][1;0;3][1;6;4][1;7;2][1;8;9][2;1;2][2;5;7][2;7;5][3;1;4][3;2;7][3;3;2][3;5;5][3;7;3][3;8;8][4;5;8][4;8;7][5;1;8][5;2;5][5;5;9][5;7;4][5;8;1][6;3;8][6;6;6][7;0;2][7;1;9][7;2;3][7;8;4][8;0;8][8;1;6]"
*/


using namespace std;

#define UNASSIGNED 0
#define N 9



bool BuscarNoAsignado(int **matriz, int &fila, int &columna);
bool EsSeguro(int **matriz, int fila, int columna, int numero);

/*
asigna valores a todas las ubicaciones no asignadas para resolver el Sudoku
 */
bool ResolverSudoku(int **matriz)
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

/* Busca en la matriz para encontrar una entrada que aún no esté asignada. */
bool BuscarNoAsignado(int **matriz, int &fila, int &columna)
{
    for (fila = 0; fila < N; fila++)
        for (columna = 0; columna < N; columna++)
            if (matriz[fila][columna] == UNASSIGNED)
                return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la fila especificada coincide con el número dado.
 */
bool VerificarFila(int **matriz, int fila, int numero)
{
    for (int columna = 0; columna < N; columna++)
        if (matriz[fila][columna] == numero)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la columna especificada coincide
   el numero1 dado */
bool VerificarColumna(int **matriz, int columna, int numero)
{
    for (int fila = 0; fila < N; fila++)
        if (matriz[fila][columna] == numero)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada dentro del cuadro 3x3 especificado coincide
   el numero1 dado */
bool VerificarSubmatriz(int **matriz, int boxStartfila, int boxStartcolumna, int numero)
{
    for (int fila = 0; fila < 3; fila++)
        for (int columna = 0; columna < 3; columna++)
            if (matriz[fila+boxStartfila][columna+boxStartcolumna] == numero)
                return true;
    return false;
}

/* Devuelve si se puede asignar un número a la fila dada, ubicación de columna.
 */
bool EsSeguro(int **matriz, int fila, int columna, int numero)
{
    return !VerificarFila(matriz, fila, numero) && !VerificarColumna(matriz, columna, numero) &&
           !VerificarSubmatriz(matriz, fila - fila % 3 , columna - columna % 3, numero);
}

/* Imprime la matriz */
void Mostrarmatriz(int **matriz)
{
    for (int fila = 0; fila < N; fila++)
    {
        for (int columna = 0; columna < N; columna++)
            cout<<matriz[fila][columna]<<"  ";
        cout<<endl;
    }
}

/* Aqui se debe hacer el envio al archivo csv */
void csvmatriz(int **matriz)
{
  ofstream archivo;
  archivo.open("solucionparalelo.csv");
  for(int g=0;g<N;g++)
    for(int h=0;h<N;h++)
      if(h<8)
        archivo<<matriz[g][h]<<",";
      else
        archivo<<matriz[g][h]<<"\n";
  archivo.close();
}


/* Convierte numero1 del argv[] que es string a integer */
int Convertir_numero(string valor)
{
	int n = atoi(valor.c_str());
    return n;
}

/* Verifica si el valor del argv[] es un numero1 */
bool EsNumero(string valor)
{
	if(Convertir_numero(valor)==0)
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

void VerEntrada(int **matriz, string argumento, string pos_i, string pos_j, string numero)
{
  int fila,columna,numero1;
  if (argumento.substr(0,1)=="[")
  {
    cout<<endl;
    while(argumento!="")
    {
      pos_i=argumento.substr(1,1);
      pos_j=argumento.substr(3,1);
      numero=argumento.substr(5,1);
      if(EsNumero(pos_i) && EsNumero(pos_j) && EsNumero(numero) &&
        Convertir_numero(pos_i)>=0 && Convertir_numero(pos_i)<9 && Convertir_numero(pos_j)>=0 &&
        Convertir_numero(pos_j)<9 && Convertir_numero(numero)>=1 && Convertir_numero(numero)<=9)
      {
        fila=Convertir_numero(pos_i);
        columna=Convertir_numero(pos_j);
        numero1 = Convertir_numero(numero);
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


int **asignarMem(int filas, int columnas)
{
    int *data = (int *)malloc(filas*columnas*sizeof(int));
    int **array= (int **)malloc(filas*sizeof(int*));
    for (int i=0; i<filas; i++)
        array[i] = &(data[columnas*i]);

    return array;
}


/* Main */
int main(int argc, char *argv[])
{
  int **matriz,rank,size;
  string argumento=argv[1];
	string pos_i,pos_j,numero;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  matriz = asignarMem(N,N);
  if (rank == 0)
  {
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
          matriz[i][j]=0;
  }

  MPI_Bcast(&(matriz[0][0]), 9*9, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (rank == 1)
  {
    VerEntrada(matriz,argumento,pos_i,pos_j,numero);
      if (ResolverSudoku(matriz) == true)
      {
        //Mostrarmatriz(matriz);
        csvmatriz(matriz);
        cout<<"Resultado generado con exito"<<endl;
      }
      else
          cout<<"No existe solucion "<<endl;
  }
  MPI_Finalize();
}
