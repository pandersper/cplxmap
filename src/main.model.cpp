import mvc;

import std;

using namespace std::complex_literals;

const cplx_t<double> a = -0.5 - 1.0i;
const cplx_t<double> b = 1.5 + 1.0i;

const struct_t<double> example_struct()
{
  static struct_t<double> t1;
  
  t1.a = 1.0 + 1.0i; t1.b = 2.0 + 2.0i;  t1.c = 3.0 + 3.0i; t1.d = 4.0 + 4.0i;
  t1.K = 9.8;  t1.L = 8.7;  t1.M = 7.6;  t1.N = 6.5;

  return t1;
}

static const struct_t<double> struct_function(const struct_t<double>& s)
{
  struct_t<double> s0 { s };

  s0.a = 2.0*s0.a;
  s0.b = 2.0*s0.b;
  s0.c = 2.0*s0.c;

  return s0;
}

static const path_t<double> vector_function(const path_t<double>& path)
{
  path_t<double> path0 { path };

  for (auto& z : path0)
    z *= 2.0;

  return path0;
}

// static const Line<double> set_function(const Line<double>& line)
// {
//   static Line<double> line0 { line };
  
//   s_ptr<struct_t<double>> prms0_ptr { line0.get_parameters_ptr() };

//   prms0_ptr->a = - 1.0;
//   prms0_ptr->b = - 1.0;
//   prms0_ptr->c = - 1.0;

//   return line0;
// }


void test_structs()
{
  struct_t<double> t1;

  t1.a = 1.0 + 1.0i;
  t1.b = 2.0 + 2.0i;
  t1.c = 3.0 + 3.0i;
  t1.d = 4.0 + 4.0i;
  t1.K = 9.8;
  t1.L = 8.7;
  t1.M = 7.6;
  t1.N = 6.5;

  struct_t<double> t2 {t1};
  struct_t<double> t3;

  t3 = t2;
  t2 = t3;

  struct_t<double> t4 = struct_function(t3);
  
  std::println("Copied parameter structs:\n {} {} {}", to_string(t1), to_string(t2), to_string(t3));
  std::println("Passed into functions struct:\n {}", to_string(t4));

  std::print("------------------------ structs test done --------------------\n");
}

void test_types()
{
  const path_t<double> l1 { line(a, b, 9) };

  std::println("path l1 {}", to_string(l1));
  std::println("norm of path l1: {}", norm(l1));
  
  const path_t<double> l1_copy { vector_function(l1) };

  std::println("Line f(l1_copy) {}", to_string(l1_copy));
  std::println("norm of line f(l1_copy): {}", norm(l1_copy));
  
  const cplx_t<double> avg = average<double>(l1);

  std::println("avg(l1) = {}", to_string(avg));

  const cplx_t<double> avg2 = average<double>(l1_copy);

  std::println("avg(l1_copy) = {}", to_string(avg2));

  const set_t<double> s1  {strip(a, b, 0.1, true, 9) };  

  std::println("Strip s1 of width 0.1 {}", to_string(s1));
  std::println("set-norm of strip s1: {}", set_norm(s1));

  path_t<double> p1(8, cplx_t<double>{1.0, 1.0});
  
  set_t<double> s2(4, path_t<double>(10, cplx_t<double>{1.0, 1.0}));

  std::println("{}", to_string(p1));
  std::println("{}", to_string(s2));
  
  std::print("------------------------ types test done --------------------\n");
}

void test_sets()
{
   Line<double> L1{a,b};

   std::println("{}", to_string(L1));

   const cplx_t<double> a2 {-1.0, 0.0};
   const cplx_t<double> b2 {1.0, 0.0};
   double h2 {2.0};

   Rectangle<double> R1{a2,b2,h2,true};

   std::println("Rectangle R1 parameters\n {}", to_string(*(R1.get_parameters_ptr())));
   R1.set_resolution(16);
   
   path_t<double> b = R1.boundary();
   std::println("Rectangle R1 boundary with resolution 16 {}", to_string(b));

   R1.set_resolution(2048);
   b = R1.boundary();
   std::println("Boundary with resolution 2048 {}", to_string(b));

   R1.set_resolution(6);
   set_t<double> s = R1.homotopic_representation(4);
   std::println("Rectangle at 4x6 points {}", to_string(s));
   
   std::print("------------------------ sets test done --------------------\n");
}

void test_functions()
{
  std::vector<cplx_t<double>> coefs {0.0, 0.0, 1.0};

  Quadratic<double> polynomial{coefs};
  
  std::println("Quadratic polynomial z^2 \n{}", to_string(polynomial));

  std::println("f(1)={}, f(-1)={} f(i)={}\n",
	       to_string(polynomial(1.0)), to_string(polynomial(-1.0)), to_string(polynomial(1.0i)));

  Quadratic<double> p_copy { polynomial };

  std::println("Copied polynomial\n{}", to_string(p_copy));

  std::println("f_copy(1)={}, f_copy(-1)={} f_copy(i)={}\n",
	       to_string(p_copy(1.0)), to_string(p_copy(-1.0)), to_string(p_copy(1.0i)));

  struct_t<double> t1 = example_struct();
  
  p_copy.write_parameters(t1);
  p_copy(0.0);
  
  std::println("Copied polynomial with changed parameters\n{}", to_string(p_copy));
  std::println("Quadratic polynomial z^2\n{}", to_string(polynomial));

  set_t<double> domain_0 = {{2.0, 1.0}, {1.0i, 2.0i}};
  set_t<double> range_0 = set_t<double>(2, path_t<double>(2, 0.0));

  for (int i {0}; i < 2; ++i)
    for (int j {0}; j < 2; ++j)
      range_0[i][j] = p_copy(domain_0[i][j]);

  std::println("Domain {} gives range {} under p_copy", to_string(domain_0), to_string(range_0));

  std::println("------------------------ functions test done --------------------\n");
}


void test_model()
{
  const cplx_t<double> a3 {-1.0, 0.0};
  const cplx_t<double> b3 {1.0, 0.0};
  double h3 {2.0};
  
  Rectangle<double> rectangle{a3, b3, h3, true};
   
  std::vector<cplx_t<double>> coefs {1.0, 0.0, 1.0};

  Quadratic<double> quadratic{coefs};
  
  rectangle.set_resolution(8);
  
  s_ptr<Context<double>> context_ptr { std::make_shared<Context<double>>(quadratic, rectangle) };

  std::println("{}", to_string(*context_ptr));

  std::println("Rectangle boundary {}", to_string(context_ptr->m_domain_ptr->boundary()));

  context_ptr->set_resolution(8);
  context_ptr->init_range(4);

  s_ptr<range_t<double>> range_ptr = context_ptr->get_range_ptr();
  
  range_t<double> range = *range_ptr;

  set_t<double> domain {context_ptr->m_domain_ptr->homotopic_representation(4) };
  
  std::println("Context range (size: {}x{}) {}", range.m_range.size(),
	       (range.m_range)[0].size(), to_string(range.m_range));

  std::println("Applying map to domain {}", to_string(domain));

  Functor<double> map = context_ptr->get_updated_map();
  
  std::println("Fresh map from context:\n{}", to_string(map));
  
  for (unsigned i {0}; i < domain.size(); ++i)
    for (unsigned j {0}; j < domain[0].size(); ++j)
      range.m_range[i][j] = map(domain[i][j]);
  
  std::println("New range with map: {}", to_string(range.m_range));

  s_ptr<params_t<double>> dom_prms_ptr { context_ptr->get_domain_parameters_ptr() };
  s_ptr<params_t<double>> map_prms_ptr { context_ptr->get_map_parameters_ptr() };

  map_prms_ptr->m_values = struct_t<double>{9.0,4.0,0.0,0.0,0.0,0.0,0.0,999.0}; 
  std::println("Changed map parameters outside context:\n{}", to_string(*map_prms_ptr));
  std::println("{}", to_string(*context_ptr));


  Functor<double> map_1 = context_ptr->get_updated_map();
  std::println("Fetched updated map {}", to_string(map_1));
  
  for (unsigned i {0}; i < domain.size(); ++i)
    for (unsigned j {0}; j < domain[0].size(); ++j)
      range.m_range[i][j] = map_1(domain[i][j]);

  std::println("Range: {}", to_string(range.m_range));
  
  std::println("------------------------ model test done --------------------\n");
}


int main()
{
  std::print("----------------------- model module test -------------------\n");

  test_structs(); 
  test_types();  
  test_sets();
  test_functions();
  test_model();

  std::print("----------------------- module test done -------------------\n");
  
  return 0;
}

