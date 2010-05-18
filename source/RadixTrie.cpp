//**********************************************
// File:	Radix_trie.cpp
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie class for use
//		with the WordSeeker pipeline.
//**********************************************
/*
 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran
 */

#include "RadixTrie.h"

using namespace std;

extern "C" void *listener( void * );

//structure for passing to parallel processor
typedef struct args
{
		int thr_id;
		RadixTrie *job;
		OWEFArgs *my_list;
} args;

//************************************************************
//Constructors
//************************************************************

//standard constructor
RadixTrie::RadixTrie()
{
	root = NULL;
}

//create an empty trie...
RadixTrie::RadixTrie( OWEFArgs *from_input, int x )
{
	list = from_input;
	root = NULL;
}

//standard destructor
RadixTrie::~RadixTrie()
{
	trieClear( root );
	list = NULL;
	delete list;
}

// listen for MPI requests for words
#ifdef KKURZ_MPI
void* listener(void *_Object)
{
	//int flag=0;
	args *p = (args *)_Object;
	int length = (p->my_list->maxlength)+1;
	int done = 1;
	int ret, rc, size;
	MPI_Comm_size (MPI_COMM_WORLD,&size);
	char to_count[p->my_list->maxlength+1];
	MPI_Status status;
	while(done != 0)
	{
		for(int a=0; a<p->my_list->maxlength+1; a++)
		to_count[a] = (char)NULL;
		rc = MPI_Recv(&to_count, length, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("rc on slave receive: %d\n", rc);
		done = status.MPI_TAG;
		string test = to_count;
		if(done == 0)
		{
			MPI_Finalize();
			exit(0);
		}
		else if(done == 1)
		{
			ret = p->job->getCount(test);
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		else if(done == 2)
		{
			ret = p->job->getSeqs(test);
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		}
		else if(done == 3)
		{
			int x = atoi(to_count);
			ret = p->my_list->num_words[x];
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
		}
		else if(done == 4)
		{
			Scores *s = p->job->getStats(test);
			rc = MPI_Send(s, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		}
	}
	return NULL;
}
#endif

//initialized radix trie
RadixTrie::RadixTrie( OWEFArgs *from_input, SequenceFile *in )
{
	rt_size = 0;
	//point to the input parameters
	list = from_input;
	input = in;

#ifndef KKURZ_MPI
	printf( "\nCreating Radix trie...\n" );
#endif

	//set up the trie
	root = NULL;
	//RadixTrieNode *temp = NULL;
	//string word = "";
	/*for( int i = 0; i < list->maxlength; i++ )
	{
		last_loc.push_back( temp );
		last_word.push_back( word );
	}
	for( int h = 0; h < numProcs(); h++ )
	{
		vector<string> t1;
		vector<RadixTrieNode *> t2;
		for( int i = 0; i < list->maxlength; i++ )
		{
			t1.push_back( word );
			t2.push_back( temp );
		}
		last_ext.push_back( t1 );
		last_ext_loc.push_back( t2 );
		next_branch_iterator.push_back( 0 );
	}

	next_branch = 0;*/

	//count the words
	countWords( input );

#ifndef KKURZ_MPI
	//print the number of words found to the screen
	printf( "Number of words of length: \n" );
	for( int i = 0; i < list->maxlength; i++ )
		printf( "%d %d\n", i + 1, (int) list->num_words[ i ] );
#endif
#ifdef KKURZ_MPI
	printf("Rank %d trie has size %Lf GB\n", list->rank, rt_size/1000000000);
#else
	printf("Radix trie has size %Lf GB\n", rt_size/1000000000);
#endif
	//if(!list->score)
	//	output();
	if(list->missing)
	{
		stringstream stream;
		stream << list->prefix << "_" << list->maxlength << "_missing";
#ifdef KKURZ_MPI
		stream << "_" << list->pref;
#endif
		stream << ".csv";
		string filename;
		stream >> filename;
		ofstream ofile;
		ofile.open( filename.c_str() );
		string tmp;
		missSearch(root, ofile, list->maxlength, tmp);
	}
}

//************************************************************
//Accessors
//All accessors simply return the value of their associated
//variable.  No modifications allowed. If data structure does
//not store information, throw exception with (-1)
//************************************************************

//function to get word count
int RadixTrie::getCount( string & motif )
{
	string temp = motif;
	char *t = &temp[ 0 ];
	int l = temp.length();
	return trieFind( root, t, l );
}

//function to get sequence count
int RadixTrie::getSeqs( string & motif )
{
	string temp = motif;
	char *t = &temp[ 0 ];
	int l = temp.length();
	return trieFindS( root, t, l );
}

//function to get the stats from a word
Scores* RadixTrie::getStats( string & motif )
{
	string temp = motif;
	char *t = &temp[ 0 ];
	int l = temp.length();
	return trieFindStats( root, t, l );
}

//function to iteratively walk the regular expression provided and retrieve a list of all words
//in the trie that match
void RadixTrie::getRegexMatches( vector<string> &matches, string & regex )
{
	vector<string> t1;
	RadixTrieNode *tmp = root;
	if(root == NULL)
		return;
	int num_calls = 0;
	int num_n = 0;
	string temp = "";
	for( int i = 0; i < static_cast<int> ( regex.length() ); i++ )
	{
		int nb = branch_array[ regex[ i ] - 'A' ];
		if( nb != ALPH - 1 )
		{
			temp += regex[ i ];
			tmp = tmp->branch[ nb ];
		}
		else
		{
			num_n = 1;
			int pos = i;
			while( branch_array[ regex[ ++pos ] - 'A' ] == ALPH - 1 )
				num_n++;
			string partial = getNextWord( tmp, num_n );
			num_calls++;
			while( partial.compare( "" ) != 0 )
			{
				string t2 = temp + partial;
				t1.push_back( t2 );
				t2.clear();
				partial.clear();
				partial = getNextWord( tmp, num_n );
				num_calls++;
			}
			i = regex.length() + 1;
			tmp = NULL;
			resetIterator( threadNum() );
			next_branch_iterator[ threadNum() ] = 0;
		}
	}
	while( !t1.empty() )
	{
		tmp = root;
		string temp = t1.back();
		t1.pop_back();
		for( int i = 0; i < static_cast<int> ( temp.length() ); i++ )
		{
			int nb = branch_array[ temp[ i ] - 'A' ];
			if( nb != ALPH - 1 )
			{
				tmp = tmp->branch[ nb ];
			}
			else
			{
				num_n = 1;
				int pos = i;
				while( branch_array[ regex[ ++pos ] - 'A' ] == ALPH - 1 )
					num_n++;
				string partial = getNextWord( tmp, num_n );
				num_calls++;
				while( partial.compare( "" ) != 0 )
				{
					string t2 = temp + partial;
					t1.push_back( t2 );
					t2.clear();
					partial.clear();
					partial = getNextWord( tmp, num_n );
					num_calls++;
				}
				i = regex.length() + 1;
				tmp = NULL;
				resetIterator( threadNum() );
				next_branch_iterator[ threadNum() ] = 0;
			}
		}
		for( int j = temp.length(); j < static_cast<int> ( regex.length() ); j++ )
		{
			int nb = branch_array[ regex[ j ] - 'A' ];
			if( nb != ALPH - 1 && tmp->branch && tmp->branch[ nb ] )
			{
				temp += regex[ j ];
				tmp = tmp->branch[ nb ];
				if( temp.length() == regex.length() )
					matches.push_back( temp );
			}
			else
			{
				temp.clear();
				j = regex.length();
			}
		}
	}
	return;
}

//function to get the sequences from the input file
void RadixTrie::getSeqFile( vector<string> &sequences )
{
	ifstream in;
	in.open( list->seq_file.c_str() );
	if( in.fail() )
	{
		cerr << "FAILED TO OPEN INPUT FILE" << endl;
		exit( -1 );
	}
	//read in the file, keeping tags separate from the sequences
	while( !in.eof() )
	{
		string tag;
		if( in.peek() == '>' || in.peek() == ' ' )
		{
			string t;
			getline( in, t );
			continue;
		}
		else if( !in.eof() )
		{
			string t;
			while( in.peek() != '>' && !in.eof() )
			{
				getline( in, t );
				tag.append( t );
			}
			sequences.push_back( tag );
		}
		tag.clear();
	}
	in.close();
	return;
}

//************************************************************
//Modifiers
//All modifiers return their new value by default.  This may
//be ignored based on developer preference.
//
//If not implemented by the data structure, throw (-1)
//************************************************************

int RadixTrie::setCount( string motif, int count )
{
	string temp = motif;
	char *t = &temp[ 0 ];
	int l = temp.length();
	return trieSet( root, t, l, count );
}

int RadixTrie::trieSet( RadixTrieNode *node, char *s, int length, int count )
{
	if( !node )
		return (0);
	if( length == 0 )
	{
		node->data = count;
		return (node->data);
	}
	if( !node->branch )
		return (0);
	return trieSet( node->branch[ locateBranch( *s ) ], s + 1, length - 1, count );
}

//function to set the statistics of a word in the trie
int RadixTrie::setStats( string & motif, Scores *new_stats )
{
	string temp = motif;
	char *t = &temp[ 0 ];
	int l = temp.length();
	return trieStats( root, t, l, new_stats );
}

//function to set the statistics of a word in the trie
int RadixTrie::trieStats( RadixTrieNode *node, char *s, int length, Scores *new_stats )
{
	if( !node )
		return (0);
	if( length == 0 )
	{
		((RadixTrieLeaf *) node)->storage->E = new_stats->E;
		((RadixTrieLeaf *) node)->storage->E_s = new_stats->E_s;
		((RadixTrieLeaf *) node)->storage->palindrome = new_stats->palindrome;
		((RadixTrieLeaf *) node)->storage->revcomp_pres = new_stats->revcomp_pres;
		((RadixTrieLeaf *) node)->storage->Oln = new_stats->Oln;
		((RadixTrieLeaf *) node)->storage->Sln = new_stats->Sln;
		((RadixTrieLeaf *) node)->storage->pval = new_stats->pval;
		((RadixTrieLeaf *) node)->storage->OE = new_stats->OE;
		((RadixTrieLeaf *) node)->storage->count = new_stats->count;
		((RadixTrieLeaf *) node)->storage->seqs = new_stats->seqs;
		return (node->data);
	}
	if( !node->branch )
		return (0);
	return trieStats( node->branch[ locateBranch( *s ) ], s + 1, length - 1, new_stats );
}

//************************************************************
//General Purpose Functions
//May do any number of operations, should still be implmented
//for each data structure incorporated.
//************************************************************

//function to increment count
int RadixTrie::incCount(char *motif, int length )
{
	/*if(list->n_filter)
	 {
	 int N_loc = motif.find_first_not_of("AaCcGgTt");
	 if(N_loc >= 0)
	 return (-1);
	 }
	 if(list->anc_filter)
	 {
	 int anc_res = motif.find_first_of("abcdefghijklmnopqrstuvwxyz");
	 if(anc_res >= 0)
	 return (-1);
	 }*/
	int retval = trieGet( root, motif, length, length - 1 );
	if( retval == -1 )
	{
		trieAdd( root, motif, length, length - 1 );
		return (1);
	}
	else
		return retval;
}

//function to increment seqs
int RadixTrie::incSeqs(char *motif, int length )
{
	//char *t = &motif[ 0 ];
	//int l = motif.length();
	int retval = trieGet( root, motif, length, length - 1 );
	if( retval == -1 )
	{
		trieAdd( root, motif, length, length - 1 );
		return (1);
	}
	else
		return retval;
}

//function to add a new element to the radix trie
void RadixTrie::trieAdd( RadixTrieNode *& node, char *s, int length, int level )
/* add string s to the trie and attach data to it */
{
	//We need all the space saving we can get...
	if( length == 0 )
	{
		list->num_words[ level ] = (list->num_words[ level ]) + 1;
		//cout << "adding to num_words level " << level << endl;
		node->data = 1;
		node->num_seq = 1;
		node->last_seq = list->seq;
		return;
	}
	else
	{
		int idx = locateBranch( *s );
		//if( list->n_filter && idx == 4)
		//	printf("broken: %c, %d\n", *s, length);
		if( !node )
		{
			node = new RadixTrieNode();
			rt_size += sizeof(RadixTrieNode);
		}
		if( !node->branch )
		{
			node->branch = node->newPointerArray();
			rt_size += sizeof(RadixTrieNode *) * ALPH;
			//set all the pointers to NULL (avoid seg fault!)
			for( int i = 0; i < ALPH; i++ )
				node->branch[ i ] = NULL;
		}
		if( !node->branch[ idx ] )
		{
			node->branch[ idx ] = new RadixTrieNode();
			rt_size += sizeof(RadixTrieNode);
		}
		return trieAdd( node->branch[ idx ], s + 1, length - 1, level );
	}

	// If space is no longer an issue, use this...
	/*
	 if (length==0)
	 {
	 list->num_words[level] = (list->num_words[level])+1;
	 node->data = 1;
	 node->num_seq = 1;
	 node->last_seq = list->seq;
	 }
	 else
	 {
	 int idx = locateBranch(*s);
	 if(!node)
	 node = new RadixTrieNode();
	 if (!node->branch)
	 {
	 node->branch = node->newPointerArray();
	 //set all the pointers to NULL (avoid seg fault!)
	 for(int i=0; i<ALPH; i++)
	 node->branch[i]=NULL;
	 }
	 if(!node->branch[idx])
	 {
	 if(length > 1)
	 node->branch[idx] = new RadixTrieNode();
	 if(length == 1)
	 {
	 node->branch[idx] = new RadixTrieLeaf();
	 ((RadixTrieLeaf *)node->branch[idx])->storage = new Scores;
	 }
	 }
	 trieAdd (node->branch[idx], s + 1, length-1, level);
	 }
	 */
}

//function to get the count of a word in the radix trie and increment it
int RadixTrie::trieGet( RadixTrieNode * &node, char *s, int length, int level )
/* return data associated with string s, or NULL if not found */
{
	if( !node )
		return (-1);
	if( length == 0 )
	{
		if( node->data == 0 )
		{
			list->num_words[ level ] = (list->num_words[ level ]) + 1;
			//cout << "adding to num_words level " << level << " with value " << list->num_words[level] << endl;
		}	
		if( node->last_seq != list->seq )
		{
			(node->num_seq)++;
			node->last_seq = list->seq;
		}
		return (node->data)++;
	}
	if( !node->branch )
		return (-1);
	int idx = locateBranch( *s );
	//if( list->n_filter && idx == 4)
	//	printf("broken: %c, %d\n", *s, length);
	if( !node->branch[ idx ] )
		return (-1);
	return trieGet( node->branch[ idx ], s + 1, length - 1, level );
}

//function to get the count of a word in the radix trie (no increment)
int RadixTrie::trieFind( RadixTrieNode * &node, char *s, int length )
/* return data associated with string s, or NULL if not found */
{
	if( !node )
		return (0);
	if( length == 0 )
		return (node->data);
	if( !node->branch )
		return (0);
	return trieFind( node->branch[ locateBranch( *s ) ], s + 1, length - 1 );
}

//function to get the stats of a word in the radix trie (no increment)
Scores* RadixTrie::trieFindStats( RadixTrieNode * &node, char *s, int length )
/* return data associated with string s, or NULL if not found */
{
	if( !node )
		return (NULL);
	if( length == 0 )
		return ((RadixTrieLeaf *) node)->storage;
	if( !node->branch )
		return (NULL);
	return trieFindStats( node->branch[ locateBranch( *s ) ], s + 1, length - 1 );
}

//function to get the sequence count of a word in the radix trie (no increment)
int RadixTrie::trieFindS( RadixTrieNode * &node, char *s, int length )
/* return data associated with string s, or NULL if not found */
{
	if( !node )
		return (0);
	if( length == 0 )
		return (node->num_seq);
	if( !node->branch )
		return (0);
	return trieFindS( node->branch[ locateBranch( *s ) ], s + 1, length - 1 );
}

//function to delete the trie
void RadixTrie::trieClear( RadixTrieNode * node )
{
	if(!node)
		return;
	if(!node->branch)
		return;
	for( int i = 0; i < ALPH; i++ )
	{
		if( node->branch[ i ] != NULL )
		{
			trieClear( node->branch[ i ] );
		}
	}
	delete node;
	node = NULL;
	return;
}

//output to a file so we can use it in the next step
void RadixTrie::output()
{
	int num_files = (list->maxlength - list->minlength) + 1;
	ofstream ofiles[ num_files ];
	for( int i = 0; i < num_files; i++ )
	{
		stringstream stream;
		stream << list->prefix << "_" << i + list->minlength;
#ifdef KKURZ_MPI
		stream << "_" << list->pref;
#endif
		stream << ".csv";
		string filename;
		stream >> filename;
		ofstream log( list->log_file.c_str(), ios::app );
		log << "Word Counting file: " << filename << endl;
		log.close();
		ofiles[ i ].open( filename.c_str() );
		ofiles[ i ] << "#Word,Sequences,Occurs" << endl;
		for( int j = 0; j < list->num_words[ (i + list->minlength) - 1 ]; j++ )
		{
			string to_out;
			getNextWord( to_out, i + list->minlength );
			if( getCount( to_out ) >= list->min_O && getSeqs( to_out ) >= list->min_seqs )
				ofiles[ i ] << to_out << ',' << getSeqs( to_out ) << ',' << getCount( to_out ) << endl;
		}
	}
}

/*void RadixTrie::reset()
{
	for( int i = 0; i < static_cast<int> ( last_word.size() ); i++ )
	{
		last_word[ i ] = "";
	}
	for( int j = 0; j < static_cast<int> ( last_loc.size() ); j++ )
	{
		last_loc[ j ] = NULL;
	}

}*/

void RadixTrie::resetIterator( int thread )
{
	for( int i = 0; i < static_cast<int> ( last_ext[ thread ].size() ); i++ )
	{
		last_ext[ thread ][ i ] = "";
	}
	for( int j = 0; j < static_cast<int> ( last_ext_loc[ thread ].size() ); j++ )
	{
		last_ext_loc[ thread ][ j ] = NULL;
	}
}

//get the next consecutive block of words from the trie
void RadixTrie::getNextWordBlock( vector<string> &block, int length, int count )
{
	for( int i = 0; i < count; i++ )
	{
		string t;
		getNextWord( t, length );
		if( t.compare( "" ) == 0 )
			return;
		else
			block.push_back( t );
		t.clear();
	}
	return;
}

//systematically returns EVERY word stored in the trie at a given length.  It is the caller's responsibility
//to verify that the word meets any requirements of the job (i.e. min_O and min_seqs before output)
void RadixTrie::getNextWord( string &motif, int length )
{
	string ret_word = "";
	RadixTrieNode *node = NULL;
	//if we haven't returned anything yet...
	if( last_loc[ length - 1 ] == NULL && last_word[ length - 1 ].compare( "" ) == 0 )
	{
		//if we have a trie here...
		if( root && root->branch )
		{
			node = root;
			//hunt until we find a word long enough to be the "first"
			while( static_cast<int> ( ret_word.length() ) < length )
			{
				//if we can keep following this branch
				if( node->branch && node->branch[ next_branch ] )
				{
					///char x = conversion[next_branch];
					char x = 'A' + reverse_branch[ next_branch ]; // ljn 10/5/2009
					ret_word += x;
					node = node->branch[ next_branch ];
					next_branch = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else
				{
					next_branch++;
					if( next_branch >= ALPH )
					{
						while( next_branch >= ALPH )
						{
							char branch_id = ret_word[ ret_word.length() - 1 ];
							ret_word = ret_word.substr( 0, ret_word.length() - 1 );
							next_branch = (locateBranch( branch_id )) + 1;
							node = root;
							for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
							{
								if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
								{
									node = node->branch[ locateBranch( ret_word[ i ] ) ];
								}
							}
							if( ret_word.length() == 0 && next_branch >= ALPH )
							{
								last_loc[ length - 1 ] = NULL;
								last_word[ length - 1 ].clear();
								last_word[ length - 1 ] = "";
								motif = "";
								return;
							}
						}
					}
				}
			}
			last_loc[ length - 1 ] = node;
			last_word[ length - 1 ] = ret_word;
			motif = ret_word;
			return;
		}
	}
	//otherwise we have something and can start from there...
	else
	{
		char branch_id = last_word[ length - 1 ][ last_word[ length - 1 ].length() - 1 ];
		ret_word = last_word[ length - 1 ].substr( 0, last_word[ length - 1 ].length() - 1 );
		next_branch = locateBranch( branch_id ) + 1;
		node = root;
		for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
		{
			if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
			{
				node = node->branch[ locateBranch( ret_word[ i ] ) ];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while( static_cast<int> ( ret_word.length() ) < length )
		{
			//if we can keep following this branch
			if( node && node->branch && node->branch[ next_branch ] )
			{
				///char x = conversion[next_branch];
				char x = 'A' + reverse_branch[ next_branch ]; // ljn 10/5/2009
				ret_word += x;
				node = node->branch[ next_branch ];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else
			{
				next_branch++;
				if( next_branch >= ALPH )
				{
					char branch_id;
					while( next_branch >= ALPH )
					{
						branch_id = ret_word[ ret_word.length() - 1 ];
						ret_word = ret_word.substr( 0, ret_word.length() - 1 );
						next_branch = locateBranch( branch_id ) + 1;
						node = root;
						for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
						{
							if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
							{
								node = node->branch[ locateBranch( ret_word[ i ] ) ];
							}
						}
						if( ret_word.compare( "" ) == 0 && (next_branch >= ALPH || next_branch < 0) )
						{
							last_loc[ length - 1 ] = NULL;
							last_word[ length - 1 ].clear();
							last_word[ length - 1 ] = "";
							motif = "";
							return;
						}
					}
				}
			}
		}
	}
	last_loc[ length - 1 ] = node;
	last_word[ length - 1 ] = ret_word;
	motif = ret_word;
	return;
}

//systematically returns EVERY word stored in the trie at a given length from a provided root (could be the real root).  
//It is the caller's responsibility to verify that the word meets any requirements of the job (i.e. min_O and min_seqs before output)
string RadixTrie::getNextWord( RadixTrieNode *temp_root, int length )
{
	string ret_word = "";
	RadixTrieNode *node = NULL;
	//if we haven't returned anything yet...
	if( last_ext_loc[ threadNum() ][ length - 1 ] == NULL && last_ext[ threadNum() ][ length - 1 ].compare( "" ) == 0 )
	{
		//if we have a trie here...
		if( temp_root && temp_root->branch )
		{
			node = temp_root;
			//hunt until we find a word long enough to be the "first"
			while( static_cast<int> ( ret_word.length() ) < length )
			{
				//if we can keep following this branch
				if( node->branch && node->branch[ next_branch_iterator[ threadNum() ] ] )
				{
					///char x = conversion[next_branch_iterator[threadNum()]];
					char x = 'A' + reverse_branch[ next_branch_iterator[ threadNum() ] ]; // ljn 10/5/2009
					ret_word += x;
					node = node->branch[ next_branch_iterator[ threadNum() ] ];
					next_branch_iterator[ threadNum() ] = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else
				{
					next_branch_iterator[ threadNum() ]++;
					if( next_branch_iterator[ threadNum() ] >= ALPH )
					{
						while( next_branch_iterator[ threadNum() ] >= ALPH )
						{
							char branch_id = ret_word[ ret_word.length() - 1 ];
							ret_word = ret_word.substr( 0, ret_word.length() - 1 );
							next_branch_iterator[ threadNum() ] = locateBranch( branch_id ) + 1;
							node = temp_root;
							for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
							{
								if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
								{
									node = node->branch[ locateBranch( ret_word[ i ] ) ];
								}
							}
							if( ret_word.length() == 0 && next_branch_iterator[ threadNum() ] >= ALPH )
							{
								last_ext_loc[ threadNum() ][ length - 1 ] = NULL;
								last_ext[ threadNum() ][ length - 1 ].clear();
								last_ext[ threadNum() ][ length - 1 ] = "";
								return "";
							}
						}
					}
				}
			}
			last_ext_loc[ threadNum() ][ length - 1 ] = node;
			last_ext[ threadNum() ][ length - 1 ] = ret_word;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else
	{
		char branch_id = last_ext[ threadNum() ][ length - 1 ][ last_ext[ threadNum() ][ length - 1 ].length() - 1 ];
		ret_word = last_ext[ threadNum() ][ length - 1 ].substr( 0, last_ext[ threadNum() ][ length - 1 ].length() - 1 );
		next_branch_iterator[ threadNum() ] = locateBranch( branch_id ) + 1;
		node = temp_root;
		for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
		{
			if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
			{
				node = node->branch[ locateBranch( ret_word[ i ] ) ];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while( static_cast<int> ( ret_word.length() ) < length )
		{
			//if we can keep following this branch
			if( node && node->branch && node->branch[ next_branch_iterator[ threadNum() ] ] )
			{
				///char x = conversion[next_branch_iterator[threadNum()]];
				char x = 'A' + reverse_branch[ next_branch_iterator[ threadNum() ] ]; // ljn 10/5/2009
				ret_word += x;
				node = node->branch[ next_branch_iterator[ threadNum() ] ];
				next_branch_iterator[ threadNum() ] = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else
			{
				next_branch_iterator[ threadNum() ]++;
				if( next_branch_iterator[ threadNum() ] >= ALPH )
				{
					char branch_id;
					while( next_branch_iterator[ threadNum() ] >= ALPH )
					{
						branch_id = ret_word[ ret_word.length() - 1 ];
						ret_word = ret_word.substr( 0, ret_word.length() - 1 );
						next_branch_iterator[ threadNum() ] = locateBranch( branch_id ) + 1;
						node = temp_root;
						for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
						{
							if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
							{
								node = node->branch[ locateBranch( ret_word[ i ] ) ];
							}
						}
						if( ret_word.compare( "" ) == 0 && (next_branch_iterator[ threadNum() ] >= ALPH || next_branch_iterator[ threadNum() ] < 0) )
						{
							last_ext_loc[ threadNum() ][ length - 1 ] = NULL;
							last_ext[ threadNum() ][ length - 1 ].clear();
							last_ext[ threadNum() ][ length - 1 ] = "";
							return "";
						}
					}
				}
			}
		}
	}
	last_ext_loc[ threadNum() ][ length - 1 ] = node;
	last_ext[ threadNum() ][ length - 1 ] = ret_word;
	return ret_word;
}

//function to enumerate all possible strings from a string containing an 'X'
void RadixTrie::enumerateString( ofstream &ofile, vector<string> words )
{
	while( !words.empty() )
	{
		string x = words.back();
		words.pop_back();
		int pos = x.find( 'X' );
		if( pos >= 0 )
		{
			x[ pos ] = 'A';
			words.push_back( x );
			x[ pos ] = 'C';
			words.push_back( x );
			x[ pos ] = 'G';
			words.push_back( x );
			x[ pos ] = 'T';
			words.push_back( x );
		}
		else
		{
			ofile << x << endl;
		}
		x.clear();
	}
}

//function to search the trie and find what words are NOT in it
void RadixTrie::missSearch( RadixTrieNode *node, ofstream &ofile, int length_rem, string &word )
{
	//if we have the right number of characters
	if( length_rem == 0 )
	{
		return;
	}
	//if we dont have a node (specifically root)
	if( node == NULL || node->branch == NULL )
	{
		//cout << "null node" << endl;
		int temp = length_rem;
		string now = word;
		now.append( temp, 'X' );
		if( list->enumerate )
		{
			if( temp >= 0 )
			{
				vector<string> t;
				t.push_back( now );
				enumerateString( ofile, t );
				t.clear();
			}
			else
			{
				ofile << now << endl;
			}
		}
		else
		{
			ofile << now << endl;
		}
		now.clear();
		return;
	}
#ifdef KKURZ_MPI
	if(node == root)
	{
		int i = locateBranch(list->pref);
		printf("i is %d\n", i);
		if( node->branch[ i ] == NULL )
		{
			//cout << "null branch" << endl;
			int temp = length_rem;
			string now = word;
			string a;
			switch( i )
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			now += a;
			temp--;
			now.append( temp, 'X' );
			if( list->enumerate )
			{
				if( temp >= 0 )
				{
					vector<string> t;
					t.push_back( now );
					enumerateString( ofile, t );
					t.clear();
				}
				else
				{
					ofile << now << endl;
				}
			}
			else
			{
				ofile << now << endl;
			}
			now.clear();
		}
		else
		{
			//cout << "recursing" << endl;
			int temp = length_rem;
			string a;
			switch( i )
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			string now = word;
			now += a;
			missSearch( node->branch[ i ], ofile, --temp, now );
			now.clear();
		}
	}
	else
	{
#endif
	for( int i = 0; i < 4; i++ )
	{
		if( node->branch[ i ] == NULL )
		{
			//cout << "null branch" << endl;
			int temp = length_rem;
			string now = word;
			string a;
			switch( i )
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			now += a;
			temp--;
			now.append( temp, 'X' );
			if( list->enumerate )
			{
				if( temp >= 0 )
				{
					vector<string> t;
					t.push_back( now );
					enumerateString( ofile, t );
					t.clear();
				}
				else
				{
					ofile << now << endl;
				}
			}
			else
			{
				ofile << now << endl;
			}
			now.clear();
		}
		else
		{
			//cout << "recursing" << endl;
			int temp = length_rem;
			string a;
			switch( i )
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			string now = word;
			now += a;
			missSearch( node->branch[ i ], ofile, --temp, now );
			now.clear();
		}
	}
#ifdef KKURZ_MPI
	}
#endif
}

//function to count all the words in an input
void RadixTrie::countWords( SequenceFile *input ){
	vector<Sequence*> seqs, segs, ind;
	seqs = input->getSequences();
	int h;

	for( h = 0; h < static_cast<int> ( seqs.size() ); h++ ){
		string seg;
		seg = *(seqs[ h ]->getText());
		ind.push_back( seqs[ h ] );
		if( list->n_filter )
			segs = SequenceFilters::filterNxM( ind, SequenceFilters::RadixFilterDelimiterFunc );
		else
			segs = ind;
		if( list->anc_filter )
			segs = SequenceFilters::filterNxM( segs, SequenceFilters::AncestorFilterDelimiterFunc );

		for( int x = 0; x < static_cast<int> ( segs.size() ); x++ ){
			for( int i = 0; i < (static_cast<int> ( segs[ x ]->getBodyLength() )); i++ ){
#ifdef KKURZ_MPI
				if(toupper((*segs[x])[i]) != list->pref)
                                        continue;
				else{
#endif
					int length = min( static_cast<int> ( segs[ x ]->getBodyLength() ) - i, list->maxlength );
					for( int k = length; k >= 1; k-- ){
						const string *s_ptr = (*segs[x]).getText();
						char *t = (char *)&(*s_ptr)[(*segs[x]).getOffset() + i];
						incCount( t, k );
					}
#ifdef KKURZ_MPI
				}
#endif
			}
		}
		list->seq++;
		seg.clear();
		segs.clear();
		ind.clear();
	}
	seqs.clear();
}

//get a word's locations in the sequence file
void RadixTrie::getLocs(vector<int> &bit, string &motif )
{
	vector<Sequence*> seqs, segs, ind;
	seqs = input->getSequences();
	int h;

	//printf("motif: %s\n", motif.c_str());

	for( h = 0; h < static_cast<int> ( seqs.size() ); h++ )
	{
		//printf("h: %d\n", h);
		string seg;
		seg = *(seqs[ h ]->getText());
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

		for( int x = 0; x < static_cast<int> ( segs.size() ); x++ )
		{
			//printf("x: %d\n", x);
			for( int i = 0; i < (int)segs[ x ]->getBodyLength() - list->maxlength + 1; i++ )
			{
				//printf("i: %d, max: %d\n", i, (int)segs[x]->getBodyLength()-list->maxlength+1);
				int length = min( static_cast<int> ( segs[ x ]->getBodyLength() ) - i + 1, list->maxlength );
				const string *s_ptr = (*segs[x]).getText();
				char *t = (char *)&(*s_ptr)[(*segs[x]).getOffset() + i];
				char *t2 = t;
				if(length >= static_cast<int>(motif.size()))
				{
					bool match = 1;
					for(int j=0; j<static_cast<int>(motif.size()); j++)
					{
						//if(h==0 && i==0)
						//	printf("t: %c, m[j]: %c\n", *t, motif[j]);
						if( toupper(*t2) != motif[j] )
						{
							match = 0;
							j = motif.size();
						}
						t2++;
					}
					if(match == 1)
					{
						//printf("incrementing bit\n");
						bit[h]++;
						i = segs[x]->getBodyLength()*2;
					}
				}	
			}
			if(bit[h] > 0)
				x = segs.size();
		}
		list->seq++;
		seg.clear();
		segs.clear();
		ind.clear();
	}
	seqs.clear();
}

//convert all letters in word to uppercase letters
string RadixTrie::toUppercase( const string & word )
{
	string temp( word );
	for( int i = 0; i < static_cast<int> ( word.length() ); ++i )
	{
		temp[ i ] = toupper( word[ i ] );
	}
	return temp;
}

int RadixTrie::threadNum()
{
#ifdef _OPENMP
	return omp_get_thread_num();
#else
	return 0;
#endif
}

int RadixTrie::numProcs()
{
#ifdef _OPENMP
	return omp_get_num_procs();
#else
	return 1;
#endif
}

