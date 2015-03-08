
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include "sobel.h"
#include "gauss.h"
#include "dilate.h"
#include "erode.h"
#include "hyst.h"
#include "output.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include <linux/ioctl.h>

#include "jpeg_func.h"
#include "logilib.h"

#include "/home/ivan/Dev/java/workspace/JNITest/src/Hello.h"

using namespace std;

#define REG_ADDR 0x0800
#define FIFO_CMD_ADDR 0x0200

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

#define LINE_BURST 1
#define FILTER_SIZE 6


// prototype methods
int *filtering_chain_builder(list<Filter*> input_filter);
void print_array(int *array, int size);


JNIEXPORT void JNICALL Java_Hello_sayHi(JNIEnv *env, jobject obj, jstring who, jint times) { 
	jint i; 
	jboolean iscopy; 
	const char *name; 
	name = env->GetStringUTFChars(who, &iscopy); 
	for (i = 0; i < times; i++) {
		cout<<name<<endl;
	}
}

// main file
int main(int argc, char* argv[]){

	cout<<"logi"<<endl;

	if(argc <= 2){
		cout<<"Missing parameters. Please use the following input notation: "<<endl;
		cout<<"<appname> <image.jpg> <sobel, gauss, erode, dilate, hyst> <sobel, ...>"<<endl;
		return 0;
	}

	// get the input filter from the user
	list<Filter*> input_filter;

	for(int i = 0; i < argc - 2; i++){
		if(strcmp(argv[i + 2], "sobel") == 0)
			input_filter.push_back(new Sobel());
		else if (strcmp(argv[i + 2], "gauss") == 0)
			input_filter.push_back(new Gauss());
		else if (strcmp(argv[i + 2], "erode") == 0)
			input_filter.push_back(new Erode());
		else if (strcmp(argv[i + 2], "dilate") == 0)
			input_filter.push_back(new Dilate());
		else if (strcmp(argv[i + 2], "hyst") == 0)
			input_filter.push_back(new Hyst());
	}

	// create the filtering chain
	int *buffer = filtering_chain_builder(input_filter);
	print_array(buffer, FILTER_SIZE);

	return 0;
}


void place(list<Filter*> filtering_chain, Filter *new_filter, const char *source){
	list<Filter*>::iterator current = filtering_chain.begin();
	for(; current != filtering_chain.end(); current++){
		if(strcmp((*current)->getName(), new_filter->getName()) == 0){
			(*current)->set_source_id(source);
		}
	}
}

int *filtering_chain_builder(list<Filter*> input_filter){

	static int buffer[FILTER_SIZE] = {0};
	list<Filter*> filtering_chain;

	// build the filtering chain
	filtering_chain.push_back(new Gauss());
	filtering_chain.push_back(new Sobel());
	filtering_chain.push_back(new Erode());
	filtering_chain.push_back(new Dilate());
	filtering_chain.push_back(new Hyst());
	filtering_chain.push_back(new Output());

	list<Filter*>::iterator current = input_filter.begin();
	list<Filter*>::iterator previous = current++;


	// set initial to fifo
	place(filtering_chain, *previous, "fifo");

	for(current; current != input_filter.end(); current++, previous++){
		place(filtering_chain, *current, (*previous)->getName());
	}
	//set final to output
	place(filtering_chain, new Output(), (*previous)->getName());

	// print result
	current = filtering_chain.begin();
	int j = 0;
	for(current; current != filtering_chain.end(); current++){
		buffer[j++] = (*current)->get_source();
	}
	
	// build array
	return buffer;
}

void print_array(int *array, int size){
	for(int i = 0; i < size; i++){
		cout<<i<<". Element: "<<array[i]<<endl;
	}
}