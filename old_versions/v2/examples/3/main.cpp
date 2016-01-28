# include <Eigen/Dense>
# include "../../Figure"

int main()
{
  Eigen::VectorXd l = Eigen::VectorXd::LinSpaced(500, 0, 10*M_PI);
  Eigen::VectorXd x = l.array().cos().matrix();
  Eigen::VectorXd y = l.array().sin().matrix();

  mgl::Figure fig;
  fig.plot3(x, y, l, "b");
  fig.save("3d.eps");

  return 0;
}
  
