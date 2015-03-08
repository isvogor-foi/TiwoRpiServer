#ifndef HYST_H
#define HYST_H

#include "filters.h"

class Hyst : public Filter{
public:
	typedef Filter super;
	virtual const char* getName();
	int source;
	virtual int get_source();
	virtual void set_source_id(const char *source);
};

#endif