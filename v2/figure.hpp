# pragma once

# include <iostream>
<<<<<<< HEAD
# include <mgl2/mgl.h>
# include <memory>
=======
# include <Eigen/Dense>
>>>>>>> 5513c4d4eba6c8f94609d63a1ac46a91748fa30e
# include <utility>
# include <stdexcept>

namespace mgl {
# include <mgl2/mgl.h>
class Figure {
  public:
    Figure();
    void setRanges(const mglData& xd, const mglData& yd, double vertMargin = 0.1);
    void setRanges(const mglData& xd, const mglData& yd, const mglData& zd);
    void grid(const bool on = true, const std::string gridType = "-", const std::string gridCol = "h");
    void xlabel(const char* label, const double pos = 0);
    void ylabel(const char* label, const double pos = 0);
    void legend(const double xPos = 1, const double yPos = 1);
    template <typename yVector> void plot(const yVector& y, const std::string style, const char* legend = 0);
    template <typename xVector, typename yVector> void plot(const xVector& x, const yVector& y, const std::string style, const char* legend = 0);
    template <typename xVector, typename yVector, typename zVector> void plot3(const xVector& x, const yVector& y, const zVector& z, const std::string style, const char* legend = 0);
    void fplot(const std::string function, const std::string style, const char* legend = 0);
    void ranges(const double xMin, const double xMax, const double yMin, const double yMax);
    void setlog(const bool logx = false, const bool logy = false, const bool logz = false);
    void save(const char* file);
    void title(const char* text);

    enum PlotType { plot2d, plot3d, plotf };

  private:
    mglGraph gr_; // graph in which the plots will be saved
    bool axis_; // plot axis?
    bool grid_; // plot grid?
    bool legend_; // plot legend
    std::pair<double, double> legendPos_; // legend position
    std::string gridType_; // grid type
    std::string gridCol_; // grid color
    double ranges_[4]; // axis ranges
    double zranges_[2]; // z axis ranges
    bool initRanges_; // have the ranges been set yet?
    bool initZRanges_;
    bool autoRanges_; // auto ranges or ranges as the user set them?
    bool xlogScale_; // needed for warning in xlabel
    bool ylogScale_; // is the y Axis in logarithmic scale? needed for range margin and warning in ylabel
    bool zlogScale_;
    double fontSizePT_; // font size in PT
    std::vector<PlotType> plotKind_;
    std::vector<mglData> xd_, yd_, zd_; // vector of plot data
    std::vector<std::string> fplots_; // vector of fplot functions
    std::vector<std::string> styles_; // vector of plot styles
};

/* plot y data
 * PRE : -
 * POST: add (range(1, length(y)))-y to plot queue with given style (must be given!) and set legend (optional) */
template <typename yVector>
void Figure::plot(const yVector& y, const std::string style, const char* legend)
{
  std::vector<double> x(y.size());
  for (std::size_t i = 0; i < y.size(); ++i){
    x[i] = i + 1;
  }
  plot(x, y, style, legend);
}

/* plot x,y data
 * PRE : -
 * POST: add x-y to plot queue with given style (must be given!) and set legend (optional) */
template <typename xVector, typename yVector> // same syntax for Eigen::VectorXd and std::vector<T>
void Figure::plot(const xVector& x, const yVector& y, const std::string style, const char* legend)
{
  if (x.size() != y.size()){
    throw std::length_error("In function Figure::plot(): Vectors must have same sizes!");
  }

  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());

  setRanges(xd, yd);
  // put plot data in queue
  xd_.push_back(xd);
  yd_.push_back(yd);

  // put plot style in queue
  styles_.push_back(style);

  // save what kind of plot we're drawing
  plotKind_.push_back(plot2d);

  if (legend != 0){
    gr_.AddLegend(legend, style.c_str());
  }
}

template <typename xVector, typename yVector, typename zVector>
void Figure::plot3(const xVector& x, const yVector& y, const zVector& z, const std::string style, const char* legend)
{
  gr_.SubPlot(1,1,0,"_^"); // when using 3d plot we cant cut the top and bottom margins

  if (!(x.size() == y.size() && y.size() == z.size())){
      throw std::length_error("In function Figure::plot(): Vectors must have same sizes!");
  }

  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size()),
          zd(z.data(), z.size());

  // set ranges
  setRanges(xd, yd, zd);
  // push back
  xd_.push_back(xd);
  yd_.push_back(yd);
  zd_.push_back(zd);

  // put plot style in queue
  styles_.push_back(style);

  // save plot type
  plotKind_.push_back(plot3d);

  // add legend?
  if (legend != 0){
    gr_.AddLegend(legend, style.c_str());
  }
}

}
