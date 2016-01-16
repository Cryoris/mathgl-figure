# pragma once

# include <iostream>
# include <mgl2/mgl.h>
# include <memory>
# include <Eigen/Dense>
# include <utility>
# include <stdexcept>
# include <cassert>
# include <memory>

# include "MglPlot.hpp"

namespace mgl {
# include <mgl2/mgl.h>

template<typename Scalar>
mglData make_mgldata(const std::vector<Scalar>& v) {
  return mglData(v.data(), v.size());
}

template<typename Derived>
mglData make_mgldata(const Eigen::MatrixBase<Derived>& vec) {
  assert(vec.rows() == 1 || vec.cols() == 1);
  std::vector<typename Derived::Scalar> v(vec.cols());
  if(vec.rows() == 1) {
    for(int i = 0; i < vec.cols(); i++)
      v[i] = vec[i];
  } else
    for(int i = 0; i < vec.rows(); i++)
      v[i] = vec[i];
  return mglData(v.data(), v.size());
}

class Figure {
public:
  Figure();
  void setRanges(const mglData& xd, const mglData& yd, double vertMargin = 0.1);
  void setRanges(const mglData& xd, const mglData& yd, const mglData& zd);
  void grid(const bool on = true, const std::string gridType = "-", const std::string gridCol = "h");
  void xlabel(const std::string& label, const double pos = 0);
  void ylabel(const std::string& label, const double pos = 0);
  void legend(const double xPos = 1, const double yPos = 1);
  template <typename yVector> void plot(const yVector& y, const std::string &style, const std::string& legend = "");
  template <typename xVector, typename yVector> void plot(const xVector& x, const yVector& y, const std::string& style, const std::string& legend = 0);
  template <typename xVector, typename yVector, typename zVector> void plot3(const xVector& x, const yVector& y, const zVector& z, const std::string &style, const std::string& legend = 0);
  void fplot(const std::string& function, const std::string &style, const std::string& legend = "");
  void ranges(const double xMin, const double xMax, const double yMin, const double yMax);
  void setlog(const bool logx = false, const bool logy = false, const bool logz = false);
  void save(const std::string& file);
  void title(const std::string& text);

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

  std::vector<std::unique_ptr<MglPlot> > plots_;
};

/* plot y data
 * PRE : -
 * POST: add (range(1, length(y)))-y to plot queue with given style (must be given!) and set legend (optional) */
template <typename yVector>
void Figure::plot(const yVector& y, const std::string &style, const std::string &legend)
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
void Figure::plot(const xVector& x, const yVector& y, const std::string &style, const std::string &legend)
{
  if (x.size() != y.size()){
    throw std::length_error("In function Figure::plot(): Vectors must have same sizes!");
  }

  mglData xd = make_mgldata(x);
  mglData yd = make_mgldata(y);

  setRanges(xd, yd);
  plots_.emplace_back(std::unique_ptr<MglPlot2d>(new MglPlot2d(xd, yd, style)));
  if (!legend.empty()){
    gr_.AddLegend(legend.c_str(), style.c_str());
  }
}

template <typename xVector, typename yVector, typename zVector>
void Figure::plot3(const xVector& x, const yVector& y, const zVector& z, const std::string& style, const std::string& legend)
{
  gr_.SubPlot(1,1,0,"_^"); // when using 3d plot we cant cut the top and bottom margins

  if (!(x.size() == y.size() && y.size() == z.size())){
    throw std::length_error("In function Figure::plot(): Vectors must have same sizes!");
  }

  mglData xd = make_mglData(x);
  mglData yd = make_mglData(y);
  mglData zd = make_mglData(z);


  setRanges(xd, yd, zd);
  plots_.emplace_back(std::unique_ptr<MglPlot3d>(new MglPlot3d(xd, yd, zd, style)));

  // add legend?
  if (!legend.empty()){
    gr_.AddLegend(legend.c_str(), style.c_str());
  }
}

} // end namespace
