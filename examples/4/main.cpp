# include <iostream>
# include <Eigen/Dense>
# include "../../src/Figure"

using namespace Eigen;

int main() {
  VectorXd x(5);
  x << 0.1, 0.001, 0, 0.00001, 0.0001;

  mgl::Figure fig;
  fig.setlog(true, true);
  fig.plot(x);
  fig.save("4.eps");

  return 0;
}
  
