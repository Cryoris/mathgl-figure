# include <Eigen/Dense>
# include <figure/figure.hpp>

int main () {
  const int n = 20;
  Eigen::VectorXd v = Eigen::VectorXd::Random(n),
                  w = Eigen::VectorXd::LinSpaced(n, 1, n),
                  z = Eigen::VectorXd::LinSpaced(n, 2000, 2000 + n);
  mgl::Figure fig;
  fig.bar(v, "r+");
  fig.save("random");

  mgl::Figure fig2;
  fig2.bar(z, w);
  fig2.save("linear");
  return 0;
}
