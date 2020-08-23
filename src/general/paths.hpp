#ifndef PATHS_HPP_INCLUDED
#define PATHS_HPP_INCLUDED

/**
 * \file    Paths.hpp
 * \brief   The locations for all folders relevant for export
 * \warning Paths must already exist!
*/

#include <string>


/// Population back-up
std::string const BACKUP_IMPORT_PATH = "backup";
std::string const BACKUP_EXPORT_PATH = "backup";

/// Export for visualisation
std::string const OUTPUT_BIN_PATH = "output/bin";
std::string const OUTPUT_VTK_PATH = "output/vtk";

#endif // PATHS_HPP_INCLUDED
