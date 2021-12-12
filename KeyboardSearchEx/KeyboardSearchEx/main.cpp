#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>           //for sorting vectors
#include <assert.h>
#include <chrono>              //measure time to implement dictionary

#include "trie.hpp"


//change according to file that contains list of words and frequency
#define FILENAME "wordsandfrequency.txt"


using namespace std;

//map of number typed to correspondant letters
const map<char, string> keys{ {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5',"jkl"},
        {'6', "mno"}, {'7', "pqrs"}, {'8',"tuv"}, {'9',"wxyz"} };
//           KEYBOARD 
//
//   ------- ------- -------
//  |       |       |       |      C : erase letter
//  |   C   |   <-  |   ->  |  <-,-> : go through letters    
//   ------- ------- -------
//  |   1   |   2   |   3   |      1 : ponctuation marks (function
//  |  ,.?! | a,b,c | d,e,f |          as a multi-tap keyboard)
//   ------- ------- -------
//  |   4   |   5   |   6   |
//  | g,h,i | j,k,l | m,n,o |    2-9 : letters
//   ------- ------- -------
//  |   7   |   8   |   9   |
//  |p,q,r,s| t,u,v |w,x,y,z|
//   ------- ------- -------       * : next suggested word
//  |   *   |   0   |   #   |      0 : escape
//  |       |  ESC  |       |      # : upper/lower case
//   ------- ------- -------
//



//
//Print on console vector content
template <typename iteratorT>
void print(iteratorT first, const iteratorT end) {
    for (; first != end; first++) {
        cout << *first << " ";
    }
    cout << "\n";
}


//
//Convert file .csv,.txt to a trie implemented dictionary
void file_to_dictionary(const string filename, Trie* dict) {
    ifstream infile;
    string new_word, frequency;
    infile.open(filename, ifstream::in);
    if (infile.is_open()) {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        while (infile.good()) {
            getline(infile, new_word, ',');
            getline(infile, frequency, '\n');
            dict->insert(new_word, stoi(frequency));
            //cout << "word added = "<<new_word<<" Frequency = "<<stoi(frequency)<<"\n";
        }
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        infile.close();
        cout << "\nDictionary implemented!\n";
        std::cout << "Dictionary construction time : " << elapsed_seconds.count() << " seconds"<<std::endl;
    }
    else cout << "Error opening file";

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

    sort(c1->begin(), c1->end());  // O(n1 logn1)
    sort(c2->begin(), c2->end());  // O(n2 logn2)

    vector<T> difference1, difference2;
    difference1.reserve(c1->size());
    difference2.reserve(c2->size());

    set_difference(c1->begin(), c1->end(),c2->begin(), c2->end(), back_inserter(difference1));
    // O(2*[N1 + N2 - 1])

    set_difference(c2->begin(), c2->end(), c1->begin(), c1->end(), back_inserter(difference2));
    // O(2*[N1 + N2 - 1])

    *c1 = move(difference1);  // O(1)
    *c2 = move(difference2);  // O(1)
}


//
//for sorting in descending order
struct comp_desc {
    template<typename T>
    bool operator()(const T& l, const T& r) const {
        return l > r;
    }
};


//
//Returns vector of word suggestions, while changing the current_words vector
vector<string>* suggestions(const char typed_num, const int* characters_typed, Trie* dictionary, vector<string>* current_words) {
    vector<pair<int, string>> suggested_pair;                        //vector of suggested words (where dictionary.search = 1)
    vector<string>* suggested_words          = new vector<string>;
    vector<string>* added_words              = new vector<string>;   //vector of new words formed concatenating previous words with typed letter
    vector<string>  old_current_words;                               //vector of old current words. Used after to eliminate previous of size < current size
                                                                     //        This ensures only words of size characters_typed will be suggested

        string typed_chars = keys.find(typed_num)->second;          //convert number typed to chars
        old_current_words = *current_words;

        for (int it = 0; it < typed_chars.length(); ++it) { //for each letter that matches number typed
            if (*characters_typed == 1) {                                         //only for first letter
                added_words->push_back(string(1, typed_chars[it]));
            }
            else {
                for (int it2 = 0; it2 < current_words->size(); ++it2) {           //for 2 or more letters added
                    string temp = current_words->at(it2);                         //'word'
                    temp.push_back(typed_chars[it]);                              //'word+typed_letter'
                    //cout << "temp = " << temp << '\n';
                    added_words->push_back(temp);
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
            else {
                                    //cout << *it2 << " Its a suggested word\n";
                current_words->push_back(*it2);
                suggested_pair.push_back(make_pair(search_result,*it2));
                //cout << "Word: " << *it2 << " Freq: " << search_result<<"\n";
            }
        }
        //cout << "\nOld Suggested Words : ";
        //print(old_current_words.begin(), old_current_words.end());
        if (*characters_typed > 1)                                  //needed for the first letter typed
            remove_intersection(current_words, &old_current_words);
        //cout << "\nCurrent Words : ";
        //print(current_words->begin(), current_words->end());


        sort(suggested_pair.begin(), suggested_pair.end(),comp_desc());      //sorting in descending order
        for (int i = 0; i < suggested_pair.size(); i++) {
             suggested_words->push_back(suggested_pair[i].second);
        }
        
        return suggested_words;
}




//
//Clear one line of the console
void Clear(){
    cout << "\x1b[1F\x1b[2K";
}




int main() {

    Trie dictionary;
    file_to_dictionary(FILENAME, &dictionary);

    vector<string>* current_words = new vector<string>;        //words that are a suggestion or a tree branch
    vector<string>* suggested_words = new vector<string>;      //words that are a suggestion
    vector<string>* phrase = new vector<string>;
    char typed_num;                                            //number typed by the user in the prompt line
    int characters_typed = 0;                                  //quantity of characters typed
    int flag_two_esc = 0;                                      //used to identify if two ESCAPE ('0') were pressed. If true, it'll 
                                                               //a dot ('.') in place
    string ponctuation_marks[4] = { ",", ".", "!", "?"};

    cout << "\n------SUGGESTED WORDS MECHANISM------ ------- ------- -------\n";
    cout << "                                     |       |       |       |\n";
    cout << "  1 - ,.!?                           |   C   |   <-  |   ->  |\n";
    cout << "2:9 - keyboard letters                ------- ------- -------\n";
    cout << "  0 - ESC                            |   1   |   2   |   3   |\n";
    cout << "  * - next suggested word            |  ,.?! | a,b,c | d,e,f |\n";
    cout << "  C - erase letter                    ------- ------- ------- \n";
    cout << "  e - exit                           |   4   |   5   |   6   |\n";
    cout << "  c - clear search                   | g,h,i | j,k,l | m,n,o |\n";
    cout << "  v - view current words considered   ------- ------- -------\n";
    cout << "  # : Upper/lower case               |   7   |   8   |   9   |\n";
    cout << "                                     |p,q,r,s| t,u,v |w,x,y,z|\n";
    cout << "                                      ------- ------- -------\n";
    cout << "                                     |   *   |   0   |   #   |\n";
    cout << "                                     |       |  ESC  |       |\n";
    cout << " ------------------------------------ ------- ------- -------\n\n";

    while(true) {
        cout << "                 : ";
        cin >> typed_num;

        Clear();

        if (typed_num != '0')
            flag_two_esc = 0;

        if (typed_num == 'e') break;
        else if (typed_num == 'c') {
            current_words->clear();
            phrase->clear();
            characters_typed = 0;
        }
        else if (typed_num == 'v') {
            cout << "Current words    : ";
            print(current_words->begin(), current_words->end());
        }
        else if (typed_num >= '2' && typed_num <= '9') {
            characters_typed++;
            suggested_words = suggestions(typed_num, &characters_typed, &dictionary, current_words);
            cout << "Suggested words  : ";
            print(suggested_words->begin(), suggested_words->end());
        }
        else if (typed_num == '0') {
            if (flag_two_esc) {
                phrase->push_back(".");
                cout << "         Phrase  : ";
                print(phrase->begin(), phrase->end());
            }
            else {                                                 
                if (!suggested_words->empty()) {                         //Assert there's at least one suggestion
                    if (phrase->empty() || phrase->back() == "." ||      //Begin of phrase or after .?! -> Upper case
                            phrase->back() == "?" || phrase->back() == "!") {
                        auto word_temp = suggested_words->front();
                        word_temp[0] = toupper(word_temp[0]);
                        suggested_words->front() = word_temp;
                    }
                    phrase->push_back(suggested_words->front());
                }
                current_words->clear();
                characters_typed = 0;
                cout << "         Phrase  : ";
                print(phrase->begin(), phrase->end());
                flag_two_esc = 1;
            }
        }
        else if (typed_num == '*') {
            string temp = suggested_words->front();
            suggested_words->erase(suggested_words->begin());
            suggested_words->push_back(temp);
            cout << "Suggested words  : ";
            print(suggested_words->begin(), suggested_words->end());
        }
        else if (typed_num == 'C') {
            cout << "To be implemented\n";
        }
        else if (typed_num == '1') {
            int i = 0;
            while (true) {
                cout << "Ponctuation mark : " << ponctuation_marks[i] << "\n";
                cin >> typed_num;
                if (typed_num == '0') {
                    phrase->push_back(ponctuation_marks[i]);
                    break;
                }
                else if (typed_num == '1')
                    i = (i + 1) % 4;
            }
            cout << "         Phrase  : ";
            print(phrase->begin(), phrase->end());
        }
        else if (typed_num == '#') {
            auto word_temp = suggested_words->front();
            if (word_temp[0] > 96)                       // ASCII > 96 => lower case
                word_temp[0] = toupper(word_temp[0]);    
            else word_temp[0] = tolower(word_temp[0]);
            suggested_words->front() = word_temp;
            cout << "Suggested words  : ";
            print(suggested_words->begin(), suggested_words->end());
        }
        else cout << "Wrong character, try again\n";
    }

    return 0;
}