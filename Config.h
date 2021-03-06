#ifndef CUDANBODY_CONFIG_H
#define CUDANBODY_CONFIG_H

// #define CNB_CUDA
#define CNB_DEBUG
#define CNB_FLOAT
#define CNB_EPSILON  1e-5  /* fractional difference to define numerical equality */
#define CNB_INDENT   std::string(5, ' ')

#ifdef CNB_DEBUG
# define PRINT(x) tee<<#x<<" = '"<<x<<"'\n";
#else
# define PRINT(x)
#endif

#ifdef CNB_FLOAT /* use float */
# define cnb_float float
# define CNB_STOF(x) std::stof(x)
# define CNB_SQRT(x) x / cnb::rsqrt(x)
# define CNB_RSQRT(x) cnb::rsqrt(x)
# define CNB_POW(x,p) powf(x,p)
#else /* use double */
# define cnb_float double
# define CNB_STOF(x) std::stod(x)
# define CNB_SQRT(x) sqrt(x)
# define CNB_RSQRT(x) 1.0 / sqrt(x)
# define CNB_POW(x,p) pow(x,p)
#endif

#define CNB_MAX(a,b) (a>b?a:b)
#define CNB_MIN(a,b) (a<b?a:b)

#include <limits>
#define CNB_FLOAT_MAX std::numeric_limits<cnb_float>::max()
#define CNB_FLOAT_MIN std::numeric_limits<cnb_float>::min()
#define DOUBLE_MAX std::numeric_limits<double>::max()
#define DOUBLE_MIN std::numeric_limits<double>::min()

/* defined in functions.cpp */
#define CHECK_RANGE(x,min,max) cnb::check_range_impl(x,min,max,#x);

#endif
