#pragma once

#define EXEC

#include<stdio.h>
#include<tchar.h>
#include<Windows.h>
#include<iostream>
#include <vector>

bool start;
int arg_number;	//for incorporating multiple use arguments
void prompt();	//for working with prompts
void after_start_selector(std::vector<std::string>);	//select the function to call if no original argument is found

void manual();

void Calledcalculator(std::istream& ist);
