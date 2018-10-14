#pragma once

#include "../UJr2.h"

namespace UJr2_funcs {
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
				if(next) {
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
				/*				Loading save directly in a global variable's
				*				constructor means that the load (e.g. creat empty file)
				*				will always be executed on load of the exe of dll
				*				will decrease performance
								try {
									load();
								}
				
								catch (...) {}
				*/
			}

			~controls() {
				delete[] all_cars;
				all_cars = nullptr;
			}

			void interaction();
			bool exit = false;

			void save() const;
			void load();
		private:
			car* all_cars;
			int car_num{0};

			void list() const;

			const char* save_filename{"VCtrl_save.dat"};

			void master_add();
			void master_find() const;
			void master_remove();

			void add_car();
			bool file_add_car(const std::string& cmd);
			bool manual_add_car(const std::string& cmd);

			void add_violation();
			bool file_add_violation(const std::string& cmd);
			bool manual_add_violation(const std::string& cmd);

			void remove_car(int id);
			void remove_violation(int code);

			void find_car() const;
			void find_car_id(int id) const;

			void find_violation() const;
			void find_violation_code(int code) const;
			void find_violation_time(int time) const;
			void find_violation_location(const std::string& location) const;
			static int find_violation_find_rel(const int list[], int code, int range);

			struct found_violation {
				violation* entry;
				int location_in_all_cars;
			};

			int core_find_index_in_all_cars(int id) const;
			std::vector<found_violation> core_find_violation(int code) const;
			std::vector<found_violation> core_find_violation_time(int time) const;
			std::vector<found_violation> core_find_violation_loaction(const std::string& location) const;
		};
	}
}
