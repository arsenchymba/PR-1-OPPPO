#include "container.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// ========== ÂÑÏÎÌÎÃÀÒÅËÜÍÛÅ ÔÓÍÊÖÈÈ ==========

static string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

static string unquote(const string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

// ========== ÏĞÅÎÁĞÀÇÎÂÀÍÈÅ ÑÒĞÎÊÈ Â COMPARISONOP ==========

VehicleContainer::ComparisonOp VehicleContainer::parseOperator(const string& op) {
    if (op == "==") return ComparisonOp::Equal;
    if (op == "!=") return ComparisonOp::NotEqual;
    if (op == ">")  return ComparisonOp::Greater;
    if (op == "<")  return ComparisonOp::Less;
    if (op == ">=") return ComparisonOp::GreaterEqual;
    if (op == "<=") return ComparisonOp::LessEqual;
    return ComparisonOp::Equal; // çíà÷åíèå ïî óìîë÷àíèş
}

// ========== ÑĞÀÂÍÅÍÈÅ ×ÈÑÅË ==========

template<typename T>
bool VehicleContainer::compareNumbers(T a, T b, ComparisonOp op) {
    switch (op) {
    case ComparisonOp::Equal:        return a == b;
    case ComparisonOp::NotEqual:     return a != b;
    case ComparisonOp::Greater:      return a > b;
    case ComparisonOp::Less:         return a < b;
    case ComparisonOp::GreaterEqual: return a >= b;
    case ComparisonOp::LessEqual:    return a <= b;
    default: return false;
    }
}

// ========== ÑĞÀÂÍÅÍÈÅ ÑÒĞÎÊ ==========

bool VehicleContainer::compareStrings(const string& a, const string& b, ComparisonOp op) {
    switch (op) {
    case ComparisonOp::Equal:        return a == b;
    case ComparisonOp::NotEqual:     return a != b;
    case ComparisonOp::Greater:      return a > b;
    case ComparisonOp::Less:         return a < b;
    case ComparisonOp::GreaterEqual: return a >= b;
    case ComparisonOp::LessEqual:    return a <= b;
    default: return false;
    }
}

// ========== ÏĞÎÂÅĞÊÀ ÓÑËÎÂÈß ÄËß ÎÄÍÎÃÎ ÎÁÚÅÊÒÀ ==========

bool VehicleContainer::matchesCondition(const Vehicle& v, const string& field,
    ComparisonOp op, const string& value) {
    // Îáùèå ïîëÿ
    if (field == "type") {
        return compareStrings(v.getType(), value, op);
    }
    if (field == "power") {
        int val = stoi(value);
        return compareNumbers(v.getPower(), val, op);
    }
    if (field == "country") {
        return compareStrings(v.getCountry(), value, op);
    }

    // Ïîëÿ ãğóçîâèêà
    if (field == "loadCapacity" && v.getType() == "Truck") {
        int val = stoi(value);
        return compareNumbers(static_cast<const Truck&>(v).getLoadCapacity(), val, op);
    }

    // Ïîëÿ àâòîáóñà
    if (field == "passengerCapacity" && v.getType() == "Bus") {
        short val = stoi(value);
        return compareNumbers(static_cast<const Bus&>(v).getPassengerCapacity(), val, op);
    }

    // Ïîëÿ ëåãêîâîãî àâòîìîáèëÿ
    if (field == "doors" && v.getType() == "Car") {
        int val = stoi(value);
        return compareNumbers(static_cast<const Car&>(v).getDoors(), val, op);
    }
    if (field == "maxSpeed" && v.getType() == "Car") {
        int val = stoi(value);
        return compareNumbers(static_cast<const Car&>(v).getMaxSpeed(), val, op);
    }

    return false;
}

// ========== ADD ==========

void VehicleContainer::add(unique_ptr<Vehicle> vehicle) {
    vehicles.push_back(move(vehicle));
    cout << "[ADD] Îáúåêò óñïåøíî äîáàâëåí. Âñåãî îáúåêòîâ: " << vehicles.size() << endl;
}

// ========== REM ==========

void VehicleContainer::remove(const string& condition) {
    if (condition.empty()) {
        cout << "[REM] Ïóñòîå óñëîâèå, íè÷åãî íå óäàëåíî" << endl;
        return;
    }

    istringstream iss(condition);
    string field, op_str, valueStr;
    iss >> field >> op_str;
    getline(iss, valueStr);

    valueStr = trim(unquote(valueStr));
    ComparisonOp op = parseOperator(op_str);

    auto it = remove_if(vehicles.begin(), vehicles.end(),
        [&](const unique_ptr<Vehicle>& v) -> bool {
            return matchesCondition(*v, field, op, valueStr);
        });

    vehicles.erase(it, vehicles.end());
    cout << "[REM] Ïîñëå óäàëåíèÿ îñòàëîñü: " << vehicles.size() << endl;
}

// ========== PRINT ==========

void VehicleContainer::printAll() const {
    if (vehicles.empty()) {
        cout << "\n=== ÊÎÍÒÅÉÍÅĞ ÏÓÑÒ ===" << endl;
        return;
    }

    cout << "\n=== ÑÎÄÅĞÆÈÌÎÅ ÊÎÍÒÅÉÍÅĞÀ ===" << endl;
    cout << "Âñåãî îáúåêòîâ: " << vehicles.size() << "\n" << endl;

    for (size_t i = 0; i < vehicles.size(); ++i) {
        cout << "[" << (i + 1) << "] ";
        vehicles[i]->print();
        cout << endl;
    }
    cout << "==============================\n" << endl;
}

// ========== SIZE ==========

size_t VehicleContainer::size() const {
    return vehicles.size();
}

// ========== ßÂÍÎÅ ÈÍÑÒÀÍÖÈĞÎÂÀÍÈÅ ØÀÁËÎÍÎÂ ==========

template bool VehicleContainer::compareNumbers<int>(int, int, ComparisonOp);
template bool VehicleContainer::compareNumbers<short>(short, short, ComparisonOp);