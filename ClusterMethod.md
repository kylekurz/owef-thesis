# Introduction #

The ClusterMethod Parent Class.


# Details #

This class provides a definition of the standard interfaces necessary for any clustering method incorporated into the OWEF. The seven abstract interfaces that must be overridden in child classes are listed in the following table:

| **Function Name** | **Input Parameters** | **Return Parameters** | **Purpose** |
|:------------------|:---------------------|:----------------------|:------------|
| buildClusters | DataStructure `*`structure, WordScoring `*`model | void | Populate the clusters variable and output the results |
| createLogos | DataStructure `*`structure, WordScoring `*`model | void | Create a visual logo representation of the provided cluster |
| scoreCluster | vector`<`string`>` cluster, DataStructure `*`structure, WordScoring `*`model | double | Calculate a score for the provided cluster |
| outputRegExp | ofstream &out\_file, vector`<`vector`<`float`>` `>` pwm | void | Output the regular expression equivalent of the supplied Position Weight Matrix to the supplied file |
| outputPwm | ofstream &out\_file, vector`<`vector`<`float`>` `>` pwm | void | Output the supplied Position Weight Matrix to the supplied file |
| computePwm | vector`<`string`>` cluster\_data, DataStructure `*`structure | vector`<`vector`<`float`>` `>` | Output the supplied Position Weight Matrix to the supplied file |
| createMotifLogo | const vector`<`vector`<`float`>` `>` pwm, const string &filename | void | Output the supplied Position Weight Matrix to the supplied file |