/*
 * @project: Inverted Index
 * @title: main.cpp
 * @author: Nikhil Pothuru
 * @date: 09/20/2018
 */

#include "Index.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <set>
using namespace std;

/*
 * Struct Name: node
 * Purpose: Used to save data such as what document a word appears in, the line in the document
 * it is in, and the position of the word in that line.
 */

struct node{
    // Word's frequency in a document
    int frequency = 0;
    // Order of appearance
    int appear = 0;
    // Create a tracker in our dictionary to save <file_path, line's position, word's position in line>
    map<int, tuple<string, int, int>> tracker;
    // Points to the next node in the list
    node* next;
};

/*
 * Function Name: compare_nocase
 * Return Type: Boolean
 * Purpose: Used to compare two strings so that the strings can be ordered alphabetically. The ordering
 * is mean't to be case insensitive.
 */

bool compare_nocase (const string& a, const string& b)
{
    // Set an iterator variable
    int i = 0;
    // While loop that loops through both strings as long as i is less than the length of both strings
    while ( (i < a.length()) && (i < b.length()) )
    {
        // Returns true if the character in a[i] < b[i]
        if (tolower(a[i]) < tolower(b[i])){
            return true;
        }
        // Returns false if the character in a[i] > b[i]
        else if (tolower(a[i]) > tolower(b[i])){
            return false;
        }
        i++;
    }
    return ( a.length() < b.length() );
}

// Getter function to access vector in main
const vector<string>& Index::getSavePath(){
    return savePath;
};

// Destructor to deal with node* memory
Index::~Index()
{
    // Iterate through disctionary
    for (auto iterator = dictionary.begin(); iterator != dictionary.end(); iterator++)
    {
        // iterator is a pointer to map value so it treats the node* as a value in a pair
        node* primary = iterator->second;
        node* temp = iterator->second;
        
        // While there is a node, run loop to delete the node
        while (primary)
        {
            temp = primary->next;
            delete primary;
            primary = temp;
        }
    }
}

/*
 * Function Name: tokenizer
 * Return Type: Void
 * Purpose: Used to tokenize the values in a file, so that each word in the file is a token.
 */
void Index::tokenizer(string file_path) {
    string word;
    int word_counter = 1;
    string single_line;
    int line_counter = 1;
    string all_lines;
    // Stream class to read files
    ifstream file;
    // Opens files
    file.open(file_path);
    // If we inputed a file, then run the following
    if(file) {
        // Reads from file and sets all_lines to all text in file
        while (getline(file, all_lines))
        {
            // Puts all_lines in the input buffer
            stringstream multi_linestream(all_lines);
            // Breaks up the lines by using '.' as a delimiter
            while (getline(multi_linestream, single_line, '.'))
            {
                // Puts each line in the input buffer
                stringstream linestream(single_line);
                // Breaks up the line into words by using ' ' as a delimiter
                while(getline(linestream, word, ' '))
                {
                    // If the first character is "", we continue.
                    // (Note*) This usally happens with all lines after the first line
                    if(word == ""){
                        continue;
                    }
                    // Checks if a key doesn't exist
                    bool notExists = dictionary.find(word) == dictionary.end();
                    // If the word isn't used as a key in the dictionary, then run the following
                    if(notExists){
                        // node* for dictionary
                        node* current = new node();
                        current->next = dictionary[word];
                        dictionary[word] = current;
                        current->frequency = 1;
                        current->tracker[current->appear] = make_tuple(file_path, line_counter, word_counter);
                        current->appear = 1;
                    }
                    // If the word is used as a key in the dictionary, run the following
                    else{
                        dictionary[word]->frequency ++;
                        dictionary[word]->tracker[dictionary[word]->appear] = make_tuple(file_path, line_counter, word_counter);
                        dictionary[word]->appear ++;
                    }
                    // Put this word in a set, this way we don't get duplicate values
                    saveWords_set.insert(word);
                    // Keeps track of the word's index
                    word_counter++;
                }
                // Keeps track of the line's index
                line_counter++;
                // Reset word counter for new line
                word_counter = 1;
            }
        }
        // Puts file_path in a vector
        savePath.push_back(file_path);
        // Close file
        file.close();
    }
    // If we didn't input a file, then print no file found
    else {
        cout << "" << endl;
        cout << "------------ No file found ------------" << endl;
        cout << "" << endl;
        // Close file
        file.close();
    }
}

/*
 * Function Name: printpost
 * Return Type: Void
 * Purpose: Used to print a posting list.
 */
void Index::printpost() {
    // For Loop is used to push all set values into a vector
    for (auto i = saveWords_set.begin(); i != saveWords_set.end(); ++i){
        saveWords_vector.push_back(*i);
    }
    
    // Used to sort values in the vector
    sort(saveWords_vector.begin(), saveWords_vector.end(), compare_nocase);
    cout << "" << endl;
    cout << "------------ Postings List ------------ " << endl;
    cout << "" << endl;
    cout << "Posting List is formatted as follows: Word -> 1st Document found in -> 2nd Document found in -> ..." << endl;
    cout << "" << endl;
    // For Loop prints the Posting List
    for (auto i = saveWords_vector.begin(); i != saveWords_vector.end(); ++i){
        cout << *i << ' ';
        int j = 0;
        while (j < dictionary[*i]->appear){
            if (j > 0 && get<0>(dictionary[*i]->tracker[j]) == get<0>(dictionary[*i]->tracker[j - 1])){
                j++;
            }
            else{
                cout << " -> " << get<0>(dictionary[*i]->tracker[j]);
                j++;
            }
        }
        cout << "" << endl;
    }
    cout << "" << endl;
    cout << "--------------------------------------- " << endl;
    /* Need to clear saveWords_vector since every call after the first call of printpost
     * will keep adding items from the set to the vector. Hence, there will be multiple
     * copies of the same element in a saveWords_vector. */
    saveWords_vector.clear();
}

/*
 * Function Name: search
 * Return Type: Void
 * Purpose: Used to find where specific words are in the added documents.
 */
void Index::search(string search_word) {
    // Checks to see if search_word is in the set
    const bool in_set = saveWords_set.find(search_word) != saveWords_set.end();
    // If the word is in the set, then it is a key value in the dictionary. Run the following code.
    if(in_set){
        int j = 0;
        cout << "" << endl;
        cout << "------------ Searched word: " << search_word << " ------------" << endl;
        cout << "" << endl;
        // While loop to run as long as j is less than the appear value associated with the key
        while (j < dictionary[search_word]->appear){
            cout << "Found in the file " << get<0>(dictionary[search_word]->tracker[j]) <<
            " at line " << get<1>(dictionary[search_word]->tracker[j]) << ". ";
            cout << "It occupies position " << get<2>(dictionary[search_word]->tracker[j]) << " in this line (first word in each line is position 1)." << endl;
            j++;
        }
        cout << "" << endl;
        cout << "--------------------------------------- " << endl;
    }
    // If the word is not in the set, then it is not a key value in the dictionary. Run the following code.
    else{
        cout << "" << endl;
        cout << "------------ Word can't be found in any of the text files that were provided ------------ " << endl;
    }
}

int main() {
    
    Index object_one;
    int file_count = 1;
    bool first_time = true;
    bool proceed = true;
    bool files_added = false;
    
    // Runs while proceed is true
    while(proceed) {
        // If this is the first time the for loop is being run, print the following line.
        if(first_time){
            cout << "" << endl;
            cout <<
            "Welcome to the Inverted Index program. The goal of this program is to map content in a database to allow for easy full text searches."
            << endl;
            first_time = false;
        }
        // Prints out all the possible options
        cout << "" << endl;
        cout << "Input one of the following numbers:" << endl;
        cout << "0: Input files" << endl;
        cout << "1: Print the dictionary of words and postings" << endl;
        cout << "2: Search words" << endl;
        cout << "3: Leave Program \n" << endl;
        cout << "" << endl;
        
        // Access the vector savePath
        const vector<string>& temp = object_one.getSavePath();
        // If the vector is not empty, then print the following
        if(!temp.empty()){
            cout << "The files you have added so far are: " << endl;
            cout << "" << endl;
            for(auto x : temp){
                cout << "File " << file_count << ": " << x << endl;
                file_count++;
            }
            files_added = true;
        }
        // Else say that no files have been added
        else{
            cout << "NO FILES HAVE BEEN ADDED SO FAR!" << endl;
            files_added = false;
        }
        cout << "" << endl;
        file_count = 1;
    
        int input_value = 4;
        cin >> input_value;
        cout << "\n";
        
        // Switch case for all the options
        switch(input_value) {
                
            case 0:
            {
                string file_path;
                cout << "Write the filename of the document: ";
                cin >> file_path;
                object_one.tokenizer(file_path);
                break;
            }
            
            case 1:
            {
                if(!files_added){
                    cout << "------------ You can't print a posting list without adding some files first. ------------" << endl;
                    break;
                }
                object_one.printpost();
                break;
            }
                
            case 2:
            {
                if(!files_added){
                    cout << "------------ You can't search for a word without adding some files first. ------------" << endl;
                    break;
                }
                string search_word;
                cout << "Type in word that you would like to search for: ";
                cin >> search_word;
                object_one.search(search_word);
                break;
            }
            
            case 3:
            {
                proceed = false;
                cout << "Thank you for using the program. Have a great day!" << endl;
                cout << " " << endl;
                return proceed;
            }
            
            default:
            {
                cout << "Enter a valid number \n" << endl;
            }
         
        }
    }
    return 0;
}
