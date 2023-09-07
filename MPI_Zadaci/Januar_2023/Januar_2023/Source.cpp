#include<iostream>
#include<mpi.h>
#define n 4

using namespace std;

int main(int argc, char**argv)
{
	int a[n][n], b[n][n], c[n][n];
	int k = 2, y;
	int rank, size;
	MPI_Datatype col, row;
	MPI_Datatype local_c_row, row_resized;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Type_vector(n, k, n, MPI_INT, &col);
	MPI_Type_commit(&col);
	MPI_Type_vector(k, n, n, MPI_INT, &row);
	MPI_Type_commit(&row);
	
	MPI_Type_vector(1, k, k, MPI_INT, &local_c_row);
	MPI_Type_commit(&local_c_row);
	MPI_Type_create_resized(local_c_row, 0, k*sizeof(int), &row_resized);
	MPI_Type_commit(&row_resized);

	int* local_a = new int[k*n];
	
	int* local_b = new int[k*n];
	
	int *local_c = new int[k*k];
	int* local_c2 = new int[n*n];
	if (rank == 0)
	{
		//Master proces
		int p = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i][j] = p++;
				b[i][j] = 1;
			}
		}

		y = 0;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
			{
				local_a[y] = a[i][j];
				y++;
			}
		}
		y = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
			{
				local_b[y] = b[i][j];
				y++;
			}
		}
		int I = 1;

		for (int i = 0; i < (n/k); i++)
		{
			for (int j = 0; j < n/k; j++)
			{
				if((i+j) != 0)
				{
					MPI_Send(&a[i * k][0], 1, row, I, 10, MPI_COMM_WORLD);
					MPI_Send(&b[0][j * k], 1, col, I, 11, MPI_COMM_WORLD);
					I++;
				}
			}
		}
	}
	else
	{
		//Radni procesi

		MPI_Recv(local_a, k * n, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
		MPI_Recv(local_b, k * n, MPI_INT, 0, 11, MPI_COMM_WORLD, &status);
	}
		/*for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << "P:" << rank<<" -> " << local_a[i * n + j] << ", " << local_b[i*k + j] << endl;
			}
		}*/
	y = 0;
	for (int i = 0; i < k; i++)
	{
		for (int z = 0; z < k; z++)
		{	
			local_c[y] = 0;
			for (int j = 0; j < n; j++)
			{
				local_c[y] += local_a[i*n + j] * local_b[j*k + z];
			}
			y++;
		}
	}
	for (int i = 0; i < k*k; i++)
	{
		cout << "p:" << rank << " -> " << "local_c["<<i<<"] = "<<local_c[i];
		cout << endl;
	}
	
	MPI_Gather(local_c, k*k, MPI_INT, local_c2, k*k, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << c[i][j] << " ";
			}
			cout << endl;
		}
	}

	/*if (rank == 0)
	{
		int I = 1;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < k; j++)
			{
				c[i][j] = local_c[i][j];
			}
		}

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if((i + j) != 0)
				{
					MPI_Recv(&c[i * k][j * k], k, row_resized, I, I, MPI_COMM_WORLD, &status);
					I++;
				}
			}
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << c[i][j] << " ";
			}
			cout << endl;
		}
	}
	else
	{
		MPI_Send(local_c, k, row_resized, 0, rank, MPI_COMM_WORLD);
	}*/
	MPI_Finalize();
	return 0;
}