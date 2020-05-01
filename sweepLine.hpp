#ifndef SWEEPLINE_H
#define SWEEPLINE_H

#include "treeSet.hpp"
#include "event.hpp"

class SweepLine {
private:
  TreeSet<Event> events;
  Map<Point2D, Set<Event>> intersections;
  Line sweepLine;
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
    events = new TreeSet<Event>(); // Yet to be implemented
    intersections = new HashMap<Point2D, Set<Event>>(); // YTBI
    sweepLine = LineSegment(Point2D(),Point2D());
    currentEventPoint = Point2D();
    before = true;
    ignoreSegmentEndings = ignoreEndings;
  };

  Event above(Event e) {
    return events.higher(e);
  };

  Event below(Event e) {
    return events.lower(e);
  };

private:
  void checkIntersection(Event a, Event b) {
    if (a == null || b == null ||
	a.type == Event.Type.INTERSECTION ||
	b.type == Event.Type.INTERSECTION)
      return;

    Point2D p = a.segment.intersects(b.segment);

    if p.invalid() return;

    if (a.segment.hasEnding(p) && b.segment.hasEnding(p) && ignoreSegmentEndings)
      return;

    Set<Event> existing = intersections.remove(p);
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
