# include "../../figure.hpp"

int main()
{
  mgl::Figure fig;
  fig.fplot("3*x^2 - 6*x", "b");
  fig.ranges(-1, 1, -3, 3);
  fig.ylabel("y Axis");
  fig.save("plot");

  return 0;
}
