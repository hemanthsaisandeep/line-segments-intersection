#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "point.hpp"
#include "lineSegment.hpp"
#include "event.hpp"
#include "sweepLine.hpp"
#include <map>
#include <list>
#include <set>
#include <iterator>


class EventQueue{
	// a sorted map holding the data as : point -> events associated.
private:
	map<Point2D, list<Event> > Events;


public:
	EventQueue(set<int, LineSegment> segments, SweepLine line){

	};





};

#endif