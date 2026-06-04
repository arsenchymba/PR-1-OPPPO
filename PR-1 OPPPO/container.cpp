#include "container.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==========

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

// БЕЗОПАСНОЕ ПРЕОБРАЗОВАНИЕ СТРОКИ В ЧИСЛО
static bool safeStoi(const string& str, int& result) {
    try {
        size_t pos;
        result = stoi(str, &pos);
        // Проверяем, что вся строка была преобразована
        return pos == str.length();
    }
    catch (const invalid_argument&) {
        return false;
    }
    catch (const out_of_range&) {
        return false;
    }
}

static bool safeStoiShort(const string& str, short& result) {
    int temp;
    if (!safeStoi(str, temp)) return false;
    if (temp < -32768 || temp > 32767) return false; // Проверка границ short
    result = static_cast<short>(temp);
    return true;
}

// ========== ПРЕОБРАЗОВАНИЕ СТРОКИ В COMPARISONOP ==========

VehicleContainer::ComparisonOp VehicleContainer::parseOperator(const string& op) {
    if (op == "==") return ComparisonOp::Equal;
    if (op == "!=") return ComparisonOp::NotEqual;
    if (op == ">")  return ComparisonOp::Greater;
    if (op == "<")  return ComparisonOp::Less;
    if (op == ">=") return ComparisonOp::GreaterEqual;
    if (op == "<=") return ComparisonOp::LessEqual;
    return ComparisonOp::Equal;
}

// ========== СРАВНЕНИЕ ЧИСЕЛ ==========

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

// ========== СРАВНЕНИЕ СТРОК ==========

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

// ========== СПЕЦИАЛИЗАЦИИ ДЛЯ TRUCK ==========

bool VehicleContainer::compareTruckField(const Truck& t, const string& field,
    const string& value, ComparisonOp op) {
    if (field == "loadCapacity") {
        int val;
        if (!safeStoi(value, val)) {
            cerr << "[WARNING] Некорректное значение для loadCapacity: " << value << endl;
            return false;
        }
        return compareNumbers(t.getLoadCapacity(), val, op);
    }
    return false;
}

// ========== СПЕЦИАЛИЗАЦИИ ДЛЯ BUS ==========

bool VehicleContainer::compareBusField(const Bus& b, const string& field,
    const string& value, ComparisonOp op) {
    if (field == "passengerCapacity") {
        short val;
        if (!safeStoiShort(value, val)) {
            cerr << "[WARNING] Некорректное значение для passengerCapacity: " << value << endl;
            return false;
        }
        return compareNumbers(b.getPassengerCapacity(), val, op);
    }
    return false;
}

// ========== СПЕЦИАЛИЗАЦИИ ДЛЯ CAR ==========

bool VehicleContainer::compareCarField(const Car& c, const string& field,
    const string& value, ComparisonOp op) {
    if (field == "doors") {
        int val;
        if (!safeStoi(value, val)) {
            cerr << "[WARNING] Некорректное значение для doors: " << value << endl;
            return false;
        }
        return compareNumbers(c.getDoors(), val, op);
    }
    if (field == "maxSpeed") {
        int val;
        if (!safeStoi(value, val)) {
            cerr << "[WARNING] Некорректное значение для maxSpeed: " << value << endl;
            return false;
        }
        return compareNumbers(c.getMaxSpeed(), val, op);
    }
    return false;
}

// ========== ПРОВЕРКА УСЛОВИЯ ДЛЯ ОДНОГО ОБЪЕКТА ==========

bool VehicleContainer::matchesCondition(const Vehicle& v, const string& field,
    ComparisonOp op, const string& value) {
    // Общие поля
    if (field == "type") {
        return compareStrings(v.getType(), value, op);
    }
    if (field == "power") {
        int val;
        if (!safeStoi(value, val)) {
            cerr << "[WARNING] Некорректное значение для power: " << value << endl;
            return false;
        }
        return compareNumbers(v.getPower(), val, op);
    }
    if (field == "country") {
        return compareStrings(v.getCountry(), value, op);
    }

    // Поля специфичные для каждого типа
    if (v.getType() == "Truck") {
        return compareTruckField(static_cast<const Truck&>(v), field, value, op);
    }
    if (v.getType() == "Bus") {
        return compareBusField(static_cast<const Bus&>(v), field, value, op);
    }
    if (v.getType() == "Car") {
        return compareCarField(static_cast<const Car&>(v), field, value, op);
    }

    // Неизвестное поле
    cerr << "[WARNING] Неизвестное поле для сравнения: " << field << endl;
    return false;
}

// ========== ADD ==========

void VehicleContainer::add(unique_ptr<Vehicle> vehicle) {
    vehicles.push_back(move(vehicle));
    cout << "[ADD] Объект успешно добавлен. Всего объектов: " << vehicles.size() << endl;
}

// ========== REM ==========

void VehicleContainer::remove(const string& condition) {
    if (condition.empty()) {
        cout << "[REM] Пустое условие, ничего не удалено" << endl;
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
    cout << "[REM] После удаления осталось: " << vehicles.size() << endl;
}

// ========== PRINT ==========

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

// ========== SIZE ==========

size_t VehicleContainer::size() const {
    return vehicles.size();
}

// ========== ЯВНОЕ ИНСТАНЦИРОВАНИЕ ШАБЛОНОВ ==========

template bool VehicleContainer::compareNumbers<int>(int, int, ComparisonOp);
template bool VehicleContainer::compareNumbers<short>(short, short, ComparisonOp);