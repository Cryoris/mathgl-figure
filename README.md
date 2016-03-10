# MathGL Figure

## Installation

__UNIX__ (Linux, Mac OS X)

	$ mkdir build
	$ cd build
	$ cmake ..
	$ sudo make install

## Example

![opening example](https://github.com/Cryoris/mathgl-figure/raw/master/Documentation/examples_from_doc/opening-example-eps-converted-to.pdf)

```c++
#include <vector>
#include <Eigen/Dense>
#include "mathgl-figure/figure.hpp"

int main()
{
  std::vector<double> x(10), y(10);
  for (int i = 1; i <= 10; ++i){ 
    x[i] = i; y[i] = std::exp(std::cos(0.2*i));
  }
  Eigen::VectorXd u = Eigen::VectorXd::LinSpaced(500, 1, 10),
                  v = ( (0.2*u.array()).cos().exp() ).matrix();
  mgl::Figure fig;
  fig.plot(x, y, " +r").label("Sample Data");
  fig.plot(u, v, "b").label("Function");
  fig.legend();
  fig.save("plot.eps");
  return 0;
}
```