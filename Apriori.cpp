#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <utility>

using std::map;
using std::multiset;
using std::pair;
using std::set;
using std::string;
using std::vector;

class ItemSet
{
public:
    ItemSet() = default; // Default constructor
    void addItem(const string &item) { _Items.insert(item); }
    const set<string> &getItemSet() const { return _Items; } // Return const reference
    bool isSubset(const ItemSet &larger) const               // Mark as const
    {
        for (const auto &it : _Items)
        {
            if (larger._Items.find(it) == larger._Items.end())
                return false;
        }
        return true;
    }

    // operators needed for insert
    bool operator==(const ItemSet &rhs) const { return _Items == rhs._Items; } // Mark as const
    friend bool operator<(const ItemSet &lhs, const ItemSet &rhs) { return lhs._Items < rhs._Items; }

    ItemSet operator+(const ItemSet &rhs) const // Mark as const and take rhs by const reference
    {
        ItemSet temp = *this;
        temp._Items.insert(rhs._Items.begin(), rhs._Items.end());
        return temp;
    }

private:
    set<string> _Items;
};

map<ItemSet, int> createPossibleItemsets(map<ItemSet, int> &prev, int setSize);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <minSupport>" << std::endl;
        return 1;
    }

    // Opening file
    string filename = argv[1];
    std::ifstream f(filename);
    string line;

    // Reading the file and inputting the itemsets into a multiset
    std::multiset<ItemSet> fullList;
    while (getline(f, line))
    {
        std::istringstream iss(line);
        string str;
        ItemSet temp;

        while (iss >> str)
        {
            temp.addItem(str);
        }
        fullList.insert(temp);
    }

    // Vector for all the possible items sets to be seen as frequent
    vector<map<ItemSet, int>> allItems;
    allItems.push_back(map<ItemSet, int>());

    // Finding and counting all possible 1 item itemsets
    for (auto itemset : fullList)
    {
        for (auto item : itemset.getItemSet())
        {
            ItemSet temp;
            temp.addItem(item);
            auto p = allItems[0].insert(std::make_pair(temp, 1));
            if (!p.second)
            {
                auto it = allItems[0].find(temp);
                it->second += 1;
            }
        }
    }

    int total = fullList.size();         // Total number of transactions in the list
    double n = std::stod(argv[2]);       // Number used for minimum support
    double minSupport = n / 100 * total; // Number of times the item set needs to appear to be considered frequent

    // Removing any itemset that is below the minimum requirement
    for (auto it = allItems[0].begin(); it != allItems[0].end();)
    {
        if (it->second < minSupport)
        {
            it = allItems[0].erase(it); // Erase returns the next iterator
        }
        else
        {
            ++it; // Move to the next item
        }
    }

    // Size of set that is being used
    int setSize = 1;
    bool canCreate = true;
    while (canCreate)
    {
        map<ItemSet, int> prevItemset = allItems[setSize - 1];
        map<ItemSet, int> curItemset = createPossibleItemsets(prevItemset, setSize);

        // Counting the instances of the itemsets
        for (auto it = curItemset.begin(); it != curItemset.end();)
        {
            for (const auto &larger : fullList)
            {
                ItemSet temp = it->first;
                if (temp.isSubset(larger))
                    it->second += 1;
            }

            if (it->second < minSupport)
            {
                it = curItemset.erase(it); // Erase returns the next iterator
            }
            else
            {
                ++it; // Move to the next element if no erase
            }
        }

        if (curItemset.empty())
            canCreate = false;
        else
            allItems.push_back(curItemset);

        ++setSize;
    }

    // Printing all itemsets and their counts
    for (int i = 0; i < allItems.size(); ++i)
    {
        for (const auto &itemset : allItems[i])
        {
            ItemSet temp = itemset.first;
            std::cout << "{ ";
            for (const auto &item : temp.getItemSet())
            {
                std::cout << item << " "; // Print each item in the itemset
            }
            std::cout << "} : " << itemset.second << std::endl; // Print the count
        }
    }

    return 0;
}

// Creates all possible itemsets that can be made from prev
// Assumes all itemsets in prev are the same size
map<ItemSet, int> createPossibleItemsets(map<ItemSet, int> &prev, int setSize)
{
    map<ItemSet, int> cur;
    for (auto i : prev)
    {
        auto j = prev.find(i.first);
        for (; j != prev.end(); ++j)
        {
            ItemSet temp = i.first;
            ItemSet temp2 = j->first;
            temp = temp + temp2;

            // Checking if the set is the correct size
            if (temp.getItemSet().size() == (setSize + 1))
                cur.insert(std::make_pair(temp, 0));
        }
    }
    return cur;
}
