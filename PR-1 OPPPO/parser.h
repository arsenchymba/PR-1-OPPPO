#ifndef PARSER_H
#define PARSER_H

#include "vehicle.h"
#include <memory>
#include <string>

using namespace std;

unique_ptr<Vehicle> parseAddCommand(const string& args);

#endif // PARSER_H#pragma once
