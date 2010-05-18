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

#include "OWEFArgs.h"

using namespace std;

//standard constructor
OWEFArgs::OWEFArgs()
{
	//no-op
}

//initialized constructor
OWEFArgs::OWEFArgs( bool count_in, string structure_type_in, string seq_file_in, int word_length_in, int min_length_in, int min_seqs_in, int min_O_in, bool ancestral_filter_in, bool N_filter_in, bool missing_in, bool enumerate_in, bool record_seqs_in, bool score_in, string model_type_in, int order_in, bool revcomp_in, bool pval_in, bool pthr_in, double pthresh_in, bool cluster_in, int seeds_in, char sort_in, char type_in, int distance_in, bool motif_score_in, bool pwm_in, bool logos_in, bool regex_in, bool word_distribution_in, int dist_count_in, bool normalize_in, bool cooccur_in, bool map_in, string mout_in, bool dist_in, string dout_in, int group_size_in, bool self_in, bool upper_lower_in, bool sequence_clustering_in, bool discover_in, bool fixed_in, int dimensions_in, bool self_compare_in, bool distributed_in, bool preserve_order_in, bool density_in, bool overlap_words_in, string cluster_type_in, int no_n_in, bool visualization_in, bool cgr_in, string prefix_in, bool parallel_in, string log_file_in )
{
	//do counting?
	count = count_in;
	//which structure?
	structure_type = structure_type_in;
	//filter for ancestral repeats?
	anc_filter = ancestral_filter_in;
	//minimum wordlength for search (usually set to 1)
	minlength = min_length_in;
	//maximumn wordlength for search (the length of reported motifs)
	maxlength = word_length_in;
	//filter n's out of input sequences?
	n_filter = N_filter_in;
	//list the missing words?
	missing = missing_in;
	//enumerate them?
	enumerate = enumerate_in;
	// record set of sequences?
	record_seqs = record_seqs_in;
	//input sequence filename
	seq_file = seq_file_in;
	//minimum number of sequences in which a word must occur
	min_seqs = min_seqs_in;
	//minimum number of times a word must occur
	min_O = min_O_in;
	//do scoring?
	score = score_in;
	//which model?
	model_type = model_type_in;
	//Markov order for scoring
	order = order_in;
	//do revcomp?
	revcomp = revcomp_in;
	//do pval?
	pval = pval_in;
	//set threshold?
	pthr = pthr_in;
	//threshold value
	pthresh = pthresh_in;
	//do clustering?
	cluster = cluster_in;
	//how many?
	seeds = seeds_in;
	//what column?
	sort = sort_in;
	//what type?
	type = type_in;
	//how big a distance?
	distance = distance_in;
	//score the motifs?
	motif_score = motif_score_in;
	//create pwm's?
	pwm = pwm_in;
	//create logos?
	logos = logos_in;
	//create regex's?
	regex = regex_in;
	//do word distributions?
	word_distribution = word_distribution_in;
	//how many words?
	dist_count = dist_count_in;
	//normalize?
	normalize = normalize_in;
	//CoOccurrence?
	cooccur = cooccur_in;
	//map?
	map = map_in;
	//map filename
	mout = mout_in;
	//dist?
	dist = dist_in;
	//distance filename
	dout = dout_in;
	//group size
	group_size = group_size_in;
	//self compare
	self = self_in;
	//case sensitive
	upper_lower = upper_lower_in;
	//do sequence clustering?
	sequence_clustering = sequence_clustering_in;
	//do module discovery?
	discover = discover_in;
	//fixed wordlength?
	fixed = fixed_in;
	//dimensions...
	dimensions = dimensions_in;
	//compare a word twice in module?
	self_compare = self_compare_in;
	//create distribution?
	distributed = distributed_in;
	//preserve order?
	preserve_order = preserve_order_in;
	//create density distribution?
	density = density_in;
	//allow overlaps?
	overlap_words = overlap_words_in;
	//Family crap
	cluster_type = cluster_type_in;
	no_n = no_n_in;
	//visualization stuff
	visualization = visualization_in;
	cgr = cgr_in;
	//prefix for unique job id's
	prefix = prefix_in;
	//run the job in parallel?
	parallel = parallel_in;
	//file to log the filenames
	log_file = log_file_in;
	//total number of words found in the sequences
	for( int i = 0; i < maxlength; i++ )
		num_words.push_back( 0 );
	//sequence number being analyzed
	seq = 0;
	//number of characters in the input (total)
	total_input_length = 0;
	//rank for MPI (when used)
	rank = -1;
	//number of MPI tasks (when used)
	numtasks = -1;
	pref = (char) NULL;

}

//standard destructor
OWEFArgs::~OWEFArgs()
{
	//no-op
	background_seqs.clear();
}

void OWEFArgs::writeLogs()
{
#ifdef KKURZ_MPI
//	if(rank == 0)
//	{
		ofstream log(log_file.c_str(), ios::app);
		//do counting?
		log << "Count: " << count << endl;
		//filter for ancestral repeats?
		log << "Ancestral filter: " << anc_filter << endl;
		//minimum wordlength for search (usually set to 1)
		log << "Minimum wordlength: " << minlength << endl;
		//maximumn wordlength for search (the length of reported motifs)
		log << "Maximum wordlength: " << maxlength << endl;
		//filter n's out of input sequences?
		log << "N-filter: " << n_filter << endl;
		//list the missing words?
		log << "Missing words: " << missing << endl;
		//enumerate them?
		log << "Enumerate: " << enumerate << endl;
		// record sequences?
		log << "Record Set of Seqs: " << record_seqs << endl;
		//input sequence filename
		log << "Input file: " << seq_file << endl;
		//minimum number of sequences in which a word must occur
		log << "Minimum sequences: " << min_seqs << endl;
		//minimum number of times a word must occur
		log << "Minimum occurrences: " << min_O << endl;
		//do scoring?
		log << "Score: " << score << endl;
		//Markov order for scoring
		log << "Markov order: " << order << endl;
		//do revcomp?
		log << "Revcomp: " << revcomp << endl;
		//do pval?
		log << "P-values: " << pval << endl;
		//set threshold
		log << "P-thr: " << pthr << endl;
		//threshold value
		log << "P-thresh: " << pthresh << endl;
		//do clustering?
		log << "Clustering: " << cluster << endl;
		//how many?
		log << "Number of clusters: " << seeds << endl;
		//what column?
		log << "Sort column: " << sort << endl;
		//what type?
		log << "Clustering type: " << type << endl;
		//how big a distance?
		log << "Maximum distance: " << distance << endl;
		//score the motifs?
		log << "Score motifs: " << motif_score << endl;
		//do word distributions?
		log << "Word distribution: " << word_distribution << endl;
		//how many words?
		log << "Distribution count: " << dist_count << endl;
		//normalize?
		log << "Normalize: " << normalize << endl;
		//cooccur?
		log << "CoOccurrence: " << cooccur << endl;
		//map?
		log << "Map cooccurrence: " << map << endl;
		//map filename
		log << "Map filename: " << mout << endl;
		//dist?
		log << "Map distances: " << dist << endl;
		//distance filename
		log << "Distance filename: " << dout << endl;
		//group size
		log << "Number of words to analyze: " << group_size << endl;
		//self compare
		log << "Self-compare: " << self << endl;
		//case sensitive
		log << "Case sensitive: " << upper_lower << endl;
		//sequence clustering
		log << "Sequence clustering: " << sequence_clustering << endl;

		//module discovery?
		log << "Module discovery: " << discover << endl;
		log << "Fixed wordlength: " << fixed << endl;
		log << "Number of words in a module: " << dimensions << endl;
		log << "Self compare: " << self_compare << endl;
		log << "Create distribution: " << distributed << endl;
		log << "Preserve order: " << preserve_order << endl;
		log << "Create density distribution: " << density << endl;
		log << "Allow overlaps: " << overlap_words << endl;
		//run the job in parallel?
		log << "Parallel: " << parallel << endl;
		//prefix for unique job id's
		log << "Job ID: " << prefix << endl << endl;
		log.close();
//	}
#endif

#ifndef KKURZ_MPI
	ofstream log( log_file.c_str(), ios::app );
	//do counting?
	log << "Count: " << count << endl;
	//filter for ancestral repeats?
	log << "Ancestral filter: " << anc_filter << endl;
	//minimum wordlength for search (usually set to 1)
	log << "Minimum wordlength: " << minlength << endl;
	//maximumn wordlength for search (the length of reported motifs)
	log << "Maximum wordlength: " << maxlength << endl;
	//filter n's out of input sequences?
	log << "N-filter: " << n_filter << endl;
	//list the missing words?
	log << "Missing words: " << missing << endl;
	//enumerate them?
	log << "Enumerate: " << enumerate << endl;
	// record sequences?
	log << "Record Set of Seqs: " << record_seqs << endl;
	//input sequence filename
	log << "Input file: " << seq_file << endl;
	//minimum number of sequences in which a word must occur
	log << "Minimum sequences: " << min_seqs << endl;
	//minimum number of times a word must occur
	log << "Minimum occurrences: " << min_O << endl;
	//do scoring?
	log << "Score: " << score << endl;
	//Markov order for scoring
	log << "Markov order: " << order << endl;
	//do revcomp?
	log << "Revcomp: " << revcomp << endl;
	//do pval?
	log << "P-values: " << pval << endl;
	//set threshold
	log << "P-thr: " << pthr << endl;
	//threshold value
	log << "P-thresh: " << pthresh << endl;
	//do clustering?
	log << "Clustering: " << cluster << endl;
	//how many?
	log << "Number of clusters: " << seeds << endl;
	//what column?
	log << "Sort column: " << sort << endl;
	//what type?
	log << "Clustering type: " << type << endl;
	//how big a distance?
	log << "Maximum distance: " << distance << endl;
	//score the motifs?
	log << "Score motifs: " << motif_score << endl;
	//do word distributions?
	log << "Word distribution: " << word_distribution << endl;
	//how many words?
	log << "Distribution count: " << dist_count << endl;
	//normalize?
	log << "Normalize: " << normalize << endl;
	//cooccur?
	log << "CoOccurrence: " << cooccur << endl;
	//map?
	log << "Map cooccurrence: " << map << endl;
	//map filename
	log << "Map filename: " << mout << endl;
	//dist?
	log << "Map distances: " << dist << endl;
	//distance filename
	log << "Distance filename: " << dout << endl;
	//group size
	log << "Number of words to analyze: " << group_size << endl;
	//self compare
	log << "Self-compare: " << self << endl;
	//case sensitive
	log << "Case sensitive: " << upper_lower << endl;
	//sequence clustering
	log << "Sequence clustering: " << sequence_clustering << endl;

	//module discovery?
	log << "Module discovery: " << discover << endl;
	log << "Fixed wordlength: " << fixed << endl;
	log << "Number of words in a module: " << dimensions << endl;
	log << "Self compare: " << self_compare << endl;
	log << "Create distribution: " << distributed << endl;
	log << "Preserve order: " << preserve_order << endl;
	log << "Create density distribution: " << density << endl;
	log << "Allow overlaps: " << overlap_words << endl;
	//run the job in parallel?
	log << "Parallel: " << parallel << endl;
	//prefix for unique job id's
	log << "Job ID: " << prefix << endl << endl;
	log.close();
#endif
}

