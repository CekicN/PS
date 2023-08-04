#include<stdio.h>
#include"mpi.h"
#include<math.h>
int main(int argc, char** argv)
{
	int size, rank;//br procesa, identifikator svakog procesa [0...size-1]
	//int x, sum;//suma n elemenata
	//int ln, level, l, send, recv, value, sum;//suma n elemenata gde je n parno

	int niz[8], k, nl, p, level, send, recv, value;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//Ovaj deo izvrsavaju svi procesi

	//if (rank == 0)
	//{
	//	x = 4;
	//	MPI_Send(&x, 1, MPI_INT, rank + 1, 5, MPI_COMM_WORLD);
	//}
	//else
	//{
	//	MPI_Recv(&x, 1, MPI_INT, rank - 1, 5, MPI_COMM_WORLD, &status);

	//	if (rank < size-1)
	//		MPI_Send(&x, 1, MPI_INT, rank + 1, 5, MPI_COMM_WORLD);

	//	printf("%d", x);
	//}


	//SUMA N CELIH BROJEVA

	/*if (rank == 0)
	{
		x = 1;
		MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&x, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

		sum = x + rank + 1;

		if (rank < size - 1)
		{
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
		if (rank == size - 1)
			printf("%d", sum);
	}*/

	//SUMA N CELIH BROJEVA GDE JE N PARNO

	//ln = (int)(log((double)size) / log(2.0));//za 16 procesa je 4
	//sum = rank;//svaki proces za sum dobija vrednost svog ranka
	//for (int i = 0; i < ln; i++)
	//{
	//	level = (int)(pow(2.0, (double)i));//1 2 4 8 broj nivoa u stablu
	//	if ((rank % level) == 0)//proveravamo ko sve moze da prodje po levelu, prvo svi, pa onda samo parni, pa deljivi sa 4, pa sa 8
	//	{
	//		l = (int)(pow(2.0, (double)(i + 1)));//2 4 8 16 od tih sto su prosli racunamo ko ide u sledeci nivo to je u if
	//		if ((rank % l) == 0)
	//		{
	//			//LEVI ide u sledeci nivo i prima sumu iz desnog
	//			recv = rank + level;//proces od kog primamo podatak
	//			MPI_Recv(&value, 1, MPI_INT, recv, 0, MPI_COMM_WORLD, &status);

	//			sum += value;//racunamo sumu
	//		}
	//		else
	//		{
	//			//DESNI salje samo sumu u proces brata u stablu
	//			send = rank - level;//proces kome saljemo podatak
	//			MPI_Send(&sum, 1, MPI_INT, send, 0, MPI_COMM_WORLD);
	//		}
	//	}
	//}

	//if (rank == 0)
	//	printf("Rez je %d", sum);

	//MPI_SCATTER 
	p = (int)(log((double)size) / log(2.0));
	if (rank == 0)
		for (int i = 0; i < 8; i++)
			niz[i] = i;
	k = 8;
	for (int i = p - 1; i >= 0; i--)//i = 2, 1, 0
	{
		level = (int)pow(2.0, (double)i);//4 2 1 samo ko ucestvuje u razmeni
		k = k / 2;//koliko podataka se salje
		if (rank % level == 0)//[0, 4], [0, 2, 4, 6], [svi]
		{

			nl = (int)pow(2.0, (double)(i + 1));//8, 4, 2 
			if (rank % nl == 0)//prolazi: [0], [0, 4], [0, 2, 4, 6] znaci ko salje
			{ 
				send = rank + level;
				MPI_Send(niz + k, k, MPI_INT,send , 0, MPI_COMM_WORLD);
			}
			else//[4], [2, 6], [1, 3, 5, 7]
			{
				recv = rank - level;
				MPI_Recv(niz, k, MPI_INT, recv, 0, MPI_COMM_WORLD, &status);
			}
		}
	}
	for (int i = 0; i < k; i++)
	{
		printf("U procesu %d se nalazi %d\n", rank, niz[i]);
	}
	MPI_Finalize();
	return 0;
}