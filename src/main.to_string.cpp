import mvc;

import std;

using namespace std::complex_literals;

const cplx_t<double> a = -1.0 - 1.0i;
const cplx_t<double> b = 1.0 + 1.0i;


void test_structs()
{
  struct_t<double> t1;

  t1.a = 1.0 + 2.0i;
  t1.K = 2.3;

  struct_t<double> t2 {t1};

  struct_t<double> t3;

  t3 = t2;
  t2 = t3;

  std::println("{}\n{}\n{}", to_string(t1), to_string(t2), to_string(t3));
}

void test_types()
{
  const path_t<double> l1 { line(a,b,9) };
  std::println("{}\n", to_string(l1));

  const set_t<double> s1 { strip(a,b,0.1,true,9) };  
  std::println("{}\n", to_string(l1));
    
  const cplx_t<double> avg = average<double>(l1);
  std::println("{}\n", to_string(avg));
  
  std::print("------------------------ types test done --------------------\n");
}

// void test_sets()
// {
//    Line<double> L1{a,b};

//    std::println("{}\n", to_string(L1));
			     
//    const cplx_t<double> a3 {-1.0};
//    const cplx_t<double> b3 {1.0};
//    double h {2.0};
  
//   Rectangle<double> R2{a3,b3,h,true};

//   std::println("{}\n", to_string(R2));

//   std::print("------------------------ sets test done --------------------\n");
// }

void test_functions()
{
  std::vector<cplx_t<double>> coefs {0.0, 0.0, 1.0};

  Quadratic<double> polynomial{coefs};

  std::println("{}\n", to_string(polynomial));
  
  std::print("------------------------ functions test done --------------------\n");
}


int main()
{
  std::print("----------------------- model module test -------------------\n");

  test_structs(); 
  test_types();
  //test_sets();
  test_functions();

  return 0;
}

