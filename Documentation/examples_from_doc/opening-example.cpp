# include <vector>
# include <Eigen/Dense>
# include <figure/figure.hpp>

int main()
{
  std::vector<double> x(10), y(10);
  for (int i = 0; i < 10; ++i){ 
    x[i] = i; y[i] = std::exp(-0.2*i)*std::cos(i);
  }
  Eigen::VectorXd u = Eigen::VectorXd::LinSpaced(500, 0, 9),
                  v = ( u.array().cos()*(-0.2*u).array().exp() ).matrix();
  mgl::Figure fig;
  fig.plot(x, y, " +r").label("Sample Data");
  fig.plot(u, v, "b").label("Function");
  fig.legend();
  fig.save("opening-example.eps");
  return 0;
}
