#include "gauss.h"
#include <string.h>
#include <iostream>

const char* Gauss::getName(){
	return "gauss";
};

void Gauss::set_source_id(const char *source){
	if(strcmp(source,"fifo") == 0) { Gauss::super::source = 0; }
	else if(strcmp(source, "sobel") == 0) { Gauss::super::source = 1; }
	else if(strcmp(source, "erode") == 0) { Gauss::super::source = 2; }
	else if(strcmp(source, "dilate") == 0) { Gauss::super::source = 3; }
	else if(strcmp(source, "hyst") == 0) { Gauss::super::source = 4; }
};

int Gauss::get_source(){
	return Gauss::super::source;
}