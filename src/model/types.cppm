export module mvc:types;
import std;

  /** types module interface independent of classes **/

export {
  
  /** model fundamental types **/
  template<std::floating_point T> using cplx_t = std::complex<T>;
  template<std::floating_point T> using path_t = std::vector<cplx_t<T>>;
  template<std::floating_point T> using set_t  = std::vector<path_t<T>>;

  template<std::floating_point T> struct struct_t
  {
    cplx_t<T> a, b, c, d;  // ought to be zero-initialized in all implementations
    T K {0}, L {0}, M {0}, N {0};

    struct_t() = default;
    
    struct_t(cplx_t<T> _a, cplx_t<T> _b, cplx_t<T> _c, cplx_t<T> _d,
	     T _K, T _L, T _M, T _N)
    {
      a = _a; b = _b; c = _c; d = _d;
      K = _K; L = _L; M = _M; N = _N;
    };
      
    struct_t(const struct_t<T>& copied)
    {
      a = copied.a; b = copied.b; c = copied.c; d = copied.d;
      K = copied.K; L = copied.L; M = copied.M; N = copied.N;
    }

    struct_t<T>& operator=(const struct_t<T>& copied)
    {
      a = copied.a; b = copied.b; c = copied.c; d = copied.d;
      K = copied.K; L = copied.L; M = copied.M; N = copied.N;

      return *this;
    }    
  };

  /** model function types **/
  template<std::floating_point T> using curve_t  = std::function<const cplx_t<T>(const T t)>;
  template<std::floating_point T> using func_t   = std::function<const cplx_t<T>(const cplx_t<T>& z, const struct_t<T>& params)>;
  template<std::floating_point T> using series_t = std::function<const cplx_t<T>(const cplx_t<T>& z, bool& converged, int cycles_max)>;

  /** pointer types **/
  template<typename F> using u_ptr = std::unique_ptr<F>;
  template<typename F> using s_ptr = std::shared_ptr<F>;
  template<typename F> using w_ptr = std::weak_ptr<F>;

  template<typename F> using s_set_t = std::shared_ptr<set_t<F>>;
  template<typename F> using s_vec_t = std::shared_ptr<std::vector<F>>;

  /** readability type **/
  using map_t = std::map<std::size_t, std::size_t>;

  /** non-fundamental (model-important) types **/
  

  template<std::floating_point T> class params_t : public std::enable_shared_from_this<params_t<T>>
  {
  private:
    
    struct null_t{ explicit null_t() = default; };
        
  public:
    
    struct_t<T> m_values;
    
    params_t(null_t) {};
    
    static std::shared_ptr<params_t> create() { return std::make_shared<params_t>(null_t()); }

    // modifies this when handing out pointer so can't be const
    s_ptr<params_t<T>> const get_ptr() { return this->shared_from_this(); }     
  };

  template<std::floating_point T> class range_t : public std::enable_shared_from_this<range_t<T>>
  {
  private:
    
    struct null_t{ explicit null_t() = default; };
        
  public:
    
    // the range data target
    set_t<T> m_range;

    range_t(null_t) {};

    static std::shared_ptr<range_t> create() { return std::make_shared<range_t>(null_t()); }
        
    // modifies this when handing out pointer so can't be const
    s_ptr<range_t> const get_ptr() { return this->shared_from_this(); }
  };
}

   /** Static methods on model-fundamental types **/

export {
  template<std::floating_point T> T norm(const path_t<T>& boundary);
  template<std::floating_point T> T set_norm(const set_t<T>& domain);
  template<std::floating_point T> std::vector<std::vector<T>> field(const set_t<T>& domain);
}

export namespace mvc {

	 inline const bool DEBUG_EXECUTIVE {true};
	 inline const bool DEBUG_SELDOM {false};
	 inline const bool DEBUG_FREQUENT {false};
	 inline const bool DEBUG_SEMIFREQUENT {false};
	 inline const bool DEBUG_SYNC {false};
	 inline const bool DEBUG_OBJECTS {false};
	 inline const bool DEBUG_VECTORS {true};

	 inline const std::chrono::duration DEBUG_SLEEP { std::chrono::milliseconds(10) };
}

template<std::floating_point T>
T norm(const path_t<T>& boundary)
{
  T  abs_total {0};
  for (auto& z : boundary)
    abs_total += abs(z);

  return abs_total / boundary.size();
}

template<std::floating_point T>
T set_norm(const set_t<T>& domain)
{
  T  abs_total {0};
  for (auto& homotopic : domain)
    for (auto& z : homotopic)
      abs_total += abs(z);
  
  return abs_total / (domain.size() * domain[0].size());
}

template<std::floating_point T>
std::vector<std::vector<T>> field(const set_t<T>& domain)
{
  static std::vector<std::vector<T>> field(domain.size(), std::vector<T>(domain[0].size()));

  T norm = set_norm(domain);
  
  for (unsigned i {0}; i < domain.size(); ++i)
    for (unsigned j {0}; j < domain[0].size(); ++j)
      field[i][j] = abs(domain[i][j]) / norm;
  
  return field;
}

// CAN'T PUT DEFINITIONS IN ANOTHER FILE WITHOUT SPECIALISATION, MUST HAVE THEM IN
// MODULE INTERFACE FILE UNLESS USING INCLUDE DIRECTIVE, BUT THEN CONTIGUOUS INCLUDES AFTER
// MODULE IMPORT IS NOT ACCOMPLISHED. MUST ONLY DO THAT WITH ONLY FUNDAMENTAL TYPES LEFT.
