#include "History.h"
#include "error.h"
using namespace std;


History::History() {
    listHead = nullptr;
    curr = nullptr;
}

History::~History() {
    while(listHead != nullptr){
        Node* temp = listHead;
        listHead = listHead->next;
        delete temp;
    }
}

bool History::hasNext() const {
    if(curr->next != nullptr) return true;
    return false;
}

bool History::hasPrevious() const {
    if(curr->prev != nullptr) return true;
    return false;
}

string History::next() {
    if(curr->next == nullptr){
        error("There is no next page.");
        return "";
    }
    Node*temp = curr;
    string title = curr->next->title;
    curr = curr->next;
    curr->prev = temp;
    return title;
}

string History::previous() {
    if(curr->prev == nullptr){
        error("There is no previous page.");
        return "";
    }
    Node* temp = curr;
    string title = curr->prev->title;
    curr = curr->prev;
    curr->next = temp;
    return title;
}

void History::goTo(const string& page) {
    Node* article = new Node;
    article->title = page;
    article->next = nullptr;
    article->prev = curr;
    listHead = article;
    curr = article;
}

