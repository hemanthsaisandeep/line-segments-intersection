#include "BentleyOttmann.hpp"

using namespace std;

int main(){
	// Point2D point1 = Point2D(1,-1);
	// Point2D point2 = Point2D(-1,1);

	// Point2D point3 = Point2D(5,-6);
	// Point2D point4 = Point2D(-5,3);

	// //cout << checkSamePoints(point1,point2) << endl;
	// LineSegment l1 = LineSegment(point1,point2);
	// LineSegment l2 = LineSegment(point3,point4);
	// l1.calculateSlopeConstant();
	// l2.calculateSlopeConstant();
	// l1.showLineSegment();

	// cout << l1.pointContained(Point2D(0,0)) << endl;

	// map<int,Point2D> ps;
	// ps.insert(pair<int,Point2D>(1,point1));
	// ps.insert(pair<int,Point2D>(3,point2));
	// set<LineSegment> xs;
	// xs.insert(l1);
	// xs.insert(l2);

	// for(auto itr:ps){
	// 	itr.second.showPoints();
	// }
	
	// // Point2D point3 = Point2D(-1,-1);
	// // Point2D point4 = Point2D(1,1);
	// // LineSegment l2 = LineSegment(point3,point4);
	// // l2.calculateSlopeConstant();
	// // l2.showLineSegment();
	
	// // l1.intersects(l2).showPoints();
	// // cout << endl;

	// // Point2D p_r = Point2D(2,1);
	// // Point2D p_l = Point2D(0,1);
	// // // cout << l2.locRight(p_r) << endl;
	// // // cout << l2.locRight(p_l) << endl;
	// // // cout << point1.hashify() << endl;
	// // // cout << point2.hashify() << endl;
	// // // cout << point3.hashify() << endl;
	// // // cout << point4.hashify() << endl;

	Point2D point1 = Point2D(-5, -5);
	Point2D point2 = Point2D(5, 5);
        Point2D point3 = Point2D(-5, 5);
        Point2D point4 = Point2D(5, -5);
        Point2D point5 = Point2D(-1, 0);
        Point2D point6 = Point2D(1, 0);
        Point2D point7 = Point2D(-1, 0);
        Point2D point8 = Point2D(6, 0);
        Point2D point9 = Point2D(0, 0);
        Point2D point10 = Point2D(0, 6);
        Point2D point11 = Point2D(4, 1);
        Point2D point12 = Point2D(4, -5);

	LineSegment seg1 = LineSegment(point1,point2);
	LineSegment seg2 = LineSegment(point3,point4);
	LineSegment seg3 = LineSegment(point5,point6);
	LineSegment seg4 = LineSegment(point7,point8);
	LineSegment seg5 = LineSegment(point9,point10);
	LineSegment seg6 = LineSegment(point11,point12);
	vector<LineSegment> segments {seg1,seg2,seg3,seg4,seg5,seg6};
	vector<LineSegment>::iterator itr;
	for (itr = segments.begin(); itr != segments.end(); itr++) {
	  itr->calculateSlopeConstant();
	  // cout << itr->slope << endl;
	}
	set<Point2D> quick = BentleyOttmann::intersections(segments);
	if (quick.empty())
	  cout << "True" << endl;
	set<Point2D>::iterator it;
	for (it = quick.begin(); it != quick.end(); it++)
	  it->showPoints();
	return 0;
}
