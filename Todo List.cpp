#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// Task Class
class Task {
public:
    int         id;
    string      title;
    string      category;
    bool        completed;

    Task(int id, const string& title, const string& category)
        : id(id), title(title), category(category), completed(false) {}

    string serialize() const {
        return to_string(id) + "|" + title + "|" + category + "|" +
               (completed ? "1" : "0");
    }

    static Task deserialize(const string& line) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        int    id       = stoi(line.substr(0, p1));
        string title    = line.substr(p1 + 1, p2 - p1 - 1);
        string category = line.substr(p2 + 1, p3 - p2 - 1);
        bool   done     = (line.substr(p3 + 1) == "1");

        Task t(id, title, category);
        t.completed = done;
        return t;
    }
};

// TaskManager Class
class TaskManager {
private:
    vector<Task> tasks;
    int          nextId;
    const string FILE_NAME = "tasks.txt";

    void saveTasks() const {
        ofstream file(FILE_NAME);
        if (!file) { cerr << "  [ERROR] Could not save.\n"; return; }
        file << nextId << "\n";
        for (const auto& t : tasks)
            file << t.serialize() << "\n";
        file.close();
    }

    void loadTasks() {
        ifstream file(FILE_NAME);
        if (!file) return;
        string line;
        getline(file, line);
        nextId = stoi(line);
        while (getline(file, line))
            if (!line.empty()) tasks.push_back(Task::deserialize(line));
        file.close();
    }

public:
    TaskManager() : nextId(1) { loadTasks(); }

    void addTask() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string title, category;
        cout << "\n  Task title    : ";
        getline(cin, title);
        if (title.empty()) { cout << "  Title cannot be empty.\n"; return; }
        cout << "  Category      : ";
        getline(cin, category);
        if (category.empty()) category = "General";
        tasks.emplace_back(nextId++, title, category);
        saveTasks();
        cout << "  Task added successfully!\n";
    }

    void markCompleted() {
        if (tasks.empty()) { cout << "  No tasks available.\n"; return; }
        viewPending();
        cout << "\n  Enter task ID to mark as completed: ";
        int id; cin >> id;
        for (auto& t : tasks) {
            if (t.id == id && !t.completed) {
                t.completed = true;
                saveTasks();
                cout << "  Task marked as completed!\n";
                return;
            }
        }
        cout << "  Task not found or already completed.\n";
    }

    void deleteTask() {
        if (tasks.empty()) { cout << "  No tasks available.\n"; return; }
        viewAll();
        cout << "\n  Enter task ID to delete: ";
        int id; cin >> id;
        auto it = remove_if(tasks.begin(), tasks.end(),
                            [id](const Task& t){ return t.id == id; });
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            saveTasks();
            cout << "  Task deleted.\n";
        } else {
            cout << "  Task ID not found.\n";
        }
    }

    void printHeader() const {
        cout << "\n  " << string(60, '-') << "\n";
        cout << "  " << left << setw(5)  << "ID"
                             << setw(30) << "Title"
                             << setw(15) << "Category"
                             << "Status\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void printTask(const Task& t) const {
        cout << "  " << left << setw(5)  << t.id
                             << setw(30) << t.title
                             << setw(15) << t.category
                             << (t.completed ? "[Done]" : "[Pending]") << "\n";
    }

    void viewPending() const {
        cout << "\n  --- Pending Tasks ---";
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (!t.completed) { printTask(t); any = true; }
        if (!any) cout << "  (no pending tasks)\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void viewCompleted() const {
        cout << "\n  --- Completed Tasks ---";
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (t.completed) { printTask(t); any = true; }
        if (!any) cout << "  (no completed tasks)\n";
        cout << "  " << string(60, '-') << "\n";
    }

    void viewAll() const {
        cout << "\n  --- All Tasks ---";
        printHeader();
        if (tasks.empty()) cout << "  (no tasks yet)\n";
        for (const auto& t : tasks) printTask(t);
        cout << "  " << string(60, '-') << "\n";
    }

    void searchByCategory() const {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string cat;
        cout << "\n  Enter category to search: ";
        getline(cin, cat);
        cout << "\n  --- Tasks in [" << cat << "] ---";
        printHeader();
        bool any = false;
        for (const auto& t : tasks)
            if (t.category == cat) { printTask(t); any = true; }
        if (!any) cout << "  No tasks found in this category.\n";
        cout << "  " << string(60, '-') << "\n";
    }
};

// Menu
void showMenu() {
    cout << "\n  ================================\n";
    cout << "       TO-DO LIST MANAGER\n";
    cout << "  ================================\n";
    cout << "  1. Add Task\n";
    cout << "  2. Mark Task as Completed\n";
    cout << "  3. View Pending Tasks\n";
    cout << "  4. View Completed Tasks\n";
    cout << "  5. View All Tasks\n";
    cout << "  6. Search by Category\n";
    cout << "  7. Delete a Task\n";
    cout << "  0. Exit\n";
    cout << "  ================================\n";
    cout << "  Your choice: ";
}

int main() {
    TaskManager manager;
    int choice;

    cout << "\n  Welcome to the CodeAlpha To-Do List Application!\n";
    cout << "  Tasks are saved automatically to tasks.txt\n";

    do {
        showMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Enter a number: ";
        }
        switch (choice) {
            case 1: manager.addTask();          break;
            case 2: manager.markCompleted();    break;
            case 3: manager.viewPending();      break;
            case 4: manager.viewCompleted();    break;
            case 5: manager.viewAll();          break;
            case 6: manager.searchByCategory(); break;
            case 7: manager.deleteTask();       break;
            case 0: cout << "\n  Goodbye! Tasks saved.\n\n"; break;
            default: cout << "  Invalid option. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}