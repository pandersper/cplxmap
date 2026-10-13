export module mvc:to_string;
import :types;
import :sets;
import :functions;
import :context;
import :computing;

import std;

export {

  template<std::floating_point T>
    const std::string to_string(const cplx_t<T>& z)
  {
    std::string str;
    
    if ( z.imag() < 0 )
      str = std::format("{:.1f}{:.1f}i", z.real(), z.imag());
    else
      str = std::format("{:.1f}+{:.1f}i", z.real(), z.imag());
    
    return std::format("{:^12}", str);
  }
  
  template<std::floating_point T>
    const std::string to_string(const struct_t<T>& params)
  {
    std::string str {"π: "};

    str += "{ ";
    str += std::format("\ta: {:^12} b: {:^12} c: {:^12} d: {:^12}\n",
		       to_string(params.a), to_string(params.b), to_string(params.c), to_string(params.d));
    str += std::format("\tK: {:^12.2} L: {:^12.2} M: {:^12.2} N: {:^12.2} ",
		       params.K, params.L, params.M, params.N);
    str += "}\n";
      
    return str;
  }

  template<std::floating_point T>
    const std::string to_string(const params_t<T>& params)
  {
    std::string str {"π: "};

    str += "{ ";
    str += std::format("\ta: {:^12} b: {:^12} c: {:^12} d: {:^12}\n",
		       to_string(params.m_values.a), to_string(params.m_values.b),
		       to_string(params.m_values.c), to_string(params.m_values.d));
    str += std::format("\tK: {:^12.2} L: {:^12.2} M: {:^12.2} N: {:^12.2} ",
		       params.m_values.K, params.m_values.L,
		       params.m_values.M, params.m_values.N);
    str += "}\n";
      
    return str;
  }
  
  const std::string to_string(map_t& map)
  {
    if (!mvc::DEBUG_VECTORS) return "";
    
    std::string str {"α:\n"};
    
    for (auto& [domain, range] : map)
      str += "  " + std::to_string(domain) + " -> " + std::to_string(range) + "\n";
    
    return str;
  } 

  /** std::vector derivatives
   */
  
  template<std::floating_point T>
    const std::string to_string(const path_t<T>& path)
  {
    if (!mvc::DEBUG_VECTORS) return "";
    
    std::string str {"γ: "};

    if (path.size() > 16)
      str += std::format("norm: {}", norm(path)) + "\n";
    else
      {
	str += "\n";
	
	for (auto& z : path)
	  str += to_string(z) + " ";

	str += "\n";
      }
    
    return str;
  }

  template<std::floating_point T>
    const std::string to_string(const set_t<T>& set)
  {
    if (!mvc::DEBUG_VECTORS) return "";
    
    std::string str {"Ω={γ}: "};

    if (set.front().size() > 16)
      str += std::format("set_norm: {}", set_norm(set)) + "\n";
    else
      {
	str += "\n";	
	for (auto& path : set)
	  {
	    for (auto& z : path)
	      str += to_string(z) + " " ;
	    str += "\n";
	  }
      }
    
    return str;
  }
  
  /**  Model
       If these friends are put in their class files the compiler don't find them.
       Don't understand why though (built in intelligence?) but the templating
       moves instantiation accross files perhaps.
  */
  
  template<std::floating_point T>
    const std::string to_string(const AbstractCurve<T>& curve)
  {
    if (!mvc::DEBUG_OBJECTS) return "";

    std::string str {"(γ): \n"};

    s_ptr<const params_t<T>> params_ptr = curve.get_parameters_ptr(); 
    
    str += to_string(*params_ptr);
  
    return str;
  }

  template<std::floating_point T>
    const std::string to_string(const Line<T>& curve)
  {
    if (!mvc::DEBUG_OBJECTS) return "";

    std::string str {"Line (γ): \n"};

    str += to_string(*curve.m_params_ptr);
    
    return str;
  }

  template<std::floating_point T>
    const std::string to_string(const ConvexSet<T>& set)
  {
    if (!mvc::DEBUG_OBJECTS) return "";

    std::string str {"(Ω): \n"};

    s_ptr<const params_t<T>> params_ptr = set.get_parameters_ptr(); 
    
    str += to_string(*params_ptr);

    return str;
  }
    
  template<std::floating_point T>
    const std::string to_string(const Rectangle<T>& set)
  {
    if (!mvc::DEBUG_OBJECTS) return "";

    std::string str {"Rectangle (Ω)}: \n"};

    s_ptr<const params_t<T>> params_ptr = set.get_parameters_ptr(); 
    
    str += to_string(*params_ptr);

    return str;
  }

  template<std::floating_point T>
    const std::string to_string(const Ellipse<T>& set)
  {
    if (!mvc::DEBUG_OBJECTS) return "";

    std::string str {"Ellipse (Ω): \n"};

    s_ptr<const params_t<T>> params_ptr = set.get_parameters_ptr(); 
    
    str += to_string(*params_ptr);

    return str;
  }
  
  template<std::floating_point T>
    const std::string to_string(const Functor<T>& functor)
  {
    if (!mvc::DEBUG_OBJECTS) return "";
    
    std::string str {""};

    return "Functor with parameters\n" +  to_string(functor.m_params);
  }

  template<std::floating_point T>
    const std::string to_string(const Quadratic<T>& functor)
  {
    if (!mvc::DEBUG_OBJECTS) return "";
    
    std::string str {""};

    return "Quadratic with parameters\n " +  to_string(functor.m_params);
  }

  template<std::floating_point T>
    const std::string to_string(const Context<T>& context)
  {
    if (!mvc::DEBUG_EXECUTIVE) return "";

    std::string str {"context:\n"};

    str += "map parameters:\n" + to_string(*(context.m_map_params_ptr));
    str += "domain parameters:\n" + to_string(*(context.m_domain_ptr->get_parameters_ptr()));
    
    str += "\t\tdomain pointer: " + std::format("{}", static_cast<void*>(context.m_domain_ptr.get())) + " (unique)" + "\n" +
           "\t\trange pointer:  " + std::format("{}", static_cast<void*>(context.m_range_ptr.get())) + " (use_count: "   +
            std::to_string(context.m_range_ptr.use_count()) + ")\n";
    
    return str;
  }

  /** Control
   */
  
  template<std::floating_point T>	
    const std::string to_string(const Computer<T>& computer)
  {
    if (!mvc::DEBUG_EXECUTIVE) return "";
    
    std::string str { std::format("Computer with id: {}\t ", computer.m_id) };

    for (auto& subdomain : computer.m_subdomains)
      str += std::format("\n\t has subdomain with norm {}", norm(subdomain));

    return str + "\n";
  }
    
  template<std::floating_point T>	
    const std::string to_string(const Manager<T>& manager)
  {
    if (!mvc::DEBUG_EXECUTIVE) return "";
      
    std::string str {"manager:\n"};

    str += "map: " + to_string(*(manager.m_context_ptr->get_map_parameters_ptr())) + "\n";
    str += "domain: " + to_string(*(manager.m_context_ptr->get_domain_parameters_ptr())) + "\n";
    
    for (std::size_t i {0}; i < Computer<T>::THREADS; ++i)    
      str += "tid: " + std::format("{}", manager.m_threads[i].get_id()) + "\t";

    str += "\n";
    
    for (std::size_t i {0}; i < Computer<T>::THREADS; ++i)    
      str += to_string(manager.m_computers[i]);

    str += "\n";
    
    return str;
  }
}
