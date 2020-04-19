// #include "point.cpp"
#include "lineSegment.hpp"

#include <iostream>
using namespace std;

int main(){
	Point2D point1 = Point2D(1,-1);
	Point2D point2 = Point2D(-1,1);
	
	//cout << checkSamePoints(point1,point2) << endl;
	LineSegment l1 = LineSegment(point1,point2);
	l1.calculateSlopeConstant();
	l1.showLineSegment();

	//cout << l1.pointContained(Point2D(0,0)) << endl;




	Point2D point3 = Point2D(-1,-1);
	Point2D point4 = Point2D(1,1);
	LineSegment l2 = LineSegment(point3,point4);
	l2.calculateSlopeConstant();
	l2.showLineSegment();


	l1.intersects(l2).showPoints();
	cout << endl;
	
	
	return 0;
}

