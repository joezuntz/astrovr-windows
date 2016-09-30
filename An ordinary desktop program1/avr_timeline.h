#pragma once
#include "stdafx.h"
#include <vector>

typedef struct timeline_entry {
	int type;
	double timepoint;
	void * data;
} timeline_entry;


// each entry is a type, a time, and some data (type of which depends on entry type)
class AVRTimeline
{
	// entries must be organized
protected:
	std::vector<timeline_entry>  entries;
	int index;
	virtual void callback(timeline_entry &entry, double timepoint) = 0;
	virtual void finished(double timepoint) = 0;
public:
	AVRTimeline();
	void append(int type, double timepoint, void * data);
	void check_for_event(double timepoint);
	void sort();

};
