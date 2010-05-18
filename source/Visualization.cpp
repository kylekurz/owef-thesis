//**********************************************
// File:	Visualization.cpp
//
// Author:	Kyle Kurz
//
// Code contributions: 	Xiaoyu Liang, Josh Welch,
//			Frank Drews, Jens Lichtenberg
//
// Date:	4.6.10
//**********************************************

#include "Visualization.h"

using namespace std;

Visualization::Visualization()
{
	//no-op
}

Visualization::Visualization(OWEFArgs *from_input, DataStructure *structure, WordScoring *model)
{
	printf("Creating visualizations\n");
	list = from_input;
	//any additional visualizations can be added like this:
	if(list->cgr)
		createCGR(structure, model);
}

Visualization::~Visualization()
{
	list = NULL;
	delete list;
}

//function to iterate through all the words in the input and create a visualization
void Visualization::createCGR(DataStructure *structure, WordScoring *model)
{
#ifdef KKURZ_MPI
	MPI_Status status;
	int rc, rec;
	char buf[list->maxlength+1];
	for(int i=0; i<list->maxlength+1; i++)
	buf[i] = (char)NULL;
#endif

	int j, num_w;
	string next_word;


#ifdef KKURZ_MPI
#ifdef DEBUG
	printf("Size of long: %ld \nSize of unsigned long: %ld\n", sizeof(long), sizeof(unsigned long));
	printf("Rank %d, pref %c, reverse %d requesting num_words from %d\n", list->rank, list->pref, branch_array[list->pref - 'A'], list->hosts.host_array[branch_array[list->pref - 'A']][0]);
#endif
	int stat;
	rc = MPI_Send(&buf, 0, MPI_CHAR, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD);
	rc = MPI_Recv(&stat, 1, MPI_INT, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD, &status);
	stat = list->maxlength;
	rc = MPI_Send(&stat, 1, MPI_INT, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD);
	rc = MPI_Recv(&rec, 1, MPI_INT, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD, &status);
	num_w = rec;
#else
	//structure->reset();
	num_w = list->num_words[list->maxlength - 1 ];

	//create an iterator for the appropriate structure
	DataStructureIterator *it;
	if(list->structure_type.compare("rt") == 0)
		it = new RadixTrieIterator(list, structure, list->maxlength);
#endif

#ifdef _OPENMP
#pragma omp parallel for default(none) shared(it, structure, num_w) private(j, next_word)
#endif
	for( j = 0; j < num_w; j++ )
	{
#ifdef KKURZ_MPI

		for(int v=0; v<list->maxlength+1; v++)
			buf[v] = (char)NULL;
		
#ifdef _OPENMP
#pragma omp critical
{
#endif
		rc = MPI_Send(&buf, 0, MPI_CHAR, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NEXT_WORD_TAG, MPI_COMM_WORLD);
		rc = MPI_Recv(&buf, list->maxlength, MPI_CHAR, list->hosts.host_array[branch_array[list->pref - 'A']][0], MPI_NEXT_WORD_TAG, MPI_COMM_WORLD, &status);
		next_word = "";
		next_word += buf;
#ifdef _OPENMP
}
#endif
#else
#ifdef _OPENMP
#pragma omp critical
{
#endif
		if( it->hasNext() )
			next_word = it->next();
#ifdef _OPENMP
}
#endif
#endif

		/**********************************************
		//Developers, do some stuff here with each word
		**********************************************/



	}
}

	






