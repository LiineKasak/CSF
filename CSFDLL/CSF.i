%module CSF
%{
  #define SWIG_FILE_WITH_INIT
  #include "../CSFDLL/CSF.h"
%}

%include "std_string.i"
%include "std_vector.i"

namespace std
{
    %template(VecInt) vector<int>;
    %template(VecFloat) vector<float>;
    %template(VecVecFloat) vector< vector<float> >;
}

%include "../CSFDLL/CSF.h"
