#ifndef DATASTORE_H_
#define DATASTORE_H_

#include <vector>
#include <unordered_map>
#include <type_traits>

#include "Exploder.h"

namespace ds {

template <typename T> struct is_vector : public std::false_type{};
template <typename T, typename A>
struct is_vector<std::vector<T, A> > : public std::true_type{};

class DataBase {
public:
  virtual ~DataBase(){}
};

template <typename T>
class DataType {
public:
  typedef T type;
};

template <typename T>
class DataItem : public DataBase {
  friend class DataStore;
public:
  DataItem():m_key(""){}
  DataItem(const char* key):m_key(key){}
  DataItem(const char* key, T& value):
    m_key(key),
    m_value(value)
  {}
  ~DataItem(){}
  DataItem& operator= (const T& value) {
    this->m_value = value;
    return *this;
  }
  const char*& Key() {return m_key;}
  T& Value() {return m_value;}

protected:
  const char* m_key;
  T m_value;
  DataType<T> type;
};

template <typename T, typename A = std::allocator<T> >
class DataVector : public DataBase {
  friend class DataStore;
public:
  DataVector():m_key(""){}
  DataVector(const char* key):m_key(key){}
  DataVector(const char* key, std::vector<T, A> & value):
    m_key(key),
    m_value(value)
  {}
  ~DataVector(){}
  DataVector& operator= (const std::vector<T, A>& value) {
    this->m_value = value;
    return *this;
  }
  const char*& Key() {return m_key;}
  std::vector<T, A>& Value() {return m_value;}

protected:
  const char* m_key;
  std::vector<T, A> m_value;
  DataType<T> type;
};

class DataStore {
public:

  ~DataStore(){
    for (const auto& itr : m_storage) {
      if (itr.second)
        delete itr.second;
    }
  }
  /**
   * storage methods
   */
  void add(const char* key) {
    m_storage.insert(
        std::make_pair(key, new DataBase())
    );
  }

  template <typename T>
  void add(const char* key) {
    putInStorage<T>(key, is_vector<T>{} );
  }

  template <typename T>
  void add(const char* key, T value) {
    typedef typename exploder::Exploder<T>::type_1 Type;
    putInStorage<Type>(key, value, is_vector<T>{} );
  }

  /**
   * retrieval methods
   */
  template <typename T>
  T& retrieve(const char* key) {
    T& ret = retrieveFromStorage<T>(key, is_vector<T>{} );
    return ret;
  }

  DataBase*& retrieveItem(const char* key) {
    const auto& itr = m_storage.find(key);
    return (*itr).second;
  }

private:
  /**
   * storage methods
   */
  template <typename T>
  void putInStorage(const char* key, std::false_type) {
    m_storage.insert(
        std::make_pair(key, new DataItem<T>(key))
    );
  }

  template <typename T>
  void putInStorage(const char* key, T& value, std::false_type) {
    m_storage.insert(
        std::make_pair(key, new DataItem<T>(key, value))
    );
  }

  template <typename T>
  void putInStorage(const char* key, std::true_type) {
    typedef typename exploder::Exploder<T>::type_1 Type;
    typedef typename exploder::Exploder<T>::type_2 Alloc;
    m_storage.insert(
        std::make_pair(key, new DataVector<Type, Alloc>(key))
    );
  }

  template <typename T>
  void putInStorage(const char* key, std::vector<T>& vec, std::true_type) {
    m_storage.insert(
        std::make_pair(key, new DataVector<T, std::allocator<T> >(key, vec))
    );
  }

  /**
   * retrieval methods
   */
  template <typename T>
  T& retrieveFromStorage(const char* key, std::false_type) {
    const auto& itr = m_storage.find(key);
    DataItem<T>* item = static_cast<DataItem<T>* >((*itr).second);
    return item->m_value;
  }

  template <typename T>
  std::vector<typename exploder::Exploder<T>::type_1, typename exploder::Exploder<T>::type_2>&
  retrieveFromStorage(const char* key, std::true_type) {
    const auto& itr = m_storage.find(key);
    typedef typename exploder::Exploder<T>::type_1 Type;
    typedef typename exploder::Exploder<T>::type_2 Alloc;
    DataVector<Type, Alloc>* item = static_cast<DataVector<Type, Alloc>* >((*itr).second);
    return item->m_value;
  }

private:
  std::unordered_map<const char*, DataBase*> m_storage;
};

}
#endif /* DATASTORE_H_ */
