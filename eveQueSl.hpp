#ifndef EVEQUESL_H
#define EVEQUESL_H

#include <vector>
#include <stack>
#include <unordered_set>
#include <set>
#include <map>
#include "lineSegment.hpp"

class Event {
public:
  enum Type {START, END, INTERSECTION};
  Type type;
  Point2D point;
  LineSegment segment;
  void * ptrSweepLine;		// Instantiated later as sweepline

  // Forward declaration of constructors
  Event();
  Event(Type t, Point2D p, LineSegment seg, void * ptrSl);

  // A function to check if two events are identical
  bool equals (Event that) const {
    if ((this->type == INTERSECTION && that.type != INTERSECTION) ||
	(this->type != INTERSECTION && that.type == INTERSECTION))
      return false;
    else if (this->type == INTERSECTION && that.type == INTERSECTION)
      return (this->point.x == that.point.x && this->point.y == that.point.y);
    else
      return this->segment.equals(that.segment);
  }

  // Forward declaration of a function to compare `this' with another event
  int compare(Event that) const;

  // Operator overloading
  bool operator==(const Event & e) const{
    if (compare(e) != 0)
      return false;
    return true;
  }

  bool operator<(const Event & e) const{
    if (compare(e) == 1)
      return false;
    return true;
  }
};

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

class SweepLine {
private:
  set<Event> events;
  set<Event>::iterator resultEvent;
  map<Point2D,set<Event>> intersections;
  map<Point2D,set<Event>>::iterator unMapIt;
  LineSegment sweep;
  Point2D currentEventPoint;
  bool before;
  double yMin, yMax;

protected:
  void * ptrEventQueue;

private:
  bool ignoreSegmentEndings;

public:
  // constructor
  SweepLine();
  SweepLine(bool ignoreEndings,double yLb, double yUb);

  // Returns the event that is immediately above the current event
  Event above(Event e) {
    // cout << __LINE__ << endl;
    resultEvent = events.upper_bound(e);
    return *resultEvent;
  }

  // Returns the event that is immediately below the current event
  Event below(Event e) {
    // cout << __LINE__ << endl;
    resultEvent = events.lower_bound(e);
    return *resultEvent;
  }

  // Returns a boolean informing if we are just before the sweepline
  bool isBefore() {
    // cout << __LINE__ << endl;
    return before;
  }

  // Returns the intersection point of the segment corresponding to
  // a possible event and the current sweepline
  // if the current event is not of INTERSECTION type
  Point2D intersects(const Event * e);

  // Returns if there are any intersections yet
  bool hasIntersections(){
    // cout << __LINE__ << endl;
    return !(this->intersections.empty());
  }

  // Returns an map of the segments
  map<Point2D,set<LineSegment>> listIntersections() {
    // cout << __LINE__ << endl;
    map<Point2D,set<LineSegment>> segments;
    map<Point2D,set<Event>>::iterator iterIntxn;
    for (iterIntxn = this->intersections.begin();
	 iterIntxn != this->intersections.end(); iterIntxn++) {
      set<Event>::iterator iterEve;
      set<LineSegment> intxnEventSet;
      for (iterEve = iterIntxn->second.begin();
	   iterEve != iterIntxn->second.end(); iterEve++)
	intxnEventSet.insert(iterEve->segment);
      pair<Point2D,set<LineSegment>> dumvar(iterIntxn->first,intxnEventSet);
      segments.insert(dumvar);
    }
    return segments;
  }

  void setLine(LineSegment line) {
    // cout << __LINE__ << endl;
    this->sweep = line;
    return;
  }

  void setQueue(void * q);

private:
  // Checks the intersection between two events
  void checkIntersection(Event a, Event b);

  // Handle an individual event based on its type
  void handle(Event e) {
    // cout << __LINE__ << endl;
    switch(e.type) {
    case Event::START:
      before = false;
      events.insert(e);
      checkIntersection(e,above(e));
      checkIntersection(e,below(e));
      break;
    case Event::END:
      before = true;
      events.erase(e);
      checkIntersection(above(e),below(e));
      break;
    case Event::INTERSECTION:
      before = true;
      set<Event> eveSet = intersections.at(e.point);
      set<Event>::iterator iterEve;
      stack<Event> toInsert;
      for (iterEve = eveSet.begin(); iterEve != eveSet.end(); iterEve++) {
	if (events.erase(e))
	  toInsert.push(e);
      }
      before = false;
      // Insert all events that we are able to remove
      while (!(toInsert.empty())) {
	// cout << __LINE__ << endl;
	Event eve = toInsert.top();
	toInsert.pop();
	events.insert(eve);
	checkIntersection(eve,above(eve));
	checkIntersection(eve,below(eve));
      }
      break;
    }
    return;
  }

  void sweepTo(Event e) {
    // cout << __LINE__ << endl;
    currentEventPoint = e.point;
    sweep = LineSegment(currentEventPoint,sweep.slope,yMin,yMax);
    return;
  }

public:
  // Handles a set of events
  void handle(unordered_set<Event,EventHash> events) {
    // cout <<events.size() << endl;
    if (events.empty())
      return;
    vector<Event> eveArray(events.begin(),events.end());
    sweepTo(eveArray[0]);

    // There is a possibility that events in the provided set might intersect
    // with each other if we are not to ignore the segment endings
    if (!ignoreSegmentEndings && events.size() > 1)
      for (size_t i = 0; i<eveArray.size()-1; i++)
	for (size_t j = i+1; j<eveArray.size(); j++)
	  this->checkIntersection(eveArray[i],eveArray[j]);

    // Handle all events in the set of events using the private handle function
    unordered_set<Event,EventHash>::iterator it;
    for (it = events.begin(); it != events.end(); it++) {
      // cout << __LINE__ << endl;
      handle(*it);
    }
    return;
  }
};

class EventQueue {
private:
  map<Point2D, unordered_set<Event,EventHash>> events;

public:
  // constructor
  EventQueue() {
  }

  EventQueue(set<LineSegment> segments, SweepLine line) {
    // cout << __LINE__ << endl;
    if (segments.empty())
      cout << "'segments' cannot be empty" << endl;
    init(segments,&line);
  }

  void offer(Point2D p, Event e) {
    // cout << __LINE__ << endl;
    unordered_set<Event,EventHash> existing;
    if (events.find(p) != events.end()) {
      unordered_set<Event,EventHash> existing = events.at(p);
      events.erase(p);
    }

    // END events should be at the start of the set
    if (e.type == Event::END)
      existing.insert(existing.begin(),e);

    // Other events should be at the end of the set
    else
      existing.insert(existing.end(),e);
    pair<Point2D,unordered_set<Event,EventHash>> dumvar(p,existing);
    events.insert(dumvar);
    return;
  }

  bool isEmpty () {
    return events.empty();
  }

  unordered_set<Event,EventHash> firstEntry() {
    // cout << __LINE__ << endl;
    if (this->isEmpty())
      cout << "Queue is empty" << endl;
    map<Point2D,unordered_set<Event,EventHash>>::iterator it;
    it = events.begin();
    unordered_set<Event,EventHash> retval = it->second;
    events.erase(it);
    return retval;
  }

private:
  void init(set<LineSegment> segments, SweepLine * ptrSwLi){
    // cout << __LINE__ << endl;
    double minY = BS;
    double maxY = -BS;
    double maxX = -BS;
    double minDeltaX = BS;
    set<double> xs;
    set<LineSegment>::iterator itr;
    for(itr = segments.begin(); itr != segments.end(); itr++){
      xs.insert(itr->start.x);
      xs.insert(itr->end.x);
      if(itr->start.y <= itr->end.y){
	minY = itr->start.y;
	maxY = itr->end.y;
      }
      else{
	minY = itr->end.y;
	maxY = itr->start.y;
      }

      offer(itr->start, Event(Event::START, itr->start, *itr, ptrSwLi));
      offer(itr->end, Event(Event::END, itr->end, *itr, ptrSwLi));
    }

    vector<double> xsVector(xs.size());
    std::copy(xs.begin(), xs.end(), xsVector.begin());

    for(size_t i=1; i<xsVector.size(); i++){
      if(xsVector.at(i) > maxX) maxX = xsVector.at(i);

      double tempDelta = xsVector.at(i) - xsVector.at(i-1);
      if(tempDelta < minDeltaX) minDeltaX = tempDelta;
    }

    double deltaY = maxY - minY;
    double slope = -deltaY*1000/minDeltaX;

    SweepLine * ptrSweepLine = static_cast<SweepLine*>(ptrSwLi);
    ptrSweepLine->
      setLine(LineSegment(Point2D(0,0), slope,  minY-1, maxY+1));
    ptrSweepLine->setQueue(this);
    return;
  }
};

Event::Event() {
  ptrSweepLine = new SweepLine;
};

Event::Event(Type t, Point2D p, LineSegment seg, void * ptrSl) {
  type = t;
  point = p;
  segment = seg;
  ptrSweepLine = ptrSl;
}

/* A function to compare two events by defining the following order
   1. Return 0, if the events are identical.
   2. Return 1, if any one of the following criterion is satisfied
   (a) Get the intersection points of `this' and 'that' with the sweep line,
   and compare the ordinates of those intersections. The ordinate of
   `this' intersection point should lie above that of `that'.
   (b) If the ordinates of intersection points are equal, compare the slopes.
   (1) They have different slopes and we are before the sweep line,
   `this' should have lesser slope.
   (2) They have different slopes and we are after the sweep line,
   `that' should have lesser slope.
   (3) If they have same slopes too,
   (a) Then the abscissa of start point of `this' should come
   after the abscissa of start point of `that'.
   (b) If their start point abscissae are also same,
   then the end point abscissa of `this' should come later.
   3. Return -1, otherwise
*/
Event::compare (Event that) const {
  // Same events
  if (this->equals(that))
    return 0;

  SweepLine *ptrSwLi = static_cast<SweepLine*>(ptrSweepLine);
  SweepLine swLi = *ptrSwLi;

  // Get intersection points of both `this' and `that' with sweep line
  Point2D inPtThis = swLi.intersects(this);
  Point2D inPtThat = swLi.intersects(&that);

  // Obtain the difference in ordinates between the Events' locations as they
  // pass through the sweep line
  double deltaY = inPtThis.y - inPtThat.y;

  if (!deltaY)
    return (deltaY>0) ? 1 : -1;
  else {
    if (this->segment.slope != that.segment.slope)
      {
	if (swLi.isBefore())
	  return (this->segment.slope>that.segment.slope) ? -1 : 1;
	else
	  return (this->segment.slope>that.segment.slope) ? 1 : -1;
      };

    // `this' and `that' have same slopes
    // and are passing through the same point in the sweep line,
    // then differentiate them using their left end (start) points.
    // Return if `that' start is encountered before.
    double deltaXStart = this->segment.start.x - that.segment.start.x;
    if (!deltaXStart)
      return (deltaXStart<0) ? -1 : 1;

    // Say their start points are the same, then compare their right end points
    return (this->segment.end.x - that.segment.end.x<0) ? -1 : 1;
  }
}

SweepLine::SweepLine() {
  // cout << __LINE__ << endl;
  before = true;
  ignoreSegmentEndings = false;
  ptrEventQueue = new EventQueue;
  sweep = LineSegment(Point2D(-BS,BS),Point2D(BS,BS));
  currentEventPoint = Point2D();
  yMin = -BS;
  yMax = BS;
}

SweepLine::SweepLine(bool ignoreEndings, double yLb, double yUb) {
  // cout << __LINE__ << endl;
  before = true;
  ignoreSegmentEndings = ignoreEndings;
  ptrEventQueue = new EventQueue;
  sweep = LineSegment(Point2D(-BS,BS),Point2D(BS,BS));
  currentEventPoint = Point2D();
  yMin = yLb;
  yMax = yUb;
}

Point2D SweepLine::intersects(const Event * e) {
  // cout << __LINE__ << endl;
  if (e->type == Event::INTERSECTION)
    return e->point;
  else
    return e->segment.intersects(this->sweep);
}

void SweepLine::setQueue(void * q) {
  // cout << __LINE__ << endl;
  ptrEventQueue = q;
  return;
}

void SweepLine::checkIntersection(Event a, Event b) {
  // cout << __LINE__ << endl;
  EventQueue *ptr = static_cast<EventQueue*>(ptrEventQueue);
  EventQueue eveQ = *ptr;
  if (a.type == Event::INTERSECTION ||
      b.type == Event::INTERSECTION)
    return;

  Point2D p = a.segment.intersects(b.segment);

  if (p.invalid()) return;

  if (a.segment.atEnding(p) && b.segment.atEnding(p) && ignoreSegmentEndings)
    return;

  unMapIt = intersections.find(p);
  set<Event> existing = intersections.at(unMapIt->first);
  intersections.erase (p);
  // if (existing == null) existing = new unordered_set<Event>;
  existing.insert(a);
  existing.insert(b);
  pair<Point2D,set<Event>> dumvar(p,existing);
  intersections.insert(dumvar);

  if (sweep.locRight(p) || (sweep.pointContained(p) &&
			    currentEventPoint.y)) {
    Event intersection = Event(Event::INTERSECTION,p,LineSegment(),this);
    eveQ.offer(p, intersection);
    return;
  }
}
#endif

// class PointHash {
// public:
//   // function to return the hash of the point
//   size_t operator()(Point2D const & p) const {
//     size_t xHash = hash<double>()(p.x);
//     size_t yHash = hash<double>()(p.y);
//     return ((xHash >= yHash) ? xHash*xHash + xHash + yHash :
// 	    xHash + yHash*yHash);
//   }
// };

// class SegmentHash {
// public:
//   // function to return the hash of an event
//   size_t operator()(LineSegment const & seg) const {
//     size_t pHash = hash<double>()(seg.start.distanceApprox(Point2D(0,0)));
//     size_t segHash = hash<double>()
//       (seg.start.distanceApprox(seg.end));
//     return ((pHash >= segHash) ? pHash*pHash + pHash + segHash :
// 	    pHash + segHash*segHash); // Szudzik's pairing function
//   }
// };
