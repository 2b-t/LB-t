#include "lbt/general/vtk_utilities.hpp"

#include "lbt/general/use_vtk.hpp"

#ifdef LBT_USE_VTK
  #include <cstdlib>
  #include <filesystem>
  #include <string>

  #include <vtkImageData.h>
  #include <vtkImageDataToPointSet.h>
  #include <vtkMetaImageWriter.h>
  #include <vtkSmartPointer.h>
  #include <vtkStructuredGrid.h>
  #include <vtkStructuredGridWriter.h>


  namespace lbt {

    void saveImageDataToVtk(vtkSmartPointer<vtkImageData> const& image_data, std::filesystem::path const& output_path, 
                            std::string const& filename) noexcept {
      std::filesystem::create_directories(output_path);

      // Convert to structured grid
      vtkSmartPointer<vtkImageDataToPointSet> image_data_to_point_set {vtkSmartPointer<vtkImageDataToPointSet>::New()};
      image_data_to_point_set->SetInputData(image_data);
      image_data_to_point_set->Update();
      vtkSmartPointer<vtkStructuredGrid> structured_grid {vtkSmartPointer<vtkStructuredGrid>::New()};
      structured_grid->DeepCopy(image_data_to_point_set->GetOutput());

      // Export structured grid as VTK
      std::filesystem::path const filename_with_extension = output_path / std::string{filename + ".vtk"};
      vtkSmartPointer<vtkStructuredGridWriter> structured_grid_writer {vtkSmartPointer<vtkStructuredGridWriter>::New()};
      structured_grid_writer->SetInputData(structured_grid);
      structured_grid_writer->SetFileName(filename_with_extension.c_str());
      structured_grid_writer->Write();

      return;
    }

    void saveImageDataToMhd(vtkSmartPointer<vtkImageData> const& image_data, std::filesystem::path const& output_path, 
                            std::string const& filename, bool const is_compress) noexcept {
      std::filesystem::create_directories(output_path);

      std::filesystem::path const filename_with_extension = output_path / std::string{filename + ".mhd"};
      vtkSmartPointer<vtkMetaImageWriter> meta_image_writer {vtkSmartPointer<vtkMetaImageWriter>::New()};
      meta_image_writer->SetFileName(filename_with_extension.c_str());
      meta_image_writer->SetInputData(image_data);
      meta_image_writer->SetCompression(is_compress);
      meta_image_writer->Write();

      return;
    }

  }
#endif // LBT_USE_VTK