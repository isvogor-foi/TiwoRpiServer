#include "output.h"
#include <string.h>
#include <iostream>

const char* Output::getName(){
	return "output";
};

void Output::set_source_id(const char *source){
	if(strcmp(source,"fifo") == 0) { Output::super::source = 0; }
	else if(strcmp(source, "gauss") == 0) { Output::super::source = 1; }
	else if(strcmp(source, "sobel") == 0) { Output::super::source = 2; }
	else if(strcmp(source, "erode") == 0) { Output::super::source = 3; }
	else if(strcmp(source, "dilate") == 0) { Output::super::source = 4; }
	else if(strcmp(source, "hyst") == 0) { Output::super::source = 5; }

};

int Output::get_source(){
	return Output::super::source;
}