#include <sstream>

#if _DEBUG
 #define LogF( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << '\n';                   \
   OutputDebugStringW( os_.str().c_str() );  \
}
#else
 #define LogF( s ) 
#endif