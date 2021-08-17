#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Transactions {
    public:
        vector<set<int>> transactions;
        map<int, int> items; // support
        float getSupport(int item);
};

float Transactions::getSupport(int item) {
    return (float)items[item]/transactions.size();
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
            set<int> transaction;

            while (ss >> item) {
                transactions->items[item] += 1;
                transaction.insert(item);
                if (ss.peek() == ' ')
                    ss.ignore();
            }
            transactions->transactions.push_back(transaction);
        }
    }
    transactionFile.close();

}


void apriori(vector<set<int>>* transactions) {
    // calculate a priori algorithm

}
int main() {

    Transactions transactions; // every transaction is a set of int (items)

    parseFile("../data/chess.dat", &transactions);
    for (auto const& x : transactions.items)
    {
        std::cout << x.first
                  << ':'
                  << transactions.getSupport(x.first)
                  << std::endl;
    }
    std::cout << "Done!" << std::endl;
    // we parse the file in Transaction (every row is a transaction)
    // a transaction has: ID, list of integers
    // itemset: collection of one or more items
    // goal: find itemset whose support (fraction of transactions that contain an itemset)  is greater than or eqaul to a minsup threshold (s(I)>=minsup).
    return 0;
}
