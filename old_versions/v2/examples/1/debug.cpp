# include <mgl2/mgl.h>

int main()
{

  mglGraph gr;
  gr.SetRanges(-1, 1, -3, 3, -1, 1);
  gr.FPlot("3*x^2 - 6*x", "b");
  gr.WriteEPS("fplot.eps");

  return 0;
}
