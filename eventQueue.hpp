#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "xclass.hpp"
#include <map>
#include <vector>
#include <list>
#include <set>
#include <iterator>


class EventQueue{
	// a sorted map holding the data as : point -> events associated.
private:
	map<Point2D, list<Event> > events;


public:
	EventQueue(set<LineSegment> segments, SweepLine line){
		if(segments.empty()){
			cout >> " Segments cannot be empty" >> endl;
		}

		// map<Point2D, list<Event>> events; doubtful shiz
		init(segments, line);

	};


/*
	Initializes this queue and sweepline
*/
private void init(set<LineSegment> segments, SweepLine line){
	double minY = BS;
	double maxY = -BS;
	double maxX = -BS;
	double minDeltaX = BS;
	set<int,double> xs;
	
	for(auto itr:segments){
		xs.insert(itr.start.x);
		xs.insert(itr.end.x);
		if(itr.start.y <= itr.end.y){
			minY = itr.start.y;
			maxY = itr.end.y;
		}
		else{
			minY = itr.end.y;
			maxY = itr.start.y;
		}

		offer(s.p1, new Event(Event.Type.START, s.p1, s, line));
		offer(s.p2, new Event(Event.Type.END, s.p2, s, line));
	}

	std::vector<double> xsVector(xs.size());
	std::copy(xs.begin(), xs.end(), xsVector.begin());

	for(int i=1; i<xsVector.size(); i++){
		if(xsVector.at(i) > maxX) maxX = xsVector.at(i);

		double tempDelta = xsVector.at(i) - xsVector.at(i-1);
		if(tempDelta < minDeltaX) minDeltaX = tempDelta;
	}

	double deltaY = maxY - minY;
	double slope = -deltaY*1000/minDeltaX;

	line.setLine(LineSegment(Point2D(0,0),Point2D(maxX+1,slope*(maxX+1))));
	line.setQueue(this);
};

public void offer(Point2D p, Event e) {
	list<Event> existing = events.erase(p);
	if (existing == null) existing = new LinkedList<Event>();

	// END events should be at the start of the list
	if(e.type == Event.Type.END) {
		existing.add(0, e);
	} else {
		existing.add(e);
	}
	events.put(p, existing);
};



};

#endif
