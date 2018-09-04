#pragma once

#include "Header.h"
#include "Platform.h"

#include "Host.h"
#include "Filemani.h"

std::vector<nstring> Get_input();
nstring parse_input();

void after_start_selector(std::vector<nstring>);			//select the function to call if no original argument is found
void manual();
void Launch(const nstring& str);
