#ifndef MGL_PLOT_H
#define MGL_PLOT_H

#include <mgl2/mgl.h>

namespace mgl {

class MglPlot {
public:
  virtual void plot(mglGraph* gr) = 0;
  virtual bool is_3d() = 0;
};

class MglPlot2d : public MglPlot {
public:

  MglPlot2d(const mglData& xd, const mglData& yd, const std::string &style)
    : xd_(xd)
    , yd_(yd)
    , style_(style)
  {}

  void plot(mglGraph * gr) {
    gr->Plot(xd_, yd_, style_.c_str());
  }

  bool is_3d() {
    return false;
  }

private:
  mglData xd_;
  mglData yd_;
  std::string style_;
};

class MglPlot3d : public MglPlot {
public:

  MglPlot3d(const mglData& xd, const mglData& yd, const mglData& zd, const std::string &style)
    : xd_(xd)
    , yd_(yd)
    , zd_(zd)
    , style_(style)
  {}

  void plot(mglGraph * gr) {
    gr->Plot(xd_, yd_, zd_, style_.c_str());
  }

  bool is_3d() {
    return true;
  }

private:
  mglData xd_;
  mglData yd_;
  mglData zd_;
  std::string style_;
};

class MglFPlot : public MglPlot {
public:

  MglFPlot(const std::string & fplot_str, const std::string &style)
    : fplot_str_(fplot_str)
    , style_(style)
  {}

  void plot(mglGraph * gr) {
    gr->FPlot(fplot_str_.c_str(), style_.c_str());
  }

  bool is_3d() {
    return false;
  }

private:
  std::string fplot_str_;
  std::string style_;
};


} // end namespace
#endif
