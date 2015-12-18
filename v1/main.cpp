# include "figurev2.hpp"

int main(){
  
  std::vector<double> t(100), y(100), z(100);
  for (int i = -50; i < 50; ++i){
    t[i + 50] = i;
    y[i + 50] = i*i - 2*i;
    z[i + 50] = i*std::exp(std::sqrt(std::abs(i))*std::sin(0.5*i));
  }

  Figure fig;
  fig.plot(t, y, "b0", "f(x)");
  fig.plot(t, z, "r0", "g(x)");
  fig.title("Some functions");
  fig.legend();
  fig.save("plot.eps");

  return 0;
}
  
