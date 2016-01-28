# pragma once

# include <iostream>
# include <memory>
# include <array>
# include <utility>
# include <stdexcept>
# include <cassert>

# include "FigureConfig.hpp"
# if FIG_HAS_EIGEN
  # include <Eigen/Dense>
# endif

# include "MglPlot.hpp"
# include "MglLabel.hpp"
# include <mgl2/mgl.h>

namespace mgl {

//!
/*!
 * Make mglData from std::vector
 */
template<typename Scalar>
typename std::enable_if<std::is_arithmetic<Scalar>::value, mglData>::type
make_mgldata(const std::vector<Scalar>& v) {
  std::vector<double> vd(v.begin(), v.end());
  return mglData(vd.data(), vd.size());
}

//!
/*!
 * Make mglData from Eigen::Vector or Eigen::RowVector
 */
# if FIG_HAS_EIGEN
template<typename Derived>
mglData make_mgldata(const Eigen::MatrixBase<Derived>& vec) {
  assert(vec.rows() == 1 || vec.cols() == 1);
  std::vector<typename Derived::Scalar> v;
  if(vec.rows() == 1) {
    v.resize(vec.cols());
    for(int i = 0; i < vec.cols(); i++)
      v[i] = vec[i];
  } else {
    v.resize(vec.rows());
    for(int i = 0; i < vec.rows(); i++)
      v[i] = vec[i];
  }
  return mglData(v.data(), v.size());
}
# endif

class Figure {
public:
  Figure();

  void setRanges(const mglData& xd, const mglData& yd, double vertMargin = 0.1);

  void setRanges(const mglData& xd, const mglData& yd, const mglData& zd);

  void grid(bool on = true, const std::string& gridType = "-", const std::string& gridCol = "h");

  void xlabel(const std::string& label, double pos = 0);

  void ylabel(const std::string& label, double pos = 0);

  void legend(const double& xPos = 1, const double& yPos = 1);

  template <typename yVector>
  void plot(const yVector& y, const std::string &style, const std::string& legend = "");

  template <typename xVector, typename yVector>
  typename std::enable_if<!std::is_same<typename std::remove_pointer<typename std::decay<yVector>::type>::type, char >::value, void>::type
  plot(const xVector& x, const yVector& y, const std::string& style, const std::string& legend = "");

  template <typename xVector, typename yVector, typename zVector>
  void plot3(const xVector& x, const yVector& y, const zVector& z, const std::string &style, const std::string& legend = "");

  void fplot(const std::string& function, const std::string &style, const std::string& legend = "");

  void ranges(const double& xMin, const double& xMax, const double& yMin, const double& yMax);

  void setlog(bool logx = false, bool logy = false, bool logz = false);

  void setHeight(int height);

  void setWidth(int width);

  void setFontSize(int size);

  void save(const std::string& file);

  void title(const std::string& text);

private:
  bool axis_; // plot axis?
  bool grid_; // plot grid?
  bool legend_; // plot legend
  std::pair<double, double> legendPos_; // legend position
  std::string gridType_; // grid type
  std::string gridCol_; // grid color
  bool has_3d_; // are there 3d plots?
  std::array<double, 4> ranges_; // axis ranges
  std::array<double, 2> zranges_; // z axis ranges
  bool autoRanges_; // auto ranges or ranges as the user set them?
  std::string title_;
  std::string xFunc_, yFunc_, zFunc_;
  MglLabel xMglLabel_, yMglLabel_;
  double fontSizePT_; // font size in PT
  int figHeight_, figWidth_;
  std::vector<std::unique_ptr<MglPlot> > plots_;
};

} // end namespace
