#include <iostream>
using namespace std;

// Define the character size  ||a to z|| => ||97 to 122||
#define CHAR_SIZE 26                        //122 - 97 + 1

class Trie {

private:
    bool isWord;
    Trie* character[CHAR_SIZE];
    int frequency;

public:
    //Constructors
    Trie() {
        this->isWord = false;

        for (int i = 0; i < CHAR_SIZE; i++)
            this->character[i] = nullptr;

        frequency = 0;
    }
    Trie(const int freq) {
        this->isWord = false;

        for (int i = 0; i < CHAR_SIZE; i++)
            this->character[i] = nullptr;

        frequency = freq;
    }

    //Methodes
    void insert(const string, const int freq);
    bool deletion(Trie*&, const string);
    const int search(const string);
    const bool haveChildren(Trie const*);

};



// Iterative function to insert a word into a Trie
void Trie::insert(const string key, const int freq) {
    // start from the root node
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // create a new node if the path doesn't exist
        if (curr->character[key[i]-'a'] == nullptr)
            curr->character[key[i]-'a'] = new Trie(freq);
        // go to the next node
        curr = curr->character[key[i]-'a'];
    }
    // mark the current node as a word
    curr->isWord = true;
}



// Iterative function to search a word in a Trie
//returns: -1 ->not in the trie,
//          0 ->in the trie but not a word,
//       freq ->word in the trie
const int Trie::search(const string key) {
    // return -1 if Trie is empty
    if (this == nullptr)
        return -1;

    Trie* curr = this;
    for (int i = 0; i < key.length(); i++) {
        // go to the next node
        curr = curr->character[key[i]-'a'];

        // if the string is invalid (reached end of a path in the Trie)
        if (curr == nullptr)
            return -1;
    }

    // return freq if the current node is a leaf and the
    // end of the string is reached
    if (curr->isWord)
        return curr->frequency;
    else return 0;
}



// Returns true if a given node has any children
const bool Trie::haveChildren(Trie const* curr) {
    for (int i = 0; i < CHAR_SIZE; i++) {
        if (curr->character[i])
            return true;    // child found
    }
    return false;
}

// Recursive function to delete a key in the Trie
bool Trie::deletion(Trie*& curr, const string key) {
    // return if Trie is empty
    if (curr == nullptr)
        return false;

    // if the end of the key is not reached
    if (key.length()) {
        // recur for the node corresponding to the next character in the key
        // and if it returns true, delete the current node (if it is non-leaf)
        if (curr != nullptr && curr->character[key[0]-'a'] != nullptr &&
            deletion(curr->character[key[0]-'a'], key.substr(1)) &&
            curr->isWord == false) {
            if (!haveChildren(curr)) {
                delete curr;
                curr = nullptr;
                return true;
            }
            else
                return false;
        }
    }

    // if the end of the key is reached
    if (key.length() == 0 && curr->isWord) {
        // if the current node is a leaf node and doesn't have any children
        if (!haveChildren(curr)) {
            // delete the current node
            delete curr;
            curr = nullptr;

            // delete the non-leaf parent nodes
            return true;
        }

        // if the current node is a leaf node and has children
        else {
            // mark the current node as a non-leaf node (DON'T DELETE IT)
            curr->isWord = false;
            // don't delete its parent nodes
            return false;
        }
    }
    return false;
}
