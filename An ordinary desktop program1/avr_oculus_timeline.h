#pragma once
#include "stdafx.h"

# include "avr_timeline.h"
class AVROculus;

// each entry is a type, a time, and some data (type of which depends on entry type)
class AVROculusTimeline : public AVRTimeline
{
	virtual void callback(timeline_entry &entry, double timepoint);
	virtual void finished(double timepoint);
public:
	AVROculusTimeline(AVROculus * oc) : oculus(oc) {};
	AVROculus * oculus;

};


AVROculusTimeline * createLongTourTimeline(AVROculus * oculus);

