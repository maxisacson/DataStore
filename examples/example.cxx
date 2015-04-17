#include <iostream>
#include <string>

#include "DataStore.h"

int main() {
  /**
   * create a store
   */
  ds::DataStore store;

  /**
   * we can put floats, ints, strings etc. in the store
   */
  store.add<float>("a_float");
  store.add<int>("an_int");
  store.add<std::string>("a_string");

  /**
   * we can access them and change their values
   */
  float& my_float = store.retrieve<float>("a_float");
  my_float = 3.1415;

  int& my_int = store.retrieve<int>("an_int");
  my_int = 7;

  std::string& my_string = store.retrieve<std::string>("a_string");
  my_string = "hello world";

  /**
   * this will update their value in storage, as illustrated below
   */
  float& my_float_again = store.retrieve<float>("a_float");
  std::cout << my_float_again << std::endl; // "3.1415"

  int& my_int_again = store.retrieve<int>("an_int");
  std::cout << my_int_again << std::endl; // "7"

  std::string& my_string_again = store.retrieve<std::string>("a_string");
  std::cout << my_string_again << std::endl; // "hello world"

  /**
   * we could also store a value and key at the same time
   */
  store.add<const char*>("my favourite colour", "green");
  std::cout << store.retrieve<const char*>("my favourite colour") << std::endl; // "green"

  /**
   * vectors can also be stored
   */
  std::vector<const char*> my_vec = {"monday", "tuesday", "wednesday",
      "thursday", "friday", "saturday", "sunday"};
  store.add<std::vector<const char*> >("the week", my_vec);

  std::vector<const char*>& the_week = store.retrieve<std::vector<const char*> >("the week");
  for (auto& d : the_week) {
    std::cout << d << " "; // "monday, tuesday, etc..."
  }
  std::cout << std::endl;

  /**
   * if we want we can also access the de facto data item being stored. Remember to cast!
   */
  ds::DataItem<float>* item = static_cast<ds::DataItem<float>*>(store.retrieveItem("a_float"));
  std::cout << item->Key() << " : " << item->Value() << std::endl; // "a_float : 3.1415"

  ds::DataVector<const char*>* vec_item = static_cast<ds::DataVector<const char*>*>(store.retrieveItem("the week"));
  std::cout << vec_item->Key() << " : " << vec_item->Value().at(0) << std::endl; // "the week : monday"


  return 0;
}
