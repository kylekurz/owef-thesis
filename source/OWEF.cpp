/*
 **********************************************
 // File:	OWEF.cpp
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define a class for use
 //		with various data structures and
 //		scoring models in the Open Word
 //		Enumeration Framework.
 **********************************************

 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "OWEF.h"

#define NUM_THREADS 4

using namespace std;

extern "C" void *ThreadStartup2( void * );

//structure for passing to parallel processor
typedef struct args
{
		int thr_id;
		OWEF *job;
} args;

//standard constructor
OWEF::OWEF()
{
	//no-op
}

#ifdef KKURZ_MPI
//initialized constructor
OWEF::OWEF(OWEFArgs *from_input)
{
	method = NULL;
	structure = NULL;
	list = NULL;
	tool = NULL;
	type = NULL;
	model = NULL;

#ifdef DEBUG
	printf("getting the job information\n");
#endif
	list = from_input;
	clock_t start, end;
	double duration;
	MPI_Status status;
	//create a sequence file object to read in the information
	SequenceFile *input;
	input = new SequenceFile(list->seq_file.c_str());
	
	//populate the list->total_input_length and list->background_seqs variables
	vector<Sequence*> seqs, segs, ind;
	seqs = input->getSequences();
	int h;

	for( h = 0; h < static_cast<int> ( seqs.size() ); h++ )
	{		
		ind.push_back( seqs[ h ] );
		if( list->n_filter )
		{
			segs = SequenceFilters::filterNxM( ind, SequenceFilters::RadixFilterDelimiterFunc );
		}
		else
		{
			segs = ind;
		}
		if( list->anc_filter )
		{
			segs = SequenceFilters::filterNxM( segs, SequenceFilters::AncestorFilterDelimiterFunc );
		}
		int tl = 0;
		for( int x = 0; x < static_cast<int> ( segs.size() ); x++ )
		{
			tl += segs[x]->getBodyLength();
		}
		ind.clear();
		segs.clear();
		list->total_input_length += tl;
		list->background_seqs.push_back(tl);
	}	

	//determine what I'm supposed to do...
	for(int i=0; i<NUMHOSTS; i++)
	{
		for(int j=0; j<NUMPERHOST; j++)
		{
			if(list->hosts.host_array[i][j] == list->rank)
			{
				list->pref = 'A' + reverse_branch[i];
#ifdef DEBUG
				printf("pref: %c\n", list->pref);
#endif
				//if I am supposed to count, set the prefix and start counting
				if(j == 0)
				{
					printf("Ranks %d, %d processing words with prefix %c\n\n", list->rank, list->hosts.host_array[i][1], list->pref);
					//do the counting
					start = clock();
					if(list->count)
					{
						
						if(list->structure_type.compare("rt") == 0)
							structure = new RadixTrie(list, input);
					}
					end = clock();
					duration = (double)(end-start)/CLOCKS_PER_SEC;
					printf("Rank %d enumeration finished in: %f seconds\n\n", list->rank, duration);
					int req, rc;
					req = 1;
					if(list->rank == 0)
					{
#ifdef DEBUG
						printf("waiting for updates\n");
#endif
						int stat;
						//make sure all counting is done before we start scoring
						for(int i=1; i<from_input->numtasks/2; i++)
						{
							rc = MPI_Recv(&stat, 1, MPI_INT, list->hosts.host_array[i][0], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
						}
#ifdef DEBUG
						printf("got everyone's updates\n");
#endif

						printf("total_input_length for rank %d is %ld\n", list->rank, list->total_input_length);
						printf("background_seqs length for rank %d is %d\n", list->rank, (int)list->background_seqs.size());

						//send go-ahead to scoring stages
						for(int i=0; i<NUMHOSTS; i++)
						{
							stat = 1;
							rc = MPI_Send(&stat, 1, MPI_INT, list->hosts.host_array[i][1], 1, MPI_COMM_WORLD);
						}
					}
					else
						rc = MPI_Send(&req,1, MPI_INT, 0, 1, MPI_COMM_WORLD);
					//create an iterator for the local structure and post a receive for requests
					DataStructureIterator *it[list->numtasks];
					for(int x=0; x<list->numtasks; x++)
					{
						if(list->structure_type.compare("rt") == 0)
							it[x] = new RadixTrieIterator(list, structure, list->maxlength);
					}
					int done = 0;
					char word[list->maxlength+2];
					while(!done)
					{
						for(int i=0; i<list->maxlength+2; i++)
						word[i] = (char)NULL;
						int stat, dest;
						string nw;
#ifdef DEBUG
						if(list->rank == 6)
						printf("Rank %d posting receive.\n", list->rank);
#endif
						rc = MPI_Recv(&word, list->maxlength+2, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
#ifdef DEBUG
						if(list->rank == 6)
						printf("Rank %d received %s with tag %d\n", list->rank, word, status.MPI_TAG);
#endif
						stat = status.MPI_TAG;
						dest = status.MPI_SOURCE;
						if(stat == MPI_DONE_TAG)
						done = 1;
						else if(stat == MPI_NUMWORDS_TAG)
						{
							int l, stat;
							stat = 1;
#ifdef DEBUG
							printf("%d sending %d to %d\n", list->rank, stat, dest);
#endif
							rc = MPI_Send(&stat, 1, MPI_INT, dest, MPI_NUMWORDS_TAG, MPI_COMM_WORLD);
#ifdef DEBUG
							printf("%d posting receive\n", list->rank);
#endif
							rc = MPI_Recv(&l, 1, MPI_INT, dest, MPI_NUMWORDS_TAG, MPI_COMM_WORLD, &status);
							int c = list->num_words[l-1];
#ifdef DEBUG
							printf("sending %d back to %d\n", c, dest);
#endif
							rc = MPI_Send(&c, 1, MPI_INT, dest, MPI_NUMWORDS_TAG, MPI_COMM_WORLD);
						}
						else if(stat == MPI_NEXT_WORD_TAG)
						{
							if(it[dest]->hasNext())
							nw = it[dest]->next();
							else
							nw = "";
#ifdef DEBUG
							printf("Rank %d sending: %s\n", list->rank, nw.c_str());
#endif
							rc = MPI_Send(&nw[0], nw.length(), MPI_CHAR, dest, MPI_NEXT_WORD_TAG, MPI_COMM_WORLD);
						}
						else if(stat == MPI_COUNT_TAG)
						{
							nw = "";
							nw += word;
#ifdef DEBUG
							printf("Rank %d counting word %s\n", list->rank, nw.c_str());
#endif
							int c = structure->getCount(nw);
#ifdef DEBUG
							printf("count is %d for word %s\n", c, nw.c_str());
#endif
							rc = MPI_Send(&c, 1, MPI_INT, dest, MPI_COUNT_TAG, MPI_COMM_WORLD);
						}
						else if(stat == MPI_SEQS_TAG)
						{
							nw = "";
							nw += word;
							int c = structure->getSeqs(nw);
#ifdef DEBUG
							printf("seqs is %d for word %s\n", c, nw.c_str());
#endif
							rc = MPI_Send(&c, 1, MPI_INT, dest, MPI_SEQS_TAG, MPI_COMM_WORLD);
						}
						else if(stat == MPI_REGEX_TAG)
						{
/*
#ifdef DEBUG
							printf("finding regex matches for %s\n", word);
#endif
							nw = "";
							nw += word;
							vector<string> matches;
							structure->getRegexMatches(matches, nw);
							//printf("got %d matches\n", (int)matches.size());
							int nts = matches.size();
							rc = MPI_Send(&nts, 1, MPI_INT, dest, MPI_REGEX_TAG, MPI_COMM_WORLD);
							for(int i=0; i<nts; i++)
								rc = MPI_Send(&matches[i][0], matches[i].length(), MPI_CHAR, dest, MPI_REGEX_TAG, MPI_COMM_WORLD);
*/
						}
						else if(stat == MPI_LOCS_TAG)
						{
							nw = "";
							nw += word;
							int zero = 0;
							vector<int> matches;
							matches.resize(list->background_seqs.size(), zero);
							structure->getLocs(matches, nw);
							//printf("got %d matches\n", (int)matches.size());
							int nts = matches.size();
							rc = MPI_Send(&nts, 1, MPI_INT, dest, MPI_LOCS_TAG, MPI_COMM_WORLD);
							for(int i=0; i<nts; i++)
								rc = MPI_Send(&matches[i], 1, MPI_INT, dest, MPI_LOCS_TAG, MPI_COMM_WORLD);
						}
						else if(stat == MPI_RESET_TAG)
						{
							it[dest] = NULL;
							if(list->structure_type.compare("rt") == 0)
								it[dest] = new RadixTrieIterator(list, structure, list->maxlength);
						}
					}
				}
				//otherwise, wait to be told to move on

				else
				{

					MPI_Status status;
					int rec, rc;

					//get the go-ahead that everyone is done counting
					rc = MPI_Recv(&rec, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

					printf("total_input_length for rank %d is %ld\n", list->rank, list->total_input_length);
					printf("background_seqs length for rank %d is %d\n", list->rank, (int)list->background_seqs.size());

					//do the scoring
					start = clock();
					if(list->score)
					{
						model = new BasicModel(list, structure);
					}
					end = clock();
					duration = (double)(end-start)/CLOCKS_PER_SEC;
					printf("Rank %d scoring finished in: %f seconds\n\n", list->rank, duration);

					//are we clustering?
					if(list->cluster)
					{
						method = new BasicMethod(list, structure, model);
						end = clock();
						duration = (double)(end-start)/CLOCKS_PER_SEC;
						printf("\n\nWord Clustering finished in: %f seconds\n\n\n", duration);
					}
					//are we doing word distribution?
					if(list->word_distribution)
					{
						start = clock();
						type = new WordDistribution(list, structure, model, input);
						end = clock();
						duration = (double)(end-start)/CLOCKS_PER_SEC;
						printf("\n\nWord distribution finished in: %f seconds\n\n\n", duration);
					}
					if(list->sequence_clustering)
					{
						start = clock();
						tool = new SequenceClustering(list, structure, model);
						end = clock();
						duration = (double)(end-start)/CLOCKS_PER_SEC;
						printf("\n\nSequence clustering finished in: %f seconds\n\n\n", duration);
					}
				}
			}
		}
	}
}
#else
//initialized constructor
OWEF::OWEF( OWEFArgs *from_input )
{
	method = NULL;
	structure = NULL;
	list = NULL;
	tool = NULL;
	type = NULL;
	model = NULL;
	list = from_input;
	//create a sequence file object to read in the information
	SequenceFile *input;
	input = new SequenceFile( list->seq_file.c_str() );
	
	//populate the list->total_input_length and list->background_seqs variables
	vector<Sequence*> seqs, segs, ind;
	seqs = input->getSequences();
	int h;
	
	for( h = 0; h < static_cast<int> ( seqs.size() ); h++ )
	{		
		ind.push_back( seqs[ h ] );
		if( list->n_filter )
		{
			segs = SequenceFilters::filterNxM( ind, SequenceFilters::RadixFilterDelimiterFunc );
		}
		else
		{
			segs = ind;
		}
		if( list->anc_filter )
		{
			segs = SequenceFilters::filterNxM( segs, SequenceFilters::AncestorFilterDelimiterFunc );
		}
		int tl = 0;
		for( int x = 0; x < static_cast<int> ( segs.size() ); x++ )
		{
			tl += segs[x]->getBodyLength();
		}
		ind.clear();
		segs.clear();
		list->total_input_length += tl;
		list->background_seqs.push_back(tl);
	}	
	
	int t_num;
	pthread_t threads[ NUM_THREADS ];
	//are we counting?
	clock_t start, end;
	double duration;
	start = clock();
	if( list->count )
	{
		if(list->structure_type.compare("rt") == 0)
			structure = new RadixTrie(list, input);
	}
	end = clock();
	duration = (double) (end - start) / CLOCKS_PER_SEC;
	printf( "\n\nEnumeration finished in: %f seconds\n\n\n", duration );
	//are we scoring?
	start = clock();
	if( list->score )
	{
		model = new BasicModel(list, structure);
	}
	end = clock();
	duration = (double) (end - start) / CLOCKS_PER_SEC;
	printf( "\n\nScoring finished in: %f seconds\n\n\n", duration );
	//are we doing module discovery?
	start = clock();
	end = clock();
	duration = (double) (end - start) / CLOCKS_PER_SEC;
	printf( "\n\nModule enumeration finished in: %f seconds\n\n\n", duration );
	//do the rest in parallel
	for( t_num = 0; t_num < NUM_THREADS; t_num++ )
	{
		//cout << "creating thread: " << t_num << endl;
		//thread_seqs[t_num] = 0;
		args *p = new args;
		p->thr_id = t_num;
		//p->thread = prefix[t_num];
		//p->sequence = tag;
		p->job = this;
		pthread_create( &threads[ t_num ], NULL, ThreadStartup2, (void *) p );
	}
	for( t_num = 0; t_num < 4; t_num++ )
		pthread_join( threads[ t_num ], NULL );
}

#endif

//standard destructor		
OWEF::~OWEF()
{
	delete structure;
	delete model;
	delete method;
	delete tool;
	delete type;
	list = NULL;
	delete list;
}

//************************************************************
//Accessors
//All accessors simply return the value of their associated
//variable.  No modifications allowed.
//************************************************************

//************************************************************
//Modifiers
//All modifiers return their new value by default.  This may
//be ignored based on developer preference.
//************************************************************

//************************************************************
//General Purpose Functions
//May do any number of operations, should still be implmented
//for each data structure incorporated.
//************************************************************

//function to iterate through the words contained in the data structure
/*string OWEF::getNextWord( int length )
{
	string motif;
	structure->getNextWord( motif, length );
	return motif;
}

//output all the information
void OWEF::output()
{
	structure->output();
}*/

void *OWEF::process( void *foo )
{
	clock_t start, end;
	//create a sequence file object to read in the information
	SequenceFile *input;
	input = new SequenceFile( list->seq_file.c_str() );
	double duration;
	args *p = (args *) foo;
	if( p->thr_id == 1 )
	{
		//are we clustering?
		if( list->cluster )
		{
				method = new BasicMethod(list, structure, model);
		}
	}
	else if( p->thr_id == 2 )
	{
		//are we doing word distribution?
		if( list->word_distribution )
		{
			start = clock();
			type = new WordDistribution( list, structure, model, input );
			end = clock();
			duration = (double) (end - start) / CLOCKS_PER_SEC;
			printf( "\n\nWord distribution finished in: %f seconds\n\n\n", duration );
		}
	}
	else if( p->thr_id == 3 )
	{
		//are we doing CoOccurrence analysis?
		if( list->cooccur )
		{
		}
	}
	else if( p->thr_id == 4 )
	{
		if( list->sequence_clustering )
		{
			start = clock();
			tool = new SequenceClustering( list, structure, model );
			end = clock();
			duration = (double) (end - start) / CLOCKS_PER_SEC;
			printf( "\n\nSequence clustering finished in: %f seconds\n\n\n", duration );
		}
	}
	return NULL;
}

void *ThreadStartup2( void *_Object )
{
	args *p = (args *) _Object;
	return p->job->process( _Object );
}

