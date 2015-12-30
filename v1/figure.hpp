# pragma once

# include <Eigen/Dense>
# include <mgl2/mgl.h>

class Figure {
  public:
    Figure();
    void setRanges(const mglData& xd, const mglData& yd);
    void grid(const bool on = true, const std::string gridType = "-", const std::string gridCol = "h");
    void xlabel(const char* label, const double pos = 0);
    void ylabel(const char* label, const double pos = 0);
    void legend(const double xPos = 1, const double yPos = 1);
    template <typename Vector> void plot(const Vector& x, const Vector& y, const char* style, const char* legend = 0);
    void ranges(const double xMin, const double xMax, const double yMin, const double yMax);
    void setlog(const bool logx = true, const bool logy = true);
    void save(const char* file);
    void title(const char* text);

  private:
    mglGraph gr_; // graph in which the plots will be saved
    bool axis_; // plot axis?
    bool grid_; // plot grid?
    bool legend_; // plot legend
    std::pair<double, double> legendPos_; // legend position
    std::string gridType_; // grid type
    std::string gridCol_; // grid color
    double ranges_[4]; // axis ranges
    bool initRanges_; // have the ranges been set yet?
    bool autoRanges_; // auto ranges or ranges as the user set them?
    bool ylogScale_; // is the y Axis in logarithmic scale? needed for range margin
    double fontSizePT_; // font size in PT
    std::vector<mglData> xd_, yd_; // vector of plot data
    std::vector<std::string> styles_; // vector of plot styles 
};

/* plot data
 * PRE : -
 * POST: add x-y to plot queue with given style (must be given!) and set legend (optional) */
template <typename Vector> // same syntax for Eigen::VectorXd and std::vector<T>
void Figure::plot(const Vector& x, const Vector& y, const char* style, const char* legend)
{
  if (x.size() != y.size())
    throw std::length_error("In function Figure::plot(): Vectors must have same sizes!");

  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());
  
  setRanges(xd, yd);
  xd_.push_back(xd);
  yd_.push_back(yd);
  styles_.push_back(style);
  
  if (legend != 0)
    gr_.AddLegend(legend, style);
}
