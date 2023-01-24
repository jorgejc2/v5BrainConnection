#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include "api.h"
#include "pros/adi.hpp"

extern pros::Motor front_left_mtr;
extern pros::Motor front_right_mtr;
extern pros::Motor back_left_mtr;
extern pros::Motor back_right_mtr;
extern pros::Imu imu_sensor;
extern pros::Controller master;
extern pros::ADIEncoder encoder;
extern pros::Rotation rotation_sensor;

#endif