/*
 // @project: Inverted Index
 // @title: Index.h
 // @author: Nikhil Pothuru
 // @date: 09/20/2018
 */

#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
using namespace std;

struct node;

bool compare_nocase (const string& first, const string& second);

class Index {
    public:
    //Index();
    void tokenizer(string file_path);
    void printpost();
    void search(string search_word);
    const vector<string>& getSavePath();
    ~Index();
    
    private:
    map<string, node*> dictionary;
    set<string> saveWords_set;
    vector<string> saveWords_vector;
    vector<string> savePath;
};
