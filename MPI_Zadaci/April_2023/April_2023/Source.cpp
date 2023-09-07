#include"mpi.h"
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<limits.h>
using namespace std;

#define n 8
int main(int argc, char** argv)
{	
	int a[n][n], b[n], c[n];
	int rank, size;
	int jcol, irow, col_id, row_id, diag_id;
	int q, y;
	MPI_Comm row_comm, col_comm, diag_comm;
	int local_min = INT_MAX;
	int color = 1;
	struct {
		int value;
		int rank;
	}in, out;
	MPI_Datatype block, bblock, bblock_resized;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	q = (int)(sqrt((double)size));//procesi su uredjeni u matricu qxq
	irow = rank / q;//0 0 0 0
					//1 1 1 1

	jcol = rank % q;//0 1 2 3
					//0 1 2 3
	int k = (n * n) / size;
	MPI_Comm_split(MPI_COMM_WORLD, irow, jcol, &row_comm);
	MPI_Comm_split(MPI_COMM_WORLD, jcol, irow, &col_comm);
	MPI_Comm_rank(row_comm, &row_id);
	MPI_Comm_rank(col_comm, &col_id);

	MPI_Type_vector(k, 1, q, MPI_INT, &block);//(0,0), (0,4),(1,0), (1,4)
	MPI_Type_commit(&block);

	MPI_Type_vector(n / q, 1, q, MPI_INT, &bblock);//0, 4
	MPI_Type_commit(&bblock);
	MPI_Type_create_resized(bblock, 0, sizeof(int), &bblock_resized);
	MPI_Type_commit(&bblock_resized);

	int* local_a = new int[k];
	int* local_b = new int[n/q];
	int* local_c = new int[n / q];
	int* local_c2 = new int[n / q];
	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				a[i][j] = i + j;
			}
			b[i] = i;
		}
	}

	if (rank == 0)
	{
		y = 0;
		for (int i = 0; i < n/q; i++)
		{
			for (int j = 0; j < n/q; j++)
			{
				local_a[y++] = a[i][j * q];
			}
		}

		int I = 1;
		for (int i = 0; i < q; i++)
		{
			for (int j = 0; j < q; j++)
			{
				if ((i+j) != 0)
				{
					MPI_Send(&a[i * (n/q)][j], 1, block, I++, 0, MPI_COMM_WORLD);
				}
			}
		}
	}
	else
	{
		MPI_Recv(local_a, k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
	
	if(col_id == 0)
		MPI_Scatter(b, 1, bblock_resized, local_b, n / q, MPI_INT, 0, row_comm);
	MPI_Bcast(local_b, n / q, MPI_INT, 0, col_comm);

	/*cout<<"P:"<< rank << "local_b[0] = " << local_b[0] << "," << "local_b[1] = " << local_b[1] << endl;*/
	/*for (int i = 0; i < 4; i++)
	{
		cout << "P:" << rank << "local_b["<< i <<"] = " << local_a[i] << endl;
	}*/

	for (int i = 0; i < (n*n)/size; i++)
	{
		if (local_a[i] < local_min)
		{
			local_min = local_a[i];
		}
	}
	/*cout << "P:" << rank << ", " << local_min << endl;*/
	in.rank = rank;
	in.value = local_min;

	MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);//nadjemo u kom procesu se nalazi najmanja vrednost matrice
	MPI_Bcast(&out, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < n/q; i++)
	{
		local_c2[i] = 0;
		for (int j = 0; j < n/q; j++)
		{
			local_c2[i] += local_a[i * (n / q) + j] * local_b[j];
		}
	}
	MPI_Reduce(local_c2, local_c, n / q, MPI_INT, MPI_SUM, out.rank % q, row_comm);
	if (row_id == out.rank % q)
	{
		MPI_Gather(local_c, n / q, MPI_INT, c, n/q, MPI_INT, out.rank / q, col_comm);
	}

	if (rank == out.rank)
	{
		for (int i = 0; i < n; i++)
		{
			cout << "c[" << i << "] = " << c[i] << endl;
		}
	}


	//**************************** b)
	if(rank == 0)
		cout<< endl<<"B)"<< endl;
	if (irow == jcol)
	{
		color = 0;
	}
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &diag_comm);
	MPI_Comm_rank(diag_comm, &diag_id);

	int msg;
	if (color == 0)
	{
		if (row_id == 0)
		{
			msg = 5;
		}
		MPI_Bcast(&msg, 1, MPI_INT, 0, diag_comm); 
		cout << "P:"<< rank <<" msg:" << msg << endl;
	}
	MPI_Finalize();
	return 0;
}
