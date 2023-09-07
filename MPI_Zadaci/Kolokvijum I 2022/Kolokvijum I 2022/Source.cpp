#include<mpi.h>
#include<iostream>

#define k 4
#define n 3
#define m 4

using namespace std;

int main(int argc, char** argv)
{
	int a[k][n], b[n][m], c[k][m];
	MPI_Datatype row_block, row_block_resized, mat;
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int l = k / size;//velicin bloka vrste

	MPI_Type_vector(l, n, n*size, MPI_INT, &row_block);
	MPI_Type_commit(&row_block);
	MPI_Type_create_resized(row_block, 0, sizeof(int) * n, &row_block_resized);
	MPI_Type_commit(&row_block_resized);

	MPI_Type_vector(n, m, m, MPI_INT, &mat);
	MPI_Type_commit(&mat);

	int* local_a = new int[n * l];
	int* local_c = new int[l * m];

	if (rank == 0)
	{
		int el = 0;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i][j] = el++;
			}
		}
		el = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				b[i][j] = el++;
			}
		}
	}

	MPI_Scatter(a, 1, row_block_resized, local_a, n * l, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, 1, mat, 0, MPI_COMM_WORLD);

	/*for (int i = 0; i < n*l; i++)
	{
		cout << "P:" << rank<<", " << local_a[i] << endl;
	}*/
	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << "P:" << rank << ", "<< b[i][j] << endl;
		}
	}*/

	int y = 0;
	for (int i = 0; i < l; i++)
	{
		for (int z = 0; z < m; z++)
		{
			local_c[y] = 0;
			for (int j = 0; j < n; j++)
			{
				local_c[y] += local_a[i * n + j] * b[j][z];
			}
			y++;
		}
	}

	for (int i = 0; i < l; i++)
	{
		MPI_Gather(local_c + m * i, m, MPI_INT, &c[size * i][0], m, MPI_INT, 0, MPI_COMM_WORLD);
	}

	if (rank == 0)
	{
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < m; j++)
			{
				cout << c[i][j] << " ";
			}
			cout << endl;
		}
	}

	MPI_Finalize();
	return 0;
}