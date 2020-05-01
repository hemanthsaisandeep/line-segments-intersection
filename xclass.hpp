#ifndef XCLASS_H
#define XCLASS_H

#include <unordered_map>
#include <list>
#include <set>
#include <iterator>
#include <functional>
#include "lineSegment.hpp"

class pointHash {
public:
  // function to return the hash of the point
  size_t operator()(Point2D const & p) const {
    size_t xHash = hash<double>()(p.x);
    size_t yHash = hash<double>()(p.y);
    return ((xHash >= yHash) ? xHash*xHash + xHash + yHash :
	    xHash + yHash*yHash);
  };
};

class SweepLine;

class Event {
public:
  enum Type {START, END, INTERSECTION};
  Type type;
  Point2D point;
  LineSegment segment;
  SweepLine * sweepLine;

protected:
  Event(Type t, Point2D p, LineSegment seg, SweepLine * s) {
    type = t;
    point = p;
    segment = seg;
    sweepLine = s;
  };

public:
  // A function to check if two events are identical
  bool equals (Event that) {
    if ((this->type == INTERSECTION && that.type != INTERSECTION) ||
	(this->type != INTERSECTION && that.type == INTERSECTION))
      return false;
    else if (this->type == INTERSECTION && that.type == INTERSECTION)
      return (this->point.x == that.point.x && this->point.y == that.point.y);
    else
      return this->segment.equals(that.segment);
  };
};

class SweepLine {
private:
  set<Event> * events;
  set<Event>::iterator resultEvent;
  unordered_map<Point2D,set<Event>,pointHash> * intersections;
  // unordered_map<Point2D,set<Event>,pointHash>::iterator resultEvent;
  LineSegment sweepLine;
  Point2D currentEventPoint;
  bool before;

protected:
  EventQueue queue;

private:
  bool ignoreSegmentEndings;

public:
  // constructor
  SweepLine() {
  };

  SweepLine(bool ignoreEndings) {
    events = new set<Event>;
    intersections = new unordered_map<Point2D, set<Event>,pointHash>;
    sweepLine = LineSegment(Point2D(),Point2D());
    currentEventPoint = Point2D();
    before = true;
    ignoreSegmentEndings = ignoreEndings;
  };

  Event above(Event e) {
    resultEvent = events->upper_bound(e);
    return *resultEvent;
  };

  Event below(Event e) {
    resultEvent = events->lower_bound(e);
    return *resultEvent;
  };

private:
  void checkIntersection(Event a, Event b) {
    if (a.type == Event::INTERSECTION ||
	b.type == Event::INTERSECTION)
      return;

    Point2D p = a.segment.intersects(b.segment);

    if p.invalid() return;

    if (a.segment.atEnding(p) && b.segment.atEnding(p) && ignoreSegmentEndings)
      return;

    set<Event> existing = intersections->erase(p);
    if (existing == null) existing = new HashSet<Event>();
    existing.add(a);
    existing.add(b);
    intersections.put(p,existing);

    if (sweepLine.locRight(p) || (sweepLine.pointContained(p) &&
				  currentEventPoint.y)) {
      Event intersection = new Event(Event.Type.INTERSECTION, p, null, this);
      queue.offer(p, intersection);
    };
};
#endif
