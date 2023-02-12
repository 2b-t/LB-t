/**\file     main.cpp
 * \mainpage Mainpage of the LBM solver
 * \author   Tobit Flatscher (github.com/2b-t)
*/

/*#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

#include "lattice/D3Q15.hpp"
#include "lattice/D3Q19.hpp"
#include "lattice/D3Q27.hpp"
#include "population/collision/collision_bgk_smagorinsky.hpp"
#include "simulation.hpp"


int main(int argc, char* argv[]) {
    using json = nlohmann::json;
    
    // Parse command-line arguments
    if (argc != 2) {
        std::string const command_name {argv[0]};
        std::cerr << "LB-t lattice-Boltzmann fluid dynamics simulation:" << std::endl;
        std::cerr << "Author: Tobit Flatscher - 2021" << std::endl;
        std::cerr << "Usage: " << command_name << " <path/to/settings_file.json>" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::string const filename {argv[1]};

    if (!std::filesystem::exists(filename)) {
        std::cerr << "Error: file '" << filename << "' not found!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Create simulation from file
    std::ifstream filestream(filename);
    json const j = json::parse(filestream);
    
    // TO DO: Add switches for lattices and data formats depending on settings
    // Add wrapper class for simulation that hides the presence of templates
    auto simulation = lbt::Simulation<BGK_Smagorinsky,lattice::D3Q15,double>::parse(j);
    simulation.run();

    return EXIT_SUCCESS;
}*/

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "general/type_definitions.hpp"
#include "geometry/vtk_import.hpp"

//
#include <vtkMetaImageWriter.h>
#include <vtkImageDataToPointSet.h>
#include <vtkStructuredGridWriter.h>
//


//#include "settings/settings_parsing.hpp"

int main(int argc, char* argv[]) {
    //
    vtkSmartPointer<vtkImageData> image_data {vtkSmartPointer<vtkImageData>::New()};
    image_data->SetDimensions(600, 300, 300);
    //image_data->AllocateScalars(VTK_DOUBLE, 3);
    image_data->AllocateScalars(VTK_FLOAT, 3);
    int resolution[3] = {};
    image_data->GetDimensions(resolution);
    for (int x = 0; x < resolution[0]; ++x) {
        for (int y = 0; y < resolution[1]; ++y) {
            for (int z = 0; z < resolution[2]; ++z) {
                double const red = static_cast<double>(x)/resolution[0];
                double const green = static_cast<double>(y)/resolution[1];
                double const blue = static_cast<double>(z)/resolution[2];
                image_data->SetScalarComponentFromDouble(x,y,z,0, red);
                image_data->SetScalarComponentFromDouble(x,y,z,1, green);
                image_data->SetScalarComponentFromDouble(x,y,z,2, blue);
            }
        }
    }
    vtkSmartPointer<vtkMetaImageWriter> writer {vtkSmartPointer<vtkMetaImageWriter>::New()};
    writer->SetFileName("res.mhd");
    writer->SetInputData(image_data);
    //writer->SetCompression(false);
    writer->Write();
    
    vtkSmartPointer<vtkImageDataToPointSet> imageDataToPointSet {vtkSmartPointer<vtkImageDataToPointSet>::New()};
    imageDataToPointSet->SetInputData(image_data);
    imageDataToPointSet->Update();
    vtkSmartPointer<vtkStructuredGrid> transform_poly_data{ vtkSmartPointer<vtkStructuredGrid>::New() };
    transform_poly_data->DeepCopy(imageDataToPointSet->GetOutput());
    
    vtkSmartPointer<vtkStructuredGridWriter> wr {vtkSmartPointer<vtkStructuredGridWriter>::New()};
    wr->SetInputData(transform_poly_data);
    wr->SetFileName("Test.vtk");
    wr->Write();
    //
    
    // Parse command-line arguments
    if ((argc != 3) && (argc != 4)) {
        std::string const command_name{ argv[0] };
        std::cerr << "Converts STL or OBJ geometry files to voxels with the resolution given by resolution_x" << std::endl;
        std::cerr << "Usage: " << command_name << " <filename.stl/.obj> <resolution_x> <reduction_rate>" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::string const filename{ argv[1] };
    int const resolution_x{ std::atoi(argv[2]) };
    double const reduction_rate{ (argc == 4) ? std::atof(argv[3]) : 0.0 };

    // Check input parameters
    if (std::filesystem::exists(filename) == false) {
        std::cerr << "Error: file '" << filename << "' not found!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (resolution_x <= 0) {
        std::cerr << "Parameter <resolution_x> (" << resolution_x << ") has to be positive!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Import geometry file
    lbt::ImportGeometry(filename, resolution_x, reduction_rate);

    return EXIT_SUCCESS;
}
