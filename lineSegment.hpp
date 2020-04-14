#ifndef LINESEGMENT_2D
#define LINESEGMENT_2D 

#include "point.cpp"

using namespace std;
#include <iostream>
#include <cmath>

class LineSegment{
	// two points 'start' and 'end' define the line segment 
public:
	Point2D start;
	Point2D end;
	double slope, constant;

public:
	// constructor function:
	/* 
		The point with lower x-coordinate is considered as start and the other point
		is considered as the end.
		In case of tie, the one with the lower y-coordinate is considered as start and 
		vice versa.
	*/
	LineSegment(Point2D p1, Point2D p2){
		try{
				if(checkSamePoints(p1,p2))
					throw 10;
				else{
					if(p1.x>p2.x){
						start = p2;
						end = p1;
					}
					else if(p1.x<p2.x){
						start = p1;
						end = p2;
					}
					else{
						start = (p1.y>p2.y)?p2:p1;
						end = (p1.y>p2.y)?p1:p2;
					}
				} 
	}
	catch(int x){
				this->slope = BS;
				this->constant = BS;
				cout << "cannot create line segment: point1 equals point2, both are (" << p1.x << p1.y << ")" << endl;
			}
};	



	/*
		Function to calculate the slope and the constant AFTER the LineSegment constructor() is called.
	*/
	void calculateSlopeConstant(){
		if(this->start.x != BS && this->start.y != BS && this->end.x != BS && this->end.x != BS){
			if(this->end.x != this->start.x ){
				this->slope = (this->end.y-this->start.y)/(this->end.x - this->start.x);
				this->constant = (this->start.y*this->end.x - this->start.x*this->end.y)/(this->end.x - this->start.x);
			}
			else{
				 /* 
				 	Indicates that the slope is undefined or infinite
				 	example x=3 is a valid line. 
					Here we just store the x coordinate
					
				 */
				this->constant = this->end.x;
				this->slope = BS;
			}
		}

	};


	/*
		Function to print the line segment 
	*/
	void showLineSegment(){
		cout << " The start point of line segment : ";
		this->start.showPoints();
		cout << " The end point of line segment : ";
		this->end.showPoints();

	};



	/*
		Function to check if a point lies in the given line segment.
		Assuming that the line segment is valid
		Return bool
		Note that if point p is either the start or the end point, it is considered 
		to be inside the line segment.
	*/
	bool pointContained(Point2D p){
		if(isnan(this->slope)){
			return (p.x==this->constant)?true:false;
		}
		else{
			// defined signum functions 
			int sign_x1 = (this->start.x - p.x)/abs(this->start.x - p.x);
			int sign_x2 = (p.x - this->end.x)/abs(p.x - this->end.x);
			int sign_y1 = (this->start.y - p.y)/abs(this->start.y - p.y) ;
			int sign_y2 = (p.y - this->end.y)/abs(p.y - this->end.y);

			// cout << sign_x1 << sign_x2 << sign_y1 << sign_y2 << endl;

		 return ( checkSamePoints(this->start,p) || 
		 	checkSamePoints(this->end,p) || 
		 	((this->slope*p.x+this->constant - p.y)==0 && sign_x1 == sign_x2 && sign_y1 == sign_y2)?true:false);
		}
	};


	/*
		Function to check if a line segment overlaps with the given line segment.
		Assuming that the line segment is valid
		Return bool
	*/
	bool lineSegmentContained(LineSegment that){
		// first check if the slopes are same or not 
		if(this->slope == that.slope){
			// check if the start of 'that' lies in this and check if end of 'this' lies in that
			return this->pointContained(that.start) && that.pointContained(this->end);
		}
		
		return false;
	};


	/*
		Function to check if two proper line segments intersect.
		If they do then return the point of intersection .
		Additional functions written are to calculate xIntercept and yIntercept. 
	*/

	double xIntercept(LineSegment that){
		if(this->slope == BS){
				return this->constant;
			}
		else if(that.slope == BS){
			return that.constant;
		}
		else{
			return (that.constant - this->constant)/(this->slope - that.slope);
		}
	};

	double yIntercept(double x){
		return this->slope*x + this->constant;
	};

	// function to check if "lines" intersect
	Point2D intersects(LineSegment that){
		// if(that. != BS ){
			if(this->slope == that.slope){
				return Point2D();
			}
			else{
				double x = this->xIntercept(that);
				if(this->slope == BS){
					Point2D result =  Point2D(x,that.yIntercept(x));
					return (this->pointContained(result) == true)?result:(Point2D());
				}
				else{
					Point2D result =  Point2D(x,this->yIntercept(x));
					return (this->pointContained(result) && that.pointContained(result)) ? result: Point2D() ;
				}
				}
			}
			// }
	};

#endif