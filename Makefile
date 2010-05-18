#****************************************************************
#
#Author: Kyle Kurz
#
#Date: 7.14.09
#
#File: Makefile for OWEF framework
#
#Default build is MPI-based and requires the mpiCC compiler to
#be present on the system.  Alternatively, you can run this 
#with the command: make MAIN=OWEF_Main.cpp
#This command allows the system to use single-node, multi-core
#compilation with only g++, and eliminates the dependencies
#on mpiCC
#
#****************************************************************

CPPFLAGS = -Wall -Werror -O2 -DKKURZ_MPI
CC = g++
MAIN = source/OWEFMain.cpp
PLAT = seq
PROJ = bin/OWEFexec
DEBUG := no

ifeq ($(PLAT),multi)
MPI = 1;
CC = mpicxx
endif

ifeq ($(PLAT),seq)
CC = g++
CPPFLAGS = -Wall -Werror -fopenmp
endif

SRCDIR := source
OBJDIR := $(if $(filter yes,$(DEBUG)),debug,release)



INCLUDE_DIRECTORIES = include
CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))

SRCS := BasicMethod.cpp BasicModel.cpp ClusterMethod.cpp DataStructure.cpp DsIterator.cpp OWEFArgs.cpp OWEF.cpp RadixTrie.cpp RadixTrieLeaf.cpp RadixTrieNode.cpp RtIterator.cpp Scores.cpp Sequence.cpp SequenceFile.cpp SequenceFilters.cpp SequenceClustering.cpp Utilities.cpp Visualization.cpp WordDistribution.cpp WordScoring.cpp 
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

all: $(OBJS)

install: $(OBJS)
	$(CC) -o $(PROJ) $(CPPFLAGS) $(MAIN) $^ -lpthread
	
utilities_test:
	g++ source/Utilities.cpp $(CPPFLAGS) -outil_test -O3

TESTFLAGS = -Wall -Werror -fopenmp

sequences_tests:
	g++ source/tests/SequencesTests.cpp $(CPPFLAGS) -oseqs_tests -g
	
data_structure_tests:
	g++ source/tests/DataStructureTests.cpp $(CPPFLAGS) -ods_test -g

suffix_array_test:
	g++ source/tests/SuffixArrayTest.cpp $(CPPFLAGS) -o SAtest -g

tests: data_structure_tests sequences_tests suffix_array_test
	g++ source/tests/SequenceFileTests.cpp $(CPPFLAGS) -oseq_file_test -g
	g++ source/tests/SuffixTreeTest.cpp $(CPPFLAGS) -osuffix_tree_test -g
	
	
	
random_fasta_generator:
	g++ source/tests/GenerateRandomFASTA.cpp $(CPPFLAGS) -ogen_random
	
documentation:
	doxygen

ifeq ($(OBJDIR),debug)
all: CPPFLAGS = -Wall -g -pg -O2 -DDEBUG
all: CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))
install: CPPFLAGS = -Wall -g -pg -O2 -DDEBUG
install: CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $(CPPFLAGS) $(OUTPUT_OPTION) $<

clean :
	rm -rf  *.o *.gch *-db OWEFexec source/*.o source/*.gch source/*-db source/OWEFexec include/*.o include/*.gch include/*-db include/OWEFexec bin/*.o bin/*.gch bin/*-db bin/OWEFexec release/*.o release/*.gch release/*-db release/OWEFexec debug/*.o debug/*.gch debug/*-db debug/OWEFexec
	
	
