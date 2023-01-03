#ifndef PADDLE_H
#define PADDLE_H

class Paddle {
public:
  Paddle(bool is_left);

  const bool is_left() const { return left_side_; }
  void move(const double dt, const bool direction);
  const double get_y() const { return y_pos_; }

private:
  bool left_side_;  // left = true, right = false
  double y_pos_;    // Position of centre of paddle
};

#endif // PADDLE_H
