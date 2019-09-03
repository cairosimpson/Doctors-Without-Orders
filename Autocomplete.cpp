#include "Autocomplete.h"
#include "hashmap.h"
#include <cctype>
#include <string>
using namespace std;

Autocomplete::Autocomplete() {
    root = new Node;
}

Autocomplete::~Autocomplete() {
    releaseTrie(root);
}

void Autocomplete::add(const string& text) {
    string word = text;
    addTitle(word, root);
}

Set<string> Autocomplete::suggestionsFor(const string& prefix, const int maxHits) const {
    if(prefix == "e")
        int bug = 0;
    Set<string> words;
    Node* node = root;
    if(prefix.size()> 0)
        root->letter = string(1,prefix.at(0));
    for(int i = 0; i < prefix.size(); i++ ){ //this loop is meant to find the node associated with the last letter of the prefix
        if(node->map.containsKey(string(1,prefix.at(i)))){
            node = node->map.get(string(1,prefix.at(i)));
        } else //if prefix isn't found, return empty set
            return words;
    }
    string possible = prefix;
    findSuggestions(possible,node, maxHits, words);
    return words;
}


void Autocomplete::addTitle(string& word, Node* node){
    if(word.length() > 0){//This function removes the first letter of the word on each call
        if(!(node->map.containsKey(string(1,word.at(0))))){
            Node* newNode = new Node;
            newNode->letter = word.at(0);
            node->map.add(string(1,word.at(0)), newNode);
            if(word.length() == 1)newNode->map.add(lastLetterMarker, nullptr);//adds a marker to every node that represents the end of a word in the trie
            word.erase(0,1); //removes front character
            addTitle(word, newNode);
        } else{ //if node already exists
            string first = string(1,word.at(0));
            word.erase(0,1);
            if(word.length() == 1)node->map.add(lastLetterMarker, nullptr); //adds a marker to every node that represents the end of a word in the trie
            addTitle(word, node->map.get(first)); //do nothing except move on to next node
        }
    }
}

void Autocomplete::findSuggestions(string possible, Node* node, const int maxHits, Set<string> &words)const{
    if(words.size() == maxHits);
    else if(!(node->map.isEmpty())){
        if((node->map.containsKey(lastLetterMarker) && node->map.get(lastLetterMarker) == nullptr)){ //if last letter of a word
            words.add(possible);
            node->map.remove(lastLetterMarker);
        }
        HashMap<string, Autocomplete::Node*>::iterator iter = node->map.begin();
        while(iter != node->map.end()){
            string nextPossible = possible + *iter; //updates possible word
            Node* nextNode = node->map.get(*iter); //updates possible node
            findSuggestions(nextPossible, nextNode, maxHits, words); //calls itself with update values
            iter++;
        }
    }
}

void Autocomplete::releaseTrie(Node* node){
    printTrie(root);
    if((node->map.isEmpty()))
        delete node;
    else{
        HashMap<string, Autocomplete::Node*>::iterator iter = node->map.begin();
        while(iter != node->map.end()){
            if(node->map.get(*iter) != nullptr)
                releaseTrie(node->map.get(*iter));
            iter++;
        }
        delete node;
    }
}

void Autocomplete::printTrie(Node* node){
    if(node->map.isEmpty() == false){
        HashMap<string, Autocomplete::Node*>::iterator iter = node->map.begin();
        while(iter != node->map.end()){
            if(node->map.get(*iter) != nullptr){
                cout<<node->letter<<endl;
                cout<<" "<<node->map.get(*iter)->letter;
                printTrie(node->map.get(*iter));
            }
            iter++;
        }
    }
}

