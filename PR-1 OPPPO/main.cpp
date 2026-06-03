#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// КЛАСС VEHICLE (абстрактный)
class Vehicle {
protected:
    int power;           // мощность двигателя (л.с.)
    string country;      // страна-производитель

public:
    Vehicle(int p, const string& c) : power(p), country(c) {}
    virtual ~Vehicle() = default;

    // Геттеры для доступа к полям (для REM)
    int getPower() const { return power; }
    string getCountry() const { return country; }

    // Чисто виртуальные методы
    virtual void print() const = 0;
    virtual string getType() const = 0;
};

// КЛАСС TRUCK
class Truck : public Vehicle {
private:
    int loadCapacity;    // грузоподъемность (тонны)

public:
    Truck(int p, const string& c, int load) : Vehicle(p, c), loadCapacity(load) {}

    void print() const override {
        cout << "Грузовик | Мощность: " << power << " л.с. | Страна: " << country
            << " | Грузоподъемность: " << loadCapacity << " т";
    }

    string getType() const override {
        return "Truck";
    }

    int getLoadCapacity() const { return loadCapacity; }
};

// КЛАСС BUS
class Bus : public Vehicle {
private:
    short passengerCapacity;   // пассажировместимость (чел)

public:
    Bus(int p, const string& c, short pass) : Vehicle(p, c), passengerCapacity(pass) {}

    void print() const override {
        cout << "Автобус   | Мощность: " << power << " л.с. | Страна: " << country
            << " | Пассажиров: " << passengerCapacity << " чел";
    }

    string getType() const override {
        return "Bus";
    }

    short getPassengerCapacity() const { return passengerCapacity; }
};

// КЛАСС CAR
class Car : public Vehicle {
private:
    int doors;      // количество дверей (2,3,4,5)
    int maxSpeed;   // максимальная скорость (км/ч)

public:
    Car(int p, const string& c, int d, int ms) : Vehicle(p, c), doors(d), maxSpeed(ms) {
        // Корректировка некорректного количества дверей
        if (doors < 2) doors = 2;
        if (doors > 5) doors = 5;
    }

    void print() const override {
        cout << "Легковой  | Мощность: " << power << " л.с. | Страна: " << country
            << " | Дверей: " << doors << " | Макс.скорость: " << maxSpeed << " км/ч";
    }

    string getType() const override {
        return "Car";
    }

    int getDoors() const { return doors; }
    int getMaxSpeed() const { return maxSpeed; }
};

// КЛАСС VEHICLE CONTAINER
class VehicleContainer {
private:
    vector<unique_ptr<Vehicle>> vehicles;   // контейнер для хранения объектов

    // Вспомогательный метод для сравнения чисел
    template<typename T>
    bool compareNumbers(T a, T b, const string& op) const {
        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == ">")  return a > b;
        if (op == "<")  return a < b;
        if (op == ">=") return a >= b;
        if (op == "<=") return a <= b;
        return false;
    }

    // Вспомогательный метод для сравнения строк
    bool compareStrings(const string& a, const string& b, const string& op) const {
        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == ">")  return a > b;
        if (op == "<")  return a < b;
        if (op == ">=") return a >= b;
        if (op == "<=") return a <= b;
        return false;
    }

public:
    // Метод ADD - добавление объекта
    void add(unique_ptr<Vehicle> vehicle) {
        vehicles.push_back(move(vehicle));
        cout << "[ADD] Объект успешно добавлен. Всего объектов: " << vehicles.size() << endl;
    }

    // Метод REM - удаление по условию
    void remove(const string& condition) {
        if (condition.empty()) {
            cout << "[REM] Пустое условие, ничего не удалено" << endl;
            return;
        }

        // Парсим условие: поле оператор значение
        istringstream iss(condition);
        string field, op, valueStr;
        iss >> field >> op;
        getline(iss, valueStr);

        // Убираем лишние пробелы в начале значения
        size_t start = valueStr.find_first_not_of(" \t");
        if (start != string::npos) {
            valueStr = valueStr.substr(start);
        }

        // Убираем кавычки, если они есть (для строк)
        if (valueStr.front() == '"' && valueStr.back() == '"') {
            valueStr = valueStr.substr(1, valueStr.length() - 2);
        }

        size_t beforeCount = vehicles.size();

        // Удаляем элементы, удовлетворяющие условию
        auto it = remove_if(vehicles.begin(), vehicles.end(),
            [&](const unique_ptr<Vehicle>& v) -> bool {
                // Сравнение по типу
                if (field == "type") {
                    return compareStrings(v->getType(), valueStr, op);
                }
                // Сравнение по мощности
                else if (field == "power") {
                    int val = stoi(valueStr);
                    return compareNumbers(v->getPower(), val, op);
                }
                // Сравнение по стране
                else if (field == "country") {
                    return compareStrings(v->getCountry(), valueStr, op);
                }
                // Сравнение по грузоподъемности (только для Truck)
                else if (field == "loadCapacity") {
                    if (v->getType() == "Truck") {
                        int val = stoi(valueStr);
                        return compareNumbers(static_cast<Truck*>(v.get())->getLoadCapacity(), val, op);
                    }
                }
                // Сравнение по пассажировместимости (только для Bus)
                else if (field == "passengerCapacity") {
                    if (v->getType() == "Bus") {
                        short val = stoi(valueStr);
                        return compareNumbers(static_cast<Bus*>(v.get())->getPassengerCapacity(), val, op);
                    }
                }
                // Сравнение по количеству дверей (только для Car)
                else if (field == "doors") {
                    if (v->getType() == "Car") {
                        int val = stoi(valueStr);
                        return compareNumbers(static_cast<Car*>(v.get())->getDoors(), val, op);
                    }
                }
                // Сравнение по максимальной скорости (только для Car)
                else if (field == "maxSpeed") {
                    if (v->getType() == "Car") {
                        int val = stoi(valueStr);
                        return compareNumbers(static_cast<Car*>(v.get())->getMaxSpeed(), val, op);
                    }
                }
                return false;
            });

        size_t removedCount = beforeCount - (vehicles.end() - it);
        vehicles.erase(it, vehicles.end());

        cout <<"После удаления осталось: " << vehicles.size() << endl;
    }

    // Метод PRINT - вывод всех объектов
    void printAll() const {
        if (vehicles.empty()) {
            cout << "\nКОНТЕЙНЕР ПУСТ" << endl;
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

    size_t size() const { return vehicles.size(); }
};

// ПАРСЕР КОМАНД

// Парсинг команды ADD
// Формат: ADD <тип> <мощность> <страна> <уникальные параметры>

unique_ptr<Vehicle> parseAddCommand(const string& args) {
    istringstream iss(args);
    string type;
    iss >> type;

    if (type == "Truck") {
        int power, loadCapacity;
        string country;
        if (iss >> power >> country >> loadCapacity) {
            return make_unique<Truck>(power, country, loadCapacity);
        }
        else {
            cout << "[ОШИБКА] Неверный формат ADD Truck: мощность страна грузоподъемность" << endl;
            return nullptr;
        }
    }
    else if (type == "Bus") {
        int power;
        short passengerCapacity;
        string country;
        if (iss >> power >> country >> passengerCapacity) {
            return make_unique<Bus>(power, country, passengerCapacity);
        }
        else {
            cout << "[ОШИБКА] Неверный формат ADD Bus: мощность страна пассажиры" << endl;
            return nullptr;
        }
    }
    else if (type == "Car") {
        int power, doors, maxSpeed;
        string country;
        if (iss >> power >> country >> doors >> maxSpeed) {
            return make_unique<Car>(power, country, doors, maxSpeed);
        }
        else {
            cout << "[ОШИБКА] Неверный формат ADD Car: мощность страна двери скорость" << endl;
            return nullptr;
        }
    }
    else {
        cout << "[ОШИБКА] Неизвестный тип транспорта: " << type << endl;
        return nullptr;
    }
}

// Главная
int main() {
    string filename;
    setlocale(LC_ALL, "Rus");
    cout << "Введите имя файла с командами: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return 1;
    }

    VehicleContainer container;
    string line;
    int lineNum = 0;

    cout << "\n НАЧАЛО ОБРАБОТКИ \n" << endl;

    while (getline(file, line)) {
        lineNum++;

        // Удаляем пробелы в начале и конце строки
        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos) continue; // пустая строка
        size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);

        if (line.empty()) continue;

        // Разбираем команду
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "ADD") {
            string args;
            getline(iss, args);
            // Убираем пробел перед аргументами
            size_t pos = args.find_first_not_of(" \t");
            if (pos != string::npos) {
                args = args.substr(pos);
            }

            cout << "[строка " << lineNum << "] ADD " << args << endl;
            auto vehicle = parseAddCommand(args);
            if (vehicle) {
                container.add(move(vehicle));
            }
        }
        else if (command == "REM") {
            string condition;
            getline(iss, condition);
            size_t pos = condition.find_first_not_of(" \t");
            if (pos != string::npos) {
                condition = condition.substr(pos);
            }

            cout << "[строка " << lineNum << "] REM " << condition << endl;
            container.remove(condition);
        }
        else if (command == "PRINT") {
            cout << "[строка " << lineNum << "] PRINT" << endl;
            container.printAll();
        }
        else {
            cout << "[строка " << lineNum << "] ПРЕДУПРЕЖДЕНИЕ: Неизвестная команда '" << command << "'" << endl;
        }
    }

    file.close();
    cout << " КОНЕЦ ОБРАБОТКИ " << endl;

    return 0;
}