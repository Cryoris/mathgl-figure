# include <iostream>
# include <utility> // pair
# include <sstream> // needed for title layout
# include <cstring> // needed for length of const char* 
# include "figure.hpp"

/* constructor: set default style
 * PRE : pointer to mglGraph will not cease to exist until operations are performed on this Figure
 * POST: Axis is true -- Grid is true and in light grey -- Legend is false -- Ranges will be set automatically */
Figure::Figure()
  : axis_(true), 
    grid_(true), 
    legend_(false), 
    gridType_(""), 
    gridCol_("h"), 
    initRanges_(true),
    autoRanges_(true),
    ylogScale_(false),
    fontSizePT_(8),
    xd_(std::vector<mglData>()), 
    yd_(std::vector<mglData>()), 
    styles_(std::vector<std::string>())
{
  gr_.SubPlot(1,1,0,"_");
  gr_.SetTuneTicks(true, 1.0); // put the 'x10^5' closer to the graph
  gr_.LoadFont("heros");
  gr_.SetFontSizePT(fontSizePT_);
  ranges_[0] = 1; ranges_[1] = 1.1; ranges_[2] = 1; ranges_[3] = 1.1;
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
    std::cout << "* Figure - Warning * for better text-alignment, xlabel should be called before setlog\n";
  }
  gr_.Label('x', label, pos);
}

/* setting y-axis label
 * PRE : -
 * POST: ylabel initialized with given position */
void Figure::ylabel(const char* label, const double pos)
{
  if (ylogScale_){
    std::cout << "* Figure - Warning * for better text-alignment, ylabel should be called before setlog\n";
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
  fplots_.push_back(function);
  fstyles_.push_back(style);
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
void Figure::setRanges(const mglData& xd, const mglData& yd)
{
  if (!autoRanges_)
    return;

  const double xMin(xd.Minimal()), xMax(xd.Maximal()),
               yMin(yd.Minimal()), yMax(yd.Maximal());

  const double xTot = xMax - xMin;
  const double yTot = yMax - yMin;
  // width/height of additional margin (in percentage) of the total width/height
  double vertMargin;
  if (ylogScale_){
    // if vertMargin would be > 0, we may set the lower yrange to 0 or even < 0 which is not allowed in logscales
    vertMargin = 0.0; 
  }
  else {
    vertMargin = 0.1; // margin top and bottom
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

/* save figure
 * PRE : file must have '.eps' ending
 * POST: write figure to 'file' in eps-format */
void Figure::save(const char* file)
{
  // setting grid and axis (must be done in the end when final ranges are known)
  if (grid_)
    gr_.Grid(gridType_.c_str() , gridCol_.c_str());
  if (axis_)
    gr_.Axis();
  if (legend_)
    gr_.Legend(legendPos_.first, legendPos_.second);

  // plotting
  for (std::size_t i = 0; i < xd_.size(); ++i){
    gr_.Plot(xd_[i], yd_[i], styles_[i].c_str());
  }
  for (std::size_t j = 0; j < fplots_.size(); ++j){
    gr_.FPlot(fplots_[j].c_str(), fstyles_[j].c_str());
  }

  if (autoRanges_ && xd_.size() == 0 && fplots_.size() > 0){
    std::cout << "* Figure - Warning * fplot can't set proper ranges itself, it has to be done manually!\n";
  }

  gr_.WriteEPS(file);
}

/* (un-)set logscaling
 * PRE : -
 * POST: linear, semilogx, semilogy or loglog scale according to bools logx and logy */
void Figure::setlog(const bool logx, const bool logy)
{
  if (logx && logy){
    gr_.SetFunc("lg(x)","lg(y)");
    xlogScale_ = true;
    ylogScale_ = true;
  }
  else if (logx && !logy){
    gr_.SetFunc("lg(x)","");
    xlogScale_ = true;
  }
  else if (!logx && logy){
    gr_.SetFunc("","lg(y)");
    ylogScale_ = true;
  }
  else {
    gr_.SetFunc("","");
  }
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
