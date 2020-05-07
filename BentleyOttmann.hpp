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
    if (segments.size() < 2) {
      set<LineSegment> dumvar;
      dumvar.insert(LineSegment());
      map<Point2D, set<LineSegment>> retval;
      retval[Point2D()] =  dumvar;
      return retval;
    }

    SweepLine sweepLine;
    EventQueue queue = EventQueue(segments,sweepLine);

    while (!queue.isEmpty()) {
      unordered_set<Event,EventHash> events = queue.firstEntry();
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
