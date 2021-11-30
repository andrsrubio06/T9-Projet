#include<list>
#include<map>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include "trie.hpp"
using namespace std;

const std::map<char, std::vector<string>> keys{ {'2', {"a","b","c"}}, {'3', {"d","e","f"}}, {'4', {"g","h","i"}}, {'5',{"j","k","l"}},
        {'6', {"m","n","o"}}, {'7', {"p","q","r","s"}}, {'8',{"t","u","v"}}, {'9',{"w","x","y","z"}}};

template <typename iteratorT>
void print(iteratorT first, iteratorT end) {
    std::cout << "Words: ";
    for (; first != end; first++) {
        std::cout << *first << " ";
    }
    std::cout << "\n";
}

void csv_to_dictionary(Trie* dict) {
    ifstream infile;
    string new_word, frequency;
    infile.open("wordsandfrequency.csv", ifstream::in);
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
    std::cout << "this = " << dict->search("this") << "\n";
    std::cout << "that = " << dict->search("that") << "\n";
    std::cout << "asdffg = " << dict->search("asdffg") << "\n";
    std::cout << "chunegols = " << dict->search("chunegols") << "\n";
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
        auto typed_chars = keys.find(typed_num);
        for (auto it = typed_chars->second.begin(); it != typed_chars->second.end(); ++it) { //for each letter that matches number typed
            auto end_loop = current_words.end();
            if (characters_typed==1) {                                                       //only for first letter added
                current_words.push_back(*it);
                std::cout << *it << " added\n";
            }
            else for (vector<string>::iterator it2 = current_words.begin(); it2 != end_loop; ++it2) {           //when there's words already
                //PAREI AQUI
                string test;
                
                //std::cout << "test = " << test<< " *it = " << *it << "\n";
                std::cout << "*it2 + *it = " << *it2 + *it << "\n";
                current_words.push_back(*it2 + *it);
              
            }
        }
        print(current_words.begin(), current_words.end());
        
        for (auto it2 = current_words.begin(); it2 != current_words.end(); ++it2) {
            int search_result = dictionary.search(*it2);
            if (search_result == -1) {
                std::cout << *it2 << " Not a tree branch. Excluded from current_words\n";
            }
            else if (search_result == 0) {
                std::cout << *it2 << " Only a prefix\n";
            }
            else if (search_result == 1) {
                std::cout << *it2 << " Its a suggested word\n";
                suggested_words.push_back(*it2);
            }


            //std::cout << *it << " ";
            //for (auto it2 = current_nodes.begin(); it2 != current_nodes.end(); ++it2) {      //for each one of the viable nodes
                //std::cout<<"Viable Nodes"
                // it2->search(*it);                                                            //search in the node if it has a children node with letter wanted

        }
    } while (true);

    return 0;
}