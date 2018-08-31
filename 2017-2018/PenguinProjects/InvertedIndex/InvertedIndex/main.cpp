/*
 // @project: Inverted Index
 // @title: main.cpp
 // @author: Nikhil Pothuru
 // @date: 08/28/2018
*/

#include <iostream>
#include <fstream>
using namespace std;

class InvertedIndex{
    
};
int main() {
    
    int start = 1;
    
    while(start == 1){
        cout <<
        "Welcome to the Inverted Index program. The goal of this program is to map content in a database to allow for easy full text searches."
        << endl;
        cout << "" << endl;
        cout << "Input one of the following numbers:" << endl;
        cout << "0: Input files" << endl;
        cout << "1: Print the dictionary of words and postings" << endl;
        cout << "2: Query words" << endl;
        cout << "3: Quit \n" << endl;
        start = 0;
    }
    
    int input_value = 3;
    cin >> input_value;
    cout << "\n";
    
    switch(input_value){
        
        case 0:
            ifstream input_file;
            string input_file_path;
            
            cout << "Write the filename of the document: ";
            cin >> input_file_path;
            input_file.open(input_file_path);
            
            cout << input_file.rdbuf() << endl; // HAVING PROBLEMS PRINTING MY TEXT FILE; Figure that out
            
    }
    
    
    return 0;
}
