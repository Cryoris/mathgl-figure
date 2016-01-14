# include "../../figure.hpp"

int main()
{
  Figure fig;
  fig.fplot("3*x^2 - 6*x", "b");
  fig.ranges(-1, 1, -3, 3);
  fig.ylabel("y Axis");
  fig.save("plot.eps");

  return 0;
}
