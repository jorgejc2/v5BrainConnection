#include "main.h"
#include <pthread.h>
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

// void encode_buffer(uint16_t * buffer, char * string) {
// 	for (int i = 0; i < 256; i++){
// 		buffer[i] = string[i] | uint8_t(i) >> 8;
// 	}
// }

void *fake_funct(void *context) {int i = 0; return 0;};

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// double curr_heading;
	// __fpurge(stdout);

	char in_buffer[256] = "IVRMessage from VEX\n";
	char text_file[13960];
	int start_pos = 0;
	cout<<"Hello code 2"<<endl; // to check if terminal reads outputs
	pros::c::serctl(SERCTL_DISABLE_COBS,NULL); // necessary for PI to communicate to brain via serial

	// pthread_t __listen1;
	// pthread_create(&__listen1, NULL, fake_funct, NULL);
	
	char buffer[256];
	RasppiComms comms = RasppiComms();
	// comms.listen();
	pros::delay(5000);
	// while(start_pos + 256 < 13960) {

	
	int read_count = 0;

	while (1) {
		pros::screen::print(TEXT_MEDIUM, 0, "READ_COUNT %d",read_count);
		comms.read_256_from_buff(buffer);
		read_count++;

		// strncpy(text_file + start_pos, buffer, 256);
		// start_pos += 256;
		
		pros::screen::print(TEXT_MEDIUM, 3, buffer);
		pros::delay(100);
	}
	while(1) {pros::screen::print(TEXT_MEDIUM, 3, "past while loop");}
	// start_pos = 0;
	// while (start_pos + 256 < 13960) {
	// 	pros::screen::print(TEXT_MEDIUM, 3, text_file + start_pos);
	// 	start_pos += 256;
	// 	pros::delay(1000);
	// }
	comms.~RasppiComms();
	/* get rasppi to read a message */
	for (int i = 0; i < 5; i++) {
		fwrite(in_buffer, sizeof(char), 21, stdout);
		cout<<in_buffer<<endl;
	}
	return;



	imu_sensor.reset(true); // true to enable blocking until calibration is done
	// pros::delay(5000);

	// double target_heading = 250; // heading will be from 0 - 360

	// double difference;
	// double kP_turn = 5; //kp turn factor
	// double max_voltage = 127;
	// // double max_velocity_ft = (200.0 / 60.0) * (M_PI*2.75/12); // (rpm/s)*(circumference/feet) = ft/s
	// double max_velocity_ft = 200;
	// double turn_voltage;
	// double turn_speed;

	// int stable_counter = 0;

	// while(1) {
	// 	curr_heading = imu_sensor.get_heading();

	// 	difference = target_heading - curr_heading;

	// 	if (difference > 180)
	// 		difference -= 360;

	// 	turn_speed = (kP_turn * difference)/180;

	// 	pros::lcd::print(4, "IMU curr_heading is: %f\n",curr_heading);
	// 	pros::lcd::print(5, "Difference is: %f\n", difference);
	// 	pros::lcd::print(6, "Current heading: %f\n", curr_heading);
	// 	pros::lcd::print(7, "Target heading: %f\n", target_heading);


	// 	front_right_mtr.move_velocity(-max_velocity_ft * turn_speed);
	// 	back_right_mtr.move_velocity(-max_velocity_ft * turn_speed);
	// 	front_left_mtr.move_velocity(max_velocity_ft * turn_speed);
	// 	back_left_mtr.move_velocity(max_velocity_ft * turn_speed);

	// 	if (fabs(difference) < 5)
	// 		++stable_counter;
	// 	if (stable_counter == 20)
	// 		break;

	// 	pros::delay(20);
	// }

	// int32_t curr_tick = encoder.get_value();
	// int target_tick = 1080;
	// while(curr_tick < target_tick) {
	// 	front_right_mtr.move_velocity(20);
	// 	front_left_mtr.move_velocity(20);
	// 	back_right_mtr.move_velocity(20);
	// 	back_left_mtr.move_velocity(20);

	// 	curr_tick = encoder.get_value();

	// 	pros::lcd::print(4, "Curr_tick is: %d\n", curr_tick);
	// 	pros::lcd::print(5, "Target tick: %d\n", target_tick);

	// 	pros::delay(20);
	// }

	// front_right_mtr.move_velocity(0);
	// front_left_mtr.move_velocity(0);
	// back_right_mtr.move_velocity(0);
	// back_left_mtr.move_velocity(0);

	/* set way points with x, y, v */
	while(1) {
		pros::lcd::print(4, "testing rotation sensor");
	}

	vector<tuple<int, int, int>> waypoints {
		tuple<int, int, int> {0, 0, 20},
		tuple<int, int, int> { -1, 1, 20}, 
		tuple<int, int, int> { -4, 5, 20}, 
		tuple<int, int, int> { -8, 10, 20}, 
		tuple<int, int, int> { -4, 15, 20}, 
		tuple<int, int, int> { 0, 20, 20},
		tuple<int, int, int> { 4, 25, 20}
	};

	int prev_ref_x = get<0>(waypoints[0]);
	int prev_ref_y = get<0>(waypoints[0]);
	int ref_x = get<0>(waypoints[1]);
	int ref_y = get<1>(waypoints[1]);
	int ref_v = get<2>(waypoints[1]);
	double target_orientation = -1 * atan2( ref_y - prev_ref_y , ref_x - prev_ref_x);
	double curr_heading;
	// double ref_vx = ref_v * cos(target_orientation) * (10 / (1000 * 60));
	// double ref_vy = ref_v * sin(target_orientation) * (10 / (1000 * 60));
	int curr_waypoint_idx = 1;
	int curr_x, curr_y;
	int curr_way_x, curr_way_y;
	double curr_v;
	int dist_to_target_x, dist_to_target_y;
	float delx, dely, deltheta, delv;
	float kx, ky, kv, ktheta;

	while(1) {
		curr_x = int(encoder.get_value() / 360);
		curr_y = rotation_sensor.get_position();
		curr_heading = imu_sensor.get_heading() * M_PI / 180;
		curr_v = (pros::c::motor_get_actual_velocity(10) + pros::c::motor_get_actual_velocity(19) + pros::c::motor_get_actual_velocity(20) + pros::c::motor_get_actual_velocity(21)) / 4;
		pros::lcd::print(4, "rotation_sensor pos is: %d\n",curr_y);

		ref_x = get<0>(waypoints[curr_waypoint_idx]);
		ref_y = get<1>(waypoints[curr_waypoint_idx]);
		ref_v = get<2>(waypoints[curr_waypoint_idx]);

		dist_to_target_x = abs(ref_x - curr_x);
		dist_to_target_y = abs(ref_y - curr_y);

		/* update new waypoint to follow if close to current waypoint*/
		if ((dist_to_target_x == 1) && (dist_to_target_y == 1)) {
			if (curr_waypoint_idx == (waypoints.size() - 1))
				break;
			/* update previous reference points */
			prev_ref_x = get<0>(waypoints[curr_waypoint_idx]); 
			prev_ref_y = get<1>(waypoints[curr_waypoint_idx]);
			++curr_waypoint_idx; // target is next waypoint
			/* update current reference points */
			ref_x = get<0>(waypoints[curr_waypoint_idx]); 
			ref_y = get<1>(waypoints[curr_waypoint_idx]);
			ref_v = get<2>(waypoints[curr_waypoint_idx]);
			target_orientation = -1 * atan2( ref_y - prev_ref_y , ref_x - prev_ref_x);
		}

		delx = cos(target_orientation) * (ref_x - curr_x) + sin(target_orientation) * (ref_y - curr_y);
		dely = -1*sin(target_orientation) * (ref_x - curr_x) + cos(target_orientation) * (ref_y - curr_y);
		deltheta = target_orientation - curr_heading;
		delv = ref_v - curr_v;

		kx = 0.1;
		ky = 0.05;
		kv = 0.5;
		ktheta = 2;

		int K_rows = 2;
		int K_cols = 4;
		int del_vec_rows = 1;
		int del_vec_cols = 4;
		float K[8] = { kx, 0, 0, kv, 0, ky, ktheta, 0 };
		float del_vec[4] = { delx, dely, deltheta, delv };
		float u[2];

		/* calculate new inputs controls */
		for (int i = 0; i < 2; i++) {
			u[i] = 0;
			for (int j = 0; j < K_cols; j++) {
				u[i] += K[i * K_cols + j] * del_vec[j];
			}
		}

		/* 
			u[0] is the steering angle 
			u[1] is the new rpm
		*/

		/* send new velocities to robot */
		double degrees = u[0] * 180 / M_PI;

		if (degrees >= 0 && degrees < 180) {
			degrees = (180 - degrees) / 180;
			front_left_mtr.move_velocity(u[1]);
			back_left_mtr.move_velocity(u[1]);
			front_right_mtr.move_velocity(u[1] * degrees);
			back_right_mtr.move_velocity(u[1]*degrees);
		}
		else {
			degrees = (degrees - 180) / 180;
			front_right_mtr.move_velocity(u[1]);
			back_right_mtr.move_velocity(u[1]);
			front_left_mtr.move_velocity(u[1]*degrees);
			back_left_mtr.move_velocity(u[1]*degrees);
		}

		pros::delay(10);

	}

	front_right_mtr.move_velocity(0);
	front_left_mtr.move_velocity(0);
	back_right_mtr.move_velocity(0);
	back_left_mtr.move_velocity(0);

}

void update_movement(int tar_x, int tar_y, double tar_heading, double tar_vel, int curr_x, int curr_y, double curr_heading, double curr_vel) {

return;

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		back_left_mtr = left;
		front_left_mtr = left;
		back_right_mtr = right;
		front_right_mtr = right;
		pros::delay(20);
	}
}
