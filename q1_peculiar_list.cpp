#include <iostream>
using namespace std;

class Node {
public:
    int   data;
    Node* next;
    Node* child;
    bool  isTarget;
    Node* regNext;

    Node(int value) {
        data     = value;
        next     = NULL;
        child    = NULL;
        isTarget = false;
        regNext  = NULL;
    }
};

class PeculiarList {
public:
    Node* registryHead;
    Node* head;

    PeculiarList() {
        registryHead = NULL;
        head         = NULL;
    }

    Node* findOrCreate(int value) {
        Node* walker = registryHead;
        while (walker != NULL) {
            if (walker->data == value)
                return walker;
            walker = walker->regNext;
        }

        Node* freshNode = new Node(value);
        freshNode->regNext = registryHead;
        registryHead       = freshNode;
        return freshNode;
    }

    void addEdge(int fromValue, int toValue, int type) {
        Node* fromNode = findOrCreate(fromValue);
        Node* toNode   = findOrCreate(toValue);

        if (type == 1)
            fromNode->child = toNode;
        else
            fromNode->next = toNode;

        toNode->isTarget = true;
    }

    void locateHead() {
        Node* walker = registryHead;
        while (walker != NULL) {
            if (walker->isTarget == false) {
                head = walker;
                return;
            }
            walker = walker->regNext;
        }
        head = NULL;
    }

    void flatten() {
        Node* cur = head;

        while (cur != NULL) {
            if (cur->child != NULL) {
                Node* savedNext = cur->next;
                cur->next  = cur->child;
                cur->child = NULL;

                Node* tail = cur->next;
                while (tail->next != NULL)
                    tail = tail->next;

                tail->next = savedNext;
            }
            cur = cur->next;
        }
    }

    void display() {
        Node* walker = head;
        while (walker != NULL) {
            cout << walker->data;
            if (walker->next != NULL)
                cout << " ";
            walker = walker->next;
        }
        cout << endl;
    }
};

int main() {
    PeculiarList list;

    int from, to, type;

    cout << "Enter edges as: from to type   (type 0 = next, 1 = child)" << endl;
    cout << "Press Ctrl+D (Linux) / Ctrl+Z then Enter (Windows) when done:" << endl;

    while (cin >> from >> to >> type) {
        list.addEdge(from, to, type);
    }

    list.locateHead();
    list.flatten();

    cout << "Flattened list:" << endl;
    list.display();

    return 0;
}
