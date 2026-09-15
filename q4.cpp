#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Node {
public:
    int   data;
    Node* next;
    Node* prev;

    Node(int value) {
        data = value;
        next = NULL;
        prev = NULL;
    }
};

class FlexibleList {
public:
    Node* head;
    int   type;

    FlexibleList(int listType) {
        head = NULL;
        type = listType;
    }

    bool isDoubly()   { return (type == 2 || type == 4); }
    bool isCircular() { return (type == 3 || type == 4); }

    void append(int value) {
        Node* fresh = new Node(value);

        if (head == NULL) {
            head = fresh;
            if (isCircular()) {
                fresh->next = fresh;
                if (isDoubly())
                    fresh->prev = fresh;
            }
            return;
        }

        Node* last = head;
        if (isCircular()) {
            while (last->next != head)
                last = last->next;
        } else {
            while (last->next != NULL)
                last = last->next;
        }

        last->next = fresh;
        if (isDoubly())
            fresh->prev = last;

        if (isCircular()) {
            fresh->next = head;
            if (isDoubly())
                head->prev = fresh;
        }
    }

    int size() {
        if (head == NULL) return 0;

        int   count  = 0;
        Node* walker = head;

        if (isCircular()) {
            do {
                count++;
                walker = walker->next;
            } while (walker != head);
        } else {
            while (walker != NULL) {
                count++;
                walker = walker->next;
            }
        }
        return count;
    }

    void rotate(int k, bool goLeft) {
        int n = size();
        if (n <= 1) return;

        k = k % n;
        if (!goLeft)
            k = (n - k) % n;
        if (k == 0) return;

        if (isCircular()) {
            for (int i = 0; i < k; i++)
                head = head->next;
            return;
        }

        Node* newTail = head;
        for (int i = 0; i < k - 1; i++)
            newTail = newTail->next;

        Node* newHead = newTail->next;

        Node* oldTail = newHead;
        while (oldTail->next != NULL)
            oldTail = oldTail->next;

        oldTail->next = head;
        if (isDoubly())
            head->prev = oldTail;

        newTail->next = NULL;
        if (isDoubly())
            newHead->prev = NULL;

        head = newHead;
    }

    void display() {
        if (head == NULL) {
            cout << "(empty list)" << endl;
            return;
        }

        Node* walker = head;
        if (isCircular()) {
            do {
                cout << walker->data;
                walker = walker->next;
                if (walker != head) cout << " -> ";
            } while (walker != head);
            cout << " -> (back to head)" << endl;
        } else {
            while (walker != NULL) {
                cout << walker->data;
                walker = walker->next;
                if (walker != NULL) cout << " -> ";
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "Select list type:" << endl;
    cout << "  1. Singly Linked List" << endl;
    cout << "  2. Doubly Linked List" << endl;
    cout << "  3. Circular Linked List" << endl;
    cout << "  4. Doubly Circular Linked List" << endl;
    cout << "Choice: ";

    int choice;
    cin >> choice;
    if (choice < 1 || choice > 4) {
        cout << "Invalid choice." << endl;
        return 0;
    }

    FlexibleList list(choice);

    cin.ignore();

    cout << "Enter the elements on one line: ";
    string line;
    getline(cin, line);

    istringstream numbers(line);
    int value;
    while (numbers >> value)
        list.append(value);

    cout << "Enter K (number of positions): ";
    int k;
    cin >> k;

    cout << "Direction (L = Left, R = Right): ";
    char direction;
    cin >> direction;

    bool goLeft = (direction == 'L' || direction == 'l');

    cout << endl << "Before rotation: ";
    list.display();

    list.rotate(k, goLeft);

    cout << "After rotation:  ";
    list.display();

    return 0;
}
