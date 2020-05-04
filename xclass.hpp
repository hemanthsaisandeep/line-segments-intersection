#ifndef XCLASS_H
#define XCLASS_H

#include <unordered_map>
#include <list>
#include <set>
#include <unordered_set>
#include <iterator>
#include <functional>
#include "lineSegment.hpp"

class PointHash {
public:
  // function to return the hash of a point
  size_t operator()(Point2D const & p) const {
    size_t xHash = hash<double>()(p.x);
    size_t yHash = hash<double>()(p.y);
    return ((xHash >= yHash) ? xHash*xHash + xHash + yHash :
	    xHash + yHash*yHash); // Szudzik's pairing function
  }
};

class SweepLine;

///////////////////////////////////////
// Defintion of `Event' class starts //
///////////////////////////////////////
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
  }

private:
  // A function to check if two events are identical
  bool equals (Event that) {
    if ((this->type == INTERSECTION && that.type != INTERSECTION) ||
	(this->type != INTERSECTION && that.type == INTERSECTION))
      return false;
    else if (this->type == INTERSECTION && that.type == INTERSECTION)
      return (this->point.x == that.point.x && this->point.y == that.point.y);
    else
      return this->segment.equals(that.segment);
  }

public:
  // Operator overloading
  bool operator==(const Event & e) {
    return this->equals(e);
  }
};
//////////////////////////////////////
// Definition of `Event' class ends //
//////////////////////////////////////

class EventHash{
public:
  // function to return the hash of an event
  size_t operator()(Event const & e) const {
    size_t pHash = hash<double>()(e.point.distanceApprox(Point2D(0,0)));
    size_t segHash = hash<double>()
      (e.segment.start.distanceApprox(e.segment.end));
    return ((pHash >= segHash) ? pHash*pHash + pHash + segHash :
	    pHash + segHash*segHash); // Szudzik's pairing function
  }
};

class EventQueue;

class SweepLine {
private:
  set<Event> * events;
  set<Event>::iterator resultEvent;
  unordered_map<Point2D,set<Event>,PointHash> * intersections;
  unordered_map<Point2D,set<Event>,PointHash>::iterator unMapIt;
  LineSegment sweepLine;
  Point2D currentEventPoint;
  bool before;

protected:
  EventQueue * queue;

private:
  bool ignoreSegmentEndings;

public:
  // constructor
  SweepLine() {
  }

  SweepLine(bool ignoreEndings) {
    events = new set<Event>;
    intersections = new unordered_map<Point2D, set<Event>,PointHash>;
    sweepLine = LineSegment(Point2D(),Point2D());
    currentEventPoint = Point2D();
    before = true;
    ignoreSegmentEndings = ignoreEndings;
  }

  Event above(Event e) {
    resultEvent = events->upper_bound(e);
    return *resultEvent;
  }

  Event below(Event e) {
    resultEvent = events->lower_bound(e);
    return *resultEvent;
  }

private:
  void checkIntersection(Event a, Event b) {
    if (a.type == Event::INTERSECTION ||
	b.type == Event::INTERSECTION)
      return;

    Point2D p = a.segment.intersects(b.segment);

    if p.invalid() return;

    if (a.segment.atEnding(p) && b.segment.atEnding(p) && ignoreSegmentEndings)
      return;

    unMapIt = intersections->find(p);
    set<Event> existing = intersections->at(unMapIt);
    intersections->erase (p);
    // if (existing == null) existing = new unordered_set<Event>;
    existing.insert(a);
    existing.insert(b);
    pair<Point2D,set<Event>>
    intersections->insert(p,existing);

    if (sweepLine.locRight(p) || (sweepLine.pointContained(p) &&
				  currentEventPoint.y)) {
      Event intersection = Event(Event::INTERSECTION,p,LineSegment(),this);
      queue->offer(p, intersection);
    }
  }
};
#endif
