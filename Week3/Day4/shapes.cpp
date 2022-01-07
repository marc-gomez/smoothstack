#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

using namespace std;

class Point {
	private:
		double x;
		double y;

	public:
		Point(double x_in, double y_in) : x(x_in), y(y_in) {}

		Point(const Point &other) {
			x = other.getX();
			y = other.getY();
		}

		Point & operator=(const Point &other) {
			if (this == &other) {
				return *this;
			}
			x = other.getX();
			y = other.getY();
			return *this;
		}

		double getX() const {
			return x;
		}

		double getY() const {
			return y;
		}
};

class Shape2D {
	public:
		double distance(const Point &a, const Point &b) const {
			return sqrt(pow(b.getX() - a.getX(), 2) + pow(a.getY() - b.getY(), 2));
		}
};

class Polygon : public Shape2D {
	public:
		Polygon(const Point &a, const Point &b, const Point &c, const Point &d) {
			points.push_back(a);
			points.push_back(b);
			points.push_back(c);
			points.push_back(d);
		}

		Polygon(const Polygon &other) {
			points = other.getPoints();
		}

		Polygon &operator=(const Polygon &other) {
			if (this == &other) {
				return *this;
			}
			points = other.getPoints();
			return *this;
		}

		Polygon(Polygon &&other) {
			points = other.getPoints();
			other.points.clear();
		}

		Polygon & operator=(Polygon &&other) {
			if (this == &other) {
				return *this;
			}
			points = other.getPoints();
			other.points.clear();
			return *this;
		}

		Polygon(const Point &a, const Point &b, const Point &c) {
			points.push_back(a);
			points.push_back(b);
			points.push_back(c);
		}

		virtual double area() const = 0;

		vector<Point> getPoints() const {
			return points;
		}

		double perimeter() const {
			double sum = 0;
			for (int i = 1; i < points.size(); ++i) {
				sum += (distance(points[i - 1], points[i]));
			}
			sum += distance(points[points.size() - 1], points[0]);
			return sum;
		}

	protected:
		vector<Point> points;
};

class Triangle : public Polygon {
	public:
		Triangle(const Point &a, const Point &b, const Point &c)
			: Polygon(a, b, c) {}

		double area() const {
			return (0.5 * (points[0].getX() * points[1].getY() + 
						points[1].getX() * points[2].getY() +
						points[2].getX() * points[0].getY() -
						points[0].getY() * points[1].getX() -
						points[1].getY() * points[2].getX() -
						points[2].getY() * points[0].getX()));
		}
};

class Quadrilateral : public Polygon {
	public:
		Quadrilateral(const Point &a, const Point &b, const Point &c, const Point &d)
			: Polygon(a, b, c, d) {}
};

class Trapezoid : public Quadrilateral {
	public:
		Trapezoid(const Point &a, const Point &b, const Point &c, const Point &d)
			: Quadrilateral(a, b, c, d) {
				// TODO
				// assert two opposite lines are parallel
			}

		double area() const{
			// TODO
			// return (h * (a + b) / 2);
			return numeric_limits<double>::max();
		}
};

class Parallelogram : public Quadrilateral {
	public:
		Parallelogram(const Point &a, const Point &b, const Point &c, const Point &d)
			: Quadrilateral(a, b, c, d) {
				assert(distance(a, b) == distance(c, d));
				assert(distance(b, c) == distance(a, d));
			}

		virtual double area() const {
			// TODO
			// return (b * h);
			return numeric_limits<double>::max();
		}
};

class Rhombus : public Parallelogram {
	public:
		Rhombus(const Point &a, const Point &b, const Point &c, const Point &d)
			: Parallelogram(a, b, c, d) {
				assert(distance(a, b) == distance(b, c) && 
						distance(b, c) == distance(c, d) &&
						distance(c, d) == distance(d, a));
			}

		virtual double area() const {
			return 0.5 * distance(points[0], points[2]) * distance(points[1], points[3]);
		}
};

class Rectangle : public Parallelogram {
	public:
		Rectangle(const Point &a, const Point &b, const Point &c, const Point &d)
			: Parallelogram(a, b, c, d) {
				// TODO
				// assert all internal angles are 90 degrees
			}

		double area() const {
			return distance(points[0], points[1]) * distance(points[1], points[2]);
		}
};

class Square : public Rhombus {
	public:
		Square(const Point &a, const Point &b, const Point &c, const Point &d)
			: Rhombus(a, b, c, d) {
				// TODO
				// assert all internal angles are 90 degrees
			}

		double area() const {
			return pow(distance(points[0], points[1]), 2);
		}
};

class Ellipse : public Shape2D {
	public:
		Ellipse(const Point &center_in, double a_in, double b_in)
			: center(center_in), a(a_in), b(b_in) {}

		Ellipse(const Ellipse &other) : Ellipse(other.getCenter(), other.getA(),  other.getB()) {}

		Ellipse &operator=(const Ellipse &other) {
			if (this == &other) {
				return *this;
			}
			center = other.getCenter();
			a = other.getA();
			b = other.getB();		
			return *this;
		}

		Ellipse(Ellipse &&other) : Ellipse(other.getCenter(), other.getA(), other.getB()) {
			other.center = Point(0, 0);
			other.a = 0;
			other.b = 0;
		}

		Ellipse & operator=(Ellipse &&other) {
			if (this == &other) {
				return *this;
			}
			center = other.getCenter();
			a = other.getA();
			b = other.getB();
			other.center = Point(0, 0);
			other.a = 0;
			other.b = 0;
			return *this;
		}

		double circumference() const {
			return (2 * M_PI * sqrt((a * a + b * b) / (2)));
		}

		double area() const {
			return M_PI * a * b;
		}

		double getA() const {
			return a;
		}

		double getB() const {
			return b;
		}

		Point getCenter() const {
			return center;
		}

	private:
		Point center;
		double a;
		double b;
};

class Circle : public Ellipse {
	public:
		Circle(const Point &center_in, double rad_in) : Ellipse(center_in, rad_in, rad_in) {}

		double diameter() const {
			return 2*getA();
		}

		double getRadius() const {
			return getA();
		}
};

int main() {

	// Test rectangle
	Point p1(0, 0);
	Point p2(2, 0);
	Point p3(2, 2);
	Point p4(0, 2);
	Rectangle r(p1, p2, p3, p4);
	assert(r.area() == 4);
	assert(r.perimeter() == 8);

	// Test Rhombus and Square
	Point r1(1, 0);
	Point r2(0, 1);
	Point r3(-1, 0);
	Point r4(0, -1);
	Rhombus rhom(r1, r2, r3, r4);
	Square sq(r1, r2, r3, r4);
	assert(rhom.area() == 2 && abs(sq.area() - rhom.area()) < 0.00001);

	// Test Triangle
	Point t1(0, 3);
	Point t2(0, 0);
	Point t3(4, 0);
	Triangle t(t1, t2, t3);
	assert(t.area() == 6);
	assert(t.perimeter() == 12);

	// Test Circle and Ellipse
	Circle c(p1, 2);
	Ellipse e(p1, 2, 3);
	assert(c.getCenter().getX() == 0 && c.getCenter().getY() == 0);
	assert(c.area() == (4 * M_PI));
	Ellipse cir = Circle(p1, 2);
	assert(cir.getB() == 2);

	// Test copy constructor with inheritance
	Square s2(p1, p2, p3, p4);
	Rhombus rhom2(s2);
	assert(abs(rhom2.area() - 4) < 0.00001);

	// Test assignment operator with inheritance
	Rhombus rhom5(rhom2);
	assert(abs(rhom5.area() - 4) < 0.00001);
	rhom5 = sq;
	assert(abs(rhom5.area() - 2) < 0.00001);

	// Test move constructor with inheritance
	Rhombus rhom3 = move(s2);
	assert(abs(rhom3.area() - 4) < 0.00001);
	assert(s2.getPoints().empty());

	// Test move assignment operator with inheritance
	Rhombus rhom4(rhom3);
	assert(abs(rhom4.area() - 4) < 0.00001);
	rhom4 = move(sq);
	assert(abs(rhom4.area() - 2) < 0.00001);
	assert(sq.getPoints().empty());

	//Test assignment operator with inheritance on Ellipse
	Ellipse e2(e);
	e2 = c;
	assert(e2.area() == (4 * M_PI));

	//Test move assignment operator with inheritance on Ellipse
	Ellipse e3(e);
	e3 = move(c);
	assert(e2.area() == (4 * M_PI));
	assert(c.getRadius() == 0);
}
