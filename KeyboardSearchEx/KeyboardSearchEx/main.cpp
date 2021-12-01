#include<map>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<assert.h>

#include "trie.hpp"

using namespace std;

//map of number typed to correspondant letters
const map<char, string> keys{ {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5',"jkl"},
        {'6', "mno"}, {'7', "pqrs"}, {'8',"tuv"}, {'9',"wxyz"} };

//
//Print on console vector content
template <typename iteratorT>
void print(iteratorT first, iteratorT end) {
    for (; first != end; first++) {
        cout << *first << " ";
    }
    cout << "\n";
}

//
//Convert file .csv,.txt to a trie implemented dictionary
void file_to_dictionary(string filename, Trie* dict) {
    ifstream infile;
    string new_word, frequency;
    infile.open(filename, ifstream::in);
    if (infile.is_open()) {
        while (infile.good()) {
            getline(infile, new_word, ',');
            getline(infile, frequency, '\n');
            dict->insert(new_word, stoi(frequency));
            //cout << "word added = "<<new_word<<" Frequency = "<<stoi(frequency)<<"\n";
            //cout << "Tamanhos = " << new_word.length() << " e " << frequency.length() << "\n";
        }
        infile.close();
    }
    else cout << "Error opening file";
    cout << "\nDictionary implemented!\n";

    /*cout << "Search Test:\n";
    cout << "a = " << dict->search("a") << "\n";
    cout << "this = " << dict->search("this") << "\n";
    cout << "beaut = " << dict->search("beaut") << "\n";
    cout << "asdffg = " << dict->search("asdffg") << "\n";
    cout << "abccc = " << dict->search("abccc") << "\n";
    */
}

//
//Remove content of vector c2 in vector c1
template<typename T>
void remove_intersection(vector<T>* c1, vector<T>* c2) {
    assert(c1 != nullptr);
    assert(c2 != nullptr);

    sort(c1->begin(), end(*c1));  // O(n1 logn1)
    sort(c2->begin(), end(*c2));  // O(n2 logn2)

    vector<T> difference1, difference2;
    difference1.reserve(c1->size());
    difference2.reserve(c2->size());

    set_difference(begin(*c1), end(*c1),
        begin(*c2), end(*c2),
        back_inserter(difference1));
    // O(2*[N1 + N2 - 1])

    set_difference(begin(*c2), end(*c2),
        begin(*c1), end(*c1),
        back_inserter(difference2));
    // O(2*[N1 + N2 - 1])

    *c1 = move(difference1);  // O(1)
    *c2 = move(difference2);  // O(1)
}

//
//Returns vector of word suggestions, while changing the current_words vector
vector<string>* suggestions(char typed_num, int* characters_typed, Trie* dictionary, vector<string>* current_words) {
    vector<string>* suggested_words = new vector<string>;           //vector of suggested words (where dictionary.search = 1)
    vector<string>* added_words = new vector<string>;               //vector of new words formed concatenating previous words with typed letter
    vector<string> old_current_words;                               //vector of old current words. Used after to eliminate previous of size < current size
                                                                    //        This ensures only words of size characters_typed will be suggested

        string typed_chars = keys.find(typed_num)->second;          //convert number typed to chars
        old_current_words = *current_words;

        for (int it = 0; it < typed_chars.length(); ++it) { //for each letter that matches number typed
            if (*characters_typed == 1) {                                         //only for first letter
                added_words->push_back(string(1, typed_chars[it]));
            }
            else {
                for (int it2 = 0; it2 < current_words->size(); ++it2) {           //for 2 or more letters added
                    string test = current_words->at(it2);                         //'word'
                    test.push_back(typed_chars[it]);                              //'word+typed_letter'
                    //cout << "test = " << test << '\n';
                    added_words->push_back(test);
                }
            }
        }

        for (auto it2 = added_words->begin(); it2 != added_words->end(); ++it2) {
            int search_result = dictionary->search(*it2);
            if (search_result == -1) {
                                    //cout << *it2 << " Not a tree branch\n";
            }
            else if (search_result == 0) {
                                    //cout << *it2 << " Only a prefix\n";
                current_words->push_back(*it2);
            }
            else if (search_result == 1) {
                                    //cout << *it2 << " Its a suggested word\n";
                current_words->push_back(*it2);
                suggested_words->push_back(*it2);
            }
        }
        //cout << "\nOld Suggested Words : ";
        //print(old_current_words.begin(), old_current_words.end());
        if (*characters_typed > 1)                                  //needed for the first letter typed
            remove_intersection(current_words, &old_current_words);
        //cout << "\nCurrent Words : ";
        //print(current_words->begin(), current_words->end());
        return suggested_words;
}




int main() {

    Trie dictionary;
    file_to_dictionary("wordsandfrequency.csv", &dictionary);

    vector<string>* current_words = new vector<string>;        //words that are a suggestion or a tree branch
    vector<string>* suggested_words = new vector<string>;      //words that are a suggestion
    char typed_num;                                            //number typed by the user in the prompt line
    int characters_typed = 0;                                  //quantity of characters typed

    cout << "\n--Suggested words mechanism--\n";
    cout << "e - exit\nc - clear search\n";
    cout << "v - view current words considered\n";

    while(true) {
        cout << "\nType number (2-9): ";
        cin >> typed_num;

            if (typed_num == 'e') break;
            else if (typed_num == 'c') {
                current_words->clear();
                characters_typed = 0;
            }
            else if (typed_num == 'v') {
                cout << "Current words    : ";
                print(current_words->begin(), current_words->end());
            }
            else { //do assertion after : number from 2 - 9
                characters_typed++;
                suggested_words = suggestions(typed_num, &characters_typed, &dictionary, current_words);
                cout << "Suggested words  : ";
                print(suggested_words->begin(), suggested_words->end());
            }
    }

    return 0;
}