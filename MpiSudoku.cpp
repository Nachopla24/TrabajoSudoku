#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>


using namespace std;

#define UNASSIGNED 0
#define N 9

bool FindUnassignedLocation(int matriz[N][N], int &row, int &col);
bool isSafe(int matriz[N][N], int row, int col, int num);

/*
asigna valores a todas las ubicaciones no asignadas para resolver el Sudoku
 */
bool SolveSudoku(int matriz[N][N])
{
    int row, col;
    if (!FindUnassignedLocation(matriz, row, col))
       return true;
    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(matriz, row, col, num))
        {
            matriz[row][col] = num;
            if (SolveSudoku(matriz))
                return true;
            matriz[row][col] = UNASSIGNED;
        }
    }
    return false;
}

/* Busca en la matriz para encontrar una entrada que aún no esté asignada. */
bool FindUnassignedLocation(int matriz[N][N], int &row, int &col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (matriz[row][col] == UNASSIGNED)
                return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la fila especificada coincide con el número dado.
 */
bool UsedInRow(int matriz[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (matriz[row][col] == num)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada en la columna especificada coincide
   el numero dado */
bool UsedInCol(int matriz[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (matriz[row][col] == num)
            return true;
    return false;
}

/* Devuelve si alguna entrada asignada dentro del cuadro 3x3 especificado coincide
   el numero dado */
bool UsedInBox(int matriz[N][N], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (matriz[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Devuelve si será legal asignar un número a la fila dada, ubicación de columna.
 */
bool isSafe(int matriz[N][N], int row, int col, int num)
{
    return !UsedInRow(matriz, row, num) && !UsedInCol(matriz, col, num) &&
           !UsedInBox(matriz, row - row % 3 , col - col % 3, num);
}

/* Imprime la matriz */
void printmatriz(int matriz[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
            cout<<matriz[row][col]<<"  ";
        cout<<endl;
    }
}

/* Aqui se debe hacer el envio al archivo csv */
void csvmatriz(int matriz[N][N])
{
  ofstream archivo;
  archivo.open("solucion.csv");
  for(int g=0;g<N;g++)
    for(int h=0;h<N;h++)
      if(h<8)
        archivo<<matriz[g][h]<<",";
      else
        archivo<<matriz[g][h]<<"\n";
  archivo.close();
}


//Convierte numero del argc que es string creo xxd
int Conv_Num(string valor)
{
	int n = atoi(valor.c_str());
    return n;
}

bool esNumero(string valor)
{
	if(Conv_Num(valor)==0)
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


/* Main
Para ejecutar de ejemplo:
./MpiSudoku "[0;1;4][0;3;8][1;0;5][2;1;7][2;3;4][2;4;6][3;0;6][3;1;9][3;2;8][3;3;5][3;4;3][3;5;7][3;6;2][3;8;4][4;1;3][4;2;5][4;4;2][5;5;4][5;7;9][5;8;3][6;4;7][6;6;3][7;0;7][7;1;8][7;3;1][7;7;6][8;3;3][8;4;9][8;6;7][8;7;4][8;8;8]"
            [x;y;z]
Donde:
./ejecutable "[x;y;z]"

x = corresponde a la fila del dato inicial;
y = corresponde a el valor de y corresponde a la columna donde debe ir el dato;
z = es valor que irá en la matriz (entre 1 y 9).
*/
int main(int argc, char *argv[])
{
  int matriz[N][N],fila,columna,numero;
  string argumento=argv[1];
	string pos_i;
	string pos_j;
	string num;

  for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			matriz[i][j]=0;

      if (argumento.substr(0,1)=="[")
    	{
    		cout<<endl;
    		while(argumento!="")
    		{
    			pos_i=argumento.substr(1,1);
    			pos_j=argumento.substr(3,1);
    			num=argumento.substr(5,1);
    			if(esNumero(pos_i) && esNumero(pos_j) && esNumero(num) && Conv_Num(pos_i)>=0 && Conv_Num(pos_i)<9 && Conv_Num(pos_j)>=0 && Conv_Num(pos_j)<9 && Conv_Num(num)>=1 && Conv_Num(num)<=9)
    			{
            fila=Conv_Num(pos_i);
      			columna=Conv_Num(pos_j);
            numero = Conv_Num(num);
            matriz[fila][columna]=numero;
          }
          else
          {
            cout<<"Error de formato"<<endl;
          }
          argumento=argumento.substr(7);
        }
        cout<<endl;
      }
      else
      {
        cout<<"Error de formato"<<endl;
      }

      if (SolveSudoku(matriz) == true)
      {
        printmatriz(matriz);
        csvmatriz(matriz);
      }
      else
          cout<<"No solution exists"<<endl;
      return 0;
}
