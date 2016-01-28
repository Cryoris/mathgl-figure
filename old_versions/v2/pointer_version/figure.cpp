# include <iostream>
# include <algorithm> // find_if, remove_copy_if
# include <utility> // pair
# include <sstream> // needed for title layout
# include <cstring> // needed for length of const char* 
# include <functional>
# include "figure.hpp"

void print(const mglData& d)
{
  for (long i = 0; i < d.GetNx(); ++i){
    std::cout << d.a[i] << " ";
  }
  std::cout << "\n";
}

/* constructor: set default style
 * PRE : pointer to mglGraph will not cease to exist until operations are performed on this Figure
 * POST: Axis is true -- Grid is true and in light grey -- Legend is false -- Ranges will be set automatically */
Figure::Figure()
  : axis_(true), 
    grid_(true), 
    legend_(false), 
    legendPos_(std::pair<double, double>(1,1)),
    gridType_(""), 
    gridCol_("h"), 
    initRanges_(true),
    initZRanges_(true),
    autoRanges_(true),
    xlogScale_(false),
    ylogScale_(false),
    zlogScale_(false),
    fontSizePT_(8),
    data_(std::vector<std::unique_ptr<mglData>[3]>()),
    fplots_(std::vector<std::string>()),
    styles_(std::vector<std::string>())
{
  gr_.SubPlot(1,1,0,"_");
  gr_.SetTuneTicks(true, 1.0); // put the 'x10^5' closer to the graph
  gr_.LoadFont("heros");
  gr_.SetFontSizePT(fontSizePT_);
  ranges_[0] = 1; ranges_[1] = 1.1; ranges_[2] = 1; ranges_[3] = 1.1; 
  zranges_[0] = 1; zranges_[1] = 1.1;
  gr_.SetRanges(1, 1.1, 1, 1.1);
};


/* change grid settings 
 * PRE : - 
 * POST: No grid if on is false, grid style is gridType, grid color is gridCol.
 *       For those arguments which are not given default settings are used. */
void Figure::grid(const bool on, const std::string gridType, const std::string gridCol)
{
  if (on)
    grid_ = true;
  else 
    grid_ = false;
  
  gridType_ = gridType;
  gridCol_ = gridCol;
}

void Figure::xlabel(const char* label, const double pos)
{
  if (xlogScale_){
    std::cout << "* Figure - Warning * for better text-alignment, xlabel should be called before plot\n";
  }
  gr_.Label('x', label, pos);
}

/* setting y-axis label
 * PRE : -
 * POST: ylabel initialized with given position */
void Figure::ylabel(const char* label, const double pos)
{
  if (ylogScale_){
    std::cout << "* Figure - Warning * for better text-alignment, ylabel should be called before plot\n";
  }
  gr_.SubPlot(1,1,0,"<_");
  gr_.Label('y', label, pos);
}

/* set or unset legend
 * PRE : -
 * POST: if on is true legend will be plotted, otherwise not */
void Figure::legend(const double xPos, const double yPos)
{
  if (std::abs(xPos) > 2 || std::abs(yPos) > 2){
    std::cout << "* Figure - Warning * Legend may be out of the graphic due to large xPos or yPos\n";
  }
  legend_ = true;
  legendPos_ = std::pair<double, double>(xPos, yPos);
}

/* plot a function given by a string
 * PRE : proper format of the input, e.g.: "3*x^2 + exp(x)", see documentation for more details 
 * POST: plot the function in given style */
void Figure::fplot(const std::string function, const std::string style, const char* legend)
{
  data_.push_back({nullptr, nullptr, nullptr}); // 'codeword' for fplot
  fplots_.push_back(function);
  styles_.push_back(style);

  if (legend != 0){
    gr_.AddLegend(legend, style.c_str());
  }
}

/* set ranges 
 * PRE : -
 * POST: new ranges will be: x = [xMin, xMax], y = [yMin, yMax] */
void Figure::ranges(const double xMin, const double xMax, const double yMin, const double yMax)
{
  if (xMin > xMax || yMin > yMax)
    throw std::range_error("In function Figure::ranges(): xMin must be smaller than xMax and yMin smaller than yMax!");

  autoRanges_ = false;
  gr_.SetRanges(xMin, xMax, yMin, yMax);
}

/* change ranges of plot
 * PRE : -
 * POST: set ranges in such a way that all data is displayed */
// need the argument vertMargin to be able to set it to 0 when plotting in 3d
void Figure::setRanges(const mglData& xd, const mglData& yd, double vertMargin)
{
  if (!autoRanges_)
    return;

  const double xMax(xd.Maximal()),
               yMax(yd.Maximal());
  double xMin(xd.Minimal()),
         yMin(yd.Minimal());
  const long N = xd.GetNx();

  // if logscaling is active and some values are 0 or negative we must make sure that the range still is positive
  // -> x(or y)Min = smallest strictly positive value of x(or y)
  std::function<bool(double)> isNegative = [](double x){ return x <= 0 ? true : false; }; 
  if (xlogScale_){
    if (xMax <= 0){
      throw std::range_error("In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal x-value must be greater than 0.");
    }
    if (&xd.a[N] != std::find_if(&xd.a[0], &xd.a[N], isNegative)){
      std::cout << "* Figure - Warning * non-positive values of x data will not appear on plot. \n";
    }
    std::vector<double> buffer(N, xMax);
    mglData xdPositive(buffer.data(), buffer.size());
    std::remove_copy_if(&xd.a[0], &xd.a[N], &xdPositive.a[0], isNegative);
    xMin = xdPositive.Minimal();
  }
  if (ylogScale_){
    if (yMax <= 0){
      throw std::range_error("In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal y-value must be greater than 0.");
    }
    if (&yd.a[N] != std::find_if(&yd.a[0], &yd.a[N], isNegative)){
      std::cout << "* Figure - Warning * non-positive values of y data will not appear on plot. \n";
    }
    std::vector<double> buffer(N, yMax);
    mglData ydPositive(buffer.data(), buffer.size());
    std::remove_copy_if(&yd.a[0], &yd.a[N], &ydPositive.a[0], isNegative);
    yMin = ydPositive.Minimal();
  }

  const double xTot = xMax - xMin;
  const double yTot = yMax - yMin;
  // width/height of additional margin (in percentage) of the total width/height
  if (ylogScale_){
    // if vertMargin would be > 0, we may set the lower yrange to 0 or even < 0 which is not allowed in logscales
    vertMargin = 0.0; 
  }

  const double horizMargin = 0.0; // margin left and right

  if (initRanges_) { // ranges have not been set yet, so set it according to data
    ranges_[0] = xMin - horizMargin*xTot; 
    ranges_[1] = xMax + horizMargin*xTot; 
    ranges_[2] = yMin - vertMargin*yTot; 
    ranges_[3] = yMax + vertMargin*yTot;
    initRanges_ = false;
  }
  else { // check in which directions ranges have to be extended
    if (ranges_[0] > xMin)
      ranges_[0] = xMin - horizMargin*xTot; 
    if (ranges_[1] < xMax)
      ranges_[1] = xMax + horizMargin*xTot; 
    if (ranges_[2] > yMin)
      ranges_[2] = yMin - vertMargin*yTot; 
    if (ranges_[3] < yMax)
      ranges_[3] = yMax + vertMargin*yTot; 
  }
  gr_.SetRanges(ranges_[0], ranges_[1], ranges_[2], ranges_[3]);
}

/* change ranges of the plotted region in 3d
 * PRE : -
 * POST: set the ranges in such a way that all data will be visible */
void Figure::setRanges(const mglData& xd, const mglData& yd, const mglData& zd)
{
  if (!autoRanges_)
    return;

  const double zMax(zd.Maximal());
  double zMin(zd.Minimal());
  const long N = zd.GetNx();

  // if logscaling is active and some values are 0 or negative we must make sure that the range still is positive
  // -> x(or y)Min = smallest strictly positive value of x(or y)
  std::function<bool(double)> isNegative = [](double x){ return x <= 0 ? true : false; }; 
  if (zlogScale_){
    if (zMax <= 0){
      throw std::range_error("In function Figure::setRanges() : Invalid ranges for logscaled plot - maximal z-value must be greater than 0.");
    }
    if (&zd.a[N] != std::find_if(&zd.a[0], &zd.a[N], isNegative)){
      std::cout << "* Figure - Warning * non-positive values of z data will not appear on plot. \n";
    }
    std::vector<double> buffer(N, zMax);
    mglData zdPositive(buffer.data(), buffer.size());
    std::remove_copy_if(&zd.a[0], &zd.a[N], &zdPositive.a[0], isNegative);
    zMin = zdPositive.Minimal();
  }
  
  const double zTot = zMax - zMin;
  // width/height of additional margin (in percentage) of the total width/height
  double vertMargin;
  if (zlogScale_){
    // if vertMargin would be > 0, we may set the lower yrange to 0 or even < 0 which is not allowed in logscales
    vertMargin = 0.0; 
  }
  else {
    vertMargin = 0.1; // margin top and bottom
  }

  if (initZRanges_) { // ranges have not been set yet, so set it according to data
    zranges_[0] = zMin - vertMargin*zTot; 
    zranges_[1] = zMax + vertMargin*zTot; 
    initZRanges_ = false;
  }
  else { // check in which directions ranges have to be extended
    if (zranges_[0] > zMin)
      zranges_[0] = zMin - vertMargin*zTot; 
    if (zranges_[1] < zMax)
      zranges_[1] = zMax + vertMargin*zTot; 
  }

  setRanges(xd, yd, 0.); // use this function to set the correct x and y ranges
  // now set all the ranges (also the z ranges)
  gr_.SetRanges(ranges_[0], ranges_[1], ranges_[2], ranges_[3], zranges_[0], zranges_[1]);
}

/* PRE : -
 * POST: if data contains something else than the nullptr in the axis ( = x, y or z ) coordinate return true, otherswise false */
bool has_data(const std::vector<std::unique_ptr<mglData>[3]>& data, const char axis)
{
  int axis_i;
  switch(axis){
    case 'x':
      axis_i = 0;
      break;
    case 'y':
      axis_i = 1;
      break;
    case 'z':
      axis_i = 2;
      break;
    default:
      throw std::logic_error("in function has_data : axis argument has invalid value\n If you don't know what that means contact the developer");
  }
  for (std::vector<std::unique_ptr<mglData>[3]>::const_iterator it = data.begin(); it != data.end(); ++it){
    if (*it[axis_i] == nullptr){
      return true;
    }
  }
  return false;
}

/* save figure
 * PRE : file must have '.eps' ending
 * POST: write figure to 'file' in eps-format */
void Figure::save(const char* file)
{
  if (has_data(data_, 'z')){ // we are dealing with a 3d plot -> set the Box
    gr_.Rotate(40, 60);
    gr_.Box();
  }
  // setting grid and axis (must be done in the end when final ranges are known)
  if (grid_){
    gr_.Grid(gridType_.c_str() , gridCol_.c_str());
  }
  if (axis_){
    gr_.Axis();
  }
  if (legend_){
    gr_.Legend(legendPos_.first, legendPos_.second);
  }

# if 1
  unsigned fIt(0), sIt(0); // iterator through fplot_ and styles_
  for(std::vector<std::unique_ptr<mglData>[3]>::iterator it = data_.begin(); it != data_.end(); ++it){
    if (*it[2] == nullptr){ // this means we have a 2d plot - will be the most common case
      gr_.Plot(*(*it[0]), *(*it[1]), styles_[sIt].c_str());
      ++sIt;
    }
    else if (*it[0] == nullptr){ // this means we have an fplot
      gr_.FPlot(fplots_[fIt], styles_[sIt].c_str());
      ++fIt; ++sIt;
    }
    else { // last case: 3d plot
      gr_.Plot(*(*it[0]), *(*it[1]), *(*it[2]), styles_[sIt].c_str());
      ++sIt;
    }
  }
# else 
  // plotting
  unsigned xIt(0), yIt(0), zIt(0), // x,y,z iterators
           fIt(0), // fplot iterator
           sIt(0); // style iterator
  for (std::vector<PlotType>::iterator it = plotKind_.begin(); it != plotKind_.end(); ++it){
    switch(*it){
      case plot2d:
        gr_.Plot(xd_[xIt], yd_[yIt], styles_[sIt].c_str());
        ++xIt; ++yIt; 
        ++sIt;
        break;
      case plot3d:
        gr_.Plot(xd_[xIt], yd_[yIt], zd_[zIt], styles_[sIt].c_str());
        ++xIt; ++yIt; ++zIt;
        ++sIt;
        break;
      case plotf:
        gr_.FPlot(fplots_[fIt].c_str(), styles_[sIt].c_str());
        ++fIt;
        ++sIt;
    }
  }
# endif
  if (autoRanges_ && has_data(data_, 'x') == 0 && fplots_.size() > 0){
    std::cout << "* Figure - Warning * fplot can't set proper ranges itself, it has to be done manually!\n";
  }

  gr_.WriteEPS(file);
}

/* (un-)set logscaling
 * PRE : -
 * POST: linear, semilogx, semilogy or loglog scale according to bools logx and logy */
void Figure::setlog(const bool logx, const bool logy, const bool logz)
{
 std::string x(""), y(""), z("");
  if (logx){
    x = "lg(x)";
    xlogScale_ = true;
  }
  if (logy){
    y = "lg(y)";
    ylogScale_ = true;
  }
  if (logz){
    z = "lg(z)";
    zlogScale_ = true;
  }

  std::cout << "options: " << x << y << z << "\n";

  gr_.SetFunc(x.c_str(), y.c_str(), z.c_str());
}

/* setting title
 * PRE : -
 * POST: title_ variable set to 'text' with small-font option ("@") */
void Figure::title(const char* text)
{
  std::stringstream ss;
  ss << "@{" << text << "}";
  gr_.Title(ss.str().c_str());
}
