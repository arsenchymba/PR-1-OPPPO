#ifndef CONTAINER_H
#define CONTAINER_H

#include "vehicle.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class VehicleContainer {
public:
    enum class ComparisonOp {
        Equal, NotEqual, Greater, Less, GreaterEqual, LessEqual
    };
private:
    vector<unique_ptr<Vehicle>> vehicles;

    // Функция преобразования строки в ComparisonOp
    static ComparisonOp parseOperator(const string& op);

    // Шаблонный метод сравнения чисел
    template<typename T>
    static bool compareNumbers(T a, T b, ComparisonOp op);

    // Метод сравнения строк
    static bool compareStrings(const string& a, const string& b, ComparisonOp op);

    // Метод проверки условия для одного объекта
    static bool matchesCondition(const Vehicle& v, const string& field,
        ComparisonOp op, const string& value);

public:
    void add(unique_ptr<Vehicle> vehicle);
    void remove(const string& condition);
    void printAll() const;
    size_t size() const;
};

#endif // CONTAINER_H