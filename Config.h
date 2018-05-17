#ifndef CUDANBODY_CONFIG_H
#define CUDANBODY_CONFIG_H

// #define CNB_CUDA
#define CNB_DEBUG
#define CNB_EPSILON    1e-5
#define CNB_TIMESTEP   1e-1  // days
#define CNB_FLOAT

#ifdef CNB_DEBUG
# define DEBUG_PRINT(x) std::cout<<#x<<" = '"<<x"'\n";
#else
# define DEBUG_PRINT(x)
#endif

#ifdef CNB_FLOAT // use float
# define cnb_float float
# define CNB_STOF(x) std::stof(x)
#else // use double
# define cnb_float double
# define CNB_STOF(x) std::stod(x)
#endif

#endif