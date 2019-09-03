#define CacheCPP_Included
#include "Cache.h"

template <typename ValueType>
Cache<ValueType>::Cache(const int maxSize) {
    SIZE =  maxSize;
    listHead = nullptr;
    listTail = nullptr;
}

template <typename ValueType>
Cache<ValueType>::~Cache() {
    while(listHead != nullptr){
        Node* temp = listHead;
        listHead = listHead->next;
        delete temp;
    }
}

template <typename ValueType>
void Cache<ValueType>::put(const std::string& key, const ValueType& value) {
    if(containsKey(key))
        moveToFront(key);

    else{
        Node* article = new Node;
        article -> key = key;
        article -> value = value;
        article->next = listHead;
        if(listHead != nullptr)
            listHead->prev = article;
        article->prev = nullptr;
        map.add(key, article);
        listHead = article;
        if(listTail == nullptr)listTail = article;
    }


    if(map.size() > SIZE){ //capacity checking
        Node *penultimate = listTail->prev;
        penultimate->next = nullptr; //second to last now becomes last, points to nothing
        Node*last = listTail;
        map.remove(last->key);
        delete last;
        listTail = penultimate; //update listTail
    }

}

template <typename ValueType>
bool Cache<ValueType>::containsKey(const std::string& key) {
    if(map.containsKey(key))return true;
    return false;
}

template <typename ValueType>
ValueType Cache<ValueType>::get(const std::string& key) {
    if(!containsKey(key)) error("Key not found.");

    Node* article = map.get(key);
    moveToFront(key);
    return article->value;
}

template <typename ValueType>
void Cache<ValueType>::moveToFront(const std::string key){
    Node *article = map.get(key);
    Node *prevArticle = article->prev;
    Node *nextArticle = article->next;

    if(prevArticle == nullptr);//if key is at front of list do nothing

    else{
        if(nextArticle == nullptr){//if key is at end of list
            prevArticle-> next = nullptr; //last article points to nothing now
            listTail = prevArticle;
        }
        else{//key is in the middle
            nextArticle->prev = prevArticle;
            prevArticle->next = nextArticle;
        }
        article->next = listHead; //rewire pointers for list continuity
        article->prev = nullptr;
        listHead->prev = article;
        listHead = article;
    }
}

template <typename ValueType>
void Cache<ValueType>::printList(){
    Node *article = listHead;
    while(article != nullptr){
        std::cout<<article->key<<std::endl;
        article = article->next;
    }
    std::cout<<std::endl<<std::endl;
}
