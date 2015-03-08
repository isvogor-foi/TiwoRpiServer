#include "hyst.h"
#include <string.h>
#include <iostream>

const char* Hyst::getName(){
	return "hyst";
};

void Hyst::set_source_id(const char *source){
	if(strcmp(source,"fifo") == 0) { Hyst::super::source = 0; }
	else if(strcmp(source, "gauss") == 0) { Hyst::super::source = 1; }
	else if(strcmp(source, "sobel") == 0) { Hyst::super::source = 2; }
	else if(strcmp(source, "erode") == 0) { Hyst::super::source = 3; }
	else if(strcmp(source, "dilate") == 0) { Hyst::super::source = 4; }

};

int Hyst::get_source(){
	return Hyst::super::source;
}