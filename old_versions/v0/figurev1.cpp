# include <iostream>
# include "figure.hpp"

/* constructor: set default style
 * PRE : pointer to mglGraph will not cease to exist until operations are performed on this Figure
 * POST: Axis is true -- Grid is true and in light grey -- Legend is false -- Ranges will be set automatically */
Figure::Figure(mglGraph* gr)
  : gr_(gr), 
    axis_(true), 
    grid_(true), 
    legend_(false), 
    ylabelActive_(false),
    gridType_(""), 
    gridCol_("h"), 
    initRanges_(true),
    autoRanges_(true),
    xd_(std::vector<mglData>()), 
    yd_(std::vector<mglData>()), 
    styles_(std::vector<std::string>())
{
  gr_->SetFontSizePT(7);
  ranges_[0] = 1; ranges_[1] = 1; ranges_[2] = 1; ranges_[3] = 1;
};

/* change grid settings 
 * PRE : - 
 * POST: No grid if on is false, grid style is gridType, grid color is gridCol.
 *       For those arguments which are not given default settings are used. */
void Figure::grid(const bool on, const char* gridType, const char* gridCol)
{
  if (on)
    grid_ = true;
  else 
    grid_ = false;
  
  if (gridType == 0)
    gridType_ = '-';
  else
    gridType_ = *gridType;

  if (gridCol == 0)
    gridCol_ = 'h';
  else
    gridCol_ = *gridCol;
}

void Figure::xlabel(const char* label, const double pos)
{
  gr_->Label('x', label, pos);
}

void Figure::ylabel(const char* label, const double pos)
{
  ylabelActive_ = true;
  gr_->Label('y', label, pos);
}

/* set or unset legend
 * PRE : -
 * POST: if on is true legend will be plotted, otherwise not */
void Figure::legend(const double xPos, const double yPos)
{
  if (std::abs(xPos) > 2 || std::abs(yPos) > 2)
    std::cout << "\n * Warning * Legend may be out of the graphic due to large xPos or yPos\n";
  legend_ = true;
  legendPos_ = std::pair<double, double>(xPos, yPos);
}

/* set ranges 
 * PRE : -
 * POST: new ranges will be: x = [xMin, xMax], y = [yMin, yMax] */
void Figure::ranges(const double xMin, const double xMax, const double yMin, const double yMax)
{
  if (xMin > xMax || yMin > yMax)
    throw std::range_error("In function Figure::ranges(): xMin must be smaller than xMax and yMin smaller than yMax!");

  autoRanges_ = false;
  gr_->SetRanges(xMin, xMax, yMin, yMax);
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

  if (initRanges_) { // ranges have not been set yet, so set it according to data
    ranges_[0] = xMin; ranges_[1] = xMax; ranges_[2] = yMin; ranges_[3] = yMax;
    initRanges_ = false;
  }
  else { // check in which directions ranges have to be extended
    if (ranges_[0] > xMin)
      ranges_[0] = xMin;
    if (ranges_[1] < xMax)
      ranges_[1] = xMax;
    if (ranges_[2] > yMin)
      ranges_[2] = yMin;
    if (ranges_[3] < yMax)
      ranges_[3] = yMax;
  }
  gr_->SetRanges(ranges_[0], ranges_[1], ranges_[2], ranges_[3]);
}

/* save figure
 * PRE : file must have '.eps' ending
 * POST: write figure to 'file' in eps-format */
void Figure::save(const char* file) const
{
  // setting everything:
  // using SubPlot because otherwise there are large margins -> SubPlot removes these margins
  // However if there is a ylabel we need the margin on the left, thats what "<" is for
  if (ylabelActive_)
    gr_->SubPlot(1,1,0,"<_");
  else
    gr_->SubPlot(1,1,0,"_");

  if (grid_)
    gr_->Grid(gridType_.c_str() , gridCol_.c_str());
  if (axis_)
    gr_->Axis();
  if (legend_)
    gr_->Legend(legendPos_.first, legendPos_.second);

  // plotting
  for (std::size_t i = 0; i < xd_.size(); ++i){
    gr_->Plot(xd_[i], yd_[i], styles_[i].c_str());
  }

  gr_->WriteEPS(file);
}

/* (un-)set logscaling
 * PRE : -
 * POST: linear, semilogx, semilogy or loglog scale according to bools logx and logy */
void Figure::setlog(const bool logx, const bool logy)
{
  if (logx && logy)
    gr_->SetFunc("lg(x)","lg(y)");
  else if (logx && !logy)
    gr_->SetFunc("lg(x)","");
  else if (!logx && logy)
    gr_->SetFunc("","lg(y)");
  else
    gr_->SetFunc("","");
}

void Figure::title(const char* text)
{
  gr_->Title(text);
}
