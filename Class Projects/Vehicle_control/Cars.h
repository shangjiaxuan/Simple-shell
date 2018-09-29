#pragma once

#include "Header.h"

#define MAX_CAR 100

namespace vehicle_control {

	struct violation {
		violation() {
			time = 0;
			code = -1;
			dealt = false;
			next = nullptr;
		}
		~violation() {
			if (next) {
				delete next;
			}
			next = nullptr;
		}
		int time;
		std::string location;
		int code;
		bool dealt;
		violation* next;
	};

	struct car {
		car() {
			violations = nullptr;
			id = -1;
			price = -1;
		}
		car(int i, std::string typ, double pric, std::string drive) {
			id = i;
			type = typ;
			price = pric;
			driver = drive;
			violations = nullptr;
		}
		~car() {
			delete violations;
			violations = nullptr;
		}
		int id;
		std::string type;
		double price;
		std::string driver;
		violation* violations;
	};

	class controls {
	public:
		controls() {
			car_num = 0;
			all_cars = new car[MAX_CAR];
			try {
				load();
			}catch(...){}
		}
		~controls() {
			delete[] all_cars;
			all_cars = nullptr;
		}
		void interaction();
	private:
		car* all_cars;
		int car_num{ 0 };

		void list();

		void save();
		void load();

		void master_add();
		void master_find();
		void master_remove();

		void add_car();
		bool file_add_car(std::string cmd);
		bool manual_add_car(std::string cmd);

		void add_violation();
		bool file_add_violation(std::string cmd);
		bool manual_add_violation(std::string cmd);

		void remove_car(int id);
		void remove_violation(int code);

		void find_car();
		void find_car_id(int id);

		void find_violation();
		void find_violation_code(int code);
		void find_violation_time(int time);
		void find_violation_location(std::string location);
		int find_violation_find_rel(int list[], int code, int range);

		struct found_violation {
			violation* entry;
			int location_in_all_cars;
		};

		int core_find_index_in_all_cars(int id);
		std::vector<found_violation> core_find_violation(int code);
		std::vector<found_violation> core_find_violation_time(int time);
		std::vector<found_violation> core_find_violation_loaction(std::string location);
	};
}

