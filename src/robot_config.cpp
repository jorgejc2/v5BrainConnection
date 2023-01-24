#include "robot_config.h"
#include "pros/adi.hpp"

pros::Motor front_left_mtr(10, false);
pros::Motor front_right_mtr(21, true);
pros::Motor back_left_mtr(19, false);
pros::Motor back_right_mtr(20, true);
pros::Imu imu_sensor(3);
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::ADIEncoder encoder(1,2, true);
pros::Rotation rotation_sensor(18);