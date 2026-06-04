#ifndef CONTAINER_H
#define CONTAINER_H

#include "vehicle.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class VehicleContainer {
private:
    vector<unique_ptr<Vehicle>> vehicles;

    template<typename T>
    static bool compareNumbers(T a, T b, const string& op);

    static bool compareStrings(const string& a, const string& b, const string& op);

public:
    void add(unique_ptr<Vehicle> vehicle);
    void remove(const string& condition);
    void printAll() const;
    size_t size() const;
};

#endif // CONTAINER_H#pragma once
