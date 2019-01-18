# TrabajoSudoku

### Para compilar la version en secuencial:
```
g++ MpiSudoku.cpp -o MpiSudoku
```

### Para ejecutar de ejemplo:

```
./MpiSudoku "[0;1;4][0;3;8][1;0;5][2;1;7][2;3;4][2;4;6][3;0;6][3;1;9][3;2;8][3;3;5][3;4;3][3;5;7][3;6;2][3;8;4][4;1;3][4;2;5][4;4;2][5;5;4][5;7;9][5;8;3][6;4;7][6;6;3][7;0;7][7;1;8][7;3;1][7;7;6][8;3;3][8;4;9][8;6;7][8;7;4][8;8;8]"
```

### Donde:
```
./ejecutable "[x;y;z]"
x: fila del dato inicial.
y: columna del dato inicial.
z: valor que irá en la matriz (entre 1 y 9).

```
