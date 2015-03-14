# Introduction #

The DataStructure Parent Class.


# Details #

This class provides a definition of the standard interfaces necessary for any data structure incorporated into the OWEF.  The three abstract interfaces that must be overridden in child classes are listed in the following table:

| **Function Name** | **Input Parameters** | **Return Parameters** | **Purpose** |
|:------------------|:---------------------|:----------------------|:------------|
| getCount | string motif | integer | Return the number of unique occurrences of a word in the database |
| getSeqs | string motif | integer | Return the number of unique sequence occurrences of a word in the database |
| getLocs | vector`<`int`>` &bit, string motif | void | Populates the bit vector with the sequences in which a word occurs |