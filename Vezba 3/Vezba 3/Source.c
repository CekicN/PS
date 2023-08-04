#include<stdio.h>
#include"mpi.h"

int main(int argc, char** argv)
{
	int size, rank;

	int N, p, a[3], b[10], prod, local_a[10], local_b[10], sumi, rez;
	double sum, pi, f, x, h;

	int mat[10][10], mat_rez[10], localA[3];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#pragma region PI
	//f(x) = 4/(1+x^2)

	/*if (rank == 0)
	{
		scanf("%d", &N);
	}
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	h = 1.0 / (double)N;
	sum = 0.0;
	for (int i = rank; i < N; i += size)
	{
		x = h * ((double)i + 0.5);
		sum += 4.0 / (1.0 + x * x);
	}
	f = h * sum;
	MPI_Reduce(&f, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("PI = %f", pi);
	}*/

#pragma endregion

#pragma region SkalarniProizvod
	//N = 6;
	//p = N / size;//p je broj iteracija za svaki proces pr: 6 / 3 = 2
	//if (rank == 0)
	//{
	//	for (int i = 0; i < N; i++)
	//	{
	//		scanf("%d", &a[i]);
	//	}
	//}

	//MPI_Scatter(a, p, MPI_INT, local_a, p, MPI_INT, 0, MPI_COMM_WORLD);
	//if (rank == 0)
	//{
	//	for (int i = 0; i < N; i++)
	//	{
	//		scanf("%d", &b[i]);
	//	}
	//}
	//MPI_Scatter(b, p, MPI_INT, local_b, p, MPI_INT, 0, MPI_COMM_WORLD);
	////svaki proces sadrzi po p podataka za a i za b
	//sumi = 0;
	//for (int i = 0; i < p; i++)
	//{
	//	prod = local_a[i] * local_b[i];
	//	sumi += prod;
	//}

	//MPI_Reduce(&sumi, &rez, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	//if (rank == 0)
	//{
	//	printf("Rez je %d", rez);
	//}
#pragma endregion
#pragma region ProizvodMatrice
	N = 3;
	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			for (int  j = 0; j < N; j++)
			{
				scanf("%d", &mat[i][j]);
			}
		}

		for (int i = 0; i < N; i++)
		{
			//scanf("%d", &b[i]);
			b[i] = 1;
		}
	}
	MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Scatter(&mat[0][0],N, MPI_INT, localA, N, MPI_INT, 0, MPI_COMM_WORLD); // uzima vrste matrice i dodeljuje ih procesima u niz a
	
	sumi = 0;
	for (int i = 0; i < N; i++)
	{

		printf("A = %d\n", localA[i]);
		sumi += localA[i] * b[i];//racunamo vrednost proizvoda vrste i niza b
	}

	MPI_Gather(&sumi, 1, MPI_INT, mat_rez, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Proizvod matrice i vektora je:\n");
		for (int i = 0; i < N; i++)
		{
			printf("%d, ", mat_rez[i]);
		}
	}
#pragma endregion


	MPI_Finalize();
	return 0;
}