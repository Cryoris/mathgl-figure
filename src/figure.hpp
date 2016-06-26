# ifndef FIGURE_H
# define FIGURE_H

# include <iostream>
# include <memory>
# include <array>
# include <utility>
# include <stdexcept>
# include <cassert>
# include <numeric>

# include "FigureConfig.hpp"
# if FIG_HAS_EIGEN
  # include <Eigen/Dense>
  # include <Eigen/Sparse>
# endif

# include "MglPlot.hpp"
# include "MglLabel.hpp"
# include "MglStyle.hpp"
# include <mgl2/mgl.h>

namespace mgl {

/* make mglData from std::vector                                    *
 * PRE: -                                                           *
 * POST: returning mglData containing the data of given std::vector */
template<typename Scalar>
typename std::enable_if<std::is_arithmetic<Scalar>::value, mglData>::type
make_mgldata(const std::vector<Scalar>& v) {
  std::vector<double> vd(v.begin(), v.end());
  return mglData(vd.data(), vd.size());
}

/* make mglData from Eigen::Vector or Eigen::RowVector                     *
 * PRE : -                                                                 *
 * POST: returning mglData containing the data of given Eigen::(Row)Vector */
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

  void addlabel(const std::string& label, const std::string& style);

  template <typename yVector>
  MglPlot& plot(const yVector& y, std::string style = "");

  template <typename xVector, typename yVector>
  typename std::enable_if<!std::is_same<typename std::remove_pointer<typename std::decay<yVector>::type>::type, char >::value, MglPlot&>::type
  plot(const xVector& x, const yVector& y, std::string style = "");

  template <typename xVector, typename yVector, typename zVector>
  MglPlot& plot3(const xVector& x, const yVector& y, const zVector& z, std::string style = "");

  MglPlot& fplot(const std::string& function, std::string style = "");

  void ranges(const double& xMin, const double& xMax, const double& yMin, const double& yMax);

  void save(const std::string& file);

  void setlog(bool logx = false, bool logy = false, bool logz = false);

  void setPlotHeight(const int height);

  void setPlotWidth(const int width);

  void setTopMargin(const int top);

  void setLeftMargin(const int left);

  void setHeight(const int height);

  void setWidth(const int width);

  void setFontSize(const int size);

  template <typename Matrix> // dense version
  MglPlot& spy(const Matrix& A, const std::string& style = "b");

# if FIG_HAS_EIGEN // only enable if Eigen is available, otherwise Eigen::SparseMatrix will not be defined
  template <typename Scalar> // sparse version
  MglPlot& spy(const Eigen::SparseMatrix<Scalar>& A, const std::string& style = "b");
# endif

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
  std::array<double, 3> aspects_; // axis aspects, e.g. -1 used to invert axis. see MathGL docu
  bool autoRanges_; // auto ranges or ranges as the user set them?
  std::string title_; // title of the plot
  std::string xFunc_, yFunc_, zFunc_; // curvature of coordinate axis
  MglLabel xMglLabel_, yMglLabel_; // x and y labels of the plot
  MglStyle styles_; // styles of the plots
  double fontSizePT_; // font size in PT
  int figHeight_, figWidth_; // height and width of the whole image
  int plotHeight_, plotWidth_; // height and width of the plot
  int leftMargin_, topMargin_; // left and top margin of plot inside the image
  std::vector<std::unique_ptr<MglPlot> > plots_; // x, y (and z) data for the plots
  std::vector<std::pair<std::string, std::string>> additionalLabels_; // manually added labels 
};

/* plot y data                                                         *
 * PRE : -                                                             *
 * POST: add [1:length(y)]-y to plot queue with given style (optional) */
template <typename yVector>
MglPlot& Figure::plot(const yVector& y, std::string style)
{
  // build a fitting x vector for the y vector
  std::vector<double> x(y.size());
  std::iota(x.begin(), x.end(), 1);
  return plot(x, y, style);
}

/* plot x,y data                                           *
 * PRE : -                                                 *
 * POST: add x-y to plot queue with given style (optional) */
template <typename xVector, typename yVector>
// the long template magic expression ensures this function is not called if yVector is a string (which would be allowed as it is a templated argument)
typename std::enable_if<!std::is_same<typename std::remove_pointer<typename std::decay<yVector>::type>::type, char >::value, MglPlot&>::type
Figure::plot(const xVector& x, const yVector& y, std::string style)
{
  // make sure the sizes of the vectors are the same
  if (x.size() != y.size()){
    std::cerr << "In function Figure::plot(): Vectors must have same sizes!";
  }

  // build mglData from the x and y vectors
  mglData xd = make_mgldata(x);
  mglData yd = make_mgldata(y);

  // if the ranges are set to auto set the new ranges 
  if(autoRanges_){
    setRanges(xd, yd, 0.); // the 0 stands no top+bottom margin
  }
  
  // check if a style is given,
  // if yes: add that style to the style queue and remove it from the style-deque,
  // if no : get a new style from the style-deque
  if (style.size() == 0) {
    style = styles_.get_next();
  }
  else {
    styles_.eliminate(style);
  }

  // put the x-y data in the plot queue
  plots_.emplace_back(std::unique_ptr<MglPlot2d>(new MglPlot2d(xd, yd, style)));
  return *plots_.back().get();
}

/* plot x,y,z data                                           *
 * PRE : -                                                   *
 * POST: add x-y-z tp plot queue with given style (optional) */
template <typename xVector, typename yVector, typename zVector>
MglPlot& Figure::plot3(const xVector& x, const yVector& y, const zVector& z, std::string style)
{

  has_3d_ = true; // needed to set zranges in save-function and call mgl::Rotate

  // make sure the sizes of the vectors are the same
  if (!(x.size() == y.size() && y.size() == z.size())){
    std::cerr << "In function Figure::plot(): Vectors must have same sizes!";
  }

  // build mglData from x, y and z vectors
  mglData xd = make_mgldata(x);
  mglData yd = make_mgldata(y);
  mglData zd = make_mgldata(z);

  // if the ranges are set to auto set the new ranges
  if(autoRanges_){
    setRanges(xd, yd, zd);
  }

  // check if a style is given,
  // if yes: add that style to the style queue and remove it from the style-deque,
  // if no : get a new style from the style-deque
  if (style.size() == 0) {
    style = styles_.get_next();
  }
  else {
    styles_.eliminate(style);
  }

  // put the x-y-z data in the plot queue
  plots_.emplace_back(std::unique_ptr<MglPlot3d>(new MglPlot3d(xd, yd, zd, style)));
  return *plots_.back().get();
}

template <typename Matrix>
MglPlot& Figure::spy(const Matrix& A, const std::string& style) {

  has_3d_ = false;
  aspects_[1] = -1; // invert y-axis

  // determine radius of dots
  std::string radius = "8";
  if (std::max(A.cols(), A.rows()) > 99) {
    radius = "5";
  }
  if (std::max(A.cols(), A.rows()) > 999) {
    radius = "3";
  }
  if (std::max(A.cols(), A.rows()) > 9999) {
    radius = "1";
  }
  
  // counting nonzero entries
  unsigned long counter = 0;
  // save positions of entries in these vectors
  // x for the col-index and y for the row-index
  std::vector<double> x, y;

  ranges_ = {0, A.cols() + 1, 0, A.rows() + 1};
  for (unsigned i = 0; i < A.rows(); ++i) {
    for (unsigned j = 0; j < A.cols(); ++j) {
      if (A(i,j) != 0) {
        ++counter;
        x.push_back(j + 1);
        // if the row is zero plot at the top, not bottom
        y.push_back(i + 1);
      }
    }
  }
  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());

  std::stringstream label;
  label << "nnz = ";
  label << counter;
  xMglLabel_ = MglLabel(label.str());

  plots_.emplace_back(std::unique_ptr<MglSpy>(new MglSpy(xd, yd, style + radius)));
  return *plots_.back().get();
}

# if FIG_HAS_EIGEN
template <typename Scalar> 
MglPlot& Figure::spy(const Eigen::SparseMatrix<Scalar>& A, const std::string& style) {

   has_3d_ = false;
   aspects_[1] = -1; // invert y-axis

  // determine radius of dots
  std::string radius = "8";
  if (std::max(A.cols(), A.rows()) > 99) {
    radius = "5";
  }
  if (std::max(A.cols(), A.rows()) > 999) {
    radius = "3";
  }
  if (std::max(A.cols(), A.rows()) > 9999) {
    radius = "1";
  }
  
  // counting nonzero entries
  unsigned long counter = 0;
  // save positions of entries in these vectors
  std::vector<double> x, y;

  ranges_ = {0, A.cols() + 1, 0, A.rows() + 1};
  // iterate over nonzero entries, using method suggested in Eigen::Sparse documentation
  for (unsigned k = 0; k < A.outerSize(); ++k) {
    for (typename Eigen::SparseMatrix<Scalar>::InnerIterator it(A, k); it; ++it) {
      ++counter;
      x.push_back( it.col() + 1 );
      y.push_back( it.row() + 1 );
    }
  }
  mglData xd(x.data(), x.size()),
          yd(y.data(), y.size());

  std::stringstream label;
  label << "nnz = ";
  label << counter;
  xMglLabel_ = MglLabel(label.str());

  plots_.emplace_back(std::unique_ptr<MglSpy>(new MglSpy(xd, yd, style + radius)));
  return *plots_.back().get();
}
# endif

} // end namespace

# endif
