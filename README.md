Paws
====

**P**hrase **A**nd **W**ord **S**imilarity.
An attempt to convert meaning present in networks to a number for every binary relation - be it
a word or a phrase.

# Overview

This project is part of my B.Sc thesis at Harokopeio University of Athens, October 2014.
The thesis focused on extracting similarity between words and phrases by using information
present in graphs depicting synonymy, antonymy and other relations between words. The goal
was to check sentence alignments and how possible alignments affect entailment and contradiction 
between the sentences that were analyzed.

# Current Data

Current word relation data in file wnsyns.tsv is from [Wordnet](http://wordnet.princeton.edu/) and 
exists for demonstration purposes. The format of the data in the tsv file represents links in the graph 
and can be replaced with your own.

# Dependencies

* g++ with C++11 support + stdlibs

# Run me

1. git clone https://github.com/andreasgrv/paws .
2. cd paws
3. make
4. ./wordsim for word similarity or ./phrasesim for phrase similarity

example: 

./wordsim 3 dispersion+

* The integer is how many neighbours away in the graph the method should go
* The string is the method to use: boolean, count, countdepth, recursive, dispersion, dispersion+, concentration, concentration+

If using wordsim insert two words separated by a space. If using phrasesim insert the first phrase and press return, then the second one
and press return. If you want to exit, type exit and press return.

Similarities range from 0 to 1 and are on a different scale depending on the method.
