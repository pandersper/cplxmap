export module mvc:statics_control;
import :types;

import std;

export namespace util
{

  template<std::floating_point T>
  const bool check_bit(s_ptr<unsigned>& bits_ptr, std::size_t id)
  {
    unsigned mask = 1 << id;
    unsigned set_bits = *bits_ptr;
  
    return set_bits & mask;
  }

  template<std::floating_point T>
  void set_bit(s_ptr<unsigned>& bits_ptr, std::size_t id, bool set, std::size_t threads)
  {
    const unsigned set_mask = 1 << id;
    const unsigned all_mask = (~((~0) << threads));
  
    if (set)
      {
	(*bits_ptr) |= set_mask;
      }
    else
      {
	(*bits_ptr) &= ~set_mask;
      }
  
    (*bits_ptr) &= all_mask;
  }

  const unsigned random_micros()
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dist(100, 1000);

    return dist(rd);
  }
}
