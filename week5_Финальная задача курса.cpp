#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <iomanip>
#include <set>
#include <map>

using namespace std;

class Date {
private:
    int Year;
    int Month;
    int Day;
public:
    Date() {
        Year = 0;
        Month = 0;
        Day = 0;
    }
    
    Date(int y, int m, int d) {
        Year = y;
        Month = m;
        Day = d;
    }
    
    int GetYear() const {
        return Year;
    };
    int GetMonth() const {
        return Month;
    };
    int GetDay() const {
        return Day;
    }

};

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        } else {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    } else {
        return lhs.GetYear() < rhs.GetYear();
    }
}

class Database {
private:
    map<Date,set<string>> Events;
public:
    void AddEvent(const Date& date, const string& event) {
        Events[date].insert(event);
    }
    
    bool DeleteEvent(const Date& date, const string& event) {
        if (Events.count(date) == 0) {
            cout << "Event not found" << endl;
            return false;
        } else if (Events[date].count(event) == 0) {
            cout << "Event not found" << endl;
            return false;
        } else {
            Events[date].erase(event); 
            cout << "Deleted successfully" << endl;
            return true;
        }
    }
    
    int DeleteDate(const Date& date) {
        int events_size = Events[date].size();
        Events[date].clear();
        cout << "Deleted " << events_size << " events" << endl;
        return events_size;
    }

    void Find(const Date& date) const {
        if (Events.count(date) != 0) {
            for (const auto& i : Events.at(date)) {
                cout << i << endl;
            }
        }
    }
  
    void Print() const {
        for (const auto& item : Events) {
            for (const auto& i : item.second) {
                cout << setw(4) << setfill('0') << item.first.GetYear() << "-" <<
                        setw(2) << setfill('0') << item.first.GetMonth() << "-" <<
                        setw(2) << setfill('0') << item.first.GetDay() << " ";
                cout << i << endl; 
            }
        }
    }
};

Date TestDateFormat(const string& str) {
    int y, m, d;
    unsigned long pos = 0;
    
    y = stoi(str, &pos);
    if (str[pos] != '-') {
        throw invalid_argument("Wrong date format: " + str);
    } 

    string substr1 = str.substr(pos + 1);
    m = stoi(substr1, &pos);
    if (substr1[pos] != '-') {
        throw invalid_argument("Wrong date format: " + str);
    }
    
    string substr2 = substr1.substr(pos + 1);
    d = stoi(substr2, &pos);
    if (substr2[pos] != '\0') {
        throw invalid_argument("Wrong date format: " + str);
    }
    
    if (m > 12 || m < 1) {
        throw runtime_error("Month value is invalid: " + to_string(m));
    }
    
    if (d < 1 || d > 31) {
        throw runtime_error("Day value is invalid: " + to_string(d));
    }

    return {y, m, d};
}

      
int main() {
    Database db;
       
    string command;
    string token;
    try {
       while (getline(cin, command)) {
           if (command == "") {
               continue;
           } else {
               string input = command;
               istringstream is(input);
               //string token;
               is >> token;
               if (token == "Add") {
                   is >> token;
                   string event = "";
                   is >> event;
                   db.AddEvent(TestDateFormat(token), event);
               } else if (token == "Del") {
                   is >> token;
                   string event = "";
                   is >> event;
                   if (event != "") {
                       db.DeleteEvent(TestDateFormat(token), event);
                   } else {
                       db.DeleteDate(TestDateFormat(token));
                   }
               } else if (token == "Find") {
                   is >> token;
                   db.Find(TestDateFormat(token));
               } else if (token == "Print") {
                   db.Print();
               } else {
                   throw runtime_error("Unknown command: " + token);
               }
           }      
       }
    } catch (invalid_argument& e) {
       cout << "Wrong date format: " + token << endl;
    } catch (runtime_error& e) {
       cout << e.what() << endl;
    } catch (...) {
       cout << "Wrong date format: " + token << endl;
    }

    return 0;
}