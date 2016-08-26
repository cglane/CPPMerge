#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
using namespace std;

template <class T>
  class NamedObj{
  public:
    vector<T> elements;
    string name;
    void push(T const&);
};

template <class T>
  void  NamedObj<T>::push(T const& elem){
    elements.push_back(elem);
};

class trans {
public:
  trans(){
    lang = ""; num = 0;
  }
  trans(string d, double r){
    lang = d;
    num = r;
  }
  string lang;
  double num;
};

class term {
public:
  string key;
  double val;
  vector<trans> translations;
};

class groupObj{
  public:
    string group;
};

class langObj{
  public:
    string key;
    vector<groupObj> group;
};

class companyObj{
  public:
    string key;
    vector<langObj> lang;
};

term merge_terms(term & a, term & b);
