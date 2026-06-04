#include "pch.h"
#include "CppUnitTest.h"
#include "vehicle.h"
#include "container.h"
#include "parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PR1_Tests
{
    // ========== ТЕСТЫ КЛАССОВ VEHICLE, TRUCK, BUS, CAR ==========
    TEST_CLASS(VehicleTests)
    {
    public:
        TEST_METHOD(TruckCreation)
        {
            Truck truck(350, "Germany", 25);

            Assert::AreEqual(350, truck.getPower());
            Assert::AreEqual(std::string("Germany"), truck.getCountry());
            Assert::AreEqual(25, truck.getLoadCapacity());
            Assert::AreEqual(std::string("Truck"), truck.getType());
        }

        TEST_METHOD(BusCreation)
        {
            Bus bus(220, "Russia", 45);

            Assert::AreEqual(220, bus.getPower());
            Assert::AreEqual(std::string("Russia"), bus.getCountry());
            Assert::AreEqual(45, (int)bus.getPassengerCapacity());
            Assert::AreEqual(std::string("Bus"), bus.getType());
        }

        TEST_METHOD(CarCreation)
        {
            Car car(150, "Japan", 4, 210);

            Assert::AreEqual(150, car.getPower());
            Assert::AreEqual(std::string("Japan"), car.getCountry());
            Assert::AreEqual(4, car.getDoors());
            Assert::AreEqual(210, car.getMaxSpeed());
            Assert::AreEqual(std::string("Car"), car.getType());
        }

        TEST_METHOD(CarDoorsCorrection)
        {
            Car carLow(100, "Test", 1, 180);
            Assert::AreEqual(2, carLow.getDoors());  // <2 → 2

            Car carHigh(100, "Test", 6, 180);
            Assert::AreEqual(5, carHigh.getDoors()); // >5 → 5

            Car carNormal(100, "Test", 4, 180);
            Assert::AreEqual(4, carNormal.getDoors());
        }
    };

    // ========== ТЕСТЫ ПАРСЕРА ==========
    TEST_CLASS(ParserTests)
    {
    public:
        TEST_METHOD(ParseTruck)
        {
            auto vehicle = parseAddCommand("Truck 350 Germany 25");

            Assert::IsNotNull(vehicle.get());
            Assert::AreEqual(std::string("Truck"), vehicle->getType());
            Assert::AreEqual(350, vehicle->getPower());
            Assert::AreEqual(std::string("Germany"), vehicle->getCountry());
        }

        TEST_METHOD(ParseBus)
        {
            auto vehicle = parseAddCommand("Bus 220 Russia 45");

            Assert::IsNotNull(vehicle.get());
            Assert::AreEqual(std::string("Bus"), vehicle->getType());
            Assert::AreEqual(220, vehicle->getPower());
            Assert::AreEqual(std::string("Russia"), vehicle->getCountry());
        }

        TEST_METHOD(ParseCar)
        {
            auto vehicle = parseAddCommand("Car 150 Japan 4 210");

            Assert::IsNotNull(vehicle.get());
            Assert::AreEqual(std::string("Car"), vehicle->getType());
            Assert::AreEqual(150, vehicle->getPower());
            Assert::AreEqual(std::string("Japan"), vehicle->getCountry());
        }

        TEST_METHOD(ParseInvalidCommand)
        {
            auto vehicle1 = parseAddCommand("Truck 350 Germany");
            Assert::IsNull(vehicle1.get());

            auto vehicle2 = parseAddCommand("Airplane 500");
            Assert::IsNull(vehicle2.get());

            auto vehicle3 = parseAddCommand("");
            Assert::IsNull(vehicle3.get());
        }
    };

    // ========== ТЕСТЫ КОНТЕЙНЕРА ==========
    TEST_CLASS(ContainerTests)
    {
    public:
        TEST_METHOD(AddVehicle)
        {
            VehicleContainer container;
            Assert::AreEqual((size_t)0, container.size());

            container.add(std::make_unique<Truck>(350, "Germany", 25));
            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(AddMultipleVehicles)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Bus>(220, "Russia", 45));
            container.add(std::make_unique<Car>(150, "Japan", 4, 210));

            Assert::AreEqual((size_t)3, container.size());
        }

        TEST_METHOD(RemoveByPower)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Truck>(400, "USA", 30));
            container.add(std::make_unique<Truck>(300, "France", 20));

            container.remove("power > 350");

            // Было: Assert::AreEqual((size_t)1, container.size());
            // Правильно: осталось 2 объекта (350 и 300)
            Assert::AreEqual((size_t)2, container.size());
        }

        TEST_METHOD(RemoveByType)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Bus>(220, "Russia", 45));
            container.add(std::make_unique<Car>(150, "Japan", 4, 210));

            container.remove("type == Bus");

            Assert::AreEqual((size_t)2, container.size());
        }

        TEST_METHOD(RemoveByCountry)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Truck>(400, "USA", 30));
            container.add(std::make_unique<Truck>(300, "Germany", 20));

            container.remove("country == Germany");

            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(RemoveByLoadCapacity)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Truck>(400, "USA", 30));
            container.add(std::make_unique<Truck>(300, "France", 20));

            container.remove("loadCapacity >= 25");

            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(RemoveNonexistent)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));
            container.add(std::make_unique<Bus>(220, "Russia", 45));

            size_t before = container.size();
            container.remove("power > 1000");

            Assert::AreEqual(before, container.size());
        }

        TEST_METHOD(RemoveEmptyCondition)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));

            size_t before = container.size();
            container.remove("");

            Assert::AreEqual(before, container.size());
        }
    };

    // ========== ТЕСТЫ ИСКЛЮЧИТЕЛЬНЫХ СИТУАЦИЙ ==========
    TEST_CLASS(ExceptionTests)
    {
    public:
        TEST_METHOD(InvalidNumberInCondition)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));

            // Должно отработать без падения
            container.remove("power > abc");

            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(InvalidFieldName)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));

            container.remove("nonexistent == 10");

            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(InvalidOperator)
        {
            VehicleContainer container;
            container.add(std::make_unique<Truck>(350, "Germany", 25));

            container.remove("power ?? 300");

            Assert::AreEqual((size_t)1, container.size());
        }

        TEST_METHOD(CarWrongDoorsValue)
        {
            Car car1(100, "Test", 0, 180);
            Assert::AreEqual(2, car1.getDoors());

            Car car2(100, "Test", 10, 180);
            Assert::AreEqual(5, car2.getDoors());
        }
    };
}