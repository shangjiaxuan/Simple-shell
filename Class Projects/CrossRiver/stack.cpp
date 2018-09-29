#include "stack.h"

using namespace std;

void state::set(int location, bool state){
	if(state){
		data=data|(0x01<<location);
	}
	else{
		data=data&(0xff-(0x01<<location));
	}
}

void state::invert(int location){
	data=data^(0x01<<location);
}

bool state::access(int location){
	return (data&(0x01<<location));
}

void state::print(std::ostream &ost){
	ost << "step " << layer << ":\t\t";
	for(int i=0; i<ITEM_NO; i++){
		ost<<access(i)<<'\t';
	}
	ost<<'\n';
}

bool state::operator==(state st){
	if(this->data==st.data){
		return true;
	}
	return false;
}

bool stack::exist(state state){
	for(int i=0; i<=filled; i++){
		if(data[i]==state){
			return true;
		}
	}
	return false;
}

void stack::pushback(state added){
	filled++;
	data[filled]=added;
}

state stack::popback(){
	if(filled<0){
		throw runtime_error("stack::popback: stack empty!");
	}
	state rtn=data[filled];
	data[filled]=state();
	filled--;
	return rtn;
}

void cross_river::go_back(){
	status=stack.popback();
	cur_layer = status.layer;
}

bool cross_river::is_safe(state state){
	for(int i=1; i<ITEM_NO-1; i++){
		if(state.access(i)!=state.access(0)&&state.access(i+1)!=state.access(0)){
			return false;
		}
	}
	return true;
}

bool cross_river::is_safe(){
	return is_safe(status);
}

bool cross_river::done(){
	if(status.data==0x0f){
		return true;
	}
	return false;
}

//true if there exists "okay" new states
//false if not
bool cross_river::stash(){
	state temp;
	//可以考虑用static减少赋值次数，简化运算，但会提高代码维护难度，就像之前写的
	//车辆管理里的默认编号计算
	bool rtn{ false };
	temp = status;
	cur_layer++;
	temp.layer = cur_layer;
	temp.invert(0);
	if (is_safe(temp) && (!visited.exist(temp))) {
		stack.pushback(temp);
		visited.pushback(temp);
		rtn = true;
	}
	for(int i=1; i<ITEM_NO; i++){
		temp.invert(i);
		if(is_safe(temp)&&(!visited.exist(temp))){
			stack.pushback(temp);
			visited.pushback(temp);
			rtn = true;
		}
		temp = status;
		temp.layer = cur_layer;
		temp.invert(0);
	}
	if(!rtn) {
		cur_layer--;
	}
	return rtn;
}

/*
void cross_river::cross(){
	Label:
	while ((!done())&&stash()) {
		go_back();
	}
	if(done()) {
		return;
	} 
	else {
		go_back();
		goto Label;
	}
}
*/

/*
void cross_river::cross() {
	while(!done()) {
		while((!done()) && stash()) {
			add_record();
			go_back();
		}
		if(!done()) {
			del_record();
			go_back();
			add_record();
		}
	}
}
*/

void cross_river::cross_verbose(std::ostream& ost) {
	cur_layer = 0;
	status.layer = cur_layer;
	add_record();
	visited.pushback(status);
	while (!done()) {
		label:
		ost << "Current state:\n";
		status.print(ost);
		print_stack(ost);
		if(!stash()) {
			ost << "No legal next state found, poping back stack!\n";
			while(true) {
				del_record();
				go_back();
				if(status.layer==traceback.peek().layer+1) {
					ost << "Popped state is a brother of the current state, visiting...\n";
					ost << "Current state:\n";
					status.print(ost);
					add_record();
					print_stack(ost);
					break;
				}
				ost << "Current state:\n";
				status.print(ost);
				ost << "Popped state is the brother of ancestor of the dead state, deleting record again...\n";
				print_stack(ost);
			}
			goto label;
		}
		ost << "Stashed all legal next states.\n";
		print_stack(ost);
		ost << "Popping the last child state.\n";
		go_back();
		add_record();
	}
	ost << "Done!\n";
	print_stack(ost);
}

void cross_river::add_record() {
	if(traceback.exist(status)) {
		return;
	}
	traceback.pushback(status);
}

void cross_river::del_record() {
	traceback.popback();
}

state stack::peek() {
	return data[filled];
}

void stack::print(ostream& ost) {
	if(!(*this)) {
		return;
	}
	for(int i=0; i<=filled; i++) {
		data[i].print(ost);
	}
	ost << endl;
}

void cross_river::print_stack(std::ostream& ost) {
	if(!stack) {
		ost << "The stack is currently empty.\n" << endl;
		return;
	}
	ost << "The following ist the current content of the stack:\n" << endl;
	stack.print(ost);
}

void cross_river::print_path(std::ostream& ost) {
	ost << "The following is the found result for the problem:\n" << endl;
	traceback.print(ost);
}

void cross_river::interaction() {
	cross_verbose(cout);
	print_path(cout);
}

