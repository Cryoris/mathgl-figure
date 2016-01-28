/* Test 2d-2:
 * - calling plot with std::vector and Eigen::VectorXd
 * - calling legend and label 
 */

# include <iostream>
# include <cassert>
# include <cmath>
# include <vector>
# include <Eigen/Dense>
# include "checkfile.hpp"
# include "../Figure"

int main()
{
  std::vector<double> xd(10), yd(10);
  for (std::size_t i = 0; i < 10; ++i){
    xd[i] = i/10.;
    yd[i] = std::cos(i/2.);
  }
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(1000, 0, 10);
  Eigen::VectorXd y = ((5*x.array()).cos()).matrix();

  mgl::Figure fig;
  fig.title("Some title");
  fig.xlabel("Some x label");
  fig.ylabel("Some y label");
  fig.plot(xd, yd, " *r", "Data");
  fig.plot(x, y, "b", "Function");
  fig.legend();
  fig.save("P2d-2.eps");
  
  assert(fileExists("P2d-2.eps"));
  return 0;
}
