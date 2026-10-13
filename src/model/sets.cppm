export module mvc:sets;
import :types;
import :statics_model;

import std;

/** classes
 */

export {
    
  template<std::floating_point T> class AbstractCurve
  {
  public:
    
    AbstractCurve()
    {
      if (mvc::DEBUG_OBJECTS) std::println("AbstractCurve default constructor");
    }

    AbstractCurve(const curve_t<T>& curve, const struct_t<T>& params_ref)
      : m_curve {curve}
    {
      *m_params_ptr = params_ref;
      
      if (mvc::DEBUG_OBJECTS) std::println("AbstractCurve two-parameter constructor.");
    }

    ~AbstractCurve() = default;
    
    AbstractCurve(const AbstractCurve<T>& copied) = default;
    AbstractCurve<T>& operator=(const AbstractCurve<T>& copied) = default;
    AbstractCurve(AbstractCurve<T>&& moved) noexcept = default;
    AbstractCurve<T>& operator=(AbstractCurve<T>&& moved) = default;

    cplx_t<T> const value(const T t) const { return this->m_curve(t); }
    unsigned get_resolution() const { return m_resolution; }
    void set_resolution(unsigned r) { m_resolution = r; }

    // modifies this when handing out shared pointer
    s_ptr<params_t<T>> const get_parameters_ptr() { return m_params_ptr->get_ptr(); } 

    const path_t<T> samples(unsigned resolution) const;
    
    friend const std::string to_string<>(const AbstractCurve<T>& curve);
    
  protected:
    unsigned m_resolution {0};
    curve_t<T> m_curve {nullptr}; 
    s_ptr<params_t<T>> m_params_ptr {params_t<T>::create()};    
  };
  
  template<std::floating_point T> class Line : public AbstractCurve<T>
  {
  public:

    using AbstractCurve<T>::AbstractCurve;

    Line(const cplx_t<T>& start, const cplx_t<T>&);

    using AbstractCurve<T>::value;
    using AbstractCurve<T>::samples;
    
    using AbstractCurve<T>::set_resolution;
    using AbstractCurve<T>::get_resolution;
    using AbstractCurve<T>::get_parameters_ptr;
    
    friend const std::string to_string<>(const Line<T>& curve);
    
  protected:
    using AbstractCurve<T>::m_resolution;  // since we are templates inheritance must be respecified
    using AbstractCurve<T>::m_curve;
    using AbstractCurve<T>::m_params_ptr;
  };

  template<std::floating_point T> class ConvexSet : public AbstractCurve<T>
  {
  public:

    using AbstractCurve<T>::AbstractCurve;

    using AbstractCurve<T>::set_resolution;
    using AbstractCurve<T>::get_resolution;
    using AbstractCurve<T>::get_parameters_ptr;
    
    virtual const path_t<T> boundary() const;

    /**
       The heavy burden of the application is carried out in this method. Consider where to
       locate it. Perhaps locate it properly and return pointers instead.
       Therefore, since it's parameter space is tiny in comparison to domain data generated
       here, the parameters can be kept in each object and be exposed with a pointer.
       Functor parameters is copied into the object by value when updated.
     */
    virtual const set_t<T> homotopic_representation(const std::size_t parts) const;

    friend const std::string to_string<>(const ConvexSet<T>& set);
    
  protected:    
    using AbstractCurve<T>::m_resolution;  // since we are templates inheritance must be respecified
    using AbstractCurve<T>::m_curve;  
    using AbstractCurve<T>::m_params_ptr;
  };

  template<std::floating_point T> class Rectangle : public ConvexSet<T>
  {
  public:

    using ConvexSet<T>::ConvexSet;
    
    Rectangle(const cplx_t<T>& start, const cplx_t<T>& end, const T height, const bool orientation);

    using ConvexSet<T>::set_resolution;
    using ConvexSet<T>::get_resolution;
    using ConvexSet<T>::get_parameters_ptr;
    
    using ConvexSet<T>::boundary;
    using ConvexSet<T>::homotopic_representation;

    friend const std::string to_string<>(const Rectangle<T>& curve);
    
    const path_t<T> corners() const;
    const cplx_t<T> centre() const;

  protected:    
    using ConvexSet<T>::m_resolution;  // since we are templates inheritance must be respecified    
    using ConvexSet<T>::m_curve;  
    using ConvexSet<T>::m_params_ptr;
  };

  template<std::floating_point T> class Ellipse : public ConvexSet<T>
  {
  public:

    using ConvexSet<T>::ConvexSet;
    
    Ellipse(const cplx_t<T>& centre, const cplx_t<T>& horisontal, T height);

    using ConvexSet<T>::set_resolution;
    using ConvexSet<T>::get_resolution;
    using ConvexSet<T>::get_parameters_ptr;
    
    using ConvexSet<T>::boundary;
    using ConvexSet<T>::homotopic_representation;

    const cplx_t<T> centre() const;

    friend const std::string to_string<> (const Ellipse<T>& curve);
    
  protected:    
    using ConvexSet<T>::m_resolution;  // since we are templates inheritance must be respecified
    using ConvexSet<T>::m_curve;     
    using ConvexSet<T>::m_params_ptr;
  };
}

/*** AbstractCurve *************************************************************
*/

template<std::floating_point T>
const path_t<T> AbstractCurve<T>::samples(unsigned resolution) const
{
  static path_t<T> subset;
  
  T dt { static_cast<T>(1.0) / (resolution-1) };
  T t {0.0};

  cplx_t<T> z {0.0, 0.0};;

  if (m_curve)
    {
      for (unsigned i = 0; i < resolution; i++)
	{
	  z = m_curve(t);
	  subset.push_back(z);
	  t += dt;
	}
    }
  
  return subset;
}

/*** Line class ***************************************************************
*/

template<std::floating_point T>
Line<T>::Line(const cplx_t<T>& start, const cplx_t<T>& end) : AbstractCurve<T>()
{
  m_params_ptr->m_values.a = start;
  m_params_ptr->m_values.b = end - start;
  
  m_curve = [this] (T t) { return m_params_ptr->m_values.a + t*(m_params_ptr->m_values.b); };

  if (mvc::DEBUG_OBJECTS)
    std::println("Line constructor.");
};

/*** ConvexSet *****************************************************************
*/

template<std::floating_point T>
const path_t<T> ConvexSet<T>::boundary() const
{
  static path_t<T> bdry { this->samples(m_resolution) }; // paths are assumed to be closed, first and last same

  //bdry.pop_back(); // and thefore last is removed

  if (mvc::DEBUG_EXECUTIVE)
    std::println("Rendered {} size boundary.",bdry.size());
  return bdry;
}

template<std::floating_point T>
const set_t<T> ConvexSet<T>::homotopic_representation(const std::size_t parts) const
{
  path_t<T> bdry(m_resolution-1, 0.0);
  set_t<T> homotopic(parts, path_t<T>(m_resolution-1, 0.0));

  bdry = this->boundary();
  
  if (mvc::DEBUG_EXECUTIVE)
    std::println("Rendering {}x{} homotopic representation from {} size boundary. Resolution {}.",
		 homotopic.size(), homotopic[0].size(), bdry.size(), m_resolution);
  
  const cplx_t<T> center = average<T>(bdry);

  translate(bdry, -center);

  std::size_t j { 0 };

  T dr { static_cast<T>(1.0) / (parts-1) };
  T r {1.0};
  
  for (std::size_t j {0}; j < parts; ++j)
  {
    
    path_t<T> path(m_resolution-1, 0.0);
    homotopic[j] = path;

    // skipped last element is same as first since homotopic curves are closed
    for (std::size_t i {0}; i < m_resolution-1; ++i)
      {
	homotopic[j][i] = r * bdry[i];
      }      
    r -= dr;
  }

  for (std::size_t j {0}; j < parts ; j++)
    translate(homotopic[j], center);

  return homotopic;
}

/*** Rectangle *****************************************************************
*/

template<std::floating_point T>
Rectangle<T>::Rectangle(const cplx_t<T>& start, const cplx_t<T>& end,
			const T height, const bool orientation) : ConvexSet<T>{}
{
  cplx_t<T> du {end - start};
  cplx_t<T> dv {du};

  // orthogonal 
  dv = orientation ? (cplx_t<T> {0.0,1.0} * dv) : (cplx_t<T> {0.0,-1.0} * dv); 
  // length 1 
  dv *= 1.0 / std::abs(dv);
  dv *= height; 

  m_params_ptr->m_values.a = start;
  m_params_ptr->m_values.b = du;
  m_params_ptr->m_values.c = dv;

  // the parameter point in [0,0.5] where dv vector is in effect
  m_params_ptr->m_values.K = 0.5 * (std::abs(du) / (std::abs(du) + std::abs(dv)));

  // the parameter point in [0.5,1] where -dv vector is in effect
  m_params_ptr->m_values.L = m_params_ptr->m_values.K + 0.5;

  m_curve = [&params = m_params_ptr->m_values, this] (T t) {
    
    T t1 { params.K }; // t for first corner
    T t3 { params.L }; // t for third corner
    
    // along base vector
    if (t >= 0 && t < t1)
      return params.a + ((t-0.0) / (t1-0.0))*params.b;
    // along side vector
    if (t >= t1 && t < 0.5)
      return params.a + params.b + ((t-t1) / (0.5-t1))*params.c;
    // along backwards base vector
    if (t >= 0.5 && t < t3)
      return params.a + params.c + (1.0 - (t-0.5) / (t3-0.5))*params.b;
    // last backward side vector, back to start 
    if (t >= t3 && t < 1)
      return params.a + (1.0 - (t-t3) / (1.0-t3))*params.c;
      
    // should not be reached (start point)
    return params.a;
  };
};

template<std::floating_point T>
const path_t<T> Rectangle<T>::corners() const
{
  static path_t<T> corners(4);

  cplx_t<T> A = m_params_ptr->m_values.a;
  cplx_t<T> B = m_params_ptr->m_values.b;
  cplx_t<T> C = m_params_ptr->m_values.c;
    
  corners[0] = A;
  corners[1] = corners[0] + B;
  corners[2] = corners[1] + C;
  corners[3] = corners[2] - B;

  return corners;
}

/*** Ellipse *******************************************************************
*/

template<std::floating_point T>
Ellipse<T>::Ellipse(const cplx_t<T>& center,
		    const cplx_t<T>& horisontal, T height)
  : ConvexSet<T>{}
{
  m_params_ptr->m_values.a = center;
  m_params_ptr->m_values.b = horisontal;
  
  m_params_ptr->m_values.c = height * (static_cast<T>(1.0) / std::abs(horisontal)) * (cplx_t<T>{0.0,1.0} * horisontal);
  
  m_curve = [&params = m_params_ptr->m_values, this] (T t) {    

    cplx_t<T> u = std::cos(t*2*std::numbers::pi)*(params.b);
    cplx_t<T> v = std::sin(t*2*std::numbers::pi)*(params.c);

    return (*m_params_ptr).a + u + v;
  };
};

// CAN'T PUT DEFINITIONS IN ANOTHER FILE WITHOUT SPECIALISATION, MUST HAVE THEM IN
// MODULE INTERFACE FILE UNLESS USING INCLUDE DIRECTIVE, BUT THEN CONTIGUOUS INCLUDES AFTER
// MODULE IMPORT IS NOT ACCOMPLISHED. MUST ONLY DO THAT WITH ONLY FUNDAMENTAL TYPES LEFT.

// TEMPLATES IMPLY PARTIAL DEFINITIONS WHICH MAKES OVERLOADING IMPOSSIBLE OR PERHAPS
// DIFFICULT WITHOUT SPECIALISATION. HAVE TO SPECIALIZE TYPES AND NOT REDEFINE THEM
// COMPLETELY, SINCE THEN THE TYPE SYSTEM WOULD VANISH: CAN'T HAVE BOTH
// OVERLOADING AND SPECIALISATION SIMULTANUEOUSLY.
//
// OVERALL: DON'T USE TEMPLATE SPECIALISATION (Beginning C++23)
//
// But then again: don't want large source files so template specialisation it is ...
