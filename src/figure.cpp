# include <iostream>
# include <utility> // pair
# include <vector>
# include <algorithm>
# include <numeric>
# include <functional>
# include <sstream> // needed for title layout
# include <limits>
# include <cstring> // needed for length of const char*
# include <memory>
# include "MglPlot.hpp"
# include "MglLabel.hpp"
# include "MglStyle.hpp"
# include "figure.hpp"

namespace mgl {


void print(const mglData& d)
{
  for (long i = 0; i < d.GetNx(); ++i){
    std::cout << d.a[i] << " ";
  }
  std::cout << "\n";
}

/* constructor: set default style                                                                              *
 * PRE : pointer to mglGraph will not cease to exist until operations are performed on this Figure             *
 * POST: Axis is true -- Grid is true and in light grey -- Legend is false -- Ranges will be set automatically */
Figure::Figure()
  : axis_(true),
    grid_(true),
    legend_(false),
    legendPos_(1,1),
    gridType_("xy"),
    gridCol_("{h7}"),
    has_3d_(false),
    ranges_({ std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
          std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()}),
    zranges_({std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()}),
    autoRanges_(true),
    styles_(MglStyle()),
    fontSizePT_(6),
    figHeight_(800),
    figWidth_(800)
{}

/* setting height of the graphic                                 *
 * PRE : height should be > 0                                    *
 * POST: height of the graphic will later be set to given height */
void Figure::setHeight(int height) {
  figHeight_ = height;
}

/* setting width of the graphic                                *
 * PRE : width should be > 0                                   *
 * POST: width of the graphic will later be set to given width */
void Figure::setWidth(int width) {
  figWidth_ = width;
}

/* setting the font size                                          *
 * PRE : size should be > 0                                       *
 * POST: font size of the graphic will later be set to given size */
void Figure::setFontSize(int size) {
  fontSizePT_ = size;
}

/* change grid settings                                                         *
 * PRE : -                                                                      *
 * POST: No grid if on is false, grid style is gridType, grid color is gridCol. *
 *       For those arguments which are not given default settings are used.      */
void Figure::grid(bool on, const std::string& gridType,  const std::string& gridCol)
{
  if (on){
    grid_ = true;
  }
  else {
    grid_ = false;
  }

  gridType_ = gridType;
  gridCol_ = gridCol;
}

/* setting x-axis label                         *
 * PRE : -                                      *
 * POST: xlabel initialized with given position */
void Figure::xlabel(const std::string& label, double pos)
{
  xMglLabel_ = MglLabel(label, pos);
}

/* setting y-axis label                         *
 * PRE : -                                      *
 * POST: ylabel initialized with given position */
void Figure::ylabel(const std::string& label, double pos)
{
  yMglLabel_ = MglLabel(label, pos);
}

/* set or unset legend                                       *
 * PRE : -                                                   *
 * POST: if on is true legend will be plotted, otherwise not */
void Figure::legend(const double& xPos, const double& yPos)
{
  // print a warning if the user has given a position which will probably not appear on the plot
  if (std::abs(xPos) > 2 || std::abs(yPos) > 2){
    std::cerr << "* Figure - Warning * Legend may be out of the graphic due to large xPos or yPos\n";
  }

  // set legend_ to true so we know we have to activate the legend later
  legend_ = true;
  // keeping track of the position of the legend
  legendPos_ = std::pair<double, double>(xPos, yPos);
}

/* plot a function given by a string                                                            *
 * PRE : proper format of the input, e.g.: "3*x^2 + exp(x)", see documentation for more details *
 * POST: plot the function in given style                                                       */
MglPlot& Figure::fplot(const std::string& function, std::string style)
{
#if NDEBUG
  std::cout << "Called fplot!\n";
#endif

  // checking if a style is given, 
  // if yes: use it and delete it from the style-container,
  // if no: get a style from the style container
  if (style.size() == 0) {
    style = styles_.get_next();
  }
  else {
    styles_.eliminate(style);
  }

  // put the plot in the plot queue 
  plots_.emplace_back(std::unique_ptr<MglFPlot>(new MglFPlot(function, style)));
  return *plots_.back().get();
}

/* set ranges                                                   *
 * PRE : -                                                      *
 * POST: new ranges will be: x = [xMin, xMax], y = [yMin, yMax] */
void Figure::ranges(const double& xMin, const double& xMax, const double& yMin, const double& yMax)
{
  // checking if the input is valid
  if (xMin > xMax || yMin > yMax){
    std::cerr << "In function Figure::ranges(): xMin must be smaller than xMax and yMin smaller than yMax!";
  }

  // ranges have been set manually, so disable automatic ranges setting
  autoRanges_ = false;

  // set ranges according to the input
  ranges_ = {xMin, xMax, yMin, yMax};
}

/* get minimal positive ( > 0 ) value of mglData                            *
 * PRE : -                                                                  *
 * POST: minimal positive value of argument,                                *
 *       std::numeric_limits<double>::max() if no positive value cotained,  *
 *       print a warning to std::cerr if a value <= 0 encountered           *
 * NOTE: this function is only used to check ranges in logarithmic scaling  * 
 *       therefore the warning                                              */
double minPositive(const mglData& d)
{
  double result = std::numeric_limits<double>::max();
  bool print_warning = false;

  // iterate over the given data
  for (long i = 0; i < d.GetNx(); ++i){
    if (d.a[i] > 0){
      // if the data point is positive, check if it is smaller than the current minimum
      result = std::min(result, d.a[i]);
    }
    else {
      // if the data point is not positive it will not appear on the plot -> print warning
      print_warning = true;
    }
  }

  if (print_warning) {
    std::cerr << "* Figure - Warning * non-positive values of data will not appear on plot. \n";
  }

  return result;
}

/* change ranges of plot                                                             *
 * PRE : -                                                                           *
 * POST: set ranges in such a way that all data is displayed                         *
 * NOTE: need the argument vertMargin to be able to set it to 0 when plotting in 3d  */
void Figure::setRanges(const mglData& xd, const mglData& yd, double vertMargin)
{
#if NDEBUG
  std::cout << "setRanges for 2dim called\n";
#endif

  // initialize data
  double xMax(xd.Maximal()), yMax(yd.Maximal());
  double xMin(xd.Minimal()), yMin(yd.Minimal());

  // check for x and y axis if they are logarithmic,
  // if yes: if the minimal value is <= 0 set xMin (or yMin) to the smallest positive number in the data
  //         and if the maximal value is <= 0 no data will appear -> error message!
  // if no:  leave the old value
  if (xFunc_ == "lg(x)"){
    if (xMax <= 0){
      std::cerr << "In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal x-value must be greater than 0.";
    }
    yMin = minPositive(xd);
  }
  if (yFunc_ == "lg(y)"){
    if (yMax <= 0){
      std::cerr << "In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal y-value must be greater than 0.";
    }
    vertMargin = 0.; // no vertical margin in logscaling yet
    yMin = minPositive(yd);
  }

  // set new ranges
  const double yTot = yMax - yMin;
  ranges_[0] = std::min(xMin , ranges_[0]);
  ranges_[1] = std::max(xMax , ranges_[1]);
  ranges_[2] = std::min(yMin - yTot*vertMargin, ranges_[2]); // adding a slight margin in linear plots on bottom
  ranges_[3] = std::max(yMax + yTot*vertMargin, ranges_[3]); // .. and top
}

/* change ranges of the plotted region in 3d                        *
 * PRE : -                                                          *
 * POST: set the ranges in such a way that all data will be visible */
void Figure::setRanges(const mglData& xd, const mglData& yd, const mglData& zd)
{
#if NDEBUG
  std::cout << "setRanges for 3dim called\n";
#endif
  // initializing data
  const double zMax(zd.Maximal());
  double zMin(zd.Minimal());

  // check if z-axis is logarithmic,
  // if yes: if the minimal value is <= 0 set zMin to the smallest positive number in the data
  //         and if the maximal value is <= 0 no data will appear -> error message!
  // if no:  leave the old value
  if (zFunc_ == "lg(z)"){
    if (zMax <= 0){
      std::cerr << "In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal z-value must be greater than 0.";
    }
    zMin = minPositive(zd);
  }
  zranges_[0] = std::min(zranges_[0], zMin);
  zranges_[1] = std::max(zranges_[1], zMax);
  setRanges(xd, yd, 0.); // use this function to set the correct x and y ranges
}


/* (un-)set logscaling                                                               *
 * PRE : -                                                                           *
 * POST: linear, semilogx, semilogy or loglog scale according to bools logx and logy */
void Figure::setlog(bool logx, bool logy, bool logz)
{
  // if logi is true set iFunc_ to "lg(i)", which will later be used to initialize the coordinate curvature (i=x,y,z)
  if(logx){
    xFunc_ = "lg(x)";
  }
  if(logy){
    yFunc_ = "lg(y)";
  }
  if(logz){
    zFunc_ = "lg(z)";
  }
}

/* setting title                                                    *
 * PRE : -                                                          *
 * POST: title_ variable set to 'text' with small-font option ("@") */
void Figure::title(const std::string& text)
{
  title_ = text;
}

/* save figure                                                       *
 * PRE : -                                                           *
 * POST: write figure to 'file' in png-format if 'file' end on .png, *
 *       and to eps-format otherwise                                 */
void Figure::save(const std::string& file) {
  mglGraph gr_; // graph in which the plots will be saved

  // Set size. This *must* be the first function called on the mglGraph
  gr_.SetSize(figWidth_, figHeight_);

  // find out which subplot type to use
  std::string subPlotType = "";
  if (yMglLabel_.str_.size() != 0){
    // if there is a ylabel leave a margin on the left
    subPlotType += "<";
  }
  if (xMglLabel_.str_.size() != 0){
    // if there is a xlabel leave a margin on the bottom
    subPlotType += "_";
  }
  if (title_.size() != 0){
    // if there is a title leave  margin on the top
    subPlotType += "^";
  }

  // set font to 'heros'. If the file is not available on the machine it will use the MathGL default (STIX)
  gr_.LoadFont("heros");
  // set the font size
  gr_.SetFontSizePT(fontSizePT_);

  // Set ranges and call rotate if necessary (to set the correct point of view)
  if (has_3d_){
    // when plotting 3d we do need all the margins and we cannot cut them off 
    // -> cannot call gr_.SubPlot(1,1,0,"<_") or similar here!
    gr_.SetRanges(ranges_[0], ranges_[1], ranges_[2], ranges_[3], zranges_[0], zranges_[1]);
    gr_.Rotate(60, 30);
  }
  else {
    // if we do not plot 3d we set our margins computed above
    gr_.SubPlot(1, 1, 0, subPlotType.c_str()); 
    gr_.SetRanges(ranges_[0], ranges_[1], ranges_[2], ranges_[3]);
  }

  // Set label - before setting curvilinear because MathGL is vulnerable to errors otherwise
  gr_.Label('x', xMglLabel_.str_.c_str(), xMglLabel_.pos_);
  gr_.Label('y', yMglLabel_.str_.c_str(), yMglLabel_.pos_);

  // Set Curvilinear functions
  gr_.SetFunc(xFunc_.c_str(), yFunc_.c_str(), zFunc_.c_str());

  // Add grid
  if (grid_){
    gr_.Grid(gridType_.c_str() , gridCol_.c_str());
  }

  // Add axis
  if (axis_){
    gr_.Axis();
  }

  gr_.Box();
  // Plot
  for(auto &p : plots_) {
    p->plot(&gr_);
  }

 // Add legend
  if (legend_){
    gr_.Legend(legendPos_.first, legendPos_.second);
  }

  // Add title
  if (title_.size() != 0){
    gr_.Title(title_.c_str());
  }

#if NDEBUG
  std::cout << "Writing to file ... \n";
#endif

  // Checking if to plot in png or eps and save file
  if(file.find(".png") != std::string::npos){
    gr_.WritePNG(file.c_str());
  }
  else if (file.find(".eps") != std::string::npos){
    gr_.WriteEPS(file.c_str());
  }
  else {
    gr_.WriteEPS((file + ".eps").c_str());
  }
}

} // end namespace mgl
