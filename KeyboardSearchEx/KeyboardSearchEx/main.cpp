#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>           //for sorting vectors
#include <assert.h>
#include <chrono>              //measure time to implement dictionary
#include <conio.h>
#include "trie.hpp"

//Change according to file that contains library of list of words and frequency
#define FILENAME "wordsandfrequency.txt"

#//define ACTIVATE_FREQ_CHANGES 0 //see viability of changing word frequency by each letter entered
                                // - open file, find word line in the file, change the frequency (too expensive in time)
                                // - have a table of data structure that stores words and frequency, update that table
                                //   and fro time to time upload the changes in the file (too expensive in memory)

#define SIZE_SCREEN 30
using namespace std;

//Keyboard map of number typed to correspondant letters
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


unsigned phrase_length = 0;     //to position horizontally the cursor on console
unsigned number_of_lines = 0;   //to position vertically   the cursor on console



//
//Set cursor to x and y position on console
void cursor_goto(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

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
void file_to_dictionary(const string filename, Trie* dict) {
    ifstream infile;
    string new_word, frequency;
    infile.open(filename, ifstream::in);
    if (infile.is_open()) {
        int cnt = 0;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        while (infile.good()) {
            getline(infile, new_word, ',');
            getline(infile, frequency, '\n');
            dict->insert(new_word, stoi(frequency));
            //cout << "word added = "<<new_word<<" Frequency = "<<stoi(frequency)<<"\n";
            cnt++;
        }
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        infile.close();
        cout << "\nDictionary implemented!\n";
        std::cout << "Dictionary construction time : " << elapsed_seconds.count() << " seconds"<<std::endl;
        std::cout << "             Number of words : " << cnt << "\n";
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

    sort(c1->begin(), c1->end());
    sort(c2->begin(), c2->end());

    vector<T> difference1, difference2;
    difference1.reserve(c1->size());
    difference2.reserve(c2->size());

    set_difference(c1->begin(), c1->end(),c2->begin(), c2->end(), back_inserter(difference1));

    set_difference(c2->begin(), c2->end(), c1->begin(), c1->end(), back_inserter(difference2));

    *c1 = move(difference1);
    *c2 = move(difference2);
}


//
//For Sorting in descending order (to choose most frequent words first)
struct comp_desc {
    template<typename T>
    bool operator()(const T& l, const T& r) const {
        return l > r;
    }
};

//
//Concatenate a number in the end of another Ex: (22,9) -> 229
unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while (y >= pow)
        pow *= 10;
    return x * pow + y;
}

//
//Clear_line one line of the console
void Clear_line() {
    cout << "\x1b[1F\x1b[2K";
}

//
//Convert a word to its correspondant in numbers on the keyboard
string word_to_chars(string word) {
    int i = 0;
    int wsize = word.size();
    int nums = 0;
    word[0] = tolower(word[0]);  //only works with lower case

    if (word[0] >= '0' && word[0] <= '9') { //it's a number
        return word;
    }

    for (i = 0; i < word.size(); i++) {
        nums = nums * 10;
        if (word[i] == 'a' || word[i] == 'b' || word[i] == 'c')
            nums = nums + 2;
        else if (word[i] == 'd' || word[i] == 'e' || word[i] == 'f')
            nums = nums + 3;
        else if (word[i] == 'g' || word[i] == 'h' || word[i] == 'i')
            nums = nums + 4;
        else if (word[i] == 'j' || word[i] == 'k' || word[i] == 'l')
            nums = nums + 5;
        else if (word[i] == 'm' || word[i] == 'n' || word[i] == 'o')
            nums = nums + 6;
        else if (word[i] == 'p' || word[i] == 'q' || word[i] == 'r' || word[i] == 's')
            nums = nums + 7;
        else if (word[i] == 't' || word[i] == 'u' || word[i] == 'v')
            nums = nums + 8;
        else nums = nums + 9;
    }
    return to_string(nums);
}


//
//Returns vector of word suggestions, while changing the current_words vector
vector<string>* suggestions(const char typed_num, const unsigned* characters_typed, Trie* dictionary, vector<string>*current_words) {
    vector<pair<int, string>> suggested_pair;                        //vector of suggested words (where dictionary.search = 1)
    vector<string>* suggested_words          = new vector<string>;
    vector<string>* added_words              = new vector<string>;   //vector of new words formed concatenating previous words with typed letter
    vector<string>  old_current_words;                               //vector of old current words. Used after to eliminate previous of size < current size
                                                                     //        This ensures only words of size characters_typed will be suggested

        string typed_chars = keys.find(typed_num)->second;          //convert number typed to chars
        old_current_words = *current_words;

        for (int it = 0; it < typed_chars.length(); ++it) { //for each letter that matches number typed
            if (*characters_typed < 10) {                                         //only for first letter
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
        for (auto it2 = added_words->begin(); it2 != added_words->end(); ++it2) { //search words in the trie
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
        if (*characters_typed > 10)                                  //needed for the first letter typed
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
//Generalized suggestion fonction, returns suggested words given a sequency of numbers, used only when erasing words
vector<string>* suggestions_from_erase(unsigned *characters_typed, string num_last_word, Trie* dictionary, vector<string>* current_words) {
    vector<string>* loc_suggested_words = new vector<string>;
    current_words->clear();                         //reiniatializes the search
    *characters_typed = 0;
    char typed_num;

    for (int i = 0; i < num_last_word.size() - 1; i++) {    //loop over num_last_word to get suggestions
        typed_num = num_last_word[i];
        *characters_typed = concatenate(*characters_typed, typed_num - 48);
        loc_suggested_words = suggestions(typed_num, characters_typed, dictionary, current_words);
    }

    return loc_suggested_words;
}


//
//Conventional multitap keyboard. Use: tap number until the word you want appears.
//Use '1' if next letter is equal to previous one (Ideally, we would set up a timer)
void add_word_multitap(vector<string>* phrase, Trie *dictionary) {

    string new_word = {};
    string typed_chars;
    char typed_num;
    char old_typed_num = '0';

    cursor_goto(0, 27);
    cout << "    Add new word : " << new_word;
    typed_num = _getch();

    while (true) {
        cursor_goto(20, 27);
        printf("\33[2K\r");
        if (typed_num == '0' || typed_num == 'e') {
            phrase->push_back(new_word);
            //ADD TO THE FILE, with frequency 100, if not in dictionary
            if (dictionary->search(new_word) < 1) {
                dictionary->insert(new_word, 100);
                ofstream myfile;
                myfile.open(FILENAME, ios_base::app | ios_base::out);
                myfile << new_word << ",100\n";
                myfile.close();
            }
            else cout << "Word already in dictionary\n";
            phrase_length++;
            break;
        }
        else if (typed_num >= '2' && typed_num <= '9') {
            typed_chars = keys.find(typed_num)->second;
            old_typed_num = typed_num;
            int it = 0;
            while (typed_num == old_typed_num) {
                cout << "    Add new word : " << new_word << typed_chars[it] << "\n";
                cursor_goto(20 + new_word.size(), 27);
                typed_num = _getch();
                printf("\33[2K\r");
                if (typed_num == old_typed_num)
                    it = (it + 1) % typed_chars.length();
            }
            cout << "\n";
            new_word.push_back(typed_chars[it]);
            phrase_length++;
        }
        else if (typed_num == 'C') {
            if (!new_word.empty())
                new_word.pop_back();
            cout << "    Add new word : " << new_word << "\n";
            cursor_goto(20 + new_word.size(), 27);
            typed_num = _getch();
        }
        else if (typed_num == '1') {
            cout << "    Add new word : " << new_word << "\n";
            cursor_goto(20 + new_word.size(), 27);
            typed_num = _getch();
        
        }
        else{    cout << "Wrong character, try again\n";
            cursor_goto(20 + new_word.size(), 27);
            typed_num = _getch();
        }
    }

}

//
//Add . , ! or ? in the phrase. Switch beetwen marks by tapping '1'
void add_ponctuation_mark(vector<string>* phrase, string* ponctuation_marks) {
    int i = 0;
    char typed_num;
    phrase->push_back(ponctuation_marks[i]);
    while (true) {
        cursor_goto(0,23);
        cout << "         Phrase  : ";
        print(phrase->begin(), phrase->end());
        cursor_goto(20 + phrase_length, 23 + number_of_lines);
        typed_num = _getch();
        Clear_line();
        if (typed_num == '0' || (typed_num >= '2' && typed_num <= '9')) {
            break;
        }
        else if (typed_num == '1') {
            i = (i + 1) % 4;
            phrase->pop_back();
            phrase->push_back(ponctuation_marks[i]);
        }

    }
}

//
//Convert first digit of word into uper or lower
void upper_lower(string &word) {
    if (!word.empty()) {
        if (word[0] > 96)                       // ASCII > 96 => lower case
            word[0] = toupper(word[0]);
        else word[0] = tolower(word[0]);
    }
}

//
//Rotate through vector of words
void rotate_suggestions(vector<string>* suggested_words) {
    if (!suggested_words->empty()) {
        string temp = suggested_words->front();
        suggested_words->erase(suggested_words->begin());
        suggested_words->push_back(temp);
    }
}

//
// Add numbers instead of words in the phrase
void add_number(vector<string>* phrase) {
    string numbers = {};
    char typed_num;
    while (true) {
        cursor_goto(0, 27);
        cout << "   Number format : " << numbers;
        typed_num = _getch();
        phrase_length++;
        printf("\33[2K\r");
        if (typed_num == '0' || typed_num == 'e') {
            phrase->push_back(numbers);
            break;
        }
        else if (typed_num >= '0' && typed_num <= '9')
            numbers = numbers + to_string(typed_num - 48);
    }
}

//
// Main fonction
int main() {

    Trie dictionary;
    file_to_dictionary(FILENAME, &dictionary);

    vector<string>* current_words = new vector<string>;        //words that are a suggestion or a tree branch
    vector<string>* suggested_words = new vector<string>;      //words that are a suggestion
    vector<string>* phrase = new vector<string>;
    char typed_num;                                            //number typed by the user in the prompt line
    unsigned characters_typed = 0;                             //quantity of characters typed
    int flag_two_esc = 0;                                      //used to identify if two ESCAPE ('0') were pressed. If true, it'll 
                                                               //a dot ('.') in place
    string ponctuation_marks[4] = { ",", ".", "!", "?"};


    cout << "\n------SUGGESTED WORDS MECHANISM------ ------- ------- -------\n";
    cout << "                                     |       |       |       |\n";
    cout << "  1 - ,.!?                           |   C   |   <-  |   ->  |\n";
    cout << "2:9 - keyboard letters                ------- ------- -------\n";
    cout << "  0 - ESC                            |   1   |   2   |   3   |\n";
    cout << "  * - next suggested word            |  ,.?! | a,b,c | d,e,f |\n";
    cout << "  # : Upper/lower case                ------- ------- ------- \n";
    cout << "  C - erase letter                   |   4   |   5   |   6   |\n";
    cout << "  c - clear search                   | g,h,i | j,k,l | m,n,o |\n";
    cout << "  n : change words to number          ------- ------- -------\n";
    cout << "  a : add new words (Multitap key)   |   7   |   8   |   9   |\n";
    cout << "  e - exit                           |p,q,r,s| t,u,v |w,x,y,z|\n";
    cout << "                                      ------- ------- -------\n";
    cout << "                                     |   *   |   0   |   #   |\n";
    cout << "                                     |       |  ESC  |       |\n";
    cout << " ------------------------------------ ------- ------- -------\n\n";




    while(true) {
        cursor_goto(0, 23);
        printf("\33[2K\n\33[2K\n\33[2K\n\33[2K\n\33[2K\n\33[2K"); //Clear console, 6 lines
        cursor_goto(0, 23);
        cout << "         Phrase  : ";
        print(phrase->begin(), phrase->end());

        if (characters_typed > 0) {
            cursor_goto(0, 27);
            cout << "Suggested words  : ";
            print(suggested_words->begin(), suggested_words->end());
        }
        else {
            cursor_goto(0, 27);
            cout << "                 : ";
        }

        cursor_goto(20 + phrase_length, 23+number_of_lines);
        typed_num = _getch();                                    //get digit from user
        cursor_goto(0, 24+number_of_lines);
        

        if (typed_num == 'e') break;

        else if (typed_num == 'c') {                //CLEAR SEARCH
            current_words->clear();
            phrase->clear();
            characters_typed = 0;
            phrase_length = 0;
        }

        else if (typed_num == 'v') {                 //VIEW CURRENT WORDS (used for debugging)
            cout << "Current words    : ";
            print(current_words->begin(), current_words->end());
        }

        else if (typed_num >= '2' && typed_num <= '9') {      //GET WORD SUGGESTIONS
            characters_typed = concatenate(characters_typed,typed_num - 48);                //add digit in characters_typed
            suggested_words = suggestions(typed_num, &characters_typed, &dictionary, current_words); //get suggestions

            if(!phrase->empty() && !flag_two_esc) phrase->pop_back();

            if (!suggested_words->empty()) {                         //Assert there's at least one suggestion
                if (phrase->empty() || phrase->back() == "." ||      //Begin of phrase or after .?! -> Upper case in suggestions
                    phrase->back() == "?" || phrase->back() == "!") {

                    for (int i = 0; i < suggested_words->size(); i++) {
                        auto word_temp = suggested_words->at(i);
                        word_temp[0] = toupper(word_temp[0]);
                        suggested_words->at(i) = word_temp;
                    }
                }
                phrase->push_back(suggested_words->front());        //insert suggestion in phrase
            }
            else {
                phrase->push_back(to_string(characters_typed));
            }
            
            flag_two_esc = 0;
            phrase_length++;
        }
        else if (typed_num == '0') {          //ESCAPE 
            if (flag_two_esc) {
                phrase->push_back(".");
                phrase_length++;
            }
            else {                                                 
                flag_two_esc = 1;
            }
            current_words->clear();
            suggested_words->clear();
            characters_typed = 0;
            phrase_length++;
        }

        else if (typed_num == '*') {        //ROTATE THROUGH SUGGESTIONS
            if (!suggested_words->empty()) {
                rotate_suggestions(suggested_words);
                phrase->pop_back();
                phrase->push_back(suggested_words->front());
            }
        }

        else if (typed_num == 'C') {        //ERASE LAST DIGIT
            if (!phrase->empty()) {
                if (phrase->back().size() <= 1) {   //if last word is of size 1 -> delete last word in phrase
                    phrase->pop_back();
                    suggested_words->clear();
                    characters_typed = 0;
                    if (phrase_length > 0) phrase_length--;    //because of escape
                }
                else {                              //if middle/end of words -> show last suggestions
                    string num_last_word = word_to_chars(phrase->back());
                    suggested_words = suggestions_from_erase(&characters_typed, num_last_word, &dictionary, current_words);
                    if (!suggested_words->empty()) {
                        phrase->pop_back();
                    }
                    if (phrase->empty() || phrase->back() == "." ||      //Begin of phrase or after .?! -> Upper case in suggestions
                        phrase->back() == "?" || phrase->back() == "!") {

                        for (int i = 0; i < suggested_words->size(); i++) {
                            auto word_temp = suggested_words->at(i);
                            word_temp[0] = toupper(word_temp[0]);
                            suggested_words->at(i) = word_temp;
                        }
                    }
                    if (!suggested_words->empty()) {
                        phrase->push_back(suggested_words->front());
                    }
                    else {
                        phrase->back().pop_back();
                    }
                }
            }
            if (phrase_length > 0) phrase_length--;
        }

        else if (typed_num == '1') {        //PONCTUATION MARK
            current_words->clear();
            suggested_words->clear();
            characters_typed = 0;
            flag_two_esc = 1;
            phrase_length+=2;
            add_ponctuation_mark(phrase, ponctuation_marks);
        }

        else if (typed_num == '#') {        //UPPER lower CASE
            if (!phrase->empty()) {
                upper_lower(phrase->back());
            }
        }

        else if (typed_num == 'n') {        //ADD IN NUMBER FORMAT
            add_number(phrase);
            current_words->clear();
            characters_typed = 0;
        }

        else if (typed_num == 'a') {                  //ADD WORD IN MULTITAP KEYBOARD
            add_word_multitap(phrase, &dictionary);
            current_words->clear();
            suggested_words->clear();
            characters_typed = 0;
            flag_two_esc = 1;
        }

        else cout << "Wrong character, try again\n";
    }

    return 0;
}


/*
-------------------------------------soutenance fev 10-17 -----------------------------------------------
20min de présentation
-quest-ce que vous faitez, un slide
-choix d'architectures (qui sont les agents principaux, les algortihes principaux, ...)
-comment on a géré la modularité et la evolution du code (avoir un code de qualité, que si lit facilement)
-exécuter le code
-concluer avec des améliorations qu'on pourrait faire au futur

après, 10 min de quéstions seront posés à nous

---envoyer le code en avance (48hr au moins avant la soutenance) (Ajouter un READ.md si possible)


para utilisar o getch_:
pro linux n tem conio.h, jeito é usar curses.h 
q pode ser instalado com sudo apt-get install ncurses-dev

compilar g++ main.cpp trie.hpp -lncurses -o app-keyboard

*/