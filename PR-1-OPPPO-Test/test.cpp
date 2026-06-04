#include "pch.h"
#include "..\PR-1 OPPPO\vehicle.h"
#include "..\PR-1 OPPPO\container.h"
#include "..\PR-1 OPPPO\parser.h"
#include <sstream>

using namespace std;

// ========== ТЕСТЫ ДЛЯ КЛАССА VEHICLE ==========

TEST(VehicleTest, TruckCreation) {
    Truck truck(350, "Germany", 25);

    EXPECT_EQ(truck.getPower(), 350);
    EXPECT_EQ(truck.getCountry(), "Germany");
    EXPECT_EQ(truck.getLoadCapacity(), 25);
    EXPECT_EQ(truck.getType(), "Truck");
}

TEST(VehicleTest, BusCreation) {
    Bus bus(220, "Russia", 45);

    EXPECT_EQ(bus.getPower(), 220);
    EXPECT_EQ(bus.getCountry(), "Russia");
    EXPECT_EQ(bus.getPassengerCapacity(), 45);
    EXPECT_EQ(bus.getType(), "Bus");
}

TEST(VehicleTest, CarCreation) {
    Car car(150, "Japan", 4, 210);

    EXPECT_EQ(car.getPower(), 150);
    EXPECT_EQ(car.getCountry(), "Japan");
    EXPECT_EQ(car.getDoors(), 4);
    EXPECT_EQ(car.getMaxSpeed(), 210);
    EXPECT_EQ(car.getType(), "Car");
}

TEST(VehicleTest, CarDoorsCorrection) {
    // Двери меньше 2 -> должно стать 2
    Car car1(100, "Test", 1, 180);
    EXPECT_EQ(car1.getDoors(), 2);

    // Двери больше 5 -> должно стать 5
    Car car2(100, "Test", 6, 180);
    EXPECT_EQ(car2.getDoors(), 5);

    // Двери в норме -> остаются
    Car car3(100, "Test", 3, 180);
    EXPECT_EQ(car3.getDoors(), 3);
}

TEST(VehicleTest, PrintOutput) {
    Truck truck(350, "Germany", 25);

    stringstream output;
    streambuf* oldCout = cout.rdbuf(output.rdbuf());

    truck.print();

    cout.rdbuf(oldCout);

    EXPECT_NE(output.str().find("Грузовик"), string::npos);
    EXPECT_NE(output.str().find("350"), string::npos);
    EXPECT_NE(output.str().find("Germany"), string::npos);
    EXPECT_NE(output.str().find("25"), string::npos);
}

// ========== ТЕСТЫ ДЛЯ ПАРСЕРА ==========

TEST(ParserTest, ParseTruck) {
    auto vehicle = parseAddCommand("Truck 350 Germany 25");

    ASSERT_NE(vehicle, nullptr);
    EXPECT_EQ(vehicle->getType(), "Truck");
    EXPECT_EQ(vehicle->getPower(), 350);
    EXPECT_EQ(vehicle->getCountry(), "Germany");

    Truck* truck = dynamic_cast<Truck*>(vehicle.get());
    ASSERT_NE(truck, nullptr);
    EXPECT_EQ(truck->getLoadCapacity(), 25);
}

TEST(ParserTest, ParseBus) {
    auto vehicle = parseAddCommand("Bus 220 Russia 45");

    ASSERT_NE(vehicle, nullptr);
    EXPECT_EQ(vehicle->getType(), "Bus");
    EXPECT_EQ(vehicle->getPower(), 220);
    EXPECT_EQ(vehicle->getCountry(), "Russia");

    Bus* bus = dynamic_cast<Bus*>(vehicle.get());
    ASSERT_NE(bus, nullptr);
    EXPECT_EQ(bus->getPassengerCapacity(), 45);
}

TEST(ParserTest, ParseCar) {
    auto vehicle = parseAddCommand("Car 150 Japan 4 210");

    ASSERT_NE(vehicle, nullptr);
    EXPECT_EQ(vehicle->getType(), "Car");
    EXPECT_EQ(vehicle->getPower(), 150);
    EXPECT_EQ(vehicle->getCountry(), "Japan");

    Car* car = dynamic_cast<Car*>(vehicle.get());
    ASSERT_NE(car, nullptr);
    EXPECT_EQ(car->getDoors(), 4);
    EXPECT_EQ(car->getMaxSpeed(), 210);
}

TEST(ParserTest, ParseInvalidCommand) {
    // Неверный формат
    auto vehicle1 = parseAddCommand("Truck 350 Germany");
    EXPECT_EQ(vehicle1, nullptr);

    // Неизвестный тип
    auto vehicle2 = parseAddCommand("Airplane 500 USA");
    EXPECT_EQ(vehicle2, nullptr);

    // Пустая строка
    auto vehicle3 = parseAddCommand("");
    EXPECT_EQ(vehicle3, nullptr);
}

// ========== ТЕСТЫ ДЛЯ КОНТЕЙНЕРА ==========

TEST(ContainerTest, AddVehicle) {
    VehicleContainer container;

    EXPECT_EQ(container.size(), 0);

    auto truck = make_unique<Truck>(350, "Germany", 25);
    container.add(move(truck));

    EXPECT_EQ(container.size(), 1);
}

TEST(ContainerTest, AddMultipleVehicles) {
    VehicleContainer container;

    auto truck = make_unique<Truck>(350, "Germany", 25);
    auto bus = make_unique<Bus>(220, "Russia", 45);
    auto car = make_unique<Car>(150, "Japan", 4, 210);

    container.add(move(truck));
    container.add(move(bus));
    container.add(move(car));

    EXPECT_EQ(container.size(), 3);
}

TEST(ContainerTest, RemoveByPower) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));
    container.add(make_unique<Truck>(400, "USA", 30));
    container.add(make_unique<Truck>(300, "France", 20));

    container.remove("power > 350");

    EXPECT_EQ(container.size(), 1);
}

TEST(ContainerTest, RemoveByType) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));
    container.add(make_unique<Bus>(220, "Russia", 45));
    container.add(make_unique<Car>(150, "Japan", 4, 210));

    container.remove("type == Bus");

    EXPECT_EQ(container.size(), 2);
}

TEST(ContainerTest, RemoveByCountry) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));
    container.add(make_unique<Truck>(400, "USA", 30));
    container.add(make_unique<Truck>(300, "Germany", 20));

    container.remove("country == Germany");

    EXPECT_EQ(container.size(), 1);
}

TEST(ContainerTest, RemoveByLoadCapacity) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));
    container.add(make_unique<Truck>(400, "USA", 30));
    container.add(make_unique<Truck>(300, "France", 20));

    container.remove("loadCapacity >= 25");

    EXPECT_EQ(container.size(), 1);
}

TEST(ContainerTest, RemoveNonexistent) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));
    container.add(make_unique<Bus>(220, "Russia", 45));

    size_t before = container.size();
    container.remove("power > 1000");

    EXPECT_EQ(container.size(), before);
}

TEST(ContainerTest, RemoveEmptyCondition) {
    VehicleContainer container;

    container.add(make_unique<Truck>(350, "Germany", 25));

    size_t before = container.size();
    container.remove("");

    EXPECT_EQ(container.size(), before);
}

// ========== ТЕСТЫ ИСКЛЮЧИТЕЛЬНЫХ СИТУАЦИЙ ==========

TEST(ExceptionTest, InvalidNumberInCondition) {
    VehicleContainer container;
    container.add(make_unique<Truck>(350, "Germany", 25));

    // Должно отработать без падения (выдаст предупреждение)
    EXPECT_NO_THROW(container.remove("power > abc"));

    // Размер не должен измениться
    EXPECT_EQ(container.size(), 1);
}

TEST(ExceptionTest, InvalidFieldName) {
    VehicleContainer container;
    container.add(make_unique<Truck>(350, "Germany", 25));

    // Несуществующее поле
    EXPECT_NO_THROW(container.remove("nonexistent == 10"));

    EXPECT_EQ(container.size(), 1);
}

TEST(ExceptionTest, InvalidOperator) {
    VehicleContainer container;
    container.add(make_unique<Truck>(350, "Germany", 25));

    // Неверный оператор
    EXPECT_NO_THROW(container.remove("power ?? 300"));

    EXPECT_EQ(container.size(), 1);
}

TEST(ExceptionTest, CarWrongDoorsValue) {
    // При создании Car с некорректными дверями - корректировка
    Car car1(100, "Test", 0, 180);
    EXPECT_EQ(car1.getDoors(), 2);

    Car car2(100, "Test", 10, 180);
    EXPECT_EQ(car2.getDoors(), 5);
}

TEST(ExceptionTest, StoiBoundaryForShort) {
    // Создаём автобус и пытаемся удалить по некорректному значению
    VehicleContainer container;
    container.add(make_unique<Bus>(220, "Russia", 45));

    // Должно отработать без падения
    EXPECT_NO_THROW(container.remove("passengerCapacity > 100000"));

    EXPECT_EQ(container.size(), 1);
}

// ========== ТЕСТЫ ПЕЧАТИ ==========

TEST(PrintTest, PrintEmptyContainer) {
    VehicleContainer container;

    stringstream output;
    streambuf* oldCout = cout.rdbuf(output.rdbuf());

    container.printAll();

    cout.rdbuf(oldCout);

    EXPECT_NE(output.str().find("КОНТЕЙНЕР ПУСТ"), string::npos);
}

TEST(PrintTest, PrintNonEmptyContainer) {
    VehicleContainer container;
    container.add(make_unique<Truck>(350, "Germany", 25));

    stringstream output;
    streambuf* oldCout = cout.rdbuf(output.rdbuf());

    container.printAll();

    cout.rdbuf(oldCout);

    EXPECT_NE(output.str().find("Грузовик"), string::npos);
    EXPECT_NE(output.str().find("СОДЕРЖИМОЕ"), string::npos);
}