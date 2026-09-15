#include <iostream>
#include <string>
using namespace std;

class Team {
public:
    string name;
    int    points;
    Team*  next;

    Team(string teamName, int teamPoints) {
        name   = teamName;
        points = teamPoints;
        next   = NULL;
    }
};

class Tournament {
public:
    Team* head;

    Tournament() {
        head = NULL;
    }

    bool isEmpty() {
        return head == NULL;
    }

    int countTeams() {
        if (head == NULL) return 0;
        int   total = 0;
        Team* cur   = head;
        do {
            total++;
            cur = cur->next;
        } while (cur != head);
        return total;
    }

    Team* search(string wanted) {
        if (head == NULL) return NULL;
        Team* cur = head;
        do {
            if (cur->name == wanted)
                return cur;
            cur = cur->next;
        } while (cur != head);
        return NULL;
    }

    void addTeam(string name, int points) {
        if (search(name) != NULL) {
            cout << "Error: team \"" << name << "\" already exists." << endl;
            return;
        }
        if (points < 0)   points = 0;
        if (points > 150) points = 150;

        Team* fresh = new Team(name, points);

        if (head == NULL) {
            head        = fresh;
            fresh->next = fresh;
        } else {
            Team* tail = head;
            while (tail->next != head)
                tail = tail->next;
            tail->next  = fresh;
            fresh->next = head;
        }
        cout << "Team \"" << name << "\" added with " << points << " points." << endl;
    }

    Team* removeTeam(string name) {
        if (head == NULL) {
            cout << "Error: the tournament is empty." << endl;
            return NULL;
        }

        Team* target = search(name);
        if (target == NULL) {
            cout << "Error: team \"" << name << "\" was not found." << endl;
            return NULL;
        }

        if (target->next == target) {
            delete target;
            head = NULL;
            return NULL;
        }

        Team* before = head;
        while (before->next != target)
            before = before->next;

        Team* after  = target->next;
        before->next = after;

        if (target == head)
            head = after;

        delete target;
        return after;
    }

    void display() {
        if (head == NULL) {
            cout << "(no teams)" << endl;
            return;
        }
        Team* cur = head;
        int   pos = 1;
        do {
            cout << pos << ". " << cur->name
                 << " (" << cur->points << " pts)";
            if (cur->next != head) cout << "  ->  ";
            cur = cur->next;
            pos++;
        } while (cur != head);
        cout << "  ->  (back to " << head->name << ")" << endl;
    }

    int highestPoints() {
        if (head == NULL) return -1;
        int   best = head->points;
        Team* cur  = head->next;
        while (cur != head) {
            if (cur->points > best)
                best = cur->points;
            cur = cur->next;
        }
        return best;
    }

    Team* lowestTeamFrom(Team* start) {
        if (start == NULL) return NULL;
        int   top  = highestPoints();
        Team* best = NULL;
        Team* cur  = start;
        do {
            if (cur->points < top) {
                if (best == NULL || cur->points < best->points)
                    best = cur;
            }
            cur = cur->next;
        } while (cur != start);
        return best;
    }

    void updatePoints() {
        if (head == NULL) return;
        Team* cur = head;
        int   pos = 1;
        do {
            if (pos % 2 == 1) cur->points -= 20;
            else              cur->points += 10;

            if (cur->points < 0)   cur->points = 0;
            if (cur->points > 150) cur->points = 150;

            cur = cur->next;
            pos++;
        } while (cur != head);
    }

    Team* stepForward(Team* from, int steps) {
        Team* cur = from;
        for (int i = 0; i < steps; i++)
            cur = cur->next;
        return cur;
    }

    void knockout(string startName, int k) {
        if (head == NULL) {
            cout << "Error: the tournament is empty." << endl;
            return;
        }
        if (countTeams() == 1) {
            cout << "Only one team is present, so " << head->name
                 << " is already the champion." << endl;
            return;
        }
        if (k < 1) {
            cout << "Error: the counting number must be 1 or more." << endl;
            return;
        }

        Team* start = search(startName);
        if (start == NULL) {
            cout << "Error: starting team \"" << startName << "\" was not found." << endl;
            return;
        }

        int round = 1;

        while (countTeams() > 1) {
            cout << endl << "----- Round " << round << " -----" << endl;
            cout << "Counting starts at " << start->name
                 << " with K = " << k << endl;

            int n = countTeams();

            int   steps    = (k - 1) % n;
            Team* selected = stepForward(start, steps);
            cout << "Counting ends on: " << selected->name
                 << " (" << selected->points << " pts)" << endl;

            Team* victim = selected;

            if (selected->points == highestPoints()) {
                cout << "ERROR: " << selected->name
                     << " cannot be eliminated because it has the highest points ("
                     << selected->points << ")." << endl;
                Team* lowest = lowestTeamFrom(start);
                if (lowest == NULL) {
                    cout << "All remaining teams have the same points, so the "
                         << "selected team is eliminated after all." << endl;
                } else {
                    victim = lowest;
                    cout << "Lowest scoring team " << victim->name
                         << " (" << victim->points << " pts) is eliminated instead." << endl;
                }
            }

            string victimName = victim->name;
            Team*  nextStart  = removeTeam(victimName);

            cout << "Eliminated: " << victimName << endl;

            if (countTeams() <= 1) {
                break;
            }

            updatePoints();

            cout << "Updated points of remaining teams:" << endl;
            display();

            start = nextStart;
            if (start == NULL) start = head;
            round++;
        }

        cout << endl << "==============================================" << endl;
        cout << "FIFA Knockout Challenge Champion: " << head->name
             << " (" << head->points << " pts)" << endl;
        cout << "==============================================" << endl;
    }
};

int main() {
    Tournament t;
    int    choice = 0;
    string name;
    int    points;
    int    k;

    do {
        cout << endl;
        cout << "===== FIFA Knockout Challenge =====" << endl;
        cout << "1. Add a team" << endl;
        cout << "2. Remove a team" << endl;
        cout << "3. Search for a team" << endl;
        cout << "4. Display all teams" << endl;
        cout << "5. Start the knockout challenge" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin  >> choice;

        if (choice == 1) {
            cout << "Team name (one word): ";
            cin  >> name;
            cout << "Points (0 - 150): ";
            cin  >> points;
            t.addTeam(name, points);
        }
        else if (choice == 2) {
            cout << "Team name to remove: ";
            cin  >> name;
            if (t.search(name) == NULL) {
                cout << "Error: team \"" << name << "\" was not found." << endl;
            } else {
                t.removeTeam(name);
                cout << "Team \"" << name << "\" removed." << endl;
            }
        }
        else if (choice == 3) {
            cout << "Team name to search: ";
            cin  >> name;
            Team* found = t.search(name);
            if (found == NULL)
                cout << "Error: team \"" << name << "\" was not found." << endl;
            else
                cout << "Found: " << found->name << " with "
                     << found->points << " points." << endl;
        }
        else if (choice == 4) {
            cout << "Current teams:" << endl;
            t.display();
        }
        else if (choice == 5) {
            if (t.isEmpty()) {
                cout << "Error: add some teams first." << endl;
            } else {
                cout << "Starting team: ";
                cin  >> name;
                cout << "Counting number K: ";
                cin  >> k;
                t.knockout(name, k);
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice, please try again." << endl;
        }

    } while (choice != 0);

    cout << "Goodbye!" << endl;
    return 0;
}
