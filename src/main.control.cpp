import mvc;

import std;

using namespace std::complex_literals;

void nano_sleep()
{
  std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}
void milli_sleep()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
const unsigned random_micros()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned> dist(100, 1000);
  
  return dist(rd);
}
void rand_sleep()
{
  unsigned map_sleep = random_micros(); 
  std::this_thread::sleep_for(std::chrono::milliseconds(map_sleep));
}

void setup_test()
{
  Rectangle<double> rectangle{1.0, 2.0, 2, true};  

  s_ptr<params_t<double>> domain_prms { rectangle.get_parameters_ptr() };
  
  std::vector<cplx_t<double>> coefs {1.0, 0.0, 1.0};

  Quadratic<double> quadratic{coefs};

  struct_t<double> map_prms { quadratic.get_parameters() };
  
  s_ptr<Context<double>> const context_ptr { std::make_shared<Context<double>>(quadratic, rectangle) };
  
  context_ptr->set_resolution(10'000);
  context_ptr->init_range(4);

  Manager<double> manager{context_ptr};
 
   const Functor<double> map = context_ptr->get_updated_map();

  s_ptr<range_t<double>> range_ptr = context_ptr->get_range_ptr();

  std::println("Range (size: {}x{}) {}Applying f to domain.", range_ptr->m_range.size(),
	       (range_ptr->m_range)[1].size(), to_string(range_ptr->m_range));
  
  for (int j {0}; j < 5; j++)
    {
      for (int i {0}; i < 1'000'000; ++i)
	{
	  manager.iterate();
	}
      std::println("1'000'000 iterations");      
    }

  std::println("{}", to_string(manager));
    
  set_t<double> domain {context_ptr->m_domain_ptr->homotopic_representation(4) };
  
  std::println("map: {}", to_string(map));
  std::println("domain: {}", to_string(domain));
  std::println("range: {}", to_string(range_ptr->m_range));
  
  manager.terminate();
  manager.join();
  
  std::print("------------------------ control setup test done --------------------\n");
}

void manager_test()
{

  Rectangle<double> rectangle{1.0, 2.0, 2, true};  
  
  s_ptr<params_t<double>> domain_prms { rectangle.get_parameters_ptr() };
  
  std::vector<cplx_t<double>> coefs {1.0, 0.0, 1.0};
  
  Quadratic<double> quadratic{coefs};
  
  struct_t<double> map_prms { quadratic.get_parameters() };
  
  s_ptr<Context<double>> const context_ptr { std::make_shared<Context<double>>(quadratic, rectangle) };

  s_ptr<params_t<double>> map_prms_ptr { context_ptr->get_map_parameters_ptr() };

  context_ptr->set_resolution(1'000'000);
  
  Manager<double> manager{context_ptr};

  //  manager.reconfigure_threading(8);

  const Functor<double> map = context_ptr->get_updated_map();
  s_ptr<range_t<double>> range_ptr = context_ptr->get_range_ptr();

  std::println("{}", to_string(manager));
    
  std::println("Range (size: {}x{}) {}", range_ptr->m_range.size(), (range_ptr->m_range)[1].size(), to_string(range_ptr->m_range));
  std::println("Applying f to domain");
  
  double dt = 0.5;

  struct_t<double> params = map_prms_ptr->m_values;
  
  for (int j {0}; j < 5; j++)
    {
      for (int i {0}; i < 1'000'000; ++i) manager.iterate();

      params.a += dt;
      manager.update_parameters(params);

      std::println("1M iterations at resolution 33x1M on 12 threads. Parameters update.");
      std::println("range: {}", to_string(range_ptr->m_range));
    }
  
  manager.terminate();
  manager.join();

}

void configuration_test()
{
  Rectangle<double> rectangle{1.0, 2.0, 2, true};  
  
  s_ptr<params_t<double>> domain_prms { rectangle.get_parameters_ptr() };
  
  std::vector<cplx_t<double>> coefs {1.0, 0.0, 1.0};
  
  Quadratic<double> quadratic{coefs};
  
  struct_t<double> map_prms { quadratic.get_parameters() };
  
  s_ptr<Context<double>> const context_ptr { std::make_shared<Context<double>>(quadratic, rectangle) };

  s_ptr<params_t<double>> map_prms_ptr { context_ptr->get_map_parameters_ptr() };

  context_ptr->set_resolution(32);
  
  Manager<double> manager{context_ptr};

  manager.reconfigure_threading(6);
  //manager.reconfigure_subdomains(6);

  const Functor<double> map = context_ptr->get_updated_map();
  s_ptr<range_t<double>> range_ptr = context_ptr->get_range_ptr();

  std::println("{}", to_string(manager));
    
  std::println("Range (size: {}x{}) {}", range_ptr->m_range.size(), (range_ptr->m_range)[1].size(), to_string(range_ptr->m_range));
  std::println("Applying f to domain");
  
  double dt = 0.5;

  struct_t<double> params = map_prms_ptr->m_values;
  
  for (int j {0}; j < 5; j++)
    {
      for (int i {0}; i < 100; ++i) manager.iterate();

      params.a += dt;
      manager.update_parameters(params);

      std::println("100 iterations at resolution 8x32 on 6 threads. Parameters update.");
      std::println("range: {}", to_string(range_ptr->m_range));
    }
  
  manager.terminate();
  manager.join();
}

int main()
{
  std::print("----------------------- control module test -------------------\n");

  //setup_test();
  
  //init_domain_range(3*HW_THREADS, 5*HW_THREADS+13); in manager_test
  //manager_test();
  //subdomains_test();
  //threads_test();

  configuration_test();
  
  return 0;
}
