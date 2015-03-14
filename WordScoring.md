# Introduction #

The WordScoring Parent Class


# Details #

This class provides a definition of the standard interfaces necessary for any scoring model incorporated into the OWEF. The two abstract interfaces that must be overridden in child classes are listed in the following table:

| **Function Name** | **Input Parameters** | **Return Parameters** | **Purpose** |
|:------------------|:---------------------|:----------------------|:------------|
| computeScores | Scores `*`word, string motif, DataStructure `*`structure, int order | void | Populate the word pointer variables with the scores for the given motif |
| getSeeds | void | vector`<`pair`<`string, Scores`>` `>` | Return the vector containing the top words and their scores |