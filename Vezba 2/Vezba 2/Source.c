#include<stdio.h>
#include"mpi.h"

int main(int argc, char** argv)
{
	int size, rank;

	int x, y;
	int niz[30], niz2[2];
	struct {
		double value;
		int rank;
	}in[30], out[30];

	MPI_Request request;//Koristi se za proveru da li je kompletirana neblokirajuca operacija ISend ili IRecv, u MPI_Wait ili MPI_Test
	MPI_Status status;//Sadrzi dodatne informacije o operaciji.
					  //Kod slanja sadrzi eventualne greske, a kod primanja informacije o izvoru poruke, oznaci poruke kao i broj primljenih podataka
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//MPI_IRecv razlika je samo sto zadnji parametar nije vise status, nego je request;
	//MPI_ISend razlika od MPI_Send je sto se dodaje jos request kao zadnju  parametar;
	
	//Ovi requesti se prosledjuju u funkciju za cekanje i testiranje (MPI_Wait i MPI_Test)
	
	//MPI_Wait prima request i status i iz nje se proces vraca kada se zavrsi operacija koja je inicirana sa request tj. ISend ili IRecv. Operacija je blokirajuca
	
	//MPI_Test vraca informacije o trenutnom stanju operacije koja je inicirana sa request, 
	//argument flag se postavlja na true ako je operacija zavrsena u suprotnom je false. Ova operacija nije blokirajuca

	//**** Primer 1 ****
#pragma region Primer1
	//if (rank == 0)
	//{
	//	x = 3;
	//	MPI_Irecv(&y, 1, MPI_INT, 1, 19, MPI_COMM_WORLD, &request);
	//	MPI_Send(&x, 1, MPI_INT, 1, 17, MPI_COMM_WORLD);
	//	MPI_Wait(&request, &status);//Ceka da se zavrsi IRecv jer je tom operacijom iniciran request i u status se nalaze informacije o primanju
	//}
	//else
	//{
	//	x = 5;
	//	MPI_Irecv(&y, 1, MPI_INT, 0, 17, MPI_COMM_WORLD, &request);
	//	MPI_Send(&x, 1, MPI_INT, 0, 19, MPI_COMM_WORLD);
	//	MPI_Wait(&request, &status);//Ceka da se zavrsi IRecv jer je tom operacijom iniciran request i u status se nalaze informacije o primanju
	//}
	//printf("Proces %d: y = %d", rank, y);
#pragma endregion

	//**************************
	//**** GRUPNE OPERACIJE ****
	//**************************

	//MPI_Barrier(COMM) sinhronizacioni mehanizam, tj procesi se blokiraju na toj naredbi dok svi procesi nisu prisutni tj ne dodju do te naredbe
	// 
	//Reduce obilazi promenljivu svakog procesa i izvrsava nad njom neku operaciju koja se navodi, slicno kao forEach samo kroz procese
	//npr Reduce operacija za sabiranje promenljive x svakog procesa i smestanje u promenljivu y procesa 0:
	//MPI_Reduce(&x, &y, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

#pragma region Primer2
	//x = rank + 1;//svaki proces ima za x jedan vise od ranka znaci [1, 2, 3, 4, ...];
	//MPI_Reduce(&x, &y, 1, MPI_INT, MPI_PROD, 5, MPI_COMM_WORLD);//Reduce prolazi kroz svaku vrednost x u procesu i vrsi mnozenje i rez smesta u y procesa 5
	//if (rank == 5)
	//{
	//	printf("Proces %d: Faktorijel %d", rank, y);
	//}
#pragma endregion

	//MPI_Scan razlika od reduce je sto nema rank procesa u koji ce da se smesti rezultat, tj.
	//SCAN ce da smesti neki rezultat odnosno medjurezultate u svaki proces i onda krajnju rezultat ce da se nalazi u poslednjem procesu
#pragma region Primer3

	//x = rank + 1;//svaki proces ima za x jedan vise od ranka znaci [1, 2, 3, 4, ...];
	//MPI_Scan(&x, &y, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

	//printf("Proces %d: rez %d\n", rank, y);

#pragma endregion

	//MPI_Bcast kopira podatke iz memorije root procesa u mesta u memoriji ostalih procesa, root se prosledjuje kao argument.
	//Ovu funkciju moraju pozvati svi procesi koji ucestvuju u komunikaciji tj. procesi u okviru komunikatora
	//NPR: P0 sadrzi podatak x, nakon Bcasta svi procesi (Pi i=[0..size]) u okviru komunikatora ce sadrzati podatak x
#pragma region Primer4
	//if (rank == 4)
	//{
	//	x = 6; // u ovom trenutku samo P4 sadrzi za x vrednost 6 u memoriji
	//}

	//MPI_Bcast(&x, 1, MPI_INT, 4, MPI_COMM_WORLD);// kopira vrednost iz x promenljive procesa 4 u sve ostale procese

	//printf("Proces %d: x = %d", rank, x);
#pragma endregion

	//MPI_Scatter radi kao neko razdvajanje npr da imamo niz[10] elemenata i 10 procesa nakon sketera ce svaki proces da dobije po jedan element niza
	//znaci salje se iz send_buffera, send_count podataka, procesa koji se navodi u rank, i procesi ce da prime u recv_buffer recv_count podataka
#pragma region Primer5
	//if (rank == 3)
	//{
	//	for (int i = 0; i < 8; i++)
	//		niz[i] = i;
	//}

	//MPI_Scatter(niz, 2, MPI_INT, niz2, 2, MPI_INT, 3, MPI_COMM_WORLD);//salje se po 1 podatak svakom procesu i svaki proces prima po 1 podatak
	//
	//printf("Proces %d: x = [%d, %d]", rank, niz2[0], niz2[1]);
#pragma endregion

	//MPI_Gather je obrnuto od scatter jer on uzima sad podatke iz procesa i kopira ih u neki segment nekog procesa

#pragma region Primer6
	//x = 5 + rank;
	//MPI_Gather(&x, 1, MPI_INT, niz, 1, MPI_INT, 7, MPI_COMM_WORLD);//u niz procesa 7 se redom redjaju podaci koji se uzimaju iz x promenljive svakog procesa

	//if (rank == 7)
	//{
	//	for (int i = 0; i < size; i++) printf("%d\n", niz[i]);
	//}
#pragma endregion


	//**************************
	//*****     ZADACI     *****
	//**************************

#pragma region MinMax
//struct { // strukrura za ulazni i izlazni buffer ako se koriste MINLOC ili MAXLOC
//	int value;
//	int rank;
//}in, out;
//in.value = rank + 5;
//in.rank = rank;
//
//MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);//prolazimo kroz strukture i odredjujemo koja ima mx vrednost za value i imamo i njen rank
//if (rank == 0)
//{
//	printf("Maksimalni element je %d i nalazi se u procesu %d\n", out.value, out.rank);
//}
//MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
//if (rank == 0)
//{
//	printf("Minimalni element je %d i nalazi se u procesu %d", out.value, out.rank);
//}
#pragma endregion

#pragma region MAX30
	for (int i = 0; i < 30; i++)
	{
		in[i].value = (double)(rank + i + 2.0);
		in[i].rank = rank;
	}
	MPI_Reduce(in, out, 30, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < 30; i++) {
			printf("outval %f ", out[i].value);
			printf("outrank %d\n", out[i].rank);
		}
	}
#pragma endregion


	MPI_Finalize();
	return 0;
}