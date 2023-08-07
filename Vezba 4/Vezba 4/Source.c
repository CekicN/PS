#include<stdio.h>
#include"mpi.h"
#include<stdlib.h>
#define m 2
#define n 3
#define k 4
int main(int argc, char** argv)
{	
	//IZVEDENI TIPOVI
	
	struct {
		int a;
		double b;
	}value;

	MPI_Aint  indices[2];
	MPI_Datatype old_type[2];

	MPI_Datatype newtype;
	MPI_Datatype resizedType;
	int mat[4][4];
	int niz[16];
	int size, rank;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//MPI_type_struct koristi se za formiranje tipa podataka koji se sastoji od kombinacija osnovnih  tipova
	//pr iz matrice mozemo da izvlacimo tacno pozicije koje nam trebaju i to da bude poseban tip
	
	//MPI_Address koristi se da bi se odredio array_of_diplacments, prima tu adresu izrazenu u bajtovima
	// array_of_diplacments - niz pomeraja

#pragma region Primer_MPI_ADDRESS
	//nalazi pocetake gde se nalaze promenljive u memoriji i smesta ih u indices koji je tipa MPI_Aint
	//MPI_Address(&value.a, &indices[0]);
	//MPI_Address(&value.b, &indices[1]);
	//MPI_Address(&value.c, &indices[2]);

	//indices[1] = indices[1] - indices[0];//kada oduzmemo dobijemo koliko je udaljen podatak od posetnog baseAddr
	//indices[2] = indices[2] - indices[0];
	//indices[0] = 0;
	//for (int i = 0; i < 3; i++)
	//{
	//	printf("Index[%d] = %d\n", i, indices[i]);
	//}
#pragma endregion

	//MPI_Type_contiguous formira novi tip kopiranjem na uzastopne mem lokacije 
	//npr:  MPI_Type_contiguous(2,MPI_DOUBLE,&MPI_2D_POINT); ce kreirati novi tip za dva MPI_Double uzastopno

	//MPI_Type_vector omogucava kreiranje novog tipa na osnovu starog, koji se nalaze na odvojenim lokacijama u memoriji
	//i gde se svaki blok sastoji od istog broja elemenata starog tipa

#pragma region PRIMERI_VEKTOR

	// kreiramo tip koji ce da sadrzi 4 bloka,
		//svaki blok ce da ima po 2 podatka MPI_INT,
		//i razmak izmedju blokova je 4 podatka MPI_INT 
		//(razmak je od pocetka jednog bloka do pocetka drugog bloka, 
		//sto mu dodje 2 za blok i jos 2 za razmak sto znaci da je stride = 4)

		//count, blocklen, stride, stari_tip, novi_tip
	//MPI_Type_vector(4, 2, 4, MPI_INT, &newtype);
	//MPI_Type_commit(&newtype);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			mat[i][j] = i + j;
	//		}
	//	}

	//	
	//	MPI_Send(&mat[0][1], 1, newtype, 1, 0, MPI_COMM_WORLD);//saljemo od drugog elementa podatke novog tipa

	//}
	//else
	//{
	//	MPI_Recv(mat, 1, newtype, 0, 0, MPI_COMM_WORLD, &status);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			printf("%d ", mat[i][j]);
	//		}
	//		printf("\n");
	//	}
	//}
#pragma endregion
	//MPI_Type_indexed omogucava da blokovi budu razlicitih velicina, kao i da razmaci izmedju blokova(stride) budu razlicitih velicina
	//umesto blocklen kao kod vektora se prosledjuje niz blocklenova takodje i za razmake

#pragma region PRIMER_Type_indexed
	//int blocklen[] = { 3, 2, 3, 4 };
	//int displacement[] = {0, 5, 8, 12};//ovaj stride je u stvari displacements i racuna se od baseaddr tj od prvog elementa
	//MPI_Type_indexed(4, blocklen, displacement, MPI_INT, &newtype);
	//MPI_Type_commit(&newtype);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			mat[i][j] = i + j;
	//		}
	//	}

	//	
	//	MPI_Send(&mat[0][0], 1, newtype, 1, 0, MPI_COMM_WORLD);//saljemo od drugog elementa podatke novog tipa

	//}
	//else
	//{	
	//	MPI_Recv(mat, 1, newtype, 0, 0, MPI_COMM_WORLD, &status);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			printf("%d ", mat[i][j]);
	//		}
	//		printf("\n");
	//	}
	//}
#pragma endregion
#pragma region PRIMER_NOVI-TIP_U_NIZ
	//MPI_Type_vector(4, 2, 4, MPI_INT, &newtype); // na bilo koji nacin kreiramo novi tip
	//MPI_Type_commit(&newtype);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			mat[i][j] = i + j;
	//		}
	//	}

	//	
	//	MPI_Send(&mat[0][1], 1, newtype, 1, 0, MPI_COMM_WORLD);//saljemo od drugog elementa podatke novog tipa

	//}
	//else
	//{
	//	MPI_Recv(niz, 8, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);// u niz primamo 8 podatka MPI_INT a salje se kao 1 podatak novog tipa

	//	for (int i = 0; i < 8; i++)
	//		printf("%d, ", niz[i]);
	//}
#pragma endregion
	
	//MPI_Type_create_subarray tip se kreira opisom n-dimenzionalnog podpolja unutar n-dimenzionalnog polja
#pragma region PRIMER_SUBARRAY
	//int array_size[] = { 4, 4 };
	//int subarray_size[] = { 2, 2 };
	//int subarray_start[] = { 1, 1 };
	////Kreiramo tip koji iz matrice izvlaci podmatricu definisanu sa indeksima odakle pocinje(subarray_start),
	////koja je valicina podmatrice(subarray_size)
	////velicina same matrice (array_size)
	//MPI_Type_create_subarray(2, array_size, subarray_size, subarray_start, MPI_ORDER_C, MPI_INT, &newtype);
	//MPI_Type_commit(&newtype);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			mat[i][j] = i + j;
	//		}
	//	}

	//	
	//	MPI_Send(&mat[0][0], 1, newtype, 1, 0, MPI_COMM_WORLD);//saljemo od drugog elementa podatke novog tipa

	//}
	//else
	//{	
	//	MPI_Recv(mat, 1, newtype, 0, 0, MPI_COMM_WORLD, &status);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			printf("%d ", mat[i][j]);
	//		}
	//		printf("\n");
	//	}
	//}


#pragma endregion

#pragma region ZAD1
	/*MPI_Type_vector(4, 1, 4, MPI_INT, &newtype);
	MPI_Type_commit(&newtype);
	if (rank == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mat[i][j] = i + j;
				printf("%d ", mat[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		MPI_Send(mat, 1, newtype, 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(mat, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				printf("%d ", mat[i][j]);
			}
			printf("\n");
		}
	}*/
#pragma endregion
#pragma region ZAD2
	//n = 4;
	////Ovo je hardkodirano za matricu 4x4
	//int blocklen[] = {4, 3, 2, 1};
	//int stride[] = { 0, 5, 10, 15 };

	////U opstem slucaju da se izvuku podaci gornje trougaone matrice radi se 
	//for (int i = 0; i < n; i++)
	//{
	//	blocklen[i] = n - i;
	//	stride[i] = (n + 1) * i;
	//}

	////Za donju trougaonu ide 
	///*for (int i = 0; i < n; i++)
	//{
	//	blocklen[i] = i + 1;
	//	stride[i] = n * i;
	//}*/
	//MPI_Type_indexed(4, blocklen, stride, MPI_INT, &newtype);
	//MPI_Type_commit(&newtype);
	//if (rank == 0)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			mat[i][j] = i + j;
	//			printf("%d ", mat[i][j]);
	//		}
	//		printf("\n");
	//	}
	//	printf("\n");
	//	MPI_Send(mat, 1, newtype, 1, 0, MPI_COMM_WORLD);
	//}
	//else
	//{
	//	for (int i = 0; i < 4; i++)
	//		for (int j = 0; j < 4; j++)
	//			mat[i][j] = 0;
	//	MPI_Recv(mat, 1, newtype, 0, 0, MPI_COMM_WORLD, &status);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			printf("%d ", mat[i][j]);
	//		}
	//		printf("\n");
	//	}
	//}
	
#pragma endregion
#pragma region ZAD3
	//int blocklens[] = { 1, 1 };
	//old_type[0] = MPI_INT;
	//old_type[1] = MPI_DOUBLE;

	//MPI_Address(&value.a, &indices[0]);
	//MPI_Address(&value.b, &indices[1]);
	//indices[1] -= indices[0];
	//indices[0] = 0;

	////koliko sadrzi promenljivih, za svaku promenljivu koliko podataka(blocklens), pomeraj, stari tipovi
	//MPI_Type_struct(2, blocklens, indices, old_type, &newtype);
	//MPI_Type_commit(&newtype);
	//if (rank == 0)
	//{
	//	scanf("%d %lf", &value.a, &value.b);
	//}

	//MPI_Bcast(&value, 1, newtype, 0, MPI_COMM_WORLD);

	//printf("PROCES: %d, INT: %d , DOUBLE: %lf\n", rank, value.a, value.b);

#pragma endregion

	//Kada zelimo da posaljemo vise izvedenih tipova, moramo da vodimo racuna jer se odma nakon slanja odma salje sledeci blok podatka
	// _ je jedno polje starog tipa
	// [] izmedju se nalazi podatak koji se uzima za novi tip
	//Pr [__]__[__]__ da je ovo izveden tip sa koji ima 2 bloka od po 2 podatka sa stride 4 odnosno razmak izmedju je 2 _
	//  i hocemo da posaljemo 3 takva medjutim poslednje dve crtice se ne vide pa dodje do spajanja
	// [__]__[__][__]__[__][__]__[__] ovo nije bas dobro jer se spajaju [__][__] a treba sve da bude odvojeno sa po 2 crtice (__)
	//Za to se koristi MPI_Type_create_resized
	//extent je nova velicina tipa tj. stavimo da velicina tipa bude produzena za one dve crtice sto se ne vide
	
#pragma region Primer

	//n = 4;
	//MPI_Type_vector(n, n / size, n, MPI_INT, &newtype);//tip da svaki proces uzme jednak broj kolona tj n/p kolona
	//MPI_Type_commit(&newtype);

	//if (rank == 0)
	//{
	//	for (int i = 0; i < n; i++)
	//		for (int j = 0; j < n; j++)
	//			mat[i][j] = i + j;
	//}
	//int nizz[8];

	////Sketerujemo matricu tako da svaki proces dobije jednak broj kolona i to se smesti u nizz 
	//// 
	////MPI_Scatter(mat, 1, newtype, nizz, n * n / size, MPI_INT, 0, MPI_COMM_WORLD);
	//// 
	////P0 ce da primi dobro u niz ali nakon njega ce sketerovanje da se nastavi odma nakon te kolone u memoriji i moze da se salje bilo sta
	////sto je greska jer treba da se nastavi pd sledece kolone i zbog toga treba da odradimo resize

	//MPI_Type_create_resized(newtype, 0, n / size * sizeof(int), &resizedType);
	//MPI_Type_commit(&resizedType);
	//MPI_Scatter(mat, 1, resizedType, nizz, n * n / size, MPI_INT, 0, MPI_COMM_WORLD);

	//for (int i = 0; i < n * n / size; i++)
	//{
	//	printf("PROCES %d: %d \n", rank, nizz[i]);
	//}
#pragma endregion

#pragma region ZAD4
	/*m = 2;
	n = 3;
	k = 4;

	int** A = malloc(m * sizeof(int*));
	for (int i = 0; i < m; i++)
		A[i] = malloc(n * sizeof(int));

	int** B = malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		B[i] = malloc(k * sizeof(int));

	int** C = malloc(m * sizeof(int*));
	for (int i = 0; i < m; i++)
		C[i] = malloc(k * sizeof(int));

	int** local_c = malloc(m * sizeof(int*));
	for (int i = 0; i < m; i++)
		local_c[i] = malloc(k * sizeof(int));

	int* col = malloc(m * sizeof(int));
	int* row = malloc(k * sizeof(int));*/
int A[m][n];
int B[n][k];
int C[m][k];
int local_c[m][k];
int col[m];
int row[k];

	if (rank == 0)
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				A[i][j] = i;

		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				B[i][j] = i;
	}
	
	MPI_Type_vector(m, 1, n, MPI_INT, &newtype);
	MPI_Type_commit(&newtype);
	MPI_Type_create_resized(newtype, 0, sizeof(int), &resizedType);
	MPI_Type_commit(&resizedType);

	MPI_Scatter(A, 1, resizedType, col, m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, k, MPI_INT, row, k, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < k; j++)
		{
			local_c[i][j] = col[i] * row[j];
		}
	}

	MPI_Reduce(local_c, C, m * k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
			{
				printf("%d ", C[i][j]);
			}
			printf("\n");
		}
	}

#pragma endregion

	 
	MPI_Finalize();
	return 0;
}