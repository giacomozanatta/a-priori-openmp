Author: Zanatta Giacomo - 859156  
A.Y. 2020/2021
# High Performance Computing Project
## QuickStart
# Table of Contents
1. [Introduction](#Introduction)
   1. [Frequent Itemset Mining](#frequent-itemset-mining )
   2. [A  Priori Algorithm](#a-priori-algorithm)
   3. [Information about the project](#information-about-the-project)
2. [Implementation](#implementation)


## Introduction
### Frequent Itemset Mining 
This project aims to implements a Frequent-Itemset Mining algorithm, both in sequential and parallel way, and to analyze and discuss performance and implementation choices.
The Frequent Itemset problems is defined as follows:  
 - An **Itemset** is a collection of one or more items.
   - **Support**: frequency of occurrence of an itemset in a set of Transaction. This can be represented also as a fraction. If the support for an itemset is 40%, for example, we expect that in 40% of our transaction the itemset is present.  
 - **Frequent Itemset**: is an itemset whose support is greater than or equal to a threshold (minsup).
 - The goal, starting from a set of Transaction T over a set of items I, is to find all itemsets with items contained in I that have the support above or equal the threshold defined.
### A Priori Algorithm
We implemented the A-Priori Algorithm, that is based on the main principle that if an itemset is frequent then also all of its subset must be frequent. Viceversa, if an itemset is not frequent, than all of its superset cannot be frequent.  
Is a level-base approach to find Frequent Itemset 
We will discuss about the implementations and performances on the next sections.
### Information about the project
The implementation was made using C++ on a macOS environment and the gcc compiler.  
For the parallel implementation, openMP library was used. It is an High-Level library for create multi-threading program. The usage of OpenMP avoids to change the code too much and it is easier to understand and more readable since it is based on directives.

## Implementation
The A-Priori implementation was done as follows:
- Since we are working with a list of transactions, and a transaction is a set of items, we store the transaction in a vector of vector of int. We scan transaction sequentially, so using vectors fit well for this requirement.  
- When we parse the file in the transaction structure, we create also another data structure that contains the counting of items. This avoid to scan the whole transaction list another time when we generate C1. We use a map from int (item) to int (the number of this item in transactions) for this purpose.  
- The first step of A-Priori is to get the first Candidate set. We use the previous mentioned map to generate C1, i.e. the candidate set of itemset with length 1.  
  - The candidate set has type of **map<vector\<int\>, \<int\>\>**. We use this data structure for the main reason that accessing elements in map has logarithmic complexity. The key is **vector\<int\>** type because we need to store itemsets, that could have different length, step-based (i.e. on step 1, all item contains only one element, on step 2, we have two elements and so on).  
- While the current candidate set is not empty:  
  - We generate L: for every item in the current candidate set (that is the key of the C map, **vector\<int\>**), we count of many times it appears in all transasctions (support). If the support is above the minimum support, this item is inserted into the set L.
  - We generate the next C starting from L. The process of generating C consists of two phase:
    - joining: the joining phase will merge two different sets of length N-1 to K sets of length N.
    - prune: the pruning phase removes sets in C of length N that does not have all the subsets in L.

## Dataset
We tested the implementation with the retial dataset.  
This dataset is not dense in terms of items and contains circa 80000 transactions of different length.  
Other dataset tested are the chess ones, but this 
## Parallel Implementation with OPENMP
Now we are going to analyze how to introduce parallelism on this project.  
