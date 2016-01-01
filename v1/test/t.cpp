# include <mgl2/mgl.h>

int main()
{
  std::vector<double> x(100), y(100);
  for (int i = 0; i < 100; ++i){
    x[i] = i;
    y[i] = 1000*i*i;
  }

  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());

  mglGraph gr;
  gr.SubPlot(1,1,0,"<_");
  gr.SetRanges(0,100,0,1000*100*100);
  gr.Plot(xd, yd);
  gr.Axis();
  gr.Label('y', "My Label");
  gr.WriteEPS("plot.eps");

  return 0;
}
