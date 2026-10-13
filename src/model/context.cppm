export module mvc:context;
export import :types;
export import :sets;
export import :functions;
export import :statics_model;

import std;

export {

  template<std::floating_point T> class Context
  {
  public:   
    
    u_ptr<ConvexSet<T>> const m_domain_ptr { std::make_unique<ConvexSet<T>>() };

    Context(Functor<T> map_ref, const ConvexSet<T>& domain_ref);

    ~Context() { if (mvc::DEBUG_EXECUTIVE) std::println("Context destructor"); }
    
    Context(const Context<T>& set) = default; 
    Context<T>& operator=(const Context<T>& rhs) = default; 
    Context(Context<T>&& moved) noexcept = default;
    Context<T>& operator=(Context<T>&& rhs) = default;       

    void set_resolution(unsigned resolution);
    void init_range(std::size_t parts);
    
    s_ptr<range_t<T>> const get_range_ptr();

    Functor<T> get_updated_map();
    void write_map_and_params(const Functor<T>& functor);
 
    s_ptr<params_t<T>> const write_domain(const ConvexSet<T>& domain);

    s_ptr<params_t<T>> const get_domain_parameters_ptr();
    s_ptr<params_t<T>> const get_map_parameters_ptr();

    friend const std::string to_string<>(const Context<T>& context);
    
  private:
    std::size_t m_parts { std::thread::hardware_concurrency() };

    Functor<T> m_map;
    s_ptr<params_t<T>> m_map_params_ptr { params_t<T>::create() };    
    s_ptr<range_t<T>> m_range_ptr { range_t<T>::create() };
  };
}

/** exports depending on own classes */

template<std::floating_point T>
Context<T>::Context(Functor<T> map_ref, const ConvexSet<T>& domain_ref)
  : m_map {map_ref}    
{
  if (mvc::DEBUG_EXECUTIVE) std::println("m_domain_ptr.get(): {}", static_cast<void*>(m_domain_ptr.get()));
  
  *m_domain_ptr = domain_ref;

  m_map_params_ptr->m_values = map_ref.get_parameters();
}

template<std::floating_point T>
void Context<T>::set_resolution(unsigned resolution)
{
  m_domain_ptr->set_resolution(resolution);
}

/** Range **/
template<std::floating_point T>
void Context<T>::init_range(std::size_t parts)
{
  unsigned resolution = m_domain_ptr->get_resolution();
    
  m_range_ptr->m_range = set_t<T>(parts, path_t<T>(resolution-1, cplx_t<T>(0,0)));

  if (mvc::DEBUG_EXECUTIVE) std::println("Range {}x{} initialized.", m_range_ptr->m_range.size(),
					 ((m_range_ptr->m_range)[0]).size());
}

template<std::floating_point T>
s_ptr<range_t<T>> const Context<T>::get_range_ptr()
{
  return m_range_ptr->get_ptr();
}

/** Map **/
template<std::floating_point T>
Functor<T> Context<T>::get_updated_map()
{
  m_map.write_parameters(m_map_params_ptr->m_values);
  
  return m_map;
}

template<std::floating_point T>
void  Context<T>::write_map_and_params(const Functor<T>& map)
{
  m_map = map;
  m_map_params_ptr->m_values = m_map.get_parameters();  
}

template<std::floating_point T>
s_ptr<params_t<T>> const Context<T>::get_map_parameters_ptr()
{
  return m_map_params_ptr->get_ptr();
}

/** Domain **/
template<std::floating_point T>
s_ptr<params_t<T>> const Context<T>::write_domain(const ConvexSet<T>& domain)
{
  *m_domain_ptr = domain;
  return m_domain_ptr->get_parameters_ptr();
}

template<std::floating_point T>
s_ptr<params_t<T>> const Context<T>::get_domain_parameters_ptr()
{
  return m_domain_ptr->get_parameters_ptr();
}
