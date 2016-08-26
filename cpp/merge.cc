#include "merge.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

trans merge (trans &a, trans & b){
	string lang;
	double num;

	lang = b.lang;
	num = b.num;

	return trans(lang, num);
}

term merge_terms(term &a, term &b){
	//merge a and b give preference for b
	term result;
	result.key = b.key;
	result.val = b.val;
	bool resultChanged;
		for(auto &&transA : a.translations){
			resultChanged = false;
			for(auto &&transB : b.translations){
				if(transA.lang == transB.lang){
					result.translations.push_back(merge(transA,transB));
					resultChanged = true;
				}
			}
			if(!resultChanged){
				result.translations.push_back(transA);
			}
		}
	return result;
}

void merge_nested(vector<term> & a, vector<term> &b, vector<term> &mergedArr,size_t interval){
	bool matchedKey = true;
	for(size_t i = 0; i < b.size(); i++){
		if(a[interval].key == b[i].key){
			term mergedTerm = merge_terms(a[interval], b[i]);
			mergedArr.push_back(mergedTerm);
			matchedKey = true;
			break;
		}else {
			matchedKey = false;
		}
	}
	if(!matchedKey) mergedArr.push_back(a[interval]);
	if(++interval < a.size())merge_nested(a, b, mergedArr, interval);
	else return;
}
void map_values(vector<term> &a, vector<term>&b, map<string, int> &myMap, size_t interval){
	bool matchedKey = true;
	bool matchedLang = true;
	for(auto &&termB: b){
		if(a[interval].key == termB.key){
			for(auto &&transA : a[interval].translations){
				for(auto && transB: termB.translations){
					if(transA.lang == transB.lang){
						myMap[transB.lang] = transB.num;
						matchedLang = true;
						break;
					}else matchedLang = false;
				}
				if(!matchedLang) {myMap[transA.lang] = transA.num;}
			}
			matchedKey = true;
			break;
		}else matchedKey = false;
	}
	if(!matchedKey){
		for(auto && aTrans : a[interval].translations){
			myMap[aTrans.lang] = aTrans.num;
		}
	}
	if(++interval < a.size())map_values(a,b, myMap,interval);
	else return;
}
