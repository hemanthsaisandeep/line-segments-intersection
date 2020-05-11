#ifndef BENTLEYOTTMANN_H
#define BENTLEYOTTMANN_H
#include "eveQueSl.hpp"

class BentleyOttmann{
private:
  BentleyOttmann() {
  }

public:
  static map<Point2D, vector<LineSegment>> intxnMap(vector<LineSegment> segments) {
    if (segments.size() < 2) {	// return a mapping from a null point to a null
				// line segment
      vector<LineSegment> dumvar;	// just a dummy variable
      dumvar.push_back(LineSegment());
      map<Point2D, vector<LineSegment>> retval; // value to be returned
      retval[Point2D()] =  dumvar;
      return retval;
    }

    SweepLine sweepLine;
    EventQueue queue = EventQueue(segments,&sweepLine);

    while (!queue.isEmpty()) {
      set<Event> events = queue.firstEntry();
      sweepLine.handle(events);
    }

    return sweepLine.listIntersections();
  }

  static set<Point2D> intersections(vector<LineSegment> segments) {
    map<Point2D,vector<LineSegment>> intersectionsMap = intxnMap(segments);
    map<Point2D,vector<LineSegment>>::iterator itr;
    set<Point2D> retval;
    for (itr = intersectionsMap.begin(); itr != intersectionsMap.end(); itr++)
      retval.insert(itr->first);
    return retval;
  }
};
#endif
