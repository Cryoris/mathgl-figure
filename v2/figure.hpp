# pragma once

# include <Eigen/Dense>
# include <mgl2/mgl.h>
# include <utility>

class Figure {
  public:
    Figure();
    void setRanges(const mglData& xd, const mglData& yd);
    void grid(const bool on = true, const std::string gridType = "-", const std::string gridCol = "h");
    void xlabel(const char* label, const double pos = 0);
    void ylabel(const char* label, const double pos = 0);
    void legend(const double xPos = 1, const double yPos = 1);
    template <typename yVector> void plot(const yVector& y, const std::string style, const char* legend = 0);
    template <typename xVector, typename yVector> void plot(const xVector& x, const yVector& y, const std::string style, const char* legend = 0);
    void fplot(const std::string function, const std::string style, const char* legend = 0);
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
    bool xlogScale_; // needed for warning in xlabel
    bool ylogScale_; // is the y Axis in logarithmic scale? needed for range margin and warning in ylabel
    double fontSizePT_; // font size in PT
    std::vector<mglData> xd_, yd_; // vector of plot data
    std::vector<std::string> fplots_; // vector of fplot functions
    std::vector<std::string> styles_, fstyles_; // vector of plot styles 
};

/* plot y data 
 * PRE : -
 * POST: add (range(1, length(y)))-y to plot queue with given style (must be given!) and set legend (optional) */
template <typename yVector>
void Figure::plot(const yVector& y, const std::string style, const char* legend)
{
  const Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(y.size(), 1, y.size());
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

#if 0 // safeguard, not sure if to implement it - probably as settable option
  if (xlogScale_){
    auto xit = x.data();
    for (unsigned int i = 0; i < x.size(); ++i){
      if (*(xit + i) == 0){
        *(xit + i) = std::numeric_limits<double>::epsilon();
      }
    }
  }
  if (ylogScale_){
    auto yit = x.data();
    for (unsigned int i = 0; i < x.size(); ++i){
      if (*(yit + i) == 0){
        *(yit + i) = std::numeric_limits<double>::epsilon();
      }
    }
  }
#endif



  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());
  
  setRanges(xd, yd);
  xd_.push_back(xd);
  yd_.push_back(yd);
  styles_.push_back(style);
  
  if (legend != 0){
    gr_.AddLegend(legend, style.c_str());
  }
}
