#include "container.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Вспомогательная функция для удаления пробелов
static string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

// Вспомогательная функция для удаления кавычек
static string unquote(const string& str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

// Сравнение чисел
template<typename T>
bool VehicleContainer::compareNumbers(T a, T b, const string& op) {
    if (op == "==") return a == b;
    if (op == "!=") return a != b;
    if (op == ">")  return a > b;
    if (op == "<")  return a < b;
    if (op == ">=") return a >= b;
    if (op == "<=") return a <= b;
    return false;
}

// Сравнение строк
bool VehicleContainer::compareStrings(const string& a, const string& b, const string& op) {
    if (op == "==") return a == b;
    if (op == "!=") return a != b;
    if (op == ">")  return a > b;
    if (op == "<")  return a < b;
    if (op == ">=") return a >= b;
    if (op == "<=") return a <= b;
    return false;
}

// ADD
void VehicleContainer::add(unique_ptr<Vehicle> vehicle) {
    vehicles.push_back(move(vehicle));
    cout << "[ADD] Объект успешно добавлен. Всего объектов: " << vehicles.size() << endl;
}

// REM
void VehicleContainer::remove(const string& condition) {
    if (condition.empty()) {
        cout << "[REM] Пустое условие, ничего не удалено" << endl;
        return;
    }

    istringstream iss(condition);
    string field, op, valueStr;
    iss >> field >> op;
    getline(iss, valueStr);

    valueStr = trim(unquote(valueStr));

    auto it = remove_if(vehicles.begin(), vehicles.end(),
        [&](const unique_ptr<Vehicle>& v) -> bool {
            if (field == "type") {
                return compareStrings(v->getType(), valueStr, op);
            }
            else if (field == "power") {
                int val = stoi(valueStr);
                return compareNumbers(v->getPower(), val, op);
            }
            else if (field == "country") {
                return compareStrings(v->getCountry(), valueStr, op);
            }
            else if (field == "loadCapacity") {
                if (v->getType() == "Truck") {
                    int val = stoi(valueStr);
                    return compareNumbers(static_cast<Truck*>(v.get())->getLoadCapacity(), val, op);
                }
            }
            else if (field == "passengerCapacity") {
                if (v->getType() == "Bus") {
                    short val = stoi(valueStr);
                    return compareNumbers(static_cast<Bus*>(v.get())->getPassengerCapacity(), val, op);
                }
            }
            else if (field == "doors") {
                if (v->getType() == "Car") {
                    int val = stoi(valueStr);
                    return compareNumbers(static_cast<Car*>(v.get())->getDoors(), val, op);
                }
            }
            else if (field == "maxSpeed") {
                if (v->getType() == "Car") {
                    int val = stoi(valueStr);
                    return compareNumbers(static_cast<Car*>(v.get())->getMaxSpeed(), val, op);
                }
            }
            return false;
        });

    vehicles.erase(it, vehicles.end());
    cout << "[REM] После удаления осталось: " << vehicles.size() << endl;
}

// PRINT
void VehicleContainer::printAll() const {
    if (vehicles.empty()) {
        cout << "\n=== КОНТЕЙНЕР ПУСТ ===" << endl;
        return;
    }

    cout << "\n=== СОДЕРЖИМОЕ КОНТЕЙНЕРА ===" << endl;
    cout << "Всего объектов: " << vehicles.size() << "\n" << endl;

    for (size_t i = 0; i < vehicles.size(); ++i) {
        cout << "[" << (i + 1) << "] ";
        vehicles[i]->print();
        cout << endl;
    }
    cout << "==============================\n" << endl;
}

size_t VehicleContainer::size() const {
    return vehicles.size();
}

// Явное инстанцирование шаблона для используемых типов
template bool VehicleContainer::compareNumbers<int>(int, int, const string&);
template bool VehicleContainer::compareNumbers<short>(short, short, const string&);