#include<mpi.h>
#include<iostream>

#define n 8
using namespace std;
int main(int argc, char** argv)
{
	int a[n][n], b[n], c[n];
	int k;
	MPI_Datatype column, column_resized;
	MPI_Datatype columnB, columnB_resized;
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	k = n / size;
	MPI_Type_vector(n*k, 1, size, MPI_INT, &column);
	MPI_Type_commit(&column);
	MPI_Type_create_resized(column, 0, sizeof(int), &column_resized);
	MPI_Type_commit(&column_resized);

	MPI_Type_vector(k, 1, size, MPI_INT, &columnB);
	MPI_Type_commit(&columnB);
	MPI_Type_create_resized(columnB, 0, sizeof(int), &columnB_resized);
	MPI_Type_commit(&columnB_resized);

	int* local_a = new int[n * k];
	int* local_b = new int[k];
	int* local_c = new int[n];
	if (rank == 0)
	{
		int el = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i][j] = el++;
			}
			b[i] = 1;
		}
	}

	MPI_Scatter(a, 1, column_resized, local_a, n * k, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, 1, columnB_resized, local_b, k, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < n; i++)
	{
		local_c[i] = 0;
		for (int j = 0; j < k; j++)
		{
			local_c[i] += local_a[(i * k) + j] * local_b[j];
		}
	}

	MPI_Reduce(local_c, c, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			cout << c[i] << " ";
		}
	}
	MPI_Finalize();
	return 0;
}