#include "erode.h"
#include <string.h>
#include <iostream>

const char* Erode::getName(){
	return "erode";
};

void Erode::set_source_id(const char *source){
	if(strcmp(source,"fifo") == 0) { Erode::super::source = 0; }
	else if(strcmp(source, "gauss") == 0) { Erode::super::source = 1; }
	else if(strcmp(source, "sobel") == 0) { Erode::super::source = 2; }
	else if(strcmp(source, "dilate") == 0) { Erode::super::source = 3; }
	else if(strcmp(source, "hyst") == 0) { Erode::super::source = 4; }
};

int Erode::get_source(){
	return Erode::super::source;
}