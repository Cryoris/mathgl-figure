# include <Eigen/Dense>
# include "Figure"

int main () {
  Eigen::VectorXd t = Eigen::VectorXd::LinSpaced(500, 0, 1),
                  y = t;

  mgl::Figure fig;
  for (unsigned i = 1; i < 15; ++i) {
    fig.plot(t, y);
    y = y.cwiseProduct(t);
  }
  fig.save("many.eps");

  return 0;
}

