#pragma once

#include "Array_2D.h"

#ifndef CROSSING
#define EXT extern
#endif
#ifdef CROSSING
#define EXT
#endif

class Colors;

namespace Crossing {

	class Roads;
	class relations;

	EXT short number;
	EXT bool* in;
	EXT bool* out;
	EXT short two_ways;
	EXT short no_in;
	EXT short no_out;
	EXT int possi;

	class Roads {
	protected:
		struct route {
			short from;					//实际的来路
			short to;					//实际的去向
//			int offset;					//这个走法在二维数组中的对应的编号
		};
		friend relations;
	private:
		void get_status();
		void init(route*& rt);
	public:
//		Roads(short num);
		route* routes;
		Roads();
		~Roads();
	};

	class relations :	public Array_2D<bool>
//						public Roads
	{
	public:
		relations(Roads& rd) :Array_2D<bool>(possi, possi) { This = &rd; }
//		virtual ~relations() = default;
		const Roads* This;
		void find_relations();
		void print_rel();
//		friend Colors;
	protected:
		bool compatible(Roads::route& rt1, Roads::route& rt2);
		char rela(Roads::route rt, short pos);
		short right_dist(short s1, short s2);
		short left_dist(short s1, short s2);
	};
}


