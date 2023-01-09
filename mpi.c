#include <stdio.h>
#include <mpi.h>
#include <time.h>

void llenarMatriz(int N, int M, int matriz[N][M]);
void mostrar(int N, int M, int matriz[N][M]);
int esPrimo(int num);

int main(int argc, char *argv[])
{
    int myID, size, i, j, cant, vini,
        sumaParcial = 0,
        sumaTotal = 0;
    //////Codigo de matriz///////
    srand(time(NULL));
    int N = 1000, M = 1000;
    //////Codigo de matriz///////
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (myID == 0)
    {
        do
        {
            printf("N debe ser mayor que M\n");
            printf("Ingrese la dimension N:\n");
            scanf("%d", &N);
            printf("Ingrese la dimension M:\n");
            scanf("%d", &M);
        } while (N <= M);
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int matriz[N][M];
    if (myID == 0)
    {
        llenarMatriz(N, M, matriz);
        mostrar(N, M, matriz);
    }
    // hacer broadcast
    MPI_Bcast(matriz, N * M, MPI_INT, 0, MPI_COMM_WORLD);

    if (myID == size - 1)
    {
        cant = N / (size) + N % (size);
    }
    else
    {
        cant = N / (size);
    }
    vini = N / (size)*myID;

    for (i = vini; i < vini + cant; i++)
    {
        for (j = 0; j < M; j++)
        {
            if (esPrimo(matriz[i][j]) == 1)
            {
                // printf("En el proceso %d con valor %d la fila %d columna %d*\n", myID, matriz[i][j], i, j);
                sumaParcial += matriz[i][j];
            }
        }
    }
    printf("Suma Parcial de %d entre la fila %d y %d es: %d\n", myID, vini, vini + cant, sumaParcial);
    MPI_Reduce(&sumaParcial, &sumaTotal, 1, MPI_INT, MPI_SUM, size - 1, MPI_COMM_WORLD);

    if (myID == size - 1)
    {
        printf("La sumatoria calculada es : %d", sumaTotal);
    }
    MPI_Finalize();
    return 0;
}

int esPrimo(int num)
{
    int i, cont = 0;
    for (i = 1; i <= num; i++)
    {
        if (num % i == 0)
        {
            cont++;
        }
    }
    if (cont == 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void mostrar(int N, int M, int matriz[N][M])
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void llenarMatriz(int N, int M, int matriz[N][M])
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            matriz[i][j] = 1 + rand() % 9;
        }
    }
}