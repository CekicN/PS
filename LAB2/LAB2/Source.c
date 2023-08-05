#include<stdio.h>
#include<mpi.h>

#define N 6
int main(int argc, char* argv[])
{
	int rank, size;
	/*int source[2], result[2], root;*/
	/*int param[8], mine;*/
	/*double x, f, rez, pi, N, h;
	double PI = 3.14159265359;
	MPI_Request req;
	MPI_Status status;*/

	float a[N], b[N], dot, local_dot = 0;
	int i, n_bar;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//if (rank == 0)
	//{
	//	x = 3;
	//	MPI_Irecv(&y, 1, MPI_INT, 1, 19, MPI_COMM_WORLD, &req);//nema blikiranja zbog Irecv
	//	MPI_Send(&x, 1, MPI_INT, 1, 17, MPI_COMM_WORLD);
	//	MPI_Wait(&req, &status);
	//}
	//else if (rank == 1)
	//{
	//	x = 5;
	//	MPI_Irecv(&y, 1, MPI_INT, 0, 17, MPI_COMM_WORLD, &req);//nema blikiranja zbog Irecv
	//	MPI_Send(&x, 1, MPI_INT, 0, 19, MPI_COMM_WORLD);
	//	MPI_Wait(&req, &status);
	//}
	//printf("Proces: %d ; y = %d", rank, y);

	//root = 7;
	//source[0] = rank + 1;
	//source[1] = rank + 2;

	////Prolazi kroz sve procese i mnozi vrednosti iz source 1*2*3*4...
	////i rezultat upisuje u result promenljivu procesa root tj 7
	//MPI_Reduce(source, result, 2, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
	//if (rank == root)printf("PE:%d MPI_SUM result is %d, %d\n", rank, result[0], result[1]);//samo posledni proces ispisuje jer je pokrenuto za 8 procesa


	//if (rank == 5)
	//	root = 45;
	////Broadcast salje svim procesima vrednost iz root procesa 5
	//MPI_Bcast(&root, 1, MPI_INT, 5, MPI_COMM_WORLD);
	//printf("rank: %d, root : %d", rank, root);

	//if (rank == 3)for (int i = 0; i < 8; i++)param[i] = 23 + i;//proces 3 inicijalizuje niz sa vrednostima 23, 24, 25, ..., 30

	////MPI_Scatter deli niz na delove velicine 1 (drugi parametar fje) i smesta te vrednosti u mine svakog od procesa(cetvrti parametar)
	////po 1 podatak (5 parametar)
	//MPI_Scatter(param, 1, MPI_INT, &mine, 1, MPI_INT, 3, MPI_COMM_WORLD);

	//printf("P: %d, mine: %d", rank, mine);

	//mine = 23 + rank;
	////iz mine se uzima po 1 podataka tipa MPI_INT i redja se redom u niz param po jedan podatak istog tipa
	////tj vrednosti mine iz svih procesa smesti u niz param
	//MPI_Gather(&mine, 1, MPI_INT, param, 1, MPI_INT, 7, MPI_COMM_WORLD);
	//if (rank == 7)
	//{
	//	for (int i = 0; i < 8; i++)printf("Proces: %d, param[%d] = %d\n", rank, i,param[i]);
	//}

	//********************
	//Zadatak za PI
	/*N = 10;
	h = 1.0 / (double)N;
	x = (double)((rank * h)+ h/2.0);
	f = (4.0 / (1.0 + x * x)) + (4.0 / (1.0 + (x+size*h) * (x+ size*h)));
	rez = (double)(f * (1.0 / (double)N));
	MPI_Reduce(&rez, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("PI is %.16f, Error is %.16f\n",pi, fabs(pi - PI));
	}*/

	printf("eee");
	n_bar = N / size;

	float* local_a = (float*)malloc(n_bar * sizeof(float));
	float* local_b = (float*)malloc(n_bar * sizeof(float));

	if (rank == 0)
	{
		for ( i = 0; i < N; i++)
		{
			printf("Unesite %d. broj za a:", i);
			scanf("%f", &a[i]);
		}
	}
	MPI_Scatter(a, n_bar, MPI_FLOAT, local_a, n_bar, MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (i = 0; i < N; i++)
		{
			printf("Unesite %d. broj za b:", i);
			scanf("%f", &b[i]);
		}
	}
	MPI_Scatter(b, n_bar, MPI_FLOAT, local_b, n_bar, MPI_FLOAT, 0, MPI_COMM_WORLD);
	for ( i = 0; i < n_bar; i++)
	{
		local_dot += local_a[i] * local_b[i];
	}

	MPI_Reduce(&local_dot, &dot, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("Rezultat je %f", dot);
	}
	free(local_a);
	free(local_b);
	MPI_Finalize();
	return 0;
}