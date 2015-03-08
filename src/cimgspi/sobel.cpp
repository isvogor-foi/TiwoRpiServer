#include "sobel.h"
#include <string.h>
#include <iostream>

const char* Sobel::getName(){
	return "sobel";
};

void Sobel::set_source_id(const char *source){
	if(strcmp(source,"fifo") == 0) { Sobel::super::source = 0; }
	else if(strcmp(source, "gauss") == 0) { Sobel::super::source = 1; }
	else if(strcmp(source, "erode") == 0) { Sobel::super::source = 2; }
	else if(strcmp(source, "dilate") == 0) { Sobel::super::source = 3; }
	else if(strcmp(source, "hyst") == 0) { Sobel::super::source = 4; }
};

int Sobel::get_source(){
	return Sobel::super::source;
}