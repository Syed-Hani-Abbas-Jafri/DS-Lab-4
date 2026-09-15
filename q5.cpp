#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Node {
public:
    int   data;
    Node* next;

    Node(int value) {
        data = value;
        next = NULL;
    }
};

class SinglyLinkedList {
public:
    Node* head;

    SinglyLinkedList() {
        head = NULL;
    }

    void append(int value) {
        Node* fresh = new Node(value);

        if (head == NULL) {
            head = fresh;
            return;
        }

        Node* last = head;
        while (last->next != NULL)
            last = last->next;

        last->next = fresh;
    }

    Node* reverseGroup(Node* start, int k) {
        Node* checker = start;
        int   counter = 0;

        while (checker != NULL && counter < k) {
            checker = checker->next;
            counter++;
        }

        if (counter < k)
            return start;

        Node* previous = NULL;
        Node* current  = start;
        Node* upcoming = NULL;
        counter        = 0;

        while (counter < k) {
            upcoming       = current->next;
            current->next  = previous;
            previous       = current;
            current        = upcoming;
            counter++;
        }

        start->next = reverseGroup(current, k);

        return previous;
    }

    void reverseInGroups(int k) {
        if (k <= 1) return;
        head = reverseGroup(head, k);
    }

    void display() {
        if (head == NULL) {
            cout << "(empty list)" << endl;
            return;
        }

        Node* walker = head;
        while (walker != NULL) {
            cout << walker->data;
            walker = walker->next;
            if (walker != NULL) cout << " -> ";
        }
        cout << endl;
    }
};

int main() {
    SinglyLinkedList list;

    cout << "Enter the list elements on one line: ";
    string line;
    getline(cin, line);

    istringstream numbers(line);
    int value;
    while (numbers >> value)
        list.append(value);

    cout << "Enter K: ";
    int k;
    cin >> k;

    cout << endl << "Original list: ";
    list.display();

    list.reverseInGroups(k);

    cout << "After reversing in groups of " << k << ": ";
    list.display();

    return 0;
}
