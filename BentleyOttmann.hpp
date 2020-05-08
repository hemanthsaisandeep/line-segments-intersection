#ifndef BENTLEYOTTMANN_H
#define BENTLEYOTTMANN_H
#include "eveQueSl.hpp"

class BentleyOttmann{
private:
  BentleyOttmann() {
  }

public:
  static map<Point2D, set<LineSegment>> intxnMap(set<LineSegment> segments) {
    // cout << segments.size() << endl;
    if (segments.size() < 2) {	// return a mapping from a null point to a null
				// line segment
      set<LineSegment> dumvar;	// just a dummy variable
      dumvar.insert(LineSegment());
      map<Point2D, set<LineSegment>> retval; // value to be returned
      retval[Point2D()] =  dumvar;
      return retval;
    }

    SweepLine sweepLine;
    EventQueue queue = EventQueue(segments,sweepLine);

    while (!queue.isEmpty()) {
      set<Event> events = queue.firstEntry();
      sweepLine.handle(events);
    }

    return sweepLine.listIntersections();
  }

  static set<Point2D> intersections(set<LineSegment> segments) {
    map<Point2D,set<LineSegment>> intersectionsMap = intxnMap(segments);
    map<Point2D,set<LineSegment>>::iterator itr;
    set<Point2D> retval;
    for (itr = intersectionsMap.begin(); itr != intersectionsMap.end(); itr++)
      retval.insert(itr->first);
    return retval;
  }
};
#endif
