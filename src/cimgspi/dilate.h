#ifndef DILATE_H
#define DILATE_H

#include "filters.h"

class Dilate : public Filter{
public:
	typedef Filter super;
	virtual const char* getName();
	int source;
	virtual int get_source();
	virtual void set_source_id(const char* source);

};

#endif