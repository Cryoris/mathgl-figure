/* Test 2d-3:
 * - fplot
 * - large amount of data
 * - logscaling
 * - unsetting grid
 */

# include <iostream>
# include <vector>
# include <Eigen/Dense>
# include <cassert>
# include "checkfile.hpp"
# include "../Figure"

int main()
{
  const long n = 1e3;
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(n, 1, n);
  Eigen::VectorXd y = ((-0.01*x.array()).exp()).matrix();

  mgl::Figure fig;
  fig.setlog(true, true);
  fig.plot(x, y, "g");
  fig.fplot("exp(-x + sqrt(x)*cos(x))", "r;");
  fig.grid(false);
  fig.save("P2d-3.eps");

  assert(fileExists("P2d-3.eps"));
  return 0;
}
