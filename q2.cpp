#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Node {
public:
    int   id;
    Node* next;
    Node* prev;

    Node(int value) {
        id   = value;
        next = NULL;
        prev = NULL;
    }
};

class Train {
public:
    Node* head;

    Train() {
        head = NULL;
    }

    void append(int value) {
        Node* fresh = new Node(value);

        if (head == NULL) {
            head       = fresh;
            fresh->next = fresh;
            fresh->prev = fresh;
            return;
        }

        Node* last = head->prev;
        last->next  = fresh;
        fresh->prev = last;
        fresh->next = head;
        head->prev  = fresh;
    }

    Node* find(int value) {
        if (head == NULL) return NULL;

        Node* walker = head;
        do {
            if (walker->id == value)
                return walker;
            walker = walker->next;
        } while (walker != head);

        return NULL;
    }

    void unlink(Node* target) {
        if (target == NULL) return;

        if (target->next == target) {
            head = NULL;
            return;
        }

        target->prev->next = target->next;
        target->next->prev = target->prev;

        if (head == target)
            head = target->next;

        target->next = NULL;
        target->prev = NULL;
    }

    void moveRight(int value) {
        Node* target = find(value);
        if (target == NULL) {
            cout << "Coach " << value << " not found." << endl;
            return;
        }
        if (target == head) return;

        unlink(target);

        target->next     = head->next;
        target->prev     = head;
        head->next->prev = target;
        head->next       = target;
    }

    void moveLeft(int value) {
        Node* target = find(value);
        if (target == NULL) {
            cout << "Coach " << value << " not found." << endl;
            return;
        }
        if (target == head) return;

        unlink(target);

        target->prev     = head->prev;
        target->next     = head;
        head->prev->next = target;
        head->prev       = target;
    }

    void detach(int value) {
        Node* target = find(value);
        if (target == NULL) {
            cout << "Coach " << value << " not found." << endl;
            return;
        }

        unlink(target);
        delete target;
    }

    void setHead(int value) {
        Node* target = find(value);
        if (target == NULL) {
            cout << "Coach " << value << " not found." << endl;
            return;
        }
        head = target;
    }

    void search(int value) {
        if (head == NULL) {
            cout << "Train is empty." << endl;
            return;
        }

        int   forwardSteps = 0;
        Node* walker       = head;
        bool  foundForward = false;

        do {
            if (walker->id == value) {
                foundForward = true;
                break;
            }
            forwardSteps++;
            walker = walker->next;
        } while (walker != head);

        if (foundForward == false) {
            cout << "Coach " << value << " not found." << endl;
            return;
        }

        int backwardSteps = 0;
        walker = head;
        while (walker->id != value) {
            backwardSteps++;
            walker = walker->prev;
        }

        bool useNext = (forwardSteps <= backwardSteps);

        cout << "Direction: " << (useNext ? "NEXT" : "PREV") << endl;

        cout << "Path: ";
        walker = head;
        cout << walker->id;
        int limit = useNext ? forwardSteps : backwardSteps;
        for (int i = 0; i < limit; i++) {
            walker = useNext ? walker->next : walker->prev;
            cout << " -> " << walker->id;
        }
        cout << endl;

        cout << "Steps: " << limit << endl;
    }

    void display() {
        if (head == NULL) {
            cout << "(empty train)" << endl;
            return;
        }

        Node* walker = head;
        do {
            cout << walker->id << " ";
            walker = walker->next;
        } while (walker != head);
        cout << endl;
    }
};

int main() {
    Train train;
    string line;

    cout << "Enter initial coach IDs on one line: ";
    getline(cin, line);

    istringstream numbers(line);
    int id;
    while (numbers >> id)
        train.append(id);

    cout << "Initial List: ";
    train.display();

    cout << "Enter commands (e.g. R 40). Empty line or Ctrl+D to stop:" << endl;

    while (getline(cin, line)) {
        if (line.empty()) break;

        istringstream parts(line);
        char command;
        int  value;

        if (!(parts >> command >> value))
            continue;

        if (command == 'R' || command == 'r') {
            train.moveRight(value);
            cout << "After R " << value << ":  ";
            train.display();
        }
        else if (command == 'L' || command == 'l') {
            train.moveLeft(value);
            cout << "After L " << value << ":  ";
            train.display();
        }
        else if (command == 'D' || command == 'd') {
            train.detach(value);
            cout << "After D " << value << ":  ";
            train.display();
        }
        else if (command == 'S' || command == 's') {
            train.setHead(value);
            cout << "After S " << value << ":  ";
            train.display();
        }
        else if (command == 'F' || command == 'f') {
            cout << endl << "F " << value << ":" << endl;
            train.search(value);
            cout << "Train: ";
            train.display();
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
    }

    return 0;
}
