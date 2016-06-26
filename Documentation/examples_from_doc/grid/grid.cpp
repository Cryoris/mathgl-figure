# include <figure/figure.hpp>
# include "../prepare_data.hpp"

int main () {
  vec_t x, y;
  prepare_data(x, y, 2);

  mgl::Figure f, g;
  f.title("Set grid");
  f.grid();
  f.plot(x, y);
  f.save("grid_1.eps");

  g.title("Fine, red grid");
  g.grid(true, "!", "r");
  g.plot(x, y);
  g.save("grid_2");

  return 0;
}

