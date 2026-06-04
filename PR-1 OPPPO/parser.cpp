#include "parser.h"
#include <sstream>
#include <iostream>

using namespace std;

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