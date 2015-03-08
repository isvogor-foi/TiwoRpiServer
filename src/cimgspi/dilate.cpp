#include "dilate.h"
#include <string.h>
#include <iostream>

const char* Dilate::getName(){
	return "dilate";
};

void Dilate::set_source_id(const char* source){
	if(strcmp(source,"fifo") == 0) { Dilate::super::source = 0; }
	else if(strcmp(source, "gauss") == 0) { Dilate::super::source = 1; }
	else if(strcmp(source, "sobel") == 0) { Dilate::super::source = 2; }
	else if(strcmp(source, "erode") == 0) { Dilate::super::source = 3; }
	else if(strcmp(source, "hyst") == 0) { Dilate::super::source = 4; }
};

int Dilate::get_source(){
	return Dilate::super::source;
}