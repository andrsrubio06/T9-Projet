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
    std::cout << "beaut = " << dict->search("beaut") << "\n";
    std::cout << "asdffg = " << dict->search("asdffg") << "\n";
    std::cout << "abccc = " << dict->search("abccc") << "\n";
}


void suggestions(char typed_num, int* characters_typed, Trie* dictionary, vector<string>* current_words) {
    vector<string>* suggested_words = new vector<string>;
    vector<string>* added_words = new vector<string>;
    //if (typed_num == 'e') break;
    if (typed_num == 'c') {
        current_words->clear();
        added_words->clear();
        suggested_words->clear();
        *characters_typed = 0;
    }
    else {
        string typed_chars = keys.find(typed_num)->second;          //convert number typed to chars
        auto end_loop = current_words->size();

        for (int it = 0; it < typed_chars.length(); ++it) { //for each letter that matches number typed
            if (*characters_typed == 1) {                                                       //only for first letter
                added_words->push_back(string(1, typed_chars[it]));
            }
            else {
                for (int it2 = 0; it2 < end_loop; ++it2) {           //for 2 or more letters added
                    string test = current_words->at(it2);
                    test.push_back(typed_chars[it]);
                    std::cout << "test = " << test << '\n';
                    added_words->push_back(test);
                }
            }
        }

        for (auto it2 = added_words->begin(); it2 != added_words->end(); ++it2) {
            int search_result = dictionary->search(*it2);
            if (search_result == -1) {
                std::cout << *it2 << " Not a tree branch\n";
            }
            else if (search_result == 0) {
                std::cout << *it2 << " Only a prefix\n";
                current_words->push_back(*it2);
            }
            else if (search_result == 1) {
                std::cout << *it2 << " Its a suggested word\n";
                current_words->push_back(*it2);
                suggested_words->push_back(*it2);
            }
        }
        //current_words = added_words;
        std::cout << "\nCurrent Words : ";
        print(current_words->begin(), current_words->end());
        std::cout << "Suggested Words : ";
        print(suggested_words->begin(), suggested_words->end());

        added_words->clear();
        suggested_words->clear();
    }


}

int main() {

    Trie dictionary;
    csv_to_dictionary(&dictionary);

    vector<string>* current_words = new vector<string>;
    char typed_num;
    int characters_typed = 0;

    std::cout << "\n--Test Suggested words--\n";
    std::cout << "e - exit\nc - clear search\n";

    do {
        std::cout << "\nType number (2-9):";
        std::cin >> typed_num;
        characters_typed++;
        suggestions(typed_num, &characters_typed, &dictionary, current_words);


    } while (true);

    return 0;
}