Author: Zanatta Giacomo - 859156  
A.Y. 2020/2021
# High Performance Computing Project
## QuickStart
# Table of Contents
1. [Introduction](#Introduction)
   1. [Frequent Itemset Mining](#frequent-itemset-mining )
   2. [A  Priori Algorithm](#a-priori-algorithm)
   3. [Information about the project](#information-about-the-project)
2. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)


## Introduction
### Frequent Itemset Mining 
This project aims to implements a Frequent-Itemset Mining algorithm, both in sequential and parallel way, and to analyze and discuss performance and implementation choices.
The Frequent Itemset problems is defined as follows:  
 - An **Itemset** is a collection of one or more items.
   - The**Support**, that is the frequency of occurrence of an itemset in a set of Transaction. This can be represented also as a fraction. If the support for an itemset is 40%, for example, we expect that in 40% of our transaction the itemset is present.  
 - **Frequent Itemset**: is an itemset whose support is greater than or equal to a threshold (minsup).
 - The goal, starting from a set of Transaction T over a set of items I, is to find all itemsets with items contained in I that have the support above or equal the threshold defined.
### A Priori Algorithm
 We implemented the A-Priori Algorithm, that is based on the main principle that if an itemset is frequent then also all of its subset must be frequent. Viceversa, if an itemset is not frequent, than all of its superset cannot be frequent.  
 We will discuss about the implementations and performances on the next sections.
### Information about the project
The implementation was made using C++ on a macOS environment.

## Serial Implementation
