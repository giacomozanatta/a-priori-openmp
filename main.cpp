#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <omp.h>

using namespace std;

class Transactions {
public:
    vector<vector<int>> transactions;
    map<int, int> items; // support count per items
    map<vector<int>, int> getC1();
    int getRowSupport(vector<int> row);
    map<vector<int>, int> generateL(map<vector<int>, int> C, int minSupport);
    map<vector<int>, int> generateC(map<vector<int>, int> prevC);
    bool subsetOfTransaction(vector<int> row, vector<int> transaction);
private:
    map<vector<int>, int> joinPhase(map<vector<int>, int> L);
    map<vector<int>, int> prunePhase(map<vector<int>, int> C, map<vector<int>, int> L);
};

map<vector<int>, int> Transactions::getC1() {
    map<vector<int>, int> elements;
    for (auto&item:items) {
        vector<int> z = {item.first};
        elements[z] = item.second;
    }
    return elements;
}


int Transactions::getRowSupport(vector<int> row) {
    int support = 0;
    for (auto item : transactions) {
        if (subsetOfTransaction(row, item)) {
            support++;
        }
    }
    return support;
}

bool Transactions::subsetOfTransaction(vector<int> row, vector<int> transaction) {
    int j = 0;
    for (int i : row) {
        while(j < transaction.size() && transaction[j] != i) {
            j++;
        }
        if (j >= transaction.size()) {
            return false;
        }
    }
    return true;
}
/**
 * Find the frequent itemset from a candidate set C.
 * For every candidate item i in C, this function counts the number of transaction that contains i (support).
 * If the support of i is above the minimum support, then this item is added to the frequent itemset.
 * @param C - the candidate set
 * @param minSupport - the min support
 * @return the frequent itemset.
 */
map<vector<int>, int> Transactions::generateL(map<vector<int>, int> C, int minSupport) {
    map<vector<int>, int> L;
    for (auto&row:C) {
        // for every row in C, check if the support is > minSupport
        int supp = getRowSupport(row.first);
        if (supp>minSupport) {
            L[row.first] = supp;
        }
    }
    return L;
}
/**
 * Generate the C set (Candidate set) from the previous frequent itemset.
 * This is done in 2 phases:
 * 1. Join Phase: merge together 2 item from L. This will generate, starting from items with length N, items with length N+1.
 * 2. Prune Phase: remove from the set generated before items that does not have all k-subset to be frequent
 * @param L - the frequent itemset with item length N-1.
 * @return the Candidate set with item length N.
 */
map<vector<int>, int> Transactions::generateC(map<vector<int>, int> L) {
    map<vector<int>, int> C;

    return prunePhase(joinPhase(L), L);
}

map<vector<int>, int> Transactions::joinPhase(map<vector<int>, int> L) {
    map<vector<int>, int> joinedC;
    for (auto it=L.begin(); it!=L.end(); ++it) {
        vector<int> row = it->first;
        auto internalIt = it;
        internalIt++;
        while( internalIt != L.end()) {
            for (auto item:internalIt->first) {
                if (find(row.begin(), row.end(), item) == row.end()) { // not found
                    vector<int> joinItems = row;
                    joinItems.push_back(item);
                    sort(joinItems.begin(), joinItems.end());
                    joinedC[joinItems] ++;
                }
            }
            internalIt++;
        }
    }
    return joinedC;
}

map<vector<int>, int> Transactions::prunePhase(map<vector<int>, int> C, map<vector<int>, int> L) {
    map<vector<int>, int> prunedC;
    for (auto row: C) {
        int i;
        for(i=0; i<row.first.size();i++){
            vector<int> rowSubset = row.first;
            rowSubset.erase(rowSubset.begin()+i);
            if (!L[rowSubset]) {
                break;
            }
        }
        if(i==row.first.size()){
            prunedC[row.first]++;
        }
    }
    return prunedC;
}

void parseFile(string filePath, Transactions* transactions) {
    string rawTransaction;

    ifstream transactionFile(filePath);
    if (transactionFile.is_open())
    {
        while ( getline (transactionFile,rawTransaction) )
        {
            std::stringstream ss(rawTransaction);
            int item;
            vector<int> transaction;

            while (ss >> item) {
                transactions->items[item] += 1;
                transaction.push_back(item);
                if (ss.peek() == ' ')
                    ss.ignore();
            }
            sort(transaction.begin(), transaction.end());
            transactions->transactions.push_back(transaction);
        }
    }
    transactionFile.close();

}


void apriori(Transactions *transactions, int minSupport) {

    auto begin = std::chrono::high_resolution_clock::now();

    map<vector<int>, int> C = transactions->getC1();
    int step = 1;
    while(!C.empty()) {
        cout << "STEP  " << step << endl;
        auto begin = std::chrono::high_resolution_clock::now();

        map<vector<int>, int> L = transactions->generateL(C, minSupport);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        cout << "generate L total time: " << elapsed.count() << "ms." << endl;

        cout << "# Frequent itemset of size " << step << ": " << L.size() << endl;
        auto beginC = std::chrono::high_resolution_clock::now();
        C = transactions->generateC(L);
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        cout << "generate C total time: " << elapsed.count() << "ms." << endl;
        step++;
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        cout << "apriori step " << step << " total time: " << elapsed.count() << "ms." << endl << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    cout << "apriori serial total time: " << elapsed.count() << "ms." << endl;
}



int main(int argc, char **argv) {
    #pragma omp parallel
    {

        printf("Hello World... from thread = %d\n",
               omp_get_thread_num());
    }
    // Ending of parallel region
    if (argc < 3) {
        cout << "Usage: {executableName} {filePath} {minSupport}" << endl;
        exit(1);
    }

    Transactions transactions; // every transaction is a set of int (items)
    cout << "Parse file " << argv[1] << "..." << endl;
    parseFile(argv[1], &transactions);
    cout << "Finding frequent itemset with min support -------  " << argv[2] << endl;
    apriori(&transactions, stoi(argv[2]));
    cout << "Done!" << endl;
    // we parse the file in Transaction (every row is a transaction)
    // a transaction has: ID, list of integers
    // itemset: collection of one or more items
    // goal: find itemset whose support (fraction of transactions that contain an itemset)  is greater than or eqaul to a minsup threshold (s(I)>=minsup).
    return 0;
}
