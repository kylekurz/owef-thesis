/*
 **********************************************
 // File:	OWEF_args.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define a class for storing Open
 //		Word Enumeration Framework
 //		input variables.
 **********************************************

 Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <map>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifndef KKURZ_OWEF_ARGS
#define KKURZ_OWEF_ARGS

// hostnames are limited to 255 bytes
#define HOSTNAMELEN 256
#define NUMPERHOST 2
#define NUMHOSTS 5

#ifdef KKURZ_MPI
#define MPI_DONE_TAG 0
#define MPI_NUMWORDS_TAG 1
#define MPI_NEXT_WORD_TAG 2
#define MPI_COUNT_TAG 3
#define MPI_SEQS_TAG 4
#define MPI_REGEX_TAG 5
#define MPI_LOCS_TAG 6
#define MPI_RESET_TAG 7
#endif

typedef struct
{
		int host_array[ NUMHOSTS ][ NUMPERHOST ];
} map_struct;

/**
 * @class OWEFArgs
 * @brief  a class for storing Open	Word Enumeration Framework input variables.
 *
 * @author Kyle Kurz
 */
class OWEFArgs
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		OWEFArgs();

		/**
		 * @brief initialized constructor
		 */
		OWEFArgs( bool count, std::string structure_type, std::string seq_file, int word_length, int min_length, int min_seqs, int min_O, bool ancestral_filter, bool N_filter, bool missing, bool enumerate, bool record_seqs, bool score, std::string model_type, int order, bool revcomp, bool pval, bool pthr, double pthresh, bool cluster, int seeds, char sort, char type, int distance, bool motif_score, bool pwm, bool logos, bool regex, bool word_distribution, int dist_count, bool normalize, bool cooccur, bool map, std::string mout, bool dist, std::string dout, int group_size, bool self, bool upper_lower, bool sequence_clustering, bool discover, bool fixed, int dimensions, bool self_compare, bool distributed, bool preserve_order, bool density, bool overlap_words, std::string cluster_type, int no_n, bool visualization, bool cgr, std::string prefix, bool parallel, std::string log_file );

		void writeLogs();

		/**
		 * @brief standard destructor
		 */
		~OWEFArgs();

		//************************************************************
		//Input parameters
		//Make all input parameters available to classes based off
		//the wordseeker class
		//************************************************************
		/**
		 * @brief do counting stage?
		 */
		bool count;
		/**
		 * @brief which data structure to use?
		 */
		std::string structure_type;
		/**
		 * @brief filter for ancestral repeats?
		 */
		bool anc_filter;
		/**
		 * @brief minimum wordlength for search (usually set to 1)
		 */
		int minlength;
		/**
		 * @brief maximumn wordlength for search (the length of reported motifs)
		 */
		int maxlength;
		/**
		 * @brief filter n's out of input sequences?
		 */
		bool n_filter;
		/**
		 * @brief list the  missing words?
		 */
		bool missing;
		/**
		 * @brief enumerate them?
		 */
		bool enumerate;
		/**
		 * @brief record sets of seqs?
		 */
		bool record_seqs;
		/**
		 * @brief total number of words found in the sequences
		 */
		std::vector<long int> num_words;
		/**
		 * @brief sequence number being analyzed
		 */
		int seq;
		/**
		 * @brief input sequence filename
		 */
		std::string seq_file;
		/**
		 * @brief minimum number of sequences in which a word must occur
		 */
		int min_seqs;
		/**
		 * @brief minimum number of times a word must occur
		 */
		int min_O;
		/**
		 * @brief Markov order for scoring
		 */
		int order;
		/**
		 * @brief do scoring?
		 */
		bool score;
		/**
		 * @brief which scoring model should we use?
		 */
		std::string model_type;
		/**
		 * @brief do revcomp?
		 */
		bool revcomp;
		/**
		 * @brief do pval?
		 */
		bool pval;
		/**
		 * @brief set threshold?
		 */
		bool pthr;
		/**
		 * @brief threshold value
		 */
		double pthresh;
		/**
		 * @brief do clustering?
		 */
		bool cluster;
		/**
		 * @brief how many?
		 */
		int seeds;
		/**
		 * @brief what column?
		 */
		char sort;
		/**
		 * @brief what type?
		 */
		char type;
		/**
		 * @brief how big a distance?
		 */
		int distance;
		/**
		 * @brief score the motifs?
		 */
		bool motif_score;
		/**
		 * @brief create Cluster pwm's?
		 */
		bool pwm;
		/**
		 * @brief create Cluster logos?
		 */
		bool logos;
		/**
		 * @brief create Cluster regex's?
		 */
		bool regex;
		/**
		 * @brief do word distributions?
		 */
		bool word_distribution;
		/**
		 * @brief how many words?
		 */
		int dist_count;
		/**
		 * @brief normalize?
		 */
		bool normalize;
		/**
		 * @brief do CoOccurrence?
		 */
		bool cooccur;
		/**
		 * @brief create map?
		 */
		bool map;
		/**
		 * @brief name for map;
		 */
		std::string mout;
		/**
		 * @brief create dist map?
		 */
		bool dist;
		/**
		 * @brief name for dist;
		 */
		std::string dout;
		/**
		 * @brief number of words to analyze together
		 */
		int group_size;
		/**
		 * @brief compare words with themselves?
		 */
		bool self;
		/**
		 * @brief case sensitive
		 */
		bool upper_lower;
		/**
		 * @brief do sequence clustering?
		 */
		bool sequence_clustering;
		/**
		 * @brief prefix for unique job id's
		 */
		std::string prefix;
		/**
		 * @brief run the job with parallel code?
		 */
		bool parallel;
		/**
		 * @brief file to log the filenames of the job
		 */
		std::string log_file;
		/**
		 * @brief number of characters in the dataset (total)
		 */
		unsigned long total_input_length;
		/**
		 * @brief store data about each sequence we find (length)
		 */
		std::vector<int> background_seqs;
		/**
		 * @brief rank for MPI (when used)
		 */
		int rank;
		/**
		 * @brief number of MPI tasks (when used)
		 */
		int numtasks;
		/**
		 * @brief visualization variables
		 */
		bool visualization;
		bool cgr;
		/**
		 * @brief prefix for MPI (when used)
		 */
		char pref;
		/**
		 * @brief prefix for MPI (when used)
		 */
		map_struct hosts;
		/**
		 * @brief boolean for module discovery
		 */
		bool discover;

		/**
		 * @brief module discovery variables
		 */
		bool discovery;
		bool fixed;
		int dimensions;
		bool self_compare;
		bool distributed;
		bool preserve_order;
		bool density;
		bool overlap_words;
		//Family Stuff
		std::string cluster_type;
		int no_n;
};

#endif
