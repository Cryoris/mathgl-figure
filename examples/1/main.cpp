#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include "../../Figure"

int main() {

  // fig.fplot("3*x^2 + 2", "b");
  // fig.ranges(-1, 1, 1, 8);
  std::vector<double> x(100);
  std::vector<double> xi(100);
  std::vector<double> y1(100);
  Eigen::RowVectorXd y2(100);
  Eigen::VectorXd y3(100);
  for(int i = 0; i < 100; i++) {
    x[i] = i;
    xi[i] = i;
    y1[i] = std::exp(-i/100.);
    y2(i) = std::exp(-i/75.);
    y3(i) = std::exp(-i/85.);
  }
  mgl::Figure fig;

  fig.plot(x, y1, "g-", "1");
  fig.plot(xi, y2, "r|", "2");
  fig.plot(y3, "b-", "3");

  fig.ylabel("y Axis");
  fig.setlog(false, true, false);
  fig.legend(1, 1);
  fig.xlabel("x Axis");
  fig.save("plot.eps");

  return 0;
}
