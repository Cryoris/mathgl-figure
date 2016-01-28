# include <memory>
# include <iostream>
# include <vector>

class Foo {
  public:
    Foo(int i) : i_(i) { std::cout << "Constructor!\n"; }
    ~Foo(){ std::cout << "Destructor!\n"; }
  
  private:
    int i_;
};

class Pt {
  public:
    void add(Foo f){
      std::unique_ptr<Foo> p(new Foo(f));
      ptrs.push_back(std::move(p));
    }

  private:
    std::vector<std::unique_ptr<Foo>> ptrs;
};

int main()
{
  Pt pt;
  pt.add(Foo(2));
  pt.add(Foo(4));
  pt.add(Foo(5));

  return 0;
}
