#pragma once

#include <tuple>
#include <typeinfo>
#include <algorithm>

namespace DCEngine {
  
  // Reference: http://codereview.stackexchange.com/questions/48344/yet-another-any-class-implementation-named-some
  template<typename A = store<>>
  class some : A {
    using id = size_t;

    template<typename T>
    struct type { static void id() {} };

    template<typename T>
    static id type_id() { return reinterpret_cast<id>(&type<T>::id); }

    template<typename T>
    using decay = typename std::decay<T>::type;

    template<typename T>
    using none = typename std::enable_if<!std::is_same<some, T>::value>::type;

    //-----------------------------------------------------------------------------

    struct base {
      virtual ~base() {}
      virtual bool is(id) const = 0;
      virtual base *copy(A&) const = 0;
      virtual base *move(A&, base*&) = 0;
      virtual void free(A&) = 0;
    } *p = nullptr;

    template<typename T>
    struct data : base, std::tuple<T> {
      using std::tuple<T>::tuple;

      T       &get()      & { return std::get<0>(*this); }
      T const &get() const& { return std::get<0>(*this); }

      bool is(id i) const override { return i == type_id<T>(); }

      base *copy(A &a) const override {
        return a.template copy<data>(get());
      }

      base *move(A &a, base *&p) override {
        return a.template move<data>(std::move(get()), p);
      }

      void free(A &a) override { a.free(this); }
    };

    //-----------------------------------------------------------------------------

    template<typename T>
    T &stat() { return static_cast<data<T>*>(p)->get(); }

    template<typename T>
    T const &stat() const { return static_cast<data<T> const*>(p)->get(); }

    template<typename T>
    T &dyn() { return dynamic_cast<data<T>&>(*p).get(); }

    template<typename T>
    T const &dyn() const { return dynamic_cast<data<T> const&>(*p).get(); }

    base *move(some &s) { return s.p->move(*this, s.p); }
    base *copy(some const &s) { return s.p->copy(*this); }

    base *read(some &&s) { return s.p ? move(s) : s.p; }
    base *read(some const &s) { return s.p ? copy(s) : s.p; }

    template<typename V, typename U = decay<V>, typename = none<U>>
    base *read(V &&v) { return A::template copy<data<U>>(std::forward<V>(v)); }

    template<typename X>
    some &assign(X &&x) {
      if (!p) p = read(std::forward<X>(x));
      else {
        some t{ std::move(*this) };
        try { p = read(std::forward<X>(x)); }
        catch (...) { p = move(t); throw; }
      }
      return *this;
    }

    void swap(some &s) {
      if (!p)   p = read(std::move(s));
      else if (!s.p) s.p = move(*this);
      else {
        some t{ std::move(*this) };
        try { p = move(s); }
        catch (...) { p = move(t); throw; }
        s.p = move(t);
      }
    }

    //-----------------------------------------------------------------------------

  public:
    some() {}
    ~some() { if (p) p->free(*this); }

    some(some &&s) : p{ read(std::move(s)) } {}
    some(some const &s) : p{ read(s) } {}

    template<typename V, typename = none<decay<V>>>
    some(V &&v) : p{ read(std::forward<V>(v)) } {}

    some &operator=(some &&s) { return assign(std::move(s)); }
    some &operator=(some const &s) { return assign(s); }

    template<typename V, typename = none<decay<V>>>
    some &operator=(V &&v) { return assign(std::forward<V>(v)); }

    friend void swap(some &s, some &r) { s.swap(r); }

    void clear() { if (p) { p->free(*this); p = nullptr; } }

    bool empty() const { return p; }

    template<typename T>
    bool is() const { return p ? p->is(type_id<T>()) : false; }

    template<typename T> T      &&_() && { return std::move(stat<T>()); }
    template<typename T> T       &_()      & { return stat<T>(); }
    template<typename T> T const &_() const& { return stat<T>(); }

    template<typename T> T      &&cast() && { return std::move(dyn<T>()); }
    template<typename T> T       &cast()      & { return dyn<T>(); }
    template<typename T> T const &cast() const& { return dyn<T>(); }

    template<typename T> operator T && () && { return std::move(_<T>()); }
    template<typename T> operator T      &()      & { return _<T>(); }
    template<typename T> operator T const&() const& { return _<T>(); }
  };

}