#ifndef POINT_2D
#define POINT_2D

using namespace std;
#include <limits>
#include <iostream>
#include <cmath>

#define BS std::numeric_limits<double>::infinity()

class Point2D{
public: 
	double x;
	double y;

public:
	// constructor
	Point2D(double x, double y){
		this->x = x;
		this->y = y;
	};

	Point2D(){
		this->x = BS;
		this->y = BS;
	};

	// function to display the corresponding coordinates of the point
	void showPoints() const{
		cout << "(" << this->x << "," << this->y << ")" << endl;
	};


	// function to calculate the distance of our point wrt to an external point 
	double distanceApprox(Point2D point) const{
		return sqrt(pow(this->x - point.x,2) + pow(this->y - point.y,2));
	};

	// function to check if our point is invalid or null
	bool invalid() {
	  if (this->x != BS || this->y != BS) return false;
	  return true;
	};

	// Operator overloading
	bool operator==(const Point2D & p) const {
	  return (x == p.x && y == p.y);
	};

	bool operator<(const Point2D & p) const {
		if(this->x != p.x){
			return (this->x < p.x);
		}
		else{
			if(this->y != p.y){
				return (this->y < p.y);
			}
			else return false;
		}
	}
};
#endif
