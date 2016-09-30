#include "avr_timeline.h"
#include <algorithm>

AVRTimeline::AVRTimeline() : index(0) {

}

static bool cmp_timeline_entry(timeline_entry &a, timeline_entry &b)
{
	return a.timepoint < b.timepoint;
}

void AVRTimeline::append(int type, double timepoint, void * data) {
	timeline_entry t;
	t.type = type;
	t.timepoint = timepoint;
	t.data = data;
	entries.push_back(t);
}

void AVRTimeline::sort() {
	std::sort(entries.begin(), entries.end(), cmp_timeline_entry);
}

void AVRTimeline::check_for_event(double timepoint) {
	if (index >= entries.size()) {
		finished(timepoint);
		return;
	}

	timeline_entry &entry = entries.at(index);

	if (entry.timepoint<timepoint) {
		index++;
		callback(entry, timepoint);
	}

}
