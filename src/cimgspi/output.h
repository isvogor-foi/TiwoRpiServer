#ifndef OUTPUT_H
#define OUTPUT_H

#include "filters.h"

class Output : public Filter{
public:
	typedef Filter super;
	virtual const char* getName();
	int source;
	virtual int get_source();
	virtual void set_source_id(const char *source);
};

#endif