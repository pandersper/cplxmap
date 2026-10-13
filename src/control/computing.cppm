export module mvc:computing;
export import :runtime;
export import :statics_control;

import :model;

#define MAX_THREADS (64)
#define HW_THREADS (std::thread::hardware_concurrency())
#define RESOLUTION (32)

export
{
  template<std::floating_point T> class Computer
  {
  public:
    static inline std::size_t THREADS {HW_THREADS};
    static inline std::size_t SUBDOMAINS {HW_THREADS};

    std::atomic_flag m_wait_flag;
    
    Computer() = delete;
    Computer(std::size_t id, const Functor<T>& functor, const s_ptr<range_t<T>>& range_ptr);
    ~Computer() = default;

    Computer(const Computer<T>& copied) = delete;
    Computer<T>& operator=(const Computer<T>& copied) = delete;
    
    Computer(Computer<T>&& moved);
    Computer<T>& operator=(Computer<T>&& moved) = delete;
    
    void routine();
    unsigned get_resolution();
    
    void set_functor(const Functor<T>& functor);
    void set_subdomain(const path_t<T>& subdomain, std::size_t index, std::size_t range_index);

    void update_parameters(const struct_t<T>& params);
      
    void clear_subdomains();
    
    std::size_t subdomain_count();
    
    void set_stop_source(std::stop_source stop_source);
    void terminate();
    
    friend const std::string to_string<T>(const Computer<T>& computer);
    
  private:
    
    std::size_t m_id;
    /**
       Function with parame
     */
    Functor<T> m_functor;

    set_t<T> m_subdomains { set_t<T>(1, path_t<T>(1)) };
    s_ptr<range_t<T>> m_range_ptr;
    map_t m_subranges { map_t() };
    
    bool m_terminated {false};
    std::stop_source m_stop_source;
  };

  template<std::floating_point T> class Manager
  {

  public:
    static inline bool domain_not_changing { true };
    static inline bool map_not_changing { true };

    Manager(const s_ptr<Context<T>>& context_ptr) : m_context_ptr {context_ptr}
    {
      setup_computers();      
      init_domain_range(HW_THREADS, HW_THREADS);
    }

    ~Manager() { if (mvc::DEBUG_EXECUTIVE) std::println("Manager destructed"); }

    void init_domain_range(const std::size_t threads, const std::size_t subdomains);
    void introduce_routines();

    void reconfigure_threading(std::size_t threads);
    void reconfigure_subdomains(std::size_t subdomains);
      
    void update_parameters(const struct_t<T>& params);
    void reread_domain();
    void reread_functor();
    void reset_range();
    
    void iterate();
    void terminate();
    void join();

    friend const std::string to_string<T>(const Manager<T>& manager);
    
  private:

    std::vector<Computer<T>> m_computers;
    std::vector<std::jthread> m_threads;
    
    s_ptr<Context<T>> const m_context_ptr;

    std::binary_semaphore m_domain_change{1};
    std::binary_semaphore m_map_change{1};
    
    void setup_computers();
  };
}

/** Computers
 */

export {

  template<std::floating_point T>
    Computer<T>::Computer(std::size_t id, const Functor<T>& functor, const s_ptr<range_t<T>>& range_ptr) :
    m_id {id}, m_functor {functor}, m_range_ptr {range_ptr}
    {
      if (mvc::DEBUG_EXECUTIVE) 
	std::println("Computer {}  setup.", m_id);
    }

  template<std::floating_point T>
    Computer<T>::Computer(Computer<T>&& moved) :
    m_id {moved.m_id}, m_functor {moved.m_functor},
    m_subdomains {moved.m_subdomains}, m_subranges {moved.m_subranges},
    m_range_ptr { std::move(moved.m_range_ptr) }
    { }
  
  template<std::floating_point T>
    void Computer<T>::routine()
  {
    while (true)
      {
	if (m_terminated)
	  {
	    if (mvc::DEBUG_EXECUTIVE)
	      std::println("Computer terminated. Requesting stop for thread {}, exiting.", m_id);
	    m_stop_source.request_stop();
	    break;
	  }
	else
	  {
	    if (Manager<T>::domain_not_changing && Manager<T>::map_not_changing)  // does not wait on functor change
	      {
		set_t<T>& range = m_range_ptr->m_range;
		
		for (const auto& [domain_id, range_id] : m_subranges)
		  {
		    path_t<T>& subdomain {m_subdomains[domain_id]};
		    
		    for (std::size_t i {0}; i < subdomain.size(); ++i)
		      range[range_id][i] = m_functor(subdomain[i]);
		  }		
	      }
	    else continue;
	    
	    if (mvc::DEBUG_FREQUENT) {
	      std::print("cid: {} sleeps. ", m_id);
	      std::this_thread::sleep_for(std::chrono::milliseconds(mvc::DEBUG_SLEEP));
	      std::print("cid: {} waits. ", m_id);
	    }
	    
	    while (m_wait_flag.test_and_set(std::memory_order_acquire))
	      m_wait_flag.wait(true, std::memory_order_acquire);
	  }
      }      
    if (mvc::DEBUG_EXECUTIVE) std::print("cid: {} exited.\n", m_id);
  }
    
  template<std::floating_point T>
    unsigned Computer<T>::get_resolution()
  {
    return (m_subdomains.empty()) ? 0 : m_subdomains[0].size();
  }

  template<std::floating_point T>
    void Computer<T>::set_functor(const Functor<T>& functor)
  {
    m_functor = functor;
  }

  template<std::floating_point T>
    void Computer<T>::update_parameters(const struct_t<T>& params)
  {
    m_functor.write_parameters(params);
  }

 template<std::floating_point T>
    void Computer<T>::set_subdomain(const path_t<T>& subdomain, std::size_t index, std::size_t range_index)
  {
    if (index < m_subdomains.size())
      m_subdomains[index] = subdomain;
    else
      if (index == m_subdomains.size())
	m_subdomains.push_back(subdomain);
      else
	throw CMException("Subdomain not added in sequence.");

    m_subranges[index] = range_index;
  }
  
  template<std::floating_point T>
    void Computer<T>::clear_subdomains()
  {
    m_subdomains = set_t<T>();
    m_subranges = map_t();
  }
      
  template<std::floating_point T>
    std::size_t Computer<T>::subdomain_count()
  {
    return m_subdomains.size();
  }
  
  template<std::floating_point T>
    void Computer<T>::set_stop_source(std::stop_source stop_source)
  {
    m_stop_source = stop_source;
  }
    
  template<std::floating_point T>
    void Computer<T>::terminate()
  {
    m_terminated = true;
  }
}

/** Manager
 */

export {

  /** Initialisations and reconfigurations **/
  template<std::floating_point T>
    void Manager<T>::setup_computers()
  {
    if (mvc::DEBUG_EXECUTIVE)
      std::println("Computers setup. {}", Computer<T>::THREADS);

    m_computers.clear();
    
    for (std::size_t t_index {0}; t_index < Computer<T>::THREADS; ++t_index)
      {
	Functor<T> map { m_context_ptr->get_updated_map() };

	s_ptr<range_t<T>> range_ptr { m_context_ptr ->get_range_ptr() };
	
	Computer<T> computer{t_index, map, range_ptr};

	m_computers.push_back(std::move(computer));
      }
    
    if (mvc::DEBUG_EXECUTIVE)
      std::print("Done setting up {} unitialized computers.\n", m_computers.size());
  }
    
  template<std::floating_point T>
    void Manager<T>::reconfigure_threading(const std::size_t threads)
  {
    terminate();

    join();

    init_domain_range(threads, Computer<T>::SUBDOMAINS);
  }

  template<std::floating_point T>
    void Manager<T>::reconfigure_subdomains(const std::size_t subdomains)
  {
    init_domain_range(Computer<T>::THREADS, subdomains);
  }

  template<std::floating_point T>
    void Manager<T>::init_domain_range(const std::size_t threads, const std::size_t parts)
  {

    // Computer and subdomains
    if (Computer<T>::THREADS == threads && Computer<T>::SUBDOMAINS == parts)
      {
	if (mvc::DEBUG_EXECUTIVE)
	  std::println("Manager: same threads and subdomains. Reread only.");
      }
    else
      {
	if (Computer<T>::THREADS != threads)
	  {
	    if (mvc::DEBUG_EXECUTIVE)
	      std::println("Manager: new threading: {} ->  {} threads.", Computer<T>::THREADS, threads);
	    
	    Computer<T>::THREADS = threads;
	    
	    setup_computers();	    
	  }
	
	if (Computer<T>::SUBDOMAINS != parts)
	  {
	    Computer<T>::SUBDOMAINS = parts;
	    
	    if (mvc::DEBUG_EXECUTIVE)
	      std::println("Manager: subdomains change, {} computers with {}x{} subdomains.",
			   Computer<T>::THREADS, Computer<T>::SUBDOMAINS,
			   m_context_ptr->m_domain_ptr->get_resolution());
	  }	
      }

    // Threads per computer (these are not threads yet)
    m_threads = std::vector<std::jthread>(Computer<T>::THREADS);    
    if (mvc::DEBUG_EXECUTIVE)
      std::print("{} empty threads corresponding computers.\n", m_threads.size());

    // Map per computer
    reread_functor();

    // Reread (sample) domain 
    reread_domain();
    // Set range corresponding to domain
    reset_range();
    // Transfer computers's routines unto threads (these are now threads)
    introduce_routines();
    
    if (mvc::DEBUG_EXECUTIVE)
      std::println("{} computers and {} threads. {} parts.", m_computers.size(),
		   m_threads.size(), Computer<T>::SUBDOMAINS);
  }

  template<std::floating_point T>
    void Manager<T>::introduce_routines()
  {
    if (mvc::DEBUG_EXECUTIVE) std::println("Introducing routines.");

    m_threads.clear();
    
    for (std::size_t i {0}; i < Computer<T>::THREADS; ++i)
      {
	m_threads.push_back(std::jthread(&Computer<T>::routine, &(m_computers[i])));
	m_computers[i].set_stop_source(m_threads[i].get_stop_source());
      }
  }
  
  /** Updates **/
  template<std::floating_point T>
    void Manager<T>::reread_functor()
  {
    if (mvc::DEBUG_EXECUTIVE) std::println("Rereading functor.");

    for (std::size_t i {0}; i < Computer<T>::THREADS; ++i)
      m_computers[i].set_functor(m_context_ptr->get_updated_map());
  }
  
  template<std::floating_point T>
    void Manager<T>::reread_domain()
  {
    std::size_t parts = Computer<T>::SUBDOMAINS;
    std::size_t threads = Computer<T>::THREADS;

    if (parts < threads)
      {
	Computer<T>::SUBDOMAINS = threads;

	if (mvc::DEBUG_EXECUTIVE)
	  std::println("Increasing number of subdomains to number of threads. {} -> {}",
		       parts, Computer<T>::SUBDOMAINS);

	parts = threads;
      }
    
    const set_t<T> domain_sampled { m_context_ptr->m_domain_ptr->
     				    homotopic_representation(Computer<T>::SUBDOMAINS) };

    if (mvc::DEBUG_EXECUTIVE)
      std::println("Setting up {}x{} domains on {} computers.", domain_sampled.size(),
		   domain_sampled[0].size(), m_computers.size());    
    
    for (std::size_t iter {0}; iter < threads; iter++)
	m_computers[iter].clear_subdomains();
    
    for (std::size_t iter {0}; iter < parts; iter++)
	m_computers[iter%threads].set_subdomain(domain_sampled[iter], iter/threads, iter);

    //m_threads = std::vector<std::jthread>(Computer<T>::THREADS);
  }

  template<std::floating_point T>
    void Manager<T>::reset_range()
  {
    m_context_ptr->init_range(Computer<T>::SUBDOMAINS);
  }
  
  template<std::floating_point T>
    void Manager<T>::update_parameters(const struct_t<T>& params)
  {
    if (mvc::DEBUG_FREQUENT) std::println("Updating all parameters.");

    for (std::size_t i {0}; i < Computer<T>::THREADS; ++i)
      m_computers[i].update_parameters(params);
  }

  /** Controls **/
  template<std::floating_point T>
    void Manager<T>::iterate()
  {
    for (auto& computer : m_computers)
      {
	computer.m_wait_flag.clear(std::memory_order_release);
	computer.m_wait_flag.notify_one();
      }
    if (mvc::DEBUG_FREQUENT) std::println("Updated all computers.");	
  }

  template<std::floating_point T>
    void Manager<T>::terminate()
  {
    if (mvc::DEBUG_EXECUTIVE) std::println("Terminating threads.");
    
    for (auto& computer : m_computers)
      computer.terminate();

    iterate();
  }
    
  template<std::floating_point T>
    void Manager<T>::join()
  {
    if (mvc::DEBUG_EXECUTIVE) std::println("Joining threads.");
    
    for (auto& thread : m_threads)
      thread.join();
    
    if (mvc::DEBUG_EXECUTIVE) std::println("All threads done.");
  }
 
}
