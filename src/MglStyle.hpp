# include <deque>
# include <string>
# include <algorithm>
# include <mgl2/mgl.h>

/* create cross join B x A, such that:
 * A = {0, 1}, B = {a, b, bc} 
 * => C = {0a, 1a, 0b, 1b, 0c, 1c}     */
void crossjoin (const std::deque<std::string>& A, const std::deque<std::string>& B, std::deque<std::string>& res) 
{
  for (auto b : B) {
    for (auto a : A) {
      res.push_back(a + b);
    }
  }
}

class MglStyle {
  public: 
    void get_new (std::deque<std::string>& new_deque);

    MglStyle();

    MglStyle(const std::string& already_used);

    template <class Container>
    MglStyle(const Container& already_used);

    std::string get_next();

    void eliminate (const std::string& already_used);

  private:
    std::deque<std::string> styles_;
};

void MglStyle::get_new (std::deque<std::string>& new_deque) 
{
  std::deque<std::string> colors = { "b", "r", "g", "c", "m" },
                        linetypes = { "-", ":", ";", "|" };
  crossjoin(colors, linetypes, new_deque);
}

/* create new styles_ deque with styles from get_new */
MglStyle::MglStyle() 
{
  get_new(styles_);
}

/* create new styles_ deque and remove 'already_used' */
MglStyle::MglStyle (const std::string& already_used) 
{
  get_new(styles_);
  // if already_used is contained in styles_ remove it
  eliminate(already_used);
}
  
/* creates new styles_ deque and remove all strings in already_used */
template <class Container>
MglStyle::MglStyle (const Container& already_used_cont) 
{
 get_new(styles_);

 // iterate over all strings in already_used_cont and remove them 
 for (auto already_used : already_used_cont) {
    eliminate(already_used);
 }
 
 // if all available styles have been used start from the beginning
 if (styles_.size() == 0) {
   get_new(styles_);
 }
}

std::string MglStyle::get_next ()
{
  // if all available styles have been used start from the beginning
  if (styles_.size() == 0) {
    get_new(styles_);
  }
  std::string next = styles_[0];
  styles_.pop_front();
  return next;
}

void MglStyle::eliminate (const std::string& already_used) {
  auto it = std::find(styles_.begin(), styles_.end(), already_used);
  if (it != styles_.end()) {
    styles_.erase(it);
  }
}
