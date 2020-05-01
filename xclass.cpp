#include "xclass.hpp"

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
int compare(Event that) {
  // Same events
  if (this->equals(that))
    return 0;

  // Get intersection points of both `this' and `that' with sweep line
  Point2D inPtThis = sweepLine->intersects(this);
  Point2D inPtThat = sweepLine->intersects(that);

  // Obtain the difference in ordinates between the Events' locations as they
  // pass through the sweep line
  double deltaY = inPtThis.y - inPtThat.y;

  if (!deltaY) return (deltaY>0) ? 1 : -1;
  else {
    if (this->segment.slope != that->segment.slope)
      {
	if (sweepLine.isBefore())
	  return (this->segment.slope>that.segment.slope) ? -1 : 1;
	else
	  return (this->segment.slope>that.segment.slope) ? 1 : -1;
      };

    // `this' and `that' have same slopes
    // and are passing through the same point in the sweep line,
    // then differentiate them using their left end (start) points.
    // Return if `that' start is encountered before.
    double deltaXStart = this->segment.start.x - that.segment.start.x;
    if (!deltaXStart) return (deltaXStart<0) ? -1 : 1;

    // Say their start points are the same, then compare their right end points
    return (this->segment.end.x - that.segment.end.x<0) ? -1 : 1;
  };
