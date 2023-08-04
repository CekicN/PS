#include<stdio.h>
#include"mpi.h"

int main(int argc, char** argv)
{
	int size, rank;

	int N;
	double sum, pi, f, x, h;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//f(x) = 4/(1+x^2)
	
	if (rank == 0)
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
	}
	MPI_Finalize();
	return 0;
}