#ifndef SOLVER_H
#define SOLVER_H


class Solver {
 public:
  Solver();

  double solve(const double m,
               const double xpos,
               const double ypos,
               const double target_x) const;

 private:
};

#endif // SOLVER_H
