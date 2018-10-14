#pragma once

#include "../UJr2.h"

namespace UJr2_funcs {
	namespace kmp {
		extern bool on;
		void interaction();

		class KMP {
		public:
			KMP(const char* pat, const char* tar);

			KMP(const KMP& source) {
				copy(*this, source);
			}

			KMP(KMP&& source) noexcept {
				move(*this, source);
			}

			KMP& operator=(const KMP& source) {
				copy(*this, source);
				return *this;
			}

			KMP& operator=(KMP&& source) noexcept {
				move(*this, source);
				return *this;
			}

			~KMP() {
				destroy();
			}

			void init(const char* pat, const char* tar);
			long long find(long long start) const;
			long long find() const;
			long long find_nth(long long n) const;
		private:
			void Pattern(const char* w);
			void Target(const char* t);
			void MakeNext();
			const char* pattern{nullptr};
			long long pattern_size{0};
			const char* target{nullptr};
			long long target_size{0};
			//加上字符串大小方便访问
			const long long* next{nullptr};

			//以后可以考虑改成const.
			void destroy() {
				delete[] pattern;
				delete[] target;
				delete[] next;
				pattern = nullptr;
				target = nullptr;
				next = nullptr;
				pattern_size = 0;
				target_size = 0;
			}

			static void copy(KMP& destination, const KMP& source) {
				destination.destroy();
				long long* temp_next = new long long[source.pattern_size];
				char* temp_pattern = new char[source.pattern_size];
				char* temp_target = new char[source.target_size];
				destination.pattern_size = source.pattern_size;
				destination.target_size = source.target_size;
				for(long long i = 0; i < source.pattern_size; i++) {
					temp_next[i] = source.next[i];
					temp_pattern[i] = source.pattern[i];
				}
				for(long long i = 0; i < source.target_size; i++) {
					temp_target[i] = source.target[i];
				}
				destination.pattern = temp_pattern;
				destination.target = temp_target;
				destination.next = temp_next;
			}

			static void move(KMP& destination, KMP& source) noexcept {
				if((&destination) == (&source)) {
					return;
				}
				destination.next = source.next;
				destination.pattern = source.pattern;
				destination.pattern_size = source.pattern_size;
				destination.target = source.target;
				destination.target_size = source.target_size;
				source.next = nullptr;
				source.pattern = nullptr;
				source.pattern_size = 0;
				source.target = nullptr;
				source.target_size = 0;

			}
		};

		inline long long KMPMatch(const char* pattern, const char* target) {
			KMP This(pattern, target);
			return This.find();
		}
	}
}
