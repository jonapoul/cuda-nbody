#ifndef CUDANBODY_CONFIG_H
#define CUDANBODY_CONFIG_H

// #define CNB_CUDA
#define CNB_DEBUG
#define CNB_FLOAT
#define CNB_EPSILON    1e-5  // fractional difference to define numerical equality
#define CNB_TIMESTEP   1e-1  // days

#ifdef CNB_DEBUG
# define DEBUG_PRINT(x) tee<<#x<<" = '"<<x<<"'\n";
#else
# define DEBUG_PRINT(x)
#endif

#ifdef CNB_FLOAT // use float
# define cnb_float float
# define CNB_STOF(x) std::stof(x)
# define CNB_SQRT(x) x / cnb::rsqrt(x)
# define CNB_RSQRT(x) cnb::rsqrt(x)
#else // use double
# define cnb_float double
# define CNB_STOF(x) std::stod(x)
# define CNB_SQRT(x) sqrt(x)
# define CNB_RSQRT(x) 1.0 / sqrt(x)
#endif

#ifndef MAX
# define MAX(a,b) (a>b?a:b)
# define MIN(a,b) (a<b?a:b)
#endif

#include <limits>
#define FLOAT_MAX std::numeric_limits<cnb_float>::max()

#endif