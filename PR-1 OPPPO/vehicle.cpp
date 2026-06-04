#include "vehicle.h"

using namespace std;

// ========== VEHICLE ==========
Vehicle::Vehicle(int p, const string& c) : power(p), country(c) {}

int Vehicle::getPower() const {
    return power;
}

const string& Vehicle::getCountry() const {
    return country;
}

// ========== TRUCK ==========
Truck::Truck(int p, const string& c, int load) : Vehicle(p, c), loadCapacity(load) {}

void Truck::print() const {
    cout << "Грузовик | Мощность: " << power << " л.с. | Страна: " << country
        << " | Грузоподъемность: " << loadCapacity << " т";
}

string Truck::getType() const {
    return "Truck";
}

int Truck::getLoadCapacity() const {
    return loadCapacity;
}

// ========== BUS ==========
Bus::Bus(int p, const string& c, short pass) : Vehicle(p, c), passengerCapacity(pass) {}

void Bus::print() const {
    cout << "Автобус   | Мощность: " << power << " л.с. | Страна: " << country
        << " | Пассажиров: " << passengerCapacity << " чел";
}

string Bus::getType() const {
    return "Bus";
}

short Bus::getPassengerCapacity() const {
    return passengerCapacity;
}

// ========== CAR ==========
Car::Car(int p, const string& c, int d, int ms) : Vehicle(p, c), doors(d), maxSpeed(ms) {
    if (doors < 2) doors = 2;
    if (doors > 5) doors = 5;
}

void Car::print() const {
    cout << "Легковой  | Мощность: " << power << " л.с. | Страна: " << country
        << " | Дверей: " << doors << " | Макс.скорость: " << maxSpeed << " км/ч";
}

string Car::getType() const {
    return "Car";
}

int Car::getDoors() const {
    return doors;
}

int Car::getMaxSpeed() const {
    return maxSpeed;
}