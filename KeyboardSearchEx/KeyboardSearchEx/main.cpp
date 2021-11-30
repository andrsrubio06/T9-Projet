#include<list>
#include<map>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include "trie.hpp"
using namespace std;

const map<char, string> keys{ {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5',"jkl"},
        {'6', "mno"}, {'7', "pqrs"}, {'8',"tuv"}, {'9',"wxyz"} };

template <typename iteratorT>
void print(iteratorT first, iteratorT end) {
    for (; first != end; first++) {
        std::cout << *first << " ";
    }
    std::cout << "\n";
}

void csv_to_dictionary(Trie* dict) {
    ifstream infile;
    string new_word, frequency;
    infile.open("wordsandfrequency1000words.csv", ifstream::in);
    if (infile.is_open()) {
        while (infile.good()) {
            getline(infile, new_word, ',');
            getline(infile, frequency, '\n');
            dict->insert(new_word, stoi(frequency));
            //std::cout << "word added = "<<new_word<<" Frequency = "<<stoi(frequency)<<"\n";
            //std::cout << "Tamanhos = " << new_word.length() << " e " << frequency.length() << "\n";
        }
        infile.close();
    }
    else cout << "Error opening file";
    std::cout << "\nDictionary implemented!\n";
    std::cout << "Search Test:\n";
    std::cout << "a = " << dict->search("a") << "\n";
    std::cout << "this = " << dict->search("this") << "\n";
    std::cout << "that = " << dict->search("that") << "\n";
    std::cout << "beaut = " << dict->search("beaut") << "\n";
    std::cout << "asdffg = " << dict->search("asdffg") << "\n";
    std::cout << "chunegols = " << dict->search("chunegols") << "\n";
    std::cout << "abccc = " << dict->search("abccc") << "\n";
}

int main() {

    Trie dictionary;
    csv_to_dictionary(&dictionary);

  

    vector<string> suggested_words;
    vector<string> current_words;
    char typed_num;
    int characters_typed = 0;


    do {
        std::cout << "\n\nType a number from 2-9: ";
        std::cin >> typed_num;
        characters_typed++;
        if (typed_num == 'e') break;
        string typed_chars = keys.find(typed_num)->second;
        auto end_loop = current_words.size();

        for (int it = 0; it < typed_chars.length(); ++it) { //for each letter that matches number typed
            if (characters_typed==1) {                                                       //only for first letter added
                current_words.push_back(string(1,typed_chars[it]));
                std::cout << string(1, typed_chars[it]) << " added\n";
            }
            else {
                for (int it2 = 0; it2 < end_loop-1; ++it2) {           //when there's words already
                    //PAREI AQUI
                    string test = current_words[it2];
                    test.push_back(typed_chars[it]);
                    //std::cout << "test = " << test<< " *it = " << *it << "\n";
                    std::cout << "*it2 + *it = " << test << "\n";

                    current_words.push_back(test);
                }
            }
        }
        
        for (auto it2 = current_words.begin(); it2 != current_words.end(); ++it2) {
            int search_result = dictionary.search(*it2);
            if (search_result == -1) {
                //std::cout << *it2 << " Not a tree branch. Excluded from current_words\n";
                //current_words.erase(it2);
            }
            else if (search_result == 0) {
                //std::cout << *it2 << " Only a prefix\n";
            }
            else if (search_result == 1) {
                //std::cout << *it2 << " Its a suggested word\n";
                suggested_words.push_back(*it2);
            }
            //std::cout << *it << " ";
            //for (auto it2 = current_nodes.begin(); it2 != current_nodes.end(); ++it2) {      //for each one of the viable nodes
                //std::cout<<"Viable Nodes"
                // it2->search(*it);                                                            //search in the node if it has a children node with letter wanted

        }
        std::cout << "\nCurrent Words : ";
        print(current_words.begin(), current_words.end());
        std::cout << "Suggested Words : ";
        print(suggested_words.begin(), suggested_words.end());

    } while (true);

    return 0;
}