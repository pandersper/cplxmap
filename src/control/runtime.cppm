export module mvc:runtime;
import :model;


import std;

export {

  class CMException : public std::exception
  {
  public:
    
    CMException(const char*  message) : m_message {message}
    { };
    
    const char* what() const noexcept override
    {
      return m_message;
    };
  
  private:
    const char* m_message;
  };
}




