#include<stdio.h>
#include"mpi.h"

int main(int argc, char** argv)
{
	//MPI_Comm_group kao argument uzima komunikator i vraca grupu

	//MPI_Group_excl vraca novu grupu tako sto se iz stare grupe iskljuce procesi sa identifikatorima nonmembers kojih ima count
	//navode se procesi koji se iskljuce i kreira se nova grupa

	//MPI_Group_incl vraca novu grupu tako sto procesi koji su u members ukljucuje u novu grupu i ima ih count
	//Proces members[i] u novoj gtupi ima rang i tj 0, 1, 2... , a ne stare rangove procesa

	//MPI_Group_intersection prosledjuju se dve grupe i pravi novu grupu koja je presek ove dve grupe, a redosled je kao kod prve grupe

	//MPI_Group_union radi se unija dve grupe i kreira se nova grupa

	//MPI_Group_difference uzima se razlika izmadju dve grupe i kreira se nova grupa

	//MPI_Comm_create od starog komunikatora i grupe kreira novi komunikator

	//MPI_Comm_split razbja stari komunikator na vise delova tako da svi procesi koji imaju istu vrednost za color pripadaju istom podkomunikatoru
	//key odredjuje redosled prvo ide sa najmanjim key, ako je key isti gleda se stari rankovi
	
	int members[8], parni_rank, neparni_rank;
	MPI_Group group_world,parni, neparni;
	MPI_Comm newcomm;
	int color, key;
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_group(MPI_COMM_WORLD, &group_world);
#pragma region ParniNeparni
	//PRVI NACIN SA incl i excl
	
	//int n = (size + 1) / 2;
	//for (int i = 0; i < n; i++)
	//{
	//	members[i] = 2 * i;
	//}
	//MPI_Group_incl(group_world, n, members, &parni);//U novu grupu ukljuci samo parne
	//MPI_Group_rank(parni, &parni_rank);
	//MPI_Group_excl(group_world, n, members, &neparni);//iz stare grupe iskljuci parne, tj. ostanu samo neparni za novu grupu
	//MPI_Group_rank(neparni, &neparni_rank);

	//printf("moj rank je: %d, moj even rank je %d, moj odd rank je %d\n",rank,parni_rank,neparni_rank);

	//DRUGI NACIN SA PODKOMUNIKATORIMA

	/*color = rank % 2;
	key = 5;
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &newcomm);
	MPI_Comm_rank(newcomm, &parni_rank);
	MPI_Comm_size(newcomm, &size);

	printf("moj rank je: %d, moj key je %d, moj rankc je %d moj color je %d moj size je % d\n",rank,key,parni_rank,color,size);*/

#pragma endregion



	MPI_Finalize();
	return 0;
}