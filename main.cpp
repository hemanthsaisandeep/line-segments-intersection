#include "lineSegment.hpp"

#include <iostream>
#include <iterator>
#include <set>

using namespace std;

int main(){
	Point2D point1 = Point2D(1,-1);
	Point2D point2 = Point2D(-1,1);

	Point2D point3 = Point2D(5,-6);
	Point2D point4 = Point2D(-5,3);

	//cout << checkSamePoints(point1,point2) << endl;
	LineSegment l1 = LineSegment(point1,point2);
	LineSegment l2 = LineSegment(point3,point4);
	l1.calculateSlopeConstant();
	l2.calculateSlopeConstant();
	l1.showLineSegment();

	cout << l1.pointContained(Point2D(0,0)) << endl;

	set<LineSegment> xs;
	xs.insert(l1);
	xs.insert(l2);

	for(auto itr:xs){
		itr.start.showPoints();
	}
	




	// Point2D point3 = Point2D(-1,-1);
	// Point2D point4 = Point2D(1,1);
	// LineSegment l2 = LineSegment(point3,point4);
	// l2.calculateSlopeConstant();
	// l2.showLineSegment();
	
	// l1.intersects(l2).showPoints();
	// cout << endl;

	// Point2D p_r = Point2D(2,1);
	// Point2D p_l = Point2D(0,1);
	// // cout << l2.locRight(p_r) << endl;
	// // cout << l2.locRight(p_l) << endl;
	// // cout << point1.hashify() << endl;
	// // cout << point2.hashify() << endl;
	// // cout << point3.hashify() << endl;
	// // cout << point4.hashify() << endl;
	
	return 0;
}
