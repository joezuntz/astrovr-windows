#include "avr_oculus_timeline.h"
#include <vector>
#include "avr_oculus.hh"

#define TIMELINE_TRANSLATION 1
#define TIMELINE_ROTATION 2
#define TIMELINE_TRANSITION 3

typedef struct transition_data{
	int targetView;
	
} transition_data;


typedef struct translation_data{
	double x;
	double y;
	double z;

} translation_data;

typedef struct rotation_data{
	double theta;
	double phi;
} rotation_data;

class OculusEvent {
public:
	double time;
	OculusEvent(double t) : time(t) {};
	virtual int type()=0;

};

class OculusTranslation: public OculusEvent {
public:
	double x, y, z;
	OculusTranslation(double t, double x_, double y_, double z_) : OculusEvent(t), x(x_), y(y_), z(z_) {};
	virtual int type() { return TIMELINE_TRANSLATION; };
};

class OculusRotation: public OculusEvent {
public:
	double lon, lat;
	OculusRotation(double t, double lo, double la) : OculusEvent(t), lon(lo/180.0*M_PI), lat(la / 180.0*M_PI) {};
	virtual int type() { return TIMELINE_ROTATION; };


};

class OculusTransition: public OculusEvent {
public:
	int target;
	OculusTransition(double t, int targ) : OculusEvent(t), target(targ) {};
	virtual int type() { return TIMELINE_TRANSITION; };

};



AVROculusTimeline * createLongTourTimeline(AVROculus * oculus) {
	std::vector<OculusEvent*> events;
	
	// This is the full timeline 

	events.push_back(new OculusTransition(76.0, 1));
	events.push_back(new OculusTranslation(156.0, 1.0f, 0.0f, 0.0f));
	events.push_back(new OculusTranslation(174.0, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusTransition(174.5, 2));
	events.push_back(new OculusRotation(202.0, -96.35, -2.77));
	events.push_back(new OculusTranslation(202.5, 0.0f, 0.0f, 1.0f));
	events.push_back(new OculusTranslation(229.0, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusRotation(229.5, 0.00, 0.00));
	events.push_back(new OculusTransition(229.6, 3));
	events.push_back(new OculusRotation(249.0, -100.31, -283.00));
	events.push_back(new OculusTranslation(249.5, 0.0f, 0.0f, 1.0f));
	events.push_back(new OculusRotation(253.0, -155.94, -16.77));
	events.push_back(new OculusTranslation(253.5, -1.0f, -0.5f, 1.0f));
	events.push_back(new OculusRotation(271.0, -71.21, -37.42));
	events.push_back(new OculusTranslation(271.5, 0.6f, -0.6f, 1.1f));
	events.push_back(new OculusTranslation(285.0, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusRotation(285.5, 0.00, 0.00));
	events.push_back(new OculusRotation(287.0, 179.98, -0.02));
	events.push_back(new OculusTranslation(287.5, -1.0f, 0.0f, 0.0f));
	events.push_back(new OculusTranslation(295.5, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusRotation(295.6, 0.00, 0.00));
	events.push_back(new OculusTransition(296.0, 4));
	events.push_back(new OculusTransition(342.0, 5));
	events.push_back(new OculusTransition(390.0, 6));
	events.push_back(new OculusTransition(426.0, 7));
	events.push_back(new OculusRotation(456.0,-50.49,19.35));
	events.push_back(new OculusTranslation(456.5, 1.1f, 0.7f, 1.0f));
	events.push_back(new OculusRotation(466.0, -155.94, -16.77));
	events.push_back(new OculusTranslation(466.5, -1.0f, -0.5f, 1.0f));
	events.push_back(new OculusTranslation(469.0, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusRotation(469.5, 0.00, 0.00));
	events.push_back(new OculusTransition(501.0, 8));
	events.push_back(new OculusRotation(537.0, -50.49, 19.35));
	events.push_back(new OculusTranslation(537.5, 1.1f, 0.7f, 1.0f));
	events.push_back(new OculusTranslation(562.0, 0.0f, 0.0f, 0.0f));
	events.push_back(new OculusRotation(562.3, 0.00, 0.00));
	events.push_back(new OculusTransition(562.6, 3));

	AVROculusTimeline * timeline = new AVROculusTimeline(oculus);

	for (auto & event : events) {
		timeline->append(event->type(), event->time, event);
	}

	return timeline;


}



void AVROculusTimeline::callback(timeline_entry &entry, double timepoint) {
	if (entry.type == TIMELINE_TRANSITION) {
		OculusTransition * transition = (OculusTransition*) entry.data;
		oculus->transitionTo(transition->target);
	}
	else if (entry.type == TIMELINE_TRANSLATION) {
		OculusTranslation * translation = (OculusTranslation *) entry.data;
		oculus->translateTo(translation->x, translation->y, translation->z);
	}
	else if (entry.type == TIMELINE_ROTATION) {
		OculusRotation * rotation = (OculusRotation *) entry.data;
		oculus->rotateTo(rotation->lat, rotation->lon);
	}
}


void AVROculusTimeline::finished(double timepoint){
	oculus->finishTour();
}