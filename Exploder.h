#ifndef EXPLODER_H_
#define EXPLODER_H_

namespace exploder {

template <typename>
struct Exploder;

template <typename T>
struct Exploder {
  typedef T type_1;
};

template <typename T, template <typename> class Template>
struct Exploder<Template<T> > {
  typedef T type_1;
};

template <typename T, typename U, template <typename, typename> class Template>
struct Exploder<Template<T, U> > {
  typedef T type_1;
  typedef U type_2;
};

}
#endif /* EXPLODER_H_ */
