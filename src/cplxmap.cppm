export module mvc;
export import :model;
export import :control;

export void types_info() {

  std::println("\n ---------------- trivially copyable: ");
  std::println("std::complex<double> : {}", std::is_trivially_copyable_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_trivially_copyable_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_trivially_copyable_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_trivially_copyable_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_trivially_copyable_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_trivially_copyable_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_trivially_copyable_v<set_t<double>>);
  std::println("u_ptr<double> : {}", std::is_trivially_copyable_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_trivially_copyable_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_trivially_copyable_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_trivially_copyable_v<s_ptr<path_t<double>>>);

  std::println("\n ---------------- constructible: ");
  std::println("std::complex<double> : {}", std::is_constructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_constructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_constructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_constructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_constructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_constructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_constructible_v<set_t<double>>);
  std::println("u_ptr<double> : {}", std::is_constructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_constructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_constructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_constructible_v<s_ptr<path_t<double>>>);
  
  std::println("<  trivially: ");
  std::println("std::complex<double> : {}", std::is_trivially_constructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_trivially_constructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_trivially_constructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_trivially_constructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_trivially_constructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_trivially_constructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_trivially_constructible_v<set_t<double>>);
  std::println("u_ptr<double> : {}", std::is_trivially_constructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_trivially_constructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_trivially_constructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_trivially_constructible_v<s_ptr<path_t<double>>>);
  
  std::println("<  nothrow:");
  std::println("std::complex<double> : {}", std::is_nothrow_constructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_nothrow_constructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_nothrow_constructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_nothrow_constructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_nothrow_constructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_nothrow_constructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_nothrow_constructible_v<set_t<double>>);
  std::println("u_ptr<double> : {}", std::is_nothrow_constructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_nothrow_constructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_nothrow_constructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_nothrow_constructible_v<s_ptr<path_t<double>>>);
  
  std::println("\n ---------------- destructible: ");
  std::println("std::complex<double> : {}", std::is_destructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_destructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_destructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_destructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_destructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_destructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_destructible_v<set_t<double>>);
  std::println("u_ptr<double> : {}", std::is_destructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_destructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_destructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_destructible_v<s_ptr<path_t<double>>>);
  
  std::println("<  trivially: ");
  std::println("std::complex<double> : {}", std::is_trivially_destructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_trivially_destructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_trivially_destructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_trivially_destructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_trivially_destructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_trivially_destructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_trivially_destructible_v<set_t<double>>);   
  std::println("u_ptr<double> : {}", std::is_trivially_destructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_trivially_destructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_trivially_destructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_trivially_destructible_v<s_ptr<path_t<double>>>);
  
  std::println("<  nothrow:");
  std::println("std::complex<double> : {}", std::is_nothrow_destructible_v<std::complex<double>>);
  std::println("cplx_t<double> : {}", std::is_nothrow_destructible_v<cplx_t<double>>);
  std::println("struct_t<double> : {}", std::is_nothrow_destructible_v<struct_t<double>>);
  std::println("curve_t<double> : {}", std::is_nothrow_destructible_v<curve_t<double>>);
  std::println("func_t<double> : {}", std::is_nothrow_destructible_v<func_t<double>>);
  std::println("path_t<double> : {}", std::is_nothrow_destructible_v<path_t<double>>);
  std::println("set_t<double> : {}", std::is_nothrow_destructible_v<set_t<double>>);   
  std::println("u_ptr<double> : {}", std::is_nothrow_destructible_v<u_ptr<double>>);
  std::println("s_ptr<void*> : {}", std::is_nothrow_destructible_v<s_ptr<void*>>);
  std::println("u_ptr<path_t> : {}", std::is_nothrow_destructible_v<u_ptr<path_t<double>>>);
  std::println("s_ptr<path_t> : {}", std::is_nothrow_destructible_v<s_ptr<path_t<double>>>);
};




