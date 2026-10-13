export module mvc:functions;
import :types;
import :statics_model;

import std;

export template<std::floating_point T> class Functor
{
public:
    
  Functor() = default;
  ~Functor()
  {
    if (mvc::DEBUG_OBJECTS) std::println("Quadratic destructor");
  }
  
  Functor(const Functor<T>& copied)
  {
    m_params = copied.m_params;
    m_procedure = copied.m_procedure;
  }
  
  Functor<T>& operator=(const Functor<T>& copied)
  {
    m_params = copied.m_params;
    m_procedure = copied.m_procedure;

    return *this;
  }

  Functor(Functor<T>&& moved) = delete;
  Functor<T>& operator=(Functor<T>&& moved) = delete;

  cplx_t<T> operator()(const cplx_t<T>& z) const
  {
    return this->m_procedure(z, m_params);
  }

  void write_parameters(const struct_t<T>& parameters)
  {
    m_params = parameters;
  }

  struct_t<T> get_parameters()
  {
    return m_params;
  }

  friend const std::string to_string<>(const Functor<T>& functor);
  
protected:
  struct_t<T> m_params;
  func_t<T> m_procedure;
};

export template<std::floating_point T> class Quadratic : public Functor<T> 
{

public:
  using Functor<T>::Functor;
  
  Quadratic(std::vector<cplx_t<T>>& coefficients);

  using Functor<T>::operator();

  friend const std::string to_string<>(const Quadratic<T>& functor);
  
protected:
  using Functor<T>::m_params;
  using Functor<T>::m_procedure;
};

/** Quadratic */

template<std::floating_point T>
Quadratic<T>::Quadratic(std::vector<cplx_t<T>>& coefficients)
{
  m_params.a = coefficients[0];
  m_params.b = coefficients[1];
  m_params.c = coefficients[2];

  m_procedure = [](const cplx_t<T>& z, struct_t<T> params)
  {
    const cplx_t<T> u {params.a + (params.b)*z + (params.c)*z*z};
    
    return u;
  };
}  
