#include "container.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    string filename;
    setlocale(LC_ALL, "Rus");
    cout << "Ââåäèòå èìÿ ôàéëà ñ êîìàíäàìè: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Îøèáêà: Íå óäàëîñü îòêðûòü ôàéë " << filename << endl;
        return 1;
    }

    VehicleContainer container;
    string line;
    int lineNum = 0;

    cout << "\n========== ÍÀ×ÀËÎ ÎÁÐÀÁÎÒÊÈ ==========\n" << endl;

    while (getline(file, line)) {
        lineNum++;

        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos) continue;
        size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);

        if (line.empty()) continue;

        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "ADD") {
            string args;
            getline(iss, args);
            size_t pos = args.find_first_not_of(" \t");
            if (pos != string::npos) args = args.substr(pos);

            cout << "[ñòðîêà " << lineNum << "] ADD " << args << endl;
            auto vehicle = parseAddCommand(args);
            if (vehicle) {
                container.add(move(vehicle));
            }
        }
        else if (command == "REM") {
            string condition;
            getline(iss, condition);
            size_t pos = condition.find_first_not_of(" \t");
            if (pos != string::npos) condition = condition.substr(pos);

            cout << "[ñòðîêà " << lineNum << "] REM " << condition << endl;
            container.remove(condition);
        }
        else if (command == "PRINT") {
            cout << "[ñòðîêà " << lineNum << "] PRINT" << endl;
            container.printAll();
        }
        else {
            cout << "[ñòðîêà " << lineNum << "] ÏÐÅÄÓÏÐÅÆÄÅÍÈÅ: Íåèçâåñòíàÿ êîìàíäà '" << command << "'" << endl;
        }
    }

    file.close();
    cout << "=========== ÊÎÍÅÖ ÎÁÐÀÁÎÒÊÈ ===========" << endl;

    return 0;
}