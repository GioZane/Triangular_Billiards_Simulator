#ifndef PF_BILIARDO_HPP
#define PF_BILIARDO_HPP
#include <vector>

namespace pf {

struct Point {
  double x;
  double y;
};

//Forward declaration
class Ball;
class Border;
struct CollisionResult;

class Ball {
private:
  Point coordba_{0., 0.};
  double d_{0.};            //launch angle

public:
  Ball(Point p, double t)
      : coordba_{p}, d_{t} {
  }

  //Getter methods to access member variables
  const Point &coordba() const;
  double d() const;

  //Methods for updating the position and slope of the straight line after each impact
  void move_to(const Point& new_point); 

  void set_angle(double new_s);
};

struct CollisionResult {
  bool has_hit;           //True if the collision occurs
  Ball hit;
  bool upper;             //True if the collision occurs with the upper border
  
};


class Border {
private:
  double r1_{0.}; 
  double r2_{0.};
  double L_{0.};
  double slopeup_{0.};

public:
  Border(double R1, double R2, double lenght)
      : r1_{R1}, r2_{R2}, L_{lenght}, slopeup_{0.0} {
  if (L_ != 0.0) {slopeup_ = (r2_ - r1_) / L_;} 
  else { slopeup_ = 0.0;}
}

  //Getter methods
  double r1() const;
  double r2() const;
  double L() const;
  double slopeup() const;

  void move_border(double r1, double r2, double L);
  static void initial_checks(Border const &b1, Border const &b2,
                             Ball const &ball);
   
  static CollisionResult next_collision(const Ball &b, const Border &b1, const Border &b2);
  static double NewAngle(CollisionResult const &cr, const Border &b);
  void set_r1(double val);
  void set_r2(double val);
  void set_L(double val);
  void set_slopeup(double val);
};

struct Result {
  int bounces;
  Ball result;
  std::vector<Ball> trajectory;

  Result(int bo, const Ball &ba) : bounces(bo), result(ba) {}
  Result(int bo, const Ball &ba, std::vector<Ball> &tr)
      : bounces(bo), result(ba), trajectory(tr) {}

  static Result BallSimulation(const Border &b1, const Border &b2, Ball &b);
  
};

}

#endif  


