export module mvc:statics_model;
import :types;

import std;

export
{

  template<std::floating_point T>
    const std::string cplx_string(const cplx_t<T>& z)
  {
    std::string str;
    
    if ( z.imag() < 0 )
      str = std::format("{:.1f}{:.1f}i", z.real(), z.imag());
    else
      str = std::format("{:.1f}+{:.1f}i", z.real(), z.imag());
    
    return std::format("{:^12}", str);
  }
  
  /** point sets **/

  template<std::floating_point T>
    const path_t<T> line(const cplx_t<T> start, const cplx_t<T> end, int n)
  {
    path_t<T> line(n);

    cplx_t<T> dv { end - start };

    dv = (static_cast<T>(1.0) / (n-1)) * dv;
  
    for (double i = 0; i < n; i++)
      line[i] = start + i*dv;

    return line;
  }

  template<std::floating_point T>
    const set_t<T> strip(const cplx_t<T> start, const cplx_t<T> end, T width, bool orientation, int n)
  {
    set_t<T> strip(2);

    cplx_t<T> dv {end - start};
    // orthogonal 
    dv = orientation ? (cplx_t<T> {0.0,1.0} * dv) : (cplx_t<T> {0.0,-1.0} * dv); 
    // length 1 
    dv = (1.0 / std::abs(dv))*dv; 
  
    strip[0] = line(start, end, n);
    strip[1] = line(start+width*dv, end+width*dv, n);

    return strip;
  }

  template<std::floating_point T>
    const set_t<T> rectangle(const cplx_t<T> start, const cplx_t<T> end, T height, bool orientation, int n, int m)
  {
    set_t<T> rectangle(m);

    cplx_t<T> dv {end - start};
    // orthogonal 
    dv = orientation ? (cplx_t<T> {0.0,1.0} * dv) : (cplx_t<T> {0.0,-1.0} * dv); 
    // length 1 
    dv = (1.0 / std::abs(dv))*dv;
    // each parallell segment length
    dv = (height / (m-1.0))*dv; 

    cplx_t<T> v {0.0,0.0};
  
    for (int i = 0; i < m; i++)
      {
	rectangle[i] = line(start+v, end+v, n);
	v += dv;
      }
  
    return rectangle;
  }

  /** point set manipulation and measurement **/

  template<std::floating_point T>
    const cplx_t<T> average(const path_t<T>& path)
  {
    cplx_t<T> a {0.0,0.0};

    for(std::size_t i = 0; i < path.size(); ++i) a += path[i];

    return (1.0 / path.size())*a;
  }

  template<std::floating_point T>
    void translate(path_t<T>& path, const cplx_t<T>& origo)
  {
    for(std::size_t i = 0; i < path.size(); ++i) path[i] += origo;
  }

  template<std::floating_point T> set_t<T> domainFactory(std::string json)
  {
    switch (json) {
    case "line":
      return line(cplx_t {-1.0,-1.0}, cplx_t {1.0, 1.0}, 5);
      break;
    case "strip":
      return strip(cplx_t {-1.0,-1.0}, cplx_t {1.0, 1.0}, 0.2, true, 5);
      break;
    default:
      std::println("Default case.");
      break;
    }
  }
}
