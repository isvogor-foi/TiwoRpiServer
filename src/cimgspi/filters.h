#ifndef FILTERS_H
#define FILTERS_H

//gauss
//sobel
//erode
//dilate
//hyst
//fifo

class Filter {
public:
	Filter(void){};
	~Filter(void){};
	virtual const char* getName(){};
	int source;
	virtual int get_source(){};
	virtual void set_source_id(const char *source){};

};

#endif