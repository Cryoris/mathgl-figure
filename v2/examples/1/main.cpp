#include <vector>
#include <Figure>
#include <cmath>
#include <Eigen/Dense>

int main() {

  // fig.fplot("3*x^2 + 2", "b");
  // fig.ranges(-1, 1, 1, 8);
  std::vector<double> x(100);
  Eigen::VectorXd y(100);
  for(int i = 0; i < 100; i++) {
    x[i] = i;
    y[i] = std::exp(-i/100.);
  }
  mgl::Figure fig;
  fig.plot(x, y, "g-");
  fig.ylabel("y Axis");
  fig.setlog(false, true, false);
  fig.xlabel("x Axis");
  fig.save("plot.eps");

  return 0;
}
