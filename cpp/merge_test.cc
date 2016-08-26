#include <iostream>
#include "merge.h"
#include <map>
using namespace std;
void merge_nested(vector<term> &a, vector<term>&b,vector<term>&mergedArr, size_t interval);
void map_values(vector<term> &a,vector<term>&b,map<string,int> & map, size_t interval);
int main() {
vector<term> standard, special, merged;
map<string,int> myMap;
size_t interval = 0;

term a,b,c,d;
  a.key = "me";
  b.key = "me";
  c.key = "he";
  d.key = "he";
  a.translations.push_back(trans("english",1));
  a.translations.push_back(trans("portugese",5));
  a.translations.push_back(trans("spanish",9));
  a.translations.push_back(trans("american",9));

  c.translations.push_back(trans("english",1));
  c.translations.push_back(trans("portugese",5));
  c.translations.push_back(trans("spanish",9));
  c.translations.push_back(trans("american",9));

  b.translations.push_back(trans("english",2));
  b.translations.push_back(trans("spanish",9));
  b.translations.push_back(trans("portugese",5));

  d.translations.push_back(trans("english",2));
  d.translations.push_back(trans("spanish",9));
  d.translations.push_back(trans("portugese",5));

  standard.push_back(a);
  standard.push_back(c);
  special.push_back(b);
  special.push_back(d);
  // merge_nested(standard,special,merged,interval);
  map_values(standard,special,myMap, interval);
  for(auto &&item : myMap ){
    cout<<item.first<<item.second<<endl;
  }
}
