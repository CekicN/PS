#include<stdio.h>
#include"mpi.h"
#include<limits.h>
int main(int argc, char** argv)
{
	int size, rank;

	int N, M,B, p, a[3], c[3], b[3], prod, local_a[3], local_b[10], sumi, rez;
	double sum, pi, f, x, h;

	int mat[3][3], mat_rez[10], localA[3];

	int interval_A, interval_B, xD, counter, local_min, min;
	struct {
		int value;
		int rank;
	}in_val, out_val, in_rank, out_rank;

	MPI_Status status;
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
	//N = 3;
	//if (rank == 0)
	//{
	//	for (int i = 0; i < N; i++)
	//	{
	//		for (int  j = 0; j < N; j++)
	//		{
	//			scanf("%d", &mat[i][j]);
	//		}
	//	}

	//	for (int i = 0; i < N; i++)
	//	{
	//		//scanf("%d", &b[i]);
	//		b[i] = 1;
	//	}
	//}
	//MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

	//MPI_Scatter(&mat[0][0],N, MPI_INT, localA, N, MPI_INT, 0, MPI_COMM_WORLD); // uzima vrste matrice i dodeljuje ih procesima u niz a
	//
	//sumi = 0;
	//for (int i = 0; i < N; i++)
	//{
	//	sumi += localA[i] * b[i];//racunamo vrednost proizvoda vrste i niza b
	//}

	//MPI_Gather(&sumi, 1, MPI_INT, mat_rez, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//if (rank == 0)
	//{
	//	printf("Proizvod matrice i vektora je:\n");
	//	for (int i = 0; i < N; i++)
	//	{
	//		printf("%d, ", mat_rez[i]);
	//	}
	//}
#pragma endregion
#pragma region MIN_Nepar
	//interval_A = 3;
	//interval_B = 31;
	//if (rank == 0)
	//{
	//	//scanf("%d", &xD);
	//	xD = 5;
	//}
	//MPI_Bcast(&xD, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//counter = 0;
	//local_min = INT_MAX;
	//for (int i = interval_A + 2*rank; i <= interval_B; i+= size*2)//uvek je i neparno 
	//{
	//	if (i % xD == 0)//ispitivanje osobine
	//	{
	//		counter++;
	//		if (i < local_min)
	//			local_min = i;
	//	}
	//}
	//in_rank.value = counter;
	//in_rank.rank = rank;
	//MPI_Reduce(&in_rank, &out_rank, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	//MPI_Bcast(&out_rank, 1, MPI_2INT, 0, MPI_COMM_WORLD);

	//in_val.rank = rank;
	//in_val.value = local_min;
	//MPI_Reduce(&in_val, &out_val, 1, MPI_2INT, MPI_MINLOC, out_rank.rank, MPI_COMM_WORLD);

	//if (rank == out_rank.rank)
	//{
	//	printf("Rank %d: MIN %d", out_val.rank,out_val.value);
	//}

#pragma endregion
#pragma region MnozenjeMatrice
	N = 3;
	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				scanf("%d", &mat[i][j]);
			}
		}
		for (int i = 0; i < N; i++)
		{
			b[i] = 1;
		}
	}

	MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, 1, MPI_INT, &B, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(mat, N * N, MPI_INT, 0, MPI_COMM_WORLD);

	//PRVI NACIN IZDVAJANJA KOLONA SA GRUPNIM OPERACIJAMA
	/*for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (rank == j)
			{
				p = mat[j][i];
			}
		}
		MPI_Gather(&p, 1, MPI_INT, a, 1, MPI_INT, i, MPI_COMM_WORLD);
	}*/
	//DRUGI NACIN SA PtP OPERACIJAMA
	if (rank == 0)
	{
		for (int i = 0; i < N; i++) a[i] = mat[i][0];

		for (int i = 1; i < size; i++)
		{
			for (int j = 0; j < N; j++)
			{
				local_a[j] = mat[j][i];
			}
			MPI_Send(local_a, N, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < N; i++)
	{
		c[i] = a[i] * B;
	}

	MPI_Reduce(c, mat_rez, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			printf("REZ[%d] = %d\n", i, mat_rez[i]);
		}
	}
#pragma endregion


	MPI_Finalize();
	return 0;
}