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

class ListUnit {
public:
    Node* head;
    int   type;
    bool  alive;

    ListUnit() {
        head  = NULL;
        type  = 1;
        alive = true;
    }

    bool isDoubly()   { return (type == 2 || type == 4); }
    bool isCircular() { return (type == 3 || type == 4); }

    string typeName() {
        if (type == 1) return "Singly";
        if (type == 2) return "Doubly";
        if (type == 3) return "Circular";
        return "Doubly Circular";
    }

    void linearize() {
        if (head == NULL) return;
        if (!isCircular()) return;

        Node* last = head;
        while (last->next != head)
            last = last->next;

        last->next = NULL;
        head->prev = NULL;
    }

    void rebuild() {
        if (head == NULL) return;

        head->prev  = NULL;
        Node* walker = head;
        while (walker->next != NULL) {
            if (isDoubly())
                walker->next->prev = walker;
            walker = walker->next;
        }

        if (isCircular()) {
            walker->next = head;
            if (isDoubly())
                head->prev = walker;
        }
    }

    void append(int value) {
        Node* fresh = new Node(value);

        if (head == NULL) {
            head = fresh;
            rebuild();
            return;
        }

        linearize();
        Node* last = head;
        while (last->next != NULL)
            last = last->next;
        last->next = fresh;

        rebuild();
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

    bool hasCycle() {
        Node* slow = head;
        Node* fast = head;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
                return true;
        }
        return false;
    }

    void radixSort() {
        if (head == NULL) return;
        linearize();

        int   maxValue = head->data;
        Node* walker   = head;
        while (walker != NULL) {
            if (walker->data > maxValue)
                maxValue = walker->data;
            walker = walker->next;
        }

        for (int exp = 1; maxValue / exp > 0; exp = exp * 10) {
            Node* bucketHead[10];
            Node* bucketTail[10];

            for (int i = 0; i < 10; i++) {
                bucketHead[i] = NULL;
                bucketTail[i] = NULL;
            }

            Node* current = head;
            while (current != NULL) {
                Node* keepNext = current->next;
                int   digit    = (current->data / exp) % 10;

                current->next = NULL;
                if (bucketHead[digit] == NULL) {
                    bucketHead[digit] = current;
                    bucketTail[digit] = current;
                } else {
                    bucketTail[digit]->next = current;
                    bucketTail[digit]       = current;
                }

                current = keepNext;
            }

            Node* newHead = NULL;
            Node* newTail = NULL;

            for (int i = 0; i < 10; i++) {
                if (bucketHead[i] == NULL)
                    continue;

                if (newHead == NULL) {
                    newHead = bucketHead[i];
                } else {
                    newTail->next = bucketHead[i];
                }
                newTail = bucketTail[i];
            }

            head = newHead;
        }

        rebuild();
    }

    void bubbleSort() {
        int n = size();
        if (n < 2) return;

        linearize();

        for (int pass = 0; pass < n - 1; pass++) {
            Node* current = head;
            bool  swapped = false;

            while (current->next != NULL) {
                if (current->data > current->next->data) {
                    int temp            = current->data;
                    current->data       = current->next->data;
                    current->next->data = temp;
                    swapped             = true;
                }
                current = current->next;
            }

            if (swapped == false)
                break;
        }

        rebuild();
    }

    void bubbleSortLastK(int k) {
        int n = size();
        if (k < 2 || n < 2) return;
        if (k > n) k = n;

        linearize();

        Node* start = head;
        for (int skip = 0; skip < n - k; skip++)
            start = start->next;

        for (int pass = 0; pass < k - 1; pass++) {
            Node* current = start;
            bool  swapped = false;

            while (current->next != NULL) {
                if (current->data > current->next->data) {
                    int temp            = current->data;
                    current->data       = current->next->data;
                    current->next->data = temp;
                    swapped             = true;
                }
                current = current->next;
            }

            if (swapped == false)
                break;
        }

        rebuild();
    }

    void display() {
        if (head == NULL) {
            cout << "(empty)";
            return;
        }

        Node* walker = head;
        if (isCircular()) {
            do {
                cout << walker->data << " ";
                walker = walker->next;
            } while (walker != head);
            cout << "(loops back to " << head->data << ")";
        } else {
            while (walker != NULL) {
                cout << walker->data << " ";
                walker = walker->next;
            }
        }
    }
};

int main() {
    const int MAX_LISTS = 50;
    ListUnit lists[MAX_LISTS];

    cout << "How many linked lists do you want to enter? ";
    int total;
    cin >> total;

    if (total < 1 || total > MAX_LISTS) {
        cout << "Please use a value between 1 and " << MAX_LISTS << "." << endl;
        return 0;
    }

    cin.ignore();

    int typeCount[5];
    for (int i = 0; i < 5; i++)
        typeCount[i] = 0;

    for (int i = 0; i < total; i++) {
        cout << endl << "List #" << (i + 1) << endl;
        cout << "  Type (1 Singly, 2 Doubly, 3 Circular, 4 Doubly Circular): ";

        int chosenType;
        cin >> chosenType;

        while (chosenType < 1 || chosenType > 4 ||
               (i > 0 && chosenType == lists[i - 1].type)) {
            cout << "  Invalid (must be 1-4 and different from the previous list). "
                 << "Try again: ";
            cin >> chosenType;
        }

        lists[i].type = chosenType;
        typeCount[chosenType]++;

        cin.ignore();
        cout << "  Elements on one line: ";
        string line;
        getline(cin, line);

        istringstream numbers(line);
        int value;
        while (numbers >> value)
            lists[i].append(value);
    }

    for (int t = 1; t <= 4; t++) {
        if (typeCount[t] < 3) {
            cout << endl << "Warning: type " << t
                 << " appears only " << typeCount[t]
                 << " time(s); the task asks for at least 3." << endl;
        }
    }

    cout << endl << "=== After Radix Sort on each list ===" << endl;
    for (int i = 0; i < total; i++) {
        lists[i].radixSort();
        cout << (i + 1) << ". " << lists[i].typeName() << ": ";
        lists[i].display();
        cout << endl;
    }

    bool cyclic[MAX_LISTS];
    int  cycleCount = 0;

    cout << endl << "=== Cycle detection ===" << endl;
    for (int i = 0; i < total; i++) {
        cyclic[i] = lists[i].hasCycle();
        if (cyclic[i]) cycleCount++;
        cout << (i + 1) << ". " << lists[i].typeName()
             << " -> " << (cyclic[i] ? "CYCLIC" : "not cyclic") << endl;
    }
    cout << "Number of cycles found: " << cycleCount << endl;

    cout << endl << "=== Merging cyclic pairs ===" << endl;

    int scan = 0;
    while (scan < total) {
        if (!cyclic[scan]) { scan++; continue; }

        int partner = -1;
        for (int j = scan + 1; j < total; j++) {
            if (cyclic[j]) { partner = j; break; }
        }

        if (partner == -1) {
            cout << "List " << (scan + 1) << " is cyclic but has no partner; left as is."
                 << endl;
            break;
        }

        cout << "Merging lists " << (scan + 1) << " to " << (partner + 1)
             << " (cyclic pair " << (scan + 1) << " & " << (partner + 1) << ")" << endl;

        int startCompat = scan;
        for (int j = scan; j <= partner; j++) {
            if (lists[j].type == 2 || lists[j].type == 4)
                startCompat = j + 1;
        }

        int suffixNodes = 0;
        for (int j = startCompat; j <= partner; j++)
            suffixNodes += lists[j].size();

        bool bubbleAllowed = (suffixNodes >= 2);
        bool wholeRun      = (startCompat == scan);

        Node* mergedHead = NULL;
        Node* mergedTail = NULL;

        for (int j = scan; j <= partner; j++) {
            lists[j].linearize();

            if (lists[j].head == NULL)
                continue;

            if (mergedHead == NULL)
                mergedHead = lists[j].head;
            else
                mergedTail->next = lists[j].head;

            mergedTail = lists[j].head;
            while (mergedTail->next != NULL)
                mergedTail = mergedTail->next;

            if (j != scan) {
                lists[j].head  = NULL;
                lists[j].alive = false;
            }
        }

        lists[scan].head = mergedHead;

        lists[scan].type = wholeRun ? 3 : 4;
        lists[scan].rebuild();

        if (bubbleAllowed && wholeRun) {
            lists[scan].bubbleSort();
            cout << "  Bubble Sort applied to the whole merged list." << endl;
        } else if (bubbleAllowed) {
            lists[scan].bubbleSortLastK(suffixNodes);
            cout << "  Bubble Sort applied only to lists " << (startCompat + 1)
                 << " to " << (partner + 1) << " (" << suffixNodes
                 << " nodes at the end): the Doubly list before them breaks the chain."
                 << endl;
        } else {
            cout << "  Bubble Sort NOT applied: no compatible (Singly / Circular) "
                 << "sequence is available at the end of this run." << endl;
        }

        scan = partner + 1;
    }

    cout << endl << "=== Final lists ===" << endl;
    int shown = 0;
    for (int i = 0; i < total; i++) {
        if (!lists[i].alive)
            continue;

        shown++;
        cout << shown << ". " << lists[i].typeName() << ": ";
        lists[i].display();
        cout << endl;
    }
    cout << endl << "Total cycles detected: " << cycleCount << endl;

    return 0;
}
