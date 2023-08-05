#include"mpi.h"
#include<stdio.h>
#include<math.h>
int main(int argc, char* argv[])
{
	int myrank, size;
	MPI_Status status;
	/*int x, y;*/
	/*int value, sum;*/
	int niz[10], lev, k, nl;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
#pragma region Ptp bez deadlock-a
	//*************************
	// Ptp bez deadlock-a
	//*************************
	//if (myrank == 0)
	//{
	//	//recv je blokirajuci
	//	x = 3;
	//	MPI_Recv(&y, 1, MPI_INT, 1, 19, MPI_COMM_WORLD, &status);
	//	MPI_Send(&x, 1, MPI_INT, 1, 17, MPI_COMM_WORLD);
	//}
	//else if (myrank == 1)
	//{
	//	x = 9;
	//	MPI_Send(&x, 1, MPI_INT, 0, 19, MPI_COMM_WORLD);
	//	MPI_Recv(&y, 1, MPI_INT, 0, 17, MPI_COMM_WORLD, &status);
	//}
	//printf("Proc %d y = %d", myrank, y);

#pragma endregion

#pragma region zad1
	//*************************
	// zad1
	//*************************

	//do
	//{
	//	if (myrank == 0)
	//	{
	//		scanf("%d", &value);
	//		MPI_Send(&value, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD);
	//	}
	//	else
	//	{
	//		MPI_Recv(&value, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, &status);
	//		if (myrank < size - 1)//poslednji ne treba da salje
	//			MPI_Send(&value, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD);
	//	}
	//	printf("Process %d got %d\n", myrank, value);
	//} while (value >= 0);

#pragma endregion

#pragma region zad2
	//*************************
	// zad2
	//*************************

	/*if (myrank == 0)
	{
		value = 1;
		MPI_Send(&value, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		if (myrank < size - 1)
		{
			MPI_Recv(&value, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, &status);
			sum = myrank + 1 + value;
			MPI_Send(&sum, 1, MPI_INT, myrank + 1, 0, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(&value, 1, MPI_INT, myrank - 1, 0, MPI_COMM_WORLD, &status);
			sum = myrank + 1 + value;
			printf("Rank %d , Suma je %d\n", myrank, sum);
		}
	}*/
#pragma endregion

#pragma region Domaci
//	//*************************
//	// Domaci
//	//*************************
//	sum = myrank;
//	int levels = (int)((log((double)size)) / log(2.0));
//	for (int i = 0; i < levels; i++)
//	{
//		int lv = (int)pow(2.0, (double)i);//1, 2, 4, 8
//		if (myrank % lv == 0)
//		{
//			int nextLv = (int)pow(2.0, (double)(i + 1));//2, 4, 8, 16
//			if (myrank % nextLv == 0)
//			{
//				int source = myrank + lv;
//				MPI_Recv(&value, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
//				sum += value;
//			}
//			else
//			{
//				int dest = myrank - lv;
//				MPI_Send(&sum, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
//			}
//		}
//
//	}
//	if (myrank == 0)
//	{
//		printf("Rezultat je:%d", sum);
//	}

#pragma endregion
#pragma region Realizacija MPI_SCATTER
int n = log((double)size) / log(2.0);
if (myrank == 0)
{
	/*printf("Unesite niz elemenata:\n");*/
	for (int i = 0; i < 8; i++)
	{
		/*scanf("%d", &niz[i]);*/
		niz[i] = i;
	}
}
k = 8;
for (int i = n-1; i >= 0; i--)
{
	lev = pow(2.0, i);//4 2 1
	k = k / 2;//4 2 1
	if (myrank % lev == 0)
	{
		nl = pow(2.0, i + 1);//8 4 2
		if (myrank % nl == 0)
		{
			MPI_Send(niz + k, k, MPI_INT, myrank+lev, 0, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(niz, k, MPI_INT, myrank - lev, 0, MPI_COMM_WORLD, &status);
		}
	}
}

for (int i = 0; i < k; i++)
{
	printf("Proces %d ima niz[%d]=%d\n", myrank, i, niz[i]);
}
#pragma endregion
MPI_Finalize();
return 0;
}