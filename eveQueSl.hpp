#ifndef EVEQUESL_H
#define EVEQUESL_H

#include <vector>
#include <stack>
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

  void showEvent();
};

class SweepLine {
private:
  set<Event> events;
  set<Event>::iterator resultEvent;
  map<Point2D,set<Event>> intersections;
  LineSegment sweep;
  Point2D currentEventPoint;
  bool before;


public:
  double yMin, yMax;

protected:
  void * ptrEventQueue;

private:
  bool ignoreSegmentEndings;

public:
  // constructor
  SweepLine();

  // Returns the event that is immediately above the current event
  Event above(Event e) {
    resultEvent = events.upper_bound(e);
    if (resultEvent == events.end())
      throw 9999;
    return *resultEvent;
  }

  // Returns the event that is immediately below the current event
  Event below(Event e) {
    resultEvent = events.lower_bound(e);
    if (resultEvent == events.end())
      throw 1234;
    return *resultEvent;
  }

  // Returns a boolean informing if we are just before the sweepline
  bool isBefore() {
    return before;
  }

  // Returns the intersection point of the segment corresponding to
  // a possible event and the current sweepline
  // if the current event is not of INTERSECTION type
  Point2D intersects(const Event * e);

  // Returns if there are any intersections yet
  bool hasIntersections(){

    return !(this->intersections.empty());
  }

  // Returns an map of the segments
  map<Point2D,vector<LineSegment>> listIntersections() {
    map<Point2D,vector<LineSegment>> segments;
    map<Point2D,set<Event>>::iterator iterIntxn;
    for (iterIntxn = this->intersections.begin();
	 iterIntxn != this->intersections.end(); iterIntxn++) {
      set<Event>::iterator iterEve;
      vector<LineSegment> intxnEventSet;
      for (iterEve = iterIntxn->second.begin();
	   iterEve != iterIntxn->second.end(); iterEve++)
	intxnEventSet.push_back(iterEve->segment);
      pair<Point2D,vector<LineSegment>> dumvar(iterIntxn->first,intxnEventSet);
      segments.insert(dumvar);
    }
    return segments;
  }

  void setLine(LineSegment line) {
    this->sweep = line;
    return;
  }

  void setQueue(void * ptrQ);

private:
  // Checks the intersection between two events
  void checkIntersection(Event a, Event b);

  // Handle an individual event based on its type
  void handle(Event e) {
    Event eUb;
    Event eLb;
    int flag;
    try {
      eUb = above(e);
      eLb = below(e);
    } catch(int x) {
      if (x == 1234)
	flag = -1;
      else if (x == 9999)
	flag = 1;
      cout << "Either immediately above or below event does not exist" << endl;
      return;
    }
    switch(e.type) {
    case Event::START:
      cout << __LINE__ << " Yo! A start event" << endl;
      before = false;
      events.insert(e);
      if (flag != 1)
	checkIntersection(e,eUb);
      if (flag != -1)
	checkIntersection(e,eLb);
      break;
    case Event::END:
      cout << __LINE__ << " Yo! An end event" << endl;
      before = true;
      events.erase(e);
      if (flag != 1 && flag != -1)
	checkIntersection(eUb,eLb);
      break;
    case Event::INTERSECTION:
      cout << __LINE__ << "WOAH! An intersection" << endl;
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
	Event eve = toInsert.top();
	Event eveLb, eveUb;
	try {
	  eveUb = above(eve);
	  eveLb = below(eve);
	} catch(int x) {
	  if (x == 1234)
	    flag = -1;
	  else if (x == 9999)
	    flag = 1;
	  return;
	}
	toInsert.pop();
	events.insert(eve);
	if (flag != 1)
	  checkIntersection(eve,eveUb);
	if (flag != -1)
	  checkIntersection(eve,eveLb);
      }
      break;
    }
    return;
  }

  // Updates the sweep line and moves it to the current event point
  void sweepTo(Event e) {
    currentEventPoint = e.point;
    sweep = LineSegment(currentEventPoint,sweep.slope,yMin,yMax);
    this->showSweepLine();
    return;
  }

public:
  // Handles a set of events
  void handle(set<Event> events) {
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
    set<Event>::iterator it;
    for (it = events.begin(); it != events.end(); it++) {
      handle(*it);
    }
    return;
  }

  // Display sweep line formatted as a string
  void showSweepLine() {
    cout << "The current sweep line has a slope of " << this->sweep.slope
	 << ", passes through ";
    this->sweep.start.showPoints();
    cout << ", and the current event point is ";
    this->currentEventPoint.showPoints();
  }
};

class EventQueue {
private:
  map<Point2D, set<Event>> events;

public:
  // constructor
  EventQueue() {  }

  EventQueue(vector<LineSegment> segments, SweepLine * line) {
    if (segments.empty())
      cout << "'Segments' cannot be empty" << endl;
    init(segments,line);
  }

  // Offers a new event point to the queue and inserts END events at the start
  // while events of other type are appended at the end of the queue
  void modify(Point2D p, Event e) {
    set<Event> existing;
    if (events.find(p) != events.end()) {
      set<Event> existing = events.at(p);
      events.erase(p);
    }

    // END events should be at the start of the set
    if (e.type == Event::END)
      existing.insert(existing.begin(),e);

    // Other events should be at the end of the set
    else
      existing.insert(existing.end(),e);
    pair<Point2D,set<Event>> dumvar(p,existing);
    events.insert(dumvar);	// dummy variable
    return;
  }

  // Returns true if the event queue is empty
  bool isEmpty () {
    return events.empty();
  }

  set<Event> firstEntry() {
    if (this->isEmpty())
      cout << "Queue is empty" << endl;
    map<Point2D,set<Event>>::iterator it;
    it = events.begin();
    set<Event> retval = it->second;
    events.erase(it);
    return retval;
  }

private:
  // Initializes sweep line and event queue for a given input of line segments
  void init(vector<LineSegment> segments, void * ptrSwLi){
    double minY = BS;
    double maxY = -BS;
    double minX = BS;
    double minDeltaX = BS;
    set<double> xs;
    vector<LineSegment>::iterator itr;
    for(itr = segments.begin(); itr != segments.end(); itr++){
      xs.insert(itr->start.x);
      xs.insert(itr->end.x);
      if(itr->start.y <= itr->end.y){
	if (itr->start.y < minY)
	  minY = itr->start.y;
	if (itr->end.y > maxY)
	  maxY = itr->end.y;
      }
      else{
	if (itr->end.y < minY)
	  minY = itr->end.y;
	if (itr->start.y > maxY)
	  maxY = itr->start.y;
      }

      modify(itr->start, Event(Event::START, itr->start, *itr, ptrSwLi));
      modify(itr->end, Event(Event::END, itr->end, *itr, ptrSwLi));
    }

    vector<double> xsVector(xs.size());
    std::copy(xs.begin(), xs.end(), xsVector.begin());

    for(size_t i=1; i<xsVector.size(); i++){
      if(xsVector.at(i) < minX) minX = xsVector.at(i);

      double tempDelta = xsVector.at(i) - xsVector.at(i-1);
      if(tempDelta < minDeltaX) minDeltaX = tempDelta;
    }

    double deltaY = maxY - minY;
    double slope = -deltaY*1000/minDeltaX;

    SweepLine * ptrSweepLine = static_cast<SweepLine*>(ptrSwLi);
    ptrSweepLine->yMin = minY;
    ptrSweepLine->yMax = maxY;
    ptrSweepLine->
      setLine(LineSegment(Point2D(minX-pow(slope,2),minY-pow(slope,2)),
			  slope,  minY-1, maxY+1));
    ptrSweepLine->setQueue(this);
    ptrSweepLine->showSweepLine();
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
int Event::compare (Event that) const {
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
  before = true;
  ignoreSegmentEndings = false;
  ptrEventQueue = NULL;
  sweep = LineSegment(Point2D(-BS,BS),Point2D(BS,BS));
  currentEventPoint = Point2D();
  yMin = -BS;
  yMax = BS;
}

Point2D SweepLine::intersects(const Event * e) {
  if (e->type == Event::INTERSECTION)
    return e->point;
  else
    return e->segment.intersects(this->sweep);
}

void SweepLine::setQueue(void * ptrQ) {
  ptrEventQueue = ptrQ;
  if (ptrEventQueue == NULL)
    cout << "setQueue() ain't working, dude" << endl;
  return;
}

void SweepLine::checkIntersection(Event a, Event b) {
  if (ptrEventQueue != NULL) {
    EventQueue *ptr = static_cast<EventQueue*>(ptrEventQueue);

    if (a.type != Event::INTERSECTION &&
	b.type != Event::INTERSECTION) {
      Point2D p;
      try {
	p = a.segment.intersects(b.segment);
	p.showPoints();
      } catch(int) {
	return;
      }
      if (!(p.invalid())) {
	if (!(a.segment.atEnding(p)) || !(b.segment.atEnding(p)) ||
	    !ignoreSegmentEndings) {
	  set<Event> existing;
	  map<Point2D,set<Event>>::iterator itMap;
	  itMap = intersections.find(p);
	  if (itMap != intersections.end()) {
	    set<Event> existing = intersections.at(itMap->first);
	    intersections.erase(p);
	  }

	  existing.insert(a);
	  existing.insert(b);
	  pair<Point2D,set<Event>> dumvar(p,existing); // dummy variable
	  intersections.insert(dumvar);

	  if (sweep.locRight(p) || (sweep.pointContained(p) &&
				    (p.y > currentEventPoint.y))) {
	    Event intersection = Event(Event::INTERSECTION,p,LineSegment(),this);

	    ptr->modify(p, intersection);
	    intersection.showEvent();
	  }
	}
      }
    }
  }
  // else
  //   cout << "SweepLine::setQueue() went bonkers" << endl;
}

void Event::showEvent() {
  cout << "The current event point is " << endl;
  point.showPoints();
  switch(type) {
  case Event::START:
    cout << "\033[F, a start point, " << endl;
    break;
  case Event::END:
    cout << "\033[F, an end point, " << endl;
    break;
  case Event::INTERSECTION:
    cout << "\033[F, an intersection point, " << endl;
    break;
  }
  cout << "and the associated sweepline is ";
  SweepLine * ptrSwLi = static_cast<SweepLine*>(ptrSweepLine);
  ptrSwLi->showSweepLine();
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

// class EventHash{
// public:
//   // function to return the hash of an event
//   size_t operator()(Event const & e) const {
//     size_t pHash = hash<double>()(e.point.distanceApprox(Point2D(0,0)));
//     size_t segHash = hash<double>()
//       (e.segment.start.distanceApprox(e.segment.end));
//     return ((pHash >= segHash) ? pHash*pHash + pHash + segHash :
// 	    pHash + segHash*segHash); // Szudzik's pairing function
//   }
// };
