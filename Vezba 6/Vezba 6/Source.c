#include<stdio.h>
#include"mpi.h"
#include<math.h>
#include<stdlib.h>
#define m 2
#define n 6
#define k 4
int main(int argc, char** argv)
{
	int A[m][n], B[n][k], C[m][k], tmp[m][k], col[m], row[k];
	MPI_Datatype column;
	int a[n][n], b[n], c[n], q, sub_k, y = 0;
	MPI_Datatype sub_matrix, resized_sub;

	MPI_Comm row_comm, col_comm;
	int col_id, row_id, irow, jcol;

	MPI_Comm COM;
	MPI_Group group;
	MPI_Group oldgroup;
	int ex[] = { 0 };
	int m_rank;

	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_group(MPI_COMM_WORLD, &oldgroup);

#pragma region ProizvodMatrica
	//MPI_Type_vector(m, 1, n, MPI_INT, &column);
	//MPI_Type_commit(&column);

	//MPI_Group_excl(oldgroup, 1, ex, &group);
	//MPI_Comm_create(MPI_COMM_WORLD, group, &COM);
	//MPI_Group_rank(group, &m_rank);

	////master proces samo inicijalizuje i salje ostali rade
	//if (rank == 0)
	//{
	//	for (int i = 0; i < m; i++)
	//	{
	//		for (int j = 0; j < n; j++)
	//		{
	//			A[i][j] = i + j + 1;
	//		}
	//	}
	//	for (int i = 0; i < n; i++)
	//	{
	//		for (int j = 0; j < k; j++)
	//		{
	//			B[i][j] = i + j;
	//		}
	//	}

	//	for (int i = 0; i < n; i++)
	//	{
	//		MPI_Send(&A[0][i], 1, column, i + 1, 17, MPI_COMM_WORLD);//saljemo svakom radnom procesu po kolonu matrice A
	//		MPI_Send(&B[i][0], k, MPI_INT, i + 1, 19, MPI_COMM_WORLD);//saljemo svakom radnom procesu po vrstu matrice B
	//	}
	//}
	//else
	//{
	//	MPI_Recv(col, m, MPI_INT, 0, 17, MPI_COMM_WORLD, &status);
	//	MPI_Recv(row, k, MPI_INT, 0, 19, MPI_COMM_WORLD, &status);

	//	//ovde formiramo tmp pomocnu matricu
	//	for (int i = 0; i < m; i++)
	//	{
	//		for (int j = 0; j < k; j++)
	//		{
	//			tmp[i][j] = col[i] * row[j];
	//		}
	//	}

	//	//i sad treba da uradimo reduce kako bi se sabrale pomocne martrice ali moramo da napravimo komunikator koji iskljucuje proces 0
	//	//zbog toga sto u njemu nije definisana tmp matrica i rezultat bi bio pogresan tako da se reduce radi na svim procesima sem nultog

	//	MPI_Reduce(tmp, C, m * k, MPI_INT, MPI_SUM, 0, COM);
	//}
	//if (m_rank == 0)
	//{
	//	for (int i = 0; i < m; i++)
	//	{
	//		for (int j = 0; j < k; j++)
	//		{
	//			printf("%d ", C[i][j]);
	//		}
	//		printf("\n");
	//	}
	//}
#pragma endregion
#pragma region ProizvodMatriceiVektora

	//q = (int)sqrt((double)size);//q je velicina matrice procesa
	//sub_k = n / q;//sub_k velicina podmatrice koju dobija svaki proces
	//

	///*int** sub_mat = (int**)malloc(sizeof(int*) * sub_k);
	//for (int i = 0; i < sub_k; i++)
	//{
	//	sub_mat[i] = (int*)malloc(sizeof(int) * sub_k);
	//}*/
	//int* local_a = (int*)malloc(sub_k * sub_k * sizeof(int));
	//int* local_b = (int*)malloc(sub_k * sizeof(int));

	//MPI_Type_vector(sub_k, sub_k, n, MPI_INT, &sub_matrix);
	//MPI_Type_commit(&sub_matrix);


	//if (rank == 0)
	//{
	//	for (int i = 0; i < n; i++)
	//	{
	//		for (int j = 0; j < n; j++)
	//		{
	//			a[i][j] = n * i + j;
	//		}
	//		b[i] = 1;
	//	}
	//	
	//	for (int i = 0; i < sub_k; i++)
	//	{
	//		for (int j = 0; j < sub_k; j++)
	//		{
	//			local_a[y++] = a[i][j];//u local_a procesa P0 se ubacuje prva pod matrica
	//		}
	//	}
	//	int I = 1;
	//	for (int i = 0; i < q; i++)
	//	{
	//		for (int j = 0; j < q; j++)
	//		{
	//			if ((i+j) != 0)
	//			{
	//				MPI_Send(&a[i * sub_k][j * sub_k], 1, sub_matrix, I, 2, MPI_COMM_WORLD);
	//				I++;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	MPI_Recv(local_a, sub_k * sub_k, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
	//	
	//}

	//irow = rank / q;
	//jcol = rank % q;

	//MPI_Comm_split(MPI_COMM_WORLD, irow, jcol, &row_comm);
	//MPI_Comm_split(MPI_COMM_WORLD, jcol, irow, &col_comm);
	//MPI_Comm_rank(row_comm, &row_id);
	//MPI_Comm_rank(col_comm, &col_id);

	//if (col_id == 0)
	//{
	//	MPI_Scatter(b, sub_k, MPI_INT, local_b, k, MPI_INT, 0, row_comm);//ovo se sketeruje izmadju svik procesa u prvoj vrsti
	//}

	//MPI_Bcast(local_b, sub_k, MPI_INT, 0, col_comm);//onda taj niz Bcastujemo na dole po kolonama tako da sve kolone imaju svoj local_b
	//int* my_res = (int*)malloc(sub_k * sizeof(int));
	//int* res = (int*)malloc(n * sizeof(int));

	//int index = 0;

	//for (int i = 0; i < sub_k; i++)
	//{
	//	my_res[i] = 0;
	//	for (int j = 0; j < sub_k; j++)
	//	{
	//		my_res[i] += local_a[index++] * local_b[j];
	//	}
	//}

	//MPI_Gather(my_res, sub_k, MPI_INT, res, sub_k, MPI_INT, 0, col_comm);
	//if (col_id == 0)
	//	MPI_Reduce(res, c, n, MPI_INT, MPI_SUM, 0, row_comm);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < n; i++)
	//	{
	//		printf("c[%d] = %d", i, c[i]);
	//	}
	//}

	//MPI_Scatter(a, q*q, resized_sub, local_a, 1, sub_matrix, 0, MPI_COMM_WORLD); NE MOZE SCATTER JER KAD ZAVRSI SA SLANJEM JEDNOG REDA NECE DA PRESKOCI 2 VRTE NEGO SAMO JEDNU STO JE GRESKA
	
#pragma endregion
	//Topologije procesa u 1D, 2D i 3D resetku
	//postoje kartezijan i graf topologija ali koristimo samo kartezijan za prs
	// Dimenzionisanje ide da prvo idu procesi u smeru gde se brze menja druga kordinata dok je prva fiksna 
	//MPI_Cart_create koristi se za kreiranje kartezijan topologije
	
	/*MPI_Comm vu;
	int dims[] = { 4,3 };
	int periods[] = { 1, 0 };
	int reorder = 1;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &vu);*///Kreira 2d resetku dimenzija 4x3 kod koje su vrste periodicne a kolone nisu

	//MPI_Cart_coords uzima rank procesa u resetci i vraca njegove kordinate
	//MPI_Cart_rank radi suprotno od coords jer uzima kordinate i vraca rank
#pragma region PRIMER
	/*MPI_Comm vu;
	int dims[] = { 4,3 };
	int periods[] = { 1, 0 };
	int reorder = 1;
	int coord[2], id;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &vu);

	if (rank == 5)
	{
		MPI_Cart_coords(vu, rank, 2, coord);
		printf("P:%d, Coords = %d, %d\n", rank, coord[0], coord[1]);
	}

	if (rank == 0)
	{
		coord[0] = 3; coord[1] = 1;
		MPI_Cart_rank(vu, coord, &id);
		printf("(%d, %d) = %d\n", coord[0], coord[1], id);
	}*/
#pragma endregion
	//MPI_Cart_shift izracunava rankove susednih procesa
#pragma region PRIMER
	/*MPI_Comm vu;
	int dims[] = { 4,3 };
	int periods[] = { 1, 0 };
	int reorder = 1;
	int left, right, up, down;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &vu);
	if (rank == 9)
	{
		MPI_Cart_shift(vu, 0, 1, &left, &right);
		MPI_Cart_shift(vu, 1, 1, &up, &down);
		printf("P:%d, Up:%d, Down:%d, Left:%d, Right:%d", rank, up, down, left, right);
	}*/
#pragma endregion

#pragma region PRIMER
	/*int dim[] = { size };
	int period[] = { 1 };
	MPI_Comm ring;

	MPI_Cart_create(MPI_COMM_WORLD, 1, dim, period, 0, &ring);

	int source, dest;
	int right_boundery = rank;
	int left_boundery;
	MPI_Cart_shift(ring, 0, 1, &source, &dest);

	MPI_Sendrecv(&right_boundery, 1, MPI_INT, dest, 0,
		&left_boundery, 1, MPI_INT, source, 0,
		ring, &status);
	printf("P:%d, R:%d", rank, left_boundery);*/
#pragma endregion

#pragma region Zad
	//int dim[] = { 3, 4 };
	//int period[] = { 1, 0 };
	//int reorder = 0;
	//int coords[2];
	//MPI_Comm cartcomm;
	//int source, dest;
	//int id;
	//MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &cartcomm);
	//MPI_Comm_rank(cartcomm, &id);
	//MPI_Cart_coords(cartcomm, id, 2, coords);
	//MPI_Cart_shift(cartcomm, 0, coords[1], &source, &dest);//stoji coords[1] jer ce po toj vrednosti kolone da se rotiraju
	//int outbuf = rank;
	//MPI_Sendrecv_replace(&outbuf, 1, MPI_INT, dest, 0, source, 0, cartcomm, &status
#pragma endregion





	MPI_Finalize();
	return 0;
}