#include "LineManager.h"
using namespace std;

LineManager::LineManager(const Vector<Line *>& lines) {
    treeStart = nullptr;
    createTree(0, lines.size()-1, lines, 0);
}

LineManager::~LineManager() {
    releaseTree(treeStart);
}

double LineManager::contentHeight() const {
    //if(treeStart == nullptr) return 1;
    double low = treeStart->line->lowY();
    double high = treeStart->line->highY();
    getHeight(treeStart,low , high);
    return high-low;
}

Vector<Line *> LineManager::linesInRange(double lowY, double highY) const {
    Vector<Line *> inRange;
    findInRange(lowY, highY,inRange, treeStart);
    return inRange;
}

Line* LineManager::lineAt(const double y) const {
    Vector<Line *> inRange;
    findInRange(y, y,inRange, treeStart); //calls findInRange but for a specific point not range
    if(inRange.isEmpty()) return nullptr;
    return inRange.get(0); //only has one element at most
}

LineManager::Node* LineManager::createTree(int start, int end, const Vector<Line *>& lines, int count){
    if (start > end) //classic binary base case
        return nullptr;

    int mid = (start + end)/2;

    Node *root = new Node;
    root->line = lines.get(mid);
    root->left = nullptr;
    root->right = nullptr;
    if(count == 0)treeStart = root; //if first call

    root->left =  createTree(start, mid-1, lines);//fills in left side of tree

    root->right = createTree(mid+1, end, lines);//fills in right side of tree

    return root;
}

void LineManager::findInRange(const double lowY, const double highY, Vector<Line *> & inRange, Node* t) const{
    if(t != nullptr){
        findInRange(lowY, highY, inRange, t->left);
        if(doOverlap(t->line->lowY(), t->line->highY(), lowY, highY))
            inRange.add(t->line);
        findInRange(lowY, highY, inRange, t->right);
    }
}

bool LineManager::doOverlap(const double lowX, const double highX,  double lowY,  double highY)const{
    if(lowY > highY){
        double temp = lowY;
        lowY = highY;
        highY = temp;
    }
    if(lowX <= highY && lowY<=highX) return true;
    return false;
}

void LineManager::getHeight(Node* t, double &low, double &high) const{
    if(t == nullptr) return;
    if(t->line->lowY() < low) low = t->line->lowY(); //if new absolute low is found, update the absolute low
    if(t->line->highY() > high) high = t->line->highY();//same as above but with absolute high
    getHeight(t->left, low, high);
    getHeight(t->right, low, high);
}

void LineManager::printTree(Node *t)const{
    if(t != nullptr){
        printTree(t->left);
        std::cout << t->line << std::endl;
        printTree(t->right);
    }
}

void LineManager::releaseTree(Node *t){
    if(t != nullptr){
        releaseTree(t->left);
        releaseTree(t->right);
        delete t->line;
        delete t;
    }

}
