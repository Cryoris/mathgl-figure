# include <Eigen/Dense>
# include <vector>
# include "../figure.hpp"

int main()
{
  std::vector<double> x(10), y(10);
  for (int i = 0; i < 10; ++i){ 
    x[i] = i; y[i] = std::exp(std::cos(1.3*i));
  }
  Eigen::VectorXd u = Eigen::VectorXd::LinSpaced(500, 0, 9),
                  v = ( (1.3*u.array()).cos().exp() ).matrix();
  Figure fig;
  fig.plot(x, y, " +r", "Sample Data");
  fig.plot(u, v, "b", "Function");
  fig.legend();
  fig.save("plot.eps");

  return 0;
}
