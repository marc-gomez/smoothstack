#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

using namespace std;

int N = 10;
double G = 6.673e-11;
double timestep = 0.001;
double initial_mass = 1e16;
double k = 100;

struct R {
	double x;
	double y;

	R() : x(0), y(0) {}
	R(double x_in, double y_in) : x(x_in), y(y_in) {}
};

struct V {
	double x_v;
	double y_v;

	V() : x_v(0), y_v(0) {}
	V(double x, double y) : x_v(x), y_v(y) {}
};

struct F {
	double x_f;
	double y_f;

	F() : x_f(0), y_f(0) {}
	F(double x, double y) : x_f(x), y_f(y) {}

	F & operator=(const F &other) {
		if (this == &other) {
			return *this;
		}
		x_f = other.x_f;
		y_f = other.y_f;
		return *this;
	}

	F operator*(int mult) {
		return F(x_f * mult, y_f * mult);
	}
};

struct A {
	double x_a;
	double y_a;

	A() : x_a(0), y_a(0) {}
	A(double x, double y) : x_a(x), y_a(y) {}
};

struct body {
	int ind;
	double mass;
	R position;
	V velocity;
	F *forceVector;
	F totalForce;
	A acceleration;

	body() : ind(-1), mass(0), position(R()), velocity(V()),
		forceVector(nullptr), totalForce(F()), acceleration(A()) {}

	body(int i, double m, R pos, V vel, F force, A accel)
		: ind(i), mass(m), position(pos), velocity(vel), totalForce(force), acceleration(accel) {
		forceVector = new F[N];
		}

	body & operator=(const body &other) {
		if (this == &other) {
			return *this;
		}
		if (ind != -1) {
			delete[] forceVector;
		}
		ind = other.ind;
		mass = other.mass;
		position = other.position;
		velocity = other.velocity;
		totalForce = other.totalForce;
		acceleration = other.acceleration;
		forceVector = new F[N];
		for (int i = 0; i < N; ++i) {
			forceVector[i] = other.forceVector[i];
		}
		return *this;
	}

	body(const body &other) {
		ind = other.ind;
		mass = other.mass;
		position = other.position;
		velocity = other.velocity;
		totalForce = other.totalForce;
		acceleration = other.acceleration;
		forceVector = new F[N];
		for (int i = 0; i < N; ++i) {
			forceVector[i] = other.forceVector[i];
		}
	}

	~body() {
		if (ind != -1) {
			delete[] forceVector;
		}
	}
};

int main() {
	// Change verbose to true if you want printed output
	bool verbose = false;

	int numBodies[] = {10, 20, 50, 100, 200, 500, 1000, 2000};

	for (int N_index = 0; N_index < sizeof(numBodies)/sizeof(numBodies[0]); ++N_index) {
		N = numBodies[N_index];

		// Initialize bodies
		body * bodies = new body[N];
		for (int i = 0; i < N; ++i) {
			bodies[i] = body(i, initial_mass, R(rand() % (N * 10), rand() % (N * 10)), V(), F(), A());
		}

		// Loop through steps
		// Loop k only prints final state
		for (int step = 0; step < k + 1; ++step) {

			if (verbose) {
				// Print start of step
				cout << "Step " << step << endl;
			}

			// Loop through bodies to update forceVector
			for (int bod = 0; bod < N; ++bod) {

				if (verbose) {
					// Print body stat
					[bod] (const body &b) {
						cout << "body " << bod << ": Pos (" <<
							b.position.x << ", " << b.position.y << "), Vel (" <<
							b.velocity.x_v << ", " << b.velocity.y_v << "), Acc (" <<
							b.acceleration.x_a << ", " << b.acceleration.y_a <<
							"), Force (" <<
							b.totalForce.x_f << ", " << b.totalForce.y_f << ")" << endl;
					}(bodies[bod]);
				}

				if (step < k) {
					// Calculate force between bod and all other bodies and update forceVector
					for (int others = bod + 1; others < N; ++others) {
						bodies[bod].forceVector[others] =
							[] (double m1, double m2, R pos1, R pos2) {
								if (pos1.x == pos2.x && pos1.y == pos2.y) {
									return F();
								}
								double dist = [pos1, pos2] {
									return sqrt(pow(pos1.x - pos2.x, 2) +
									pow(pos1.y - pos2.y, 2));
								}();
								double f = (-1*G*m1*m2 / (dist*dist) );
								double rad = atan(abs(pos1.y - pos2.y) /
										abs(pos1.x - pos2.x));
								F final = F(cos(rad) * f, sin(rad) * f);

								// Adjust sign/direction
								if (pos1.x < pos2.x) {
									final.x_f = abs(final.x_f);
								}
								if (pos1.y < pos2.y) {
									final.y_f = abs(final.y_f);
								}
								return final;
							}(bodies[bod].mass, bodies[others].mass,
							bodies[bod].position, bodies[others].position);
						// half-pair
						bodies[others].forceVector[bod] =
							bodies[bod].forceVector[others] * -1;
					}
				}
			}

			if (step < k) {
				// Loop through bodies again to update all other attributes
				for (int bod = 0; bod < N; ++bod) {
					body &b = bodies[bod];

					// Update totalForce
					[&b] {
						b.totalForce.x_f = 0;
						b.totalForce.y_f = 0;
						for (int i = 0; i < N; ++i) {
							b.totalForce.x_f += b.forceVector[i].x_f;
							b.totalForce.y_f += b.forceVector[i].y_f;
						}
					}();
				
					// Update acceleration
					[&b] {
						b.acceleration.x_a = b.totalForce.x_f / b.mass;
						b.acceleration.y_a = b.totalForce.y_f / b.mass;
					}();

					// Update velocity
					[&b] {
						b.velocity.x_v += timestep * b.acceleration.x_a;
						b.velocity.y_v += timestep * b.acceleration.y_a;
					}();

					// Update position
					[&b] {
						b.position.x += timestep * b.velocity.x_v;
						b.position.y += timestep * b.velocity.y_v;
					}();
				}
			}

			if (verbose) {
				// Print newline at end of step
				cout << endl;
			}
		}

		// Print interactions per second
		cout << "N: " << N << endl <<
			"Interactions/sec: " << [] {return k * N * N / timestep / 2;}() << endl << endl;

		// Free memory
		delete[] bodies;
	}
}
