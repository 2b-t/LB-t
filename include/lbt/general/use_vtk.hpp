/**
 * \file     use_vtk.hpp
 * \mainpage Simple pre-processor definition to decide whether VTK should be used or not.
 *           This depends on the presence of the definition LBT_FORCE_NO_VTK as well as if the library could be detected or not.
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#ifndef LBT_FORCE_NO_VTK
  #if __has_include (<vtkSmartPointer.h>)
    #ifndef LBT_USE_VTK
    #define LBT_USE_VTK
    #endif // LBT_USE_VTK
  #endif
#endif // LBT_FORCE_NO_VTK


