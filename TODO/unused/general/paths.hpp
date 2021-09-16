#ifndef LBT_PATHS
#define LBT_PATHS

/**
 * \file   Paths.hpp
 * \brief  The locations for all folders relevant for export
 * \author Tobit Flatscher (github.com/2b-t)
*/

#include <string>

namespace path
{
    /// Population back-up
    std::string const backupImport = "backup";
    std::string const backupExport = "backup";

    /// Export for visualisation
    std::string const outputBin = "output/bin";
    std::string const outputVtk = "output/vtk";
}

#endif // LBT_PATHS
