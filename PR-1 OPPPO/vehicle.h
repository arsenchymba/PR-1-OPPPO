#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>

using namespace std;

// КЛАСС VEHICLE (абстрактный)
class Vehicle {
protected:
    int power;           // мощность двигателя (л.с.)
    string country;      // страна-производитель

public:
    Vehicle(int p, const string& c);
    virtual ~Vehicle() = default;

    int getPower() const;
    const string& getCountry() const;

    virtual void print() const = 0;
    virtual string getType() const = 0;
};

// КЛАСС TRUCK
class Truck : public Vehicle {
private:
    int loadCapacity;    // грузоподъемность (тонны)

public:
    Truck(int p, const string& c, int load);
    void print() const override;
    string getType() const override;
    int getLoadCapacity() const;
};

// КЛАСС BUS
class Bus : public Vehicle {
private:
    short passengerCapacity;   // пассажировместимость (чел)

public:
    Bus(int p, const string& c, short pass);
    void print() const override;
    string getType() const override;
    short getPassengerCapacity() const;
};

// КЛАСС CAR
class Car : public Vehicle {
private:
    int doors;      // количество дверей (2,3,4,5)
    int maxSpeed;   // максимальная скорость (км/ч)

public:
    Car(int p, const string& c, int d, int ms);
    void print() const override;
    string getType() const override;
    int getDoors() const;
    int getMaxSpeed() const;
};

#endif // VEHICLE_H