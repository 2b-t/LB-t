#include "vtk_import.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#if __has_include (<vtkSmartPointer.h>)
  #include <vtkCleanPolyData.h>
  #include <vtkDecimatePro.h>
  #include <vtkFloatArray.h>
  #include <vtkImageCast.h>
  #include <vtkImageData.h>
  #include <vtkImageDataToPointSet.h>
  #include <vtkImageDilateErode3D.h>
  #include <vtkImageMathematics.h>
  #include <vtkImageOpenClose3D.h>
  #include <vtkImageSeparableConvolution.h>
  #include <vtkImageStencilToImage.h>
  #include <vtkImageThreshold.h>
  #include <vtkMetaImageReader.h>
  #include <vtkMetaImageWriter.h>
  #include <vtkOBJReader.h>
  #include <vtkPLYReader.h>
  #include <vtkPointData.h>
  #include <vtkPolyData.h>
  #include <vtkPolyDataReader.h>
  #include <vtkPolyDataToImageStencil.h>
  #include <vtkSmartPointer.h>
  #include <vtkSTLReader.h>
  #include <vtkStructuredGrid.h>
  #include <vtkStructuredGridWriter.h>
  #include <vtkTransform.h>
  #include <vtkTransformPolyDataFilter.h>
  #include <vtkXMLPolyDataReader.h>

  #include "../general/vtk_utilities.hpp"


  namespace lbt {

    vtkSmartPointer<vtkPolyData> Importer::loadObj(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkOBJReader> obj_reader {vtkSmartPointer<vtkOBJReader>::New()};
      obj_reader->SetFileName(filename.c_str());
      obj_reader->Update();
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      poly_data->DeepCopy(obj_reader->GetOutput());
      return poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::loadPly(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkPLYReader> ply_reader {vtkSmartPointer<vtkPLYReader>::New()};
      ply_reader->SetFileName(filename.c_str());
      ply_reader->Update();
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      poly_data->DeepCopy(ply_reader->GetOutput());
      return poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::loadStl(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkSTLReader> stl_reader {vtkSmartPointer<vtkSTLReader>::New()};
      stl_reader->SetFileName(filename.c_str());
      stl_reader->Update();
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      poly_data->DeepCopy(stl_reader->GetOutput());
      return poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::loadVtk(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkPolyDataReader> vtk_reader {vtkSmartPointer<vtkPolyDataReader>::New()};
      vtk_reader->SetFileName(filename.c_str());
      vtk_reader->Update();
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      poly_data->DeepCopy(vtk_reader->GetOutput());
      return poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::loadVtp(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkXMLPolyDataReader> vtp_reader {vtkSmartPointer<vtkXMLPolyDataReader>::New()};
      vtp_reader->SetFileName(filename.c_str());
      vtp_reader->Update();
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      poly_data->DeepCopy(vtp_reader->GetOutput());
      return poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::rotatePolyData(vtkSmartPointer<vtkPolyData> input_poly_data, double const rotation_angle, 
                                                          Axis const rotation_axis) noexcept {
      vtkSmartPointer<vtkTransform> transform {vtkSmartPointer<vtkTransform>::New()};
      
      if (rotation_axis == Axis::x) {
        transform->RotateWXYZ(rotation_angle, 1, 0, 0);
      } else if (rotation_axis == Axis::y) {
        transform->RotateWXYZ(rotation_angle, 0, 1, 0);
      } else if (rotation_axis == Axis::z) {
        transform->RotateWXYZ(rotation_angle, 0, 0, 1);
      }

      vtkSmartPointer<vtkTransformPolyDataFilter> transform_filter {vtkSmartPointer<vtkTransformPolyDataFilter>::New()};
      transform_filter->SetTransform(transform);
      transform_filter->SetInputData(input_poly_data);
      transform_filter->Update();
      vtkSmartPointer<vtkPolyData> transform_poly_data {vtkSmartPointer<vtkPolyData>::New()};
      transform_poly_data->DeepCopy(transform_filter->GetOutput());
      return transform_poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::translatePolyData(vtkSmartPointer<vtkPolyData> input_poly_data, 
                                                            std::array<double,3> const& translation) noexcept {
      vtkSmartPointer<vtkTransform> transform {vtkSmartPointer<vtkTransform>::New()};
      transform->Translate(translation.at(0), translation.at(1), translation.at(2));
      vtkSmartPointer<vtkTransformPolyDataFilter> transform_filter {vtkSmartPointer<vtkTransformPolyDataFilter>::New()};
      transform_filter->SetTransform(transform);
      transform_filter->SetInputData(input_poly_data);
      transform_filter->Update();
      vtkSmartPointer<vtkPolyData> transform_poly_data {vtkSmartPointer<vtkPolyData>::New()};
      transform_poly_data->DeepCopy(transform_filter->GetOutput());
      return transform_poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::cleanPolyData(vtkSmartPointer<vtkPolyData> poly_data) noexcept {
      vtkSmartPointer<vtkCleanPolyData> clean_poly_data {vtkSmartPointer<vtkCleanPolyData>::New()};
      clean_poly_data->SetInputData(poly_data);
      clean_poly_data->Update();

      vtkSmartPointer<vtkPolyData> cleaned_poly_data {vtkSmartPointer<vtkPolyData>::New()};
      cleaned_poly_data->DeepCopy(clean_poly_data->GetOutput());
      return cleaned_poly_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::reducePolyData(vtkSmartPointer<vtkPolyData> poly_data, double const reduction_rate) noexcept {
      vtkSmartPointer<vtkDecimatePro> decimate {vtkSmartPointer<vtkDecimatePro>::New()};
      decimate->SetInputData(poly_data);
      decimate->SetTargetReduction(reduction_rate);
      decimate->Update();

      vtkSmartPointer<vtkPolyData> reduced_poly_data {vtkSmartPointer<vtkPolyData>::New()};
      reduced_poly_data->DeepCopy(decimate->GetOutput());
      return reduced_poly_data;
    }

    vtkSmartPointer<vtkImageData> Importer::newImageData(int const resolution_x, std::array<double, 6> const& bounding_box,
                                                        char const fill_colour) noexcept {
      // Calculate domain size and resolution
      double const domain_size[3] = {bounding_box[1] - bounding_box[0], bounding_box[3] - bounding_box[2], bounding_box[5] - bounding_box[4]};
      int const resolution[3] = {resolution_x, static_cast<int>(domain_size[1]/domain_size[0]*resolution_x), static_cast<int>(domain_size[2]/domain_size[0]*resolution_x)};
      
      // Prepare voxel image: set resolution, spacing, extent, origin, image data type and background colour
      vtkSmartPointer<vtkImageData> image_data {vtkSmartPointer<vtkImageData>::New()};
      image_data->SetDimensions(resolution);
      double const spacing[3] = {domain_size[0]/resolution[0], domain_size[1]/resolution[1], domain_size[2]/resolution[2]};
      image_data->SetSpacing(spacing);
      double const origin[3] = {bounding_box[0] + spacing[0]/2.0, bounding_box[2] + spacing[1]/2.0,  bounding_box[4] + spacing[2]/2.0};
      image_data->SetOrigin(origin);
      image_data->AllocateScalars(Importer::scalar_data_type, 1);
      image_data->GetPointData()->GetScalars()->Fill(fill_colour);
      
      return image_data;
    }

    vtkSmartPointer<vtkImageData> Importer::mergeImageData(vtkSmartPointer<vtkImageData> const& image_data_1,
                                                          vtkSmartPointer<vtkImageData> const& image_data_2) noexcept {
      vtkSmartPointer<vtkImageMathematics> image_mathematics {vtkSmartPointer<vtkImageMathematics>::New()};
      if (Importer::background_colour < Importer::foreground_colour) {
        // Merge by pixelwise maximum
        image_mathematics->SetOperationToMax();
      } else {
        // Merge by pixelwise minimum
        image_mathematics->SetOperationToMin();
      }
      image_mathematics->SetInput1Data(image_data_1);
      image_mathematics->SetInput2Data(image_data_2);
      image_mathematics->Update();
      vtkSmartPointer<vtkImageData> merged_image_data {vtkSmartPointer<vtkImageData>::New()};
      merged_image_data->DeepCopy(image_mathematics->GetOutput());
      return merged_image_data;
    }

    vtkSmartPointer<vtkImageData> Importer::voxelisePolyData(vtkSmartPointer<vtkPolyData> const& poly_data, int const resolution_x,
                                                            std::optional<std::array<double, 6>> const& opt_bounding_box) noexcept {
      // Get geometry bounding box and calculate domain size and resolution
      std::array<double, 6> bounding_box = {};
      if (opt_bounding_box == std::nullopt) {
        double bounds[6] = {};
        poly_data->GetBounds(bounds);
        std::copy(std::begin(bounds), std::end(bounds), std::begin(bounding_box));
      } else {
        bounding_box = opt_bounding_box.value();
        // Check bounding box of geometry against manual bounding box
        double poly_data_bounding_box[6] = {};
        poly_data->GetBounds(poly_data_bounding_box);
        if ((poly_data_bounding_box[0] < bounding_box[0]) ||
            (poly_data_bounding_box[1] > bounding_box[1]) ||
            (poly_data_bounding_box[2] < bounding_box[2]) ||
            (poly_data_bounding_box[3] > bounding_box[3]) ||
            (poly_data_bounding_box[4] < bounding_box[4]) ||
            (poly_data_bounding_box[5] > bounding_box[5])) {
          std::clog << "Warning: Model out of bounds!" << std::endl;
        }
      }

      vtkSmartPointer<vtkImageData> image_data = Importer::newImageData(resolution_x, bounding_box);

      // Convert poly-data to image stencil
      vtkSmartPointer<vtkPolyDataToImageStencil> pd_to_img_stencil {vtkSmartPointer<vtkPolyDataToImageStencil>::New()};
      pd_to_img_stencil->SetInputData(poly_data);
      pd_to_img_stencil->SetOutputSpacing(image_data->GetSpacing());
      pd_to_img_stencil->SetOutputOrigin(image_data->GetOrigin());
      pd_to_img_stencil->SetOutputWholeExtent(image_data->GetExtent());
      pd_to_img_stencil->Update();
      // Convert image stencil to voxel image
      vtkSmartPointer<vtkImageStencilToImage> img_stencil_to_img {vtkSmartPointer<vtkImageStencilToImage>::New()};
      img_stencil_to_img->SetInputConnection(pd_to_img_stencil->GetOutputPort());
      img_stencil_to_img->SetOutsideValue(Importer::background_colour);
      img_stencil_to_img->SetInsideValue(Importer::foreground_colour);
      img_stencil_to_img->Update();

      image_data->DeepCopy(img_stencil_to_img->GetOutput());
      return image_data;
    }

    vtkSmartPointer<vtkImageData> Importer::cleanImageData(vtkSmartPointer<vtkImageData> image_data) noexcept {    
      vtkSmartPointer<vtkImageDilateErode3D> dilate {vtkSmartPointer<vtkImageDilateErode3D>::New()};
      dilate->SetInputData(image_data);
      dilate->SetDilateValue(Importer::foreground_colour);
      dilate->SetErodeValue(Importer::background_colour);
      dilate->SetKernelSize(3, 3, 3);
      dilate->Update();

      vtkSmartPointer<vtkImageData> cleaned_image_data {vtkSmartPointer<vtkImageData>::New()};
      cleaned_image_data->DeepCopy(dilate->GetOutput());
      return cleaned_image_data;
    }
    
    vtkSmartPointer<vtkImageData> Importer::caveOutImageData(vtkSmartPointer<vtkImageData> image_data) noexcept {    
      // Kernel "area" as kernel_width*kernel_height*kernel_depth
      constexpr double kernel_threshold {27.0};
      constexpr double kernel_component {1.0/3.0};
      // Separable kernel
      vtkSmartPointer<vtkFloatArray> kernel {vtkSmartPointer<vtkFloatArray>::New()};
      kernel->SetNumberOfTuples(3);
      kernel->SetNumberOfComponents(1);
      kernel->SetValue(0, kernel_component);
      kernel->SetValue(1, kernel_component);
      kernel->SetValue(2, kernel_component);

      // Perform 3D separable image convolution
      vtkSmartPointer<vtkImageSeparableConvolution> convolution {vtkSmartPointer<vtkImageSeparableConvolution>::New()};
      convolution->SetInputData(image_data);
      convolution->SetXKernel(kernel);
      convolution->SetYKernel(kernel);
      convolution->SetZKernel(kernel);
      convolution->Update();

      // Threshold the resulting inner pixels
      vtkSmartPointer<vtkImageThreshold> threshold {vtkSmartPointer<vtkImageThreshold>::New()};
      threshold->SetInputConnection(convolution->GetOutputPort());
      threshold->ThresholdBetween(std::pow(kernel_component,3)*static_cast<double>(Importer::foreground_colour), 
                                  (kernel_threshold-1.0)*std::pow(kernel_component,3)*static_cast<double>(Importer::foreground_colour));
      threshold->ReplaceInOn();
      threshold->SetInValue(Importer::foreground_colour);
      threshold->ReplaceOutOn();
      threshold->SetOutValue(Importer::background_colour);
      threshold->Update();

      // Cast image back to initial data type
      vtkSmartPointer<vtkImageCast> image_cast {vtkSmartPointer<vtkImageCast>::New()};
      image_cast->SetInputConnection(threshold->GetOutputPort());
      image_cast->SetOutputScalarType(Importer::scalar_data_type);
      image_cast->Update();

      vtkSmartPointer<vtkImageData> caved_out_image_data {vtkSmartPointer<vtkImageData>::New()};
      caved_out_image_data->DeepCopy(image_cast->GetOutput());
      return caved_out_image_data;
    }

    vtkSmartPointer<vtkPolyData> Importer::load(std::filesystem::path const& filename) noexcept {
      // Import STL/OBJ/PLY/VTK/TP file with corresponding importer
      std::filesystem::path const file_format {filename.extension()};
      vtkSmartPointer<vtkPolyData> poly_data {vtkSmartPointer<vtkPolyData>::New()};
      if (file_format == ".obj") {
        poly_data = Importer::loadObj(filename);
      } else if (file_format == ".stl") {
        poly_data = Importer::loadStl(filename);
      } else if (file_format == ".ply") {
        poly_data = Importer::loadPly(filename);
      } else if (file_format == ".vtk") {
        poly_data = Importer::loadVtk(filename);
      } else if (file_format == ".vtp") {
        poly_data = Importer::loadVtp(filename);
      } else {
        std::cerr << "Error: File ending of " << filename << " not recognised!" << std::endl;
        std::exit(EXIT_FAILURE);
      }
      return poly_data;
    }

    vtkSmartPointer<vtkImageData> Importer::importFromFile(std::filesystem::path const& filename, int const resolution_x, double const reduction_rate, 
                                                          std::optional<std::array<double, 6>> const& opt_bounding_box) noexcept {
      // Load poly-data from file
      vtkSmartPointer<vtkPolyData> poly_data = Importer::load(filename);

      // Initial output
      std::clog << "File '" << filename << "' imported." << std::endl;
      std::clog << "Number of polygons: " << poly_data->GetNumberOfPolys() << 
                  ", number of lines: " << poly_data->GetNumberOfLines() << std::endl;

      // Clean poly-data for all file formats but VTK
      std::clog << "Cleaning poly-data..." << std::endl;
      poly_data = Importer::cleanPolyData(poly_data);
      std::clog << "Number of polygons: " << poly_data->GetNumberOfPolys() << 
                  ", number of lines: " << poly_data->GetNumberOfLines() << std::endl;
      
      std::filesystem::path const file_format {filename.extension()};
      if (file_format != ".vtk") {
        // Reduce number of polygons if desired
        if (reduction_rate > 0.0) {
          std::clog << "Decimating with reduction rate " << reduction_rate << "..." << std::endl;
          poly_data = Importer::reducePolyData(poly_data, reduction_rate);
          std::clog << "Number of polygons: " << poly_data->GetNumberOfPolys() << 
                      ", number of lines: " << poly_data->GetNumberOfLines() << std::endl;
        }
      }

      // Voxelise the poly-data object
      vtkSmartPointer<vtkImageData> image_data = Importer::voxelisePolyData(poly_data, resolution_x, opt_bounding_box);
      std::clog << "Geometry voxelised!" << std::endl;
      // Output information about domain size and resolution
      double bounds[6] = {};
      image_data->GetBounds(bounds);
      double const domain_size[3] = {bounds[1] - bounds[0], bounds[3] - bounds[2], bounds[5] - bounds[4]};
      int resolution[3] = {};
      image_data->GetDimensions(resolution);
      std::clog << "Domain size (x,y,z): " << domain_size[0] << ", " << domain_size[1] << ", " << domain_size[2] << std::endl;
      std::clog << "Resolution  (x,y,z): " << resolution[0]  << ", " << resolution[1]  << ", " << resolution[2]  << std::endl;

      // Clean image-data object
      image_data = Importer::cleanImageData(image_data);

      return image_data;
    }

    Geometry Geometry::importFromFiles(std::vector<std::filesystem::path> const& filenames, int const resolution_x, double const reduction_rate,
                                      std::optional<std::array<double, 6>> const& opt_bounding_box) noexcept {
      std::array<double, 6> bounding_box = {std::numeric_limits<double>::max(), std::numeric_limits<double>::min(),
                                            std::numeric_limits<double>::max(), std::numeric_limits<double>::min(),
                                            std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};
      if (opt_bounding_box == std::nullopt) {
        // Loop over files and determine maximum bounding box
        for (auto const& f: filenames) {
          vtkSmartPointer<vtkPolyData> poly_data = Importer::load(f);
          double bounds[6] = {};
          poly_data->GetBounds(bounds);
          if (bounds[0] < bounding_box.at(0)) {
            bounding_box.at(0) = bounds[0];
          }
          if (bounds[1] > bounding_box.at(1)) {
            bounding_box.at(1) = bounds[1];
          }
          if (bounds[2] < bounding_box.at(2)) {
            bounding_box.at(2) = bounds[2];
          }
          if (bounds[3] > bounding_box.at(3)) {
            bounding_box.at(3) = bounds[3];
          }
          if (bounds[4] < bounding_box.at(4)) {
            bounding_box.at(4) = bounds[4];
          }
          if (bounds[5] > bounding_box.at(5)) {
            bounding_box.at(5) = bounds[5];
          }
        }
      } else {
        bounding_box = opt_bounding_box.value();
      }
      
      std::clog << "Bounding box set to: x = [" << bounding_box.at(0) << ", " << bounding_box.at(1) << "], " <<
                                        "y = [" << bounding_box.at(2) << ", " << bounding_box.at(3) << "], " <<
                                        "z = [" << bounding_box.at(4) << ", " << bounding_box.at(5) << "]" << std::endl;

      // Create initial empty geometry from resolution and bounding box
      vtkSmartPointer<vtkImageData> merged_image_data = Importer::newImageData(resolution_x, bounding_box);

      // Parallel import of geometry
      #pragma omp parallel for default(none) shared(merged_image_data) firstprivate(filenames, resolution_x, reduction_rate, bounding_box)
      for (auto const& f: filenames) {
        // Import geometry and voxelise it (runs some filters)
        vtkSmartPointer<vtkImageData> single_image_data = Importer::importFromFile(f, resolution_x, reduction_rate, bounding_box);

        // Merge the domain to the existing one
        #pragma omp critical
        merged_image_data = Importer::mergeImageData(merged_image_data, single_image_data);
      }

      // Cave-out resulting image-data
      merged_image_data = Importer::caveOutImageData(merged_image_data);

      // Return merged geometry
      Geometry const geometry {merged_image_data};
      return geometry;
    }

    Geometry Geometry::importFromMhd(std::filesystem::path const& filename) noexcept {
      vtkSmartPointer<vtkMetaImageReader> meta_image_reader {vtkSmartPointer<vtkMetaImageReader>::New()};
      meta_image_reader->SetFileName(filename.c_str());
      meta_image_reader->Update();
      vtkSmartPointer<vtkImageData> image_data {vtkSmartPointer<vtkImageData>::New()};
      image_data->DeepCopy(meta_image_reader->GetOutput());

      Geometry const geometry {image_data};
      return geometry;
    }

    void Geometry::saveToMhd(std::filesystem::path const& output_path, std::string const& filename, bool const is_compress) const noexcept {
      saveImageDataToMhd(image_data, output_path, filename, is_compress);
      return;
    }
    
    void Geometry::saveToVtk(std::filesystem::path const& output_path, std::string const& filename) const noexcept {
      saveImageDataToVtk(image_data, output_path, filename);
      return;
    }

    void Geometry::saveToFile(std::filesystem::path const& output_path, std::string const& filename, DataType const data_type, bool const is_compress) const noexcept {
      if (data_type == DataType::MHD) {
        saveToMhd(output_path, filename, is_compress);
      } else if (data_type == DataType::VTK) {
        saveToVtk(output_path, filename);
      }
      return;
    }

  }
#endif