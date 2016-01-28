/* TEST 2d-1 : simple line plotting with std::vector and writing to eps */

# include <iostream>
# include <vector>
# include <cassert>
# include "checkfile.hpp" // contains function fileExists
# include "../figure.hpp"

int main()
{
  // prepare data
  std::vector<double> x(100), y(100);
  for (std::size_t i = 0; i < 100; ++i){
    x[i] = i; y[i] = i*i;
  }

  mgl::Figure fig;
  fig.plot(x, y, "b");
  fig.save("P2d-1.eps");
  assert(fileExists("P2d-1.eps"));
  return 0;
}
