#include <node.h>
#include <v8.h>
#include <uv.h>
#include "merge.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <map>
using namespace v8;

term unpack_term(Isolate * , const Handle<Object> sample_term);
trans unpack_trans(Isolate * , const Handle<Object> sample_trans);
void pack_term_result(v8::Isolate * isolate, v8::Local<v8::Object> &target, term & localTerm);
void pack_translation_result(v8::Isolate * isolate, v8::Local<v8::Object> &target, trans & translation);
void merge_nested(vector<term> &a, vector<term>&b,vector<term>&mergedArr, size_t interval);
void pack_map_result(v8::Isolate *isolate, v8::Local<v8::Object> &target, map<string,int> &myMap);
void map_values(vector<term> &a, vector<term>&b, map<string,int>&myMap, size_t interval);

trans unpack_trans(Isolate * isolate, const Handle<Object> trans_obj){
  trans local_trans;
  Handle<Value> lang_Value = trans_obj-> Get(String::NewFromUtf8(isolate, "lang"));
  Handle<Value> num_Value = trans_obj-> Get(String::NewFromUtf8(isolate, "num"));

  v8::String::Utf8Value utfValue(lang_Value);
  local_trans.lang = std::string(*utfValue);

  local_trans.num = num_Value->NumberValue();
  return local_trans;
}

term unpack_term(Isolate *isolate, const Handle<Object> term_obj){
  term localTerm;
  Handle<Value> key_val = term_obj->Get(String::NewFromUtf8(isolate,"key"));
  Handle<Value> num_val = term_obj->Get(String::NewFromUtf8(isolate,"val"));
  v8::String::Utf8Value utfValue(key_val);
  localTerm.key = std::string(*utfValue);
  localTerm.val = num_val->NumberValue();
  Handle<Array> array = Handle<Array>::Cast(term_obj->Get(String::NewFromUtf8(isolate,"translations")));
  int translations_count = array->Length();
  for (int i = 0; i < translations_count ; i++) {
    trans t = unpack_trans(isolate, Handle<Object>::Cast(array->Get(i)));
    localTerm.translations.push_back(t);
  }
  return localTerm;
}
void pack_map_result(v8::Isolate* isolate, v8::Local<v8::Object>&target, map<string,int>& myMap){
  for(auto &&keyVal : myMap){
    std::string key(keyVal.first);
    target->Set(String::NewFromUtf8(isolate,key.c_str()), Number::New(isolate,keyVal.second));
  }
}
void pack_translation_result(v8::Isolate* isolate, v8::Local<v8::Object> &target, trans & localTrans){
  std::string basicStr(localTrans.lang);
  target->Set(String::NewFromUtf8(isolate,"lang"), String::NewFromUtf8(isolate,basicStr.c_str()));
  target->Set(String::NewFromUtf8(isolate,"num"), Number::New(isolate, localTrans.num));
}

void pack_term_result(v8::Isolate* isolate, v8::Local<v8::Object> &target, term & localTerm){
  std::string  basicStr(localTerm.key);
  target->Set(String::NewFromUtf8(isolate, "key"), String::NewFromUtf8(isolate,basicStr.c_str()));
  target->Set(String::NewFromUtf8(isolate,"val"),Number::New(isolate,localTerm.val));
  Local<Array> transArray = Array::New(isolate);
  for(unsigned int i = 0; i<localTerm.translations.size();i++){
    Local<Object>  transObj = Object::New(isolate);
    pack_translation_result(isolate,transObj,localTerm.translations[i]);
    transArray->Set(i,transObj);
  }
  target->Set(String::NewFromUtf8(isolate,"translations"), transArray);
}

void MergeTerms( const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate* isolate = args.GetIsolate();
  term termA = unpack_term(isolate, Handle<Object>::Cast(args[0]));
  term termB = unpack_term(isolate, Handle<Object>::Cast(args[1]));

  Local<Object>  rtnObj = Object::New(isolate);
  term mergedObject = merge_terms(termA,termB);

  pack_term_result(isolate, rtnObj, mergedObject);
  args.GetReturnValue().Set(rtnObj);
}

void MergeNestedTerms(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate *isolate = args.GetIsolate();
  vector<term> standard;
  vector<term> special;
  //Handle Two Arrays
  Handle<Array> arrA = Handle<Array>::Cast(args[0]);
  Handle<Array> arrB = Handle<Array>::Cast(args[1]);
  for( unsigned int i = 0; i< arrA->Length(); i++){
    Local<Value> element = arrA->Get(i);
    if(element->IsObject()){
        Handle<Object> b = Handle<Object>::Cast(element);
        standard.push_back(unpack_term(isolate, b));
    }
  }
  for(unsigned int i = 0; i< arrB->Length(); i++){
    Local<Value> element = arrB->Get(i);
    if(element->IsObject()){
        Handle<Object> b = Handle<Object>::Cast(element);
        special.push_back(unpack_term(isolate, b));
    }
  }
  vector<term> mergedArray;
  size_t interval = 0;
  merge_nested(standard, special, mergedArray, interval);

  Local<Array> rtnArray = Array::New(isolate);
  for(size_t i = 0; i<mergedArray.size();i++){
    Local<Object> rtnObj = Object::New(isolate);
    pack_term_result(isolate,rtnObj,mergedArray[i]);
    rtnArray->Set(i, rtnObj);
  }
  args.GetReturnValue().Set(rtnArray);
}

void MapTranslations(const v8::FunctionCallbackInfo<v8::Value> &args){
  Isolate *isolate = args.GetIsolate();
  vector<term> standard;
  vector<term> special;
  //Handle Two Arrays
  Handle<Array> arrA = Handle<Array>::Cast(args[0]);
  Handle<Array> arrB = Handle<Array>::Cast(args[1]);
  for( unsigned int i = 0; i< arrA->Length(); i++){
    Local<Value> element = arrA->Get(i);
    if(element->IsObject()){
        Handle<Object> b = Handle<Object>::Cast(element);
        standard.push_back(unpack_term(isolate, b));
    }
  }
  for(unsigned int i = 0; i< arrB->Length(); i++){
    Local<Value> element = arrB->Get(i);
    if(element->IsObject()){
        Handle<Object> b = Handle<Object>::Cast(element);
        special.push_back(unpack_term(isolate, b));
    }
  }
  map<string,int> myMap;
  size_t interval = 0;
  map_values(standard, special, myMap, interval);

  Local<Object> rtnObj = Object::New(isolate);
  pack_map_result(isolate,rtnObj,myMap);
  args.GetReturnValue().Set(rtnObj);
}
void init(Handle <Object> exports, Handle<Object> module) {
    NODE_SET_METHOD(exports, "merge_terms", MergeTerms);
    NODE_SET_METHOD(exports, "merge_nested", MergeNestedTerms);
    NODE_SET_METHOD(exports, "map_values", MapTranslations);
}

NODE_MODULE(merge, init)
