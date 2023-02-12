#ifndef LBT_SETTINGS_PARSING
#define LBT_SETTINGS_PARSING
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"


class CollisionOperator {
    using json = nlohmann::json;
    protected:
        CollisionOperator() = default;
        CollisionOperator(json const& j) noexcept {
            return;
        }
    public:
        virtual json toJson() const noexcept = 0;
};

class BgkSmagorinsky: public CollisionOperator {
    using json = nlohmann::json;
    protected:
        double smagorinsky_constant;
    public:
        constexpr BgkSmagorinsky(double const smagorinsky_constant = 0.17) noexcept
          : smagorinsky_constant{smagorinsky_constant} {
            return;
        }
        BgkSmagorinsky(json const& j) {
            smagorinsky_constant = j["smagorinskyConstant"].get<double>();
            return;
        }
        json toJson() const noexcept final {
            json j{};
            j["smagorinskyConstant"] = smagorinsky_constant;
            return j;
        }
};

enum class FloatingPointPrecision { Half = 2, Single = 4, Double = 8 };

class LbmSettings {
    using json = nlohmann::json;
    protected:
        int dimensions;
        int discrete_directions;
        FloatingPointPrecision floating_point_size;
        std::shared_ptr<CollisionOperator> collision_operator;
    public:
        LbmSettings(int const dimensions = 0, int const discrete_directions = 0, 
                    FloatingPointPrecision const floating_point_size = FloatingPointPrecision::Double,
                    std::shared_ptr<CollisionOperator> collision_operator = nullptr) noexcept
          : dimensions{dimensions}, discrete_directions{discrete_directions}, floating_point_size {floating_point_size},
            collision_operator{collision_operator} {
            return;
        }
        LbmSettings(json const& j) {
            dimensions = j["D"].get<int>();
            discrete_directions = j["Q"].get<int>();
            floating_point_size = static_cast<FloatingPointPrecision>(j["floatingPointSize"].get<int>());
            if (j["collisionOperator"]["type"] == "bgkSmagorinsky") {
                collision_operator = std::shared_ptr<BgkSmagorinsky>(j["collisionOperator"]["parameters"]);
            }
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["D"] = dimensions;
            j["Q"] = discrete_directions;
            j["floatingPointSize"] = static_cast<int>(floating_point_size);
            if (std::dynamic_pointer_cast<BgkSmagorinsky>(collision_operator) != nullptr) {
                j["collisionOperator"]["type"] = "bgkSmagorinsky";
                j["collisionOperator"]["parameters"] = collision_operator->toJson();
            }
            return j;
        }
};

class Discretisation {
    using json = nlohmann::json;
    protected:
        std::int64_t NX;
        std::int64_t NY;
        std::int64_t NZ;

    public:
        constexpr Discretisation(std::int64_t const NX = 0, std::int64_t const NY = 0, 
                                 std::int64_t const NZ = 0) noexcept
          : NX{NX}, NY{NY}, NZ{NZ} {
            return;
        }
        Discretisation(json const& j) {
            NX = j["NX"].get<std::int64_t>();
            NY = j["NY"].get<std::int64_t>();
            NZ = j["NZ"].get<std::int64_t>();
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["NX"] = NX;
            j["NY"] = NY;
            j["NZ"] = NZ;
            return j;
        }
};

class Physics {
    using json = nlohmann::json;
    protected:
        double kinematic_viscosity;
        double velocity;
        double density;

    public:
        constexpr Physics(double const kinematic_viscosity = 0.0, double const velocity = 0.0, double const density = 0.0) noexcept
          : kinematic_viscosity{kinematic_viscosity}, velocity{velocity}, density{density} {
            return;
        }
        Physics(json const& j) {
            kinematic_viscosity = j["kinematicViscosity"].get<double>();
            velocity = j["velocity"].get<double>();
            density = j["density"].get<double>();
            return;
        }

        json toJson() const noexcept {
            json j {};
            j["kinematicViscosity"] = kinematic_viscosity;
            j["velocity"] = velocity;
            j["density"] = density;
            return j;
        }
};

enum class BoundaryType {Unknown, Slip, NoSlip, Velocity, Pressure, Periodic};

std::string to_string(BoundaryType const boundary_type) noexcept {
    std::string type {"unknown"};
    switch (boundary_type) {
        case BoundaryType::Slip:
            type = "slip";
            break;
        case BoundaryType::NoSlip:
            type = "noSlip";
            break;
        case BoundaryType::Velocity:
            type = "velocity";
            break;
        case BoundaryType::Pressure:
            type = "pressure";
            break;
        case BoundaryType::Periodic:
            type = "periodic";
            break;
        default:
            break;
    }
    return type;
}

BoundaryType to_boundary_type(std::string const& name) noexcept {
    BoundaryType boundary_type {BoundaryType::Unknown};
    if (name == "slip") {
        boundary_type = BoundaryType::Slip;
    } else if (name == "noSlip") {
        boundary_type = BoundaryType::NoSlip;
    } else if (name == "velocity") {
        boundary_type = BoundaryType::Velocity;
    } else if (name == "pressure") {
        boundary_type = BoundaryType::Pressure;
    } else if (name == "periodic") {
        boundary_type = BoundaryType::Periodic;
    } else {
        boundary_type = BoundaryType::Unknown;
    }
    return boundary_type;
}

enum class ReferencePlane {Unknown, MinX, MaxX, MinY, MaxY, MinZ, MaxZ};

std::string to_string(ReferencePlane const reference_plane) noexcept {
    std::string type{ "unknown" };
    switch (reference_plane) {
        case ReferencePlane::MinX:
            type = "minX";
            break;
        case ReferencePlane::MaxX:
            type = "maxX";
            break;
        case ReferencePlane::MinY:
            type = "minY";
            break;
        case ReferencePlane::MaxY:
            type = "maxY";
            break;
        case ReferencePlane::MinZ:
            type = "minZ";
            break;
        case ReferencePlane::MaxZ:
            type = "maxZ";
            break;
        default:
            break;
    }
    return type;
}

ReferencePlane to_reference_plane(std::string const& name) noexcept {
    ReferencePlane reference_plane {ReferencePlane::Unknown};
    if (name == "minX") {
        reference_plane = ReferencePlane::MinX;
    } else if (name == "maxX") {
        reference_plane = ReferencePlane::MaxX;
    } else if (name == "minY") {
        reference_plane = ReferencePlane::MinY;
    } else if (name == "maxY") {
        reference_plane = ReferencePlane::MaxY;
    } else if (name == "minZ") {
        reference_plane = ReferencePlane::MinZ;
    } else if (name == "maxZ") {
        reference_plane = ReferencePlane::MaxZ;
    } else {
        reference_plane = ReferencePlane::Unknown;
    }
    return reference_plane;
}

class BoundaryCondition {
    using json = nlohmann::json;
    protected:
        BoundaryCondition(BoundaryType const boundary_type = BoundaryType::Slip, 
                          ReferencePlane const reference_plane = ReferencePlane::MinX)
          : boundary_type{boundary_type}, reference_plane{reference_plane} {
            return;
        }
        BoundaryCondition(json const& j) {
            boundary_type = to_boundary_type(j["boundaryType"]);
            reference_plane = to_reference_plane(j["referencePlane"]);
            return;
        }

        BoundaryType boundary_type;
        ReferencePlane reference_plane;
    public:
        virtual json toJson() const noexcept {
            json j {};
            j["boundaryType"] = to_string(boundary_type);
            j["referencePlane"] = to_string(reference_plane);
            return j;
        }
};

class SlipBoundaryCondition: public BoundaryCondition {
    using json = nlohmann::json;
    public:
        SlipBoundaryCondition(ReferencePlane const reference_plane = ReferencePlane::MinX)
            : BoundaryCondition{BoundaryType::Slip, reference_plane} {
            return;
        }
        SlipBoundaryCondition(json const& j)
          : BoundaryCondition{j} {
            return;
        }
};

class NoSlipBoundaryCondition: public BoundaryCondition {
    using json = nlohmann::json;
    public:
        NoSlipBoundaryCondition(ReferencePlane const reference_plane = ReferencePlane::MinX)
            : BoundaryCondition{BoundaryType::NoSlip, reference_plane} {
            return;
        }
        NoSlipBoundaryCondition(json const& j)
          : BoundaryCondition{j} {
            return;
        }
};

template <typename T>
class Vector {
    using json = nlohmann::json;
    protected:
        T x;
        T y;
        T z;
    public:
        constexpr Vector(double const x = 0.0, double const y = 0.0, double const z = 0.0)
          : x{x}, y{y}, z{z} {
            return;
        }
        Vector(json const& j) {
            x = j["x"].get<T>();
            y = j["y"].get<T>();
            z = j["z"].get<T>();
            return;
        }

        json toJson() const noexcept {
            json j {};
            j["x"] = x;
            j["y"] = y;
            j["z"] = z;
            return j;
        }
};

class VelocityBoundaryCondition: public BoundaryCondition {
    using json = nlohmann::json;
    protected:
        Vector<double> velocity;
    public:
        VelocityBoundaryCondition(ReferencePlane const reference_plane = ReferencePlane::MinX,
                                  Vector<double> const& velocity = Vector<double>())
            : BoundaryCondition{BoundaryType::Velocity, reference_plane}, velocity{velocity} {
            return;
        }
        VelocityBoundaryCondition(json const& j) {
            boundary_type = to_boundary_type(j["boundaryType"]);
            reference_plane = to_reference_plane(j["referencePlane"]);
            velocity = Vector<double>{j["velocity"]};
            return;
        }
        json toJson() const noexcept override {
            json j {};
            j["boundaryType"] = to_string(boundary_type);
            j["referencePlane"] = to_string(reference_plane);
            j["velocity"] = velocity.toJson();
            return j;
        }
};

class PressureBoundaryCondition: public BoundaryCondition {
    using json = nlohmann::json;
    protected:
        double density;
    public:
        PressureBoundaryCondition(ReferencePlane const reference_plane = ReferencePlane::MinX,
                                  double const density = 0.0)
            : BoundaryCondition{BoundaryType::Pressure, reference_plane}, density{density} {
            return;
        }
        PressureBoundaryCondition(json const& j) {
            boundary_type = to_boundary_type(j["boundaryType"]);
            reference_plane = to_reference_plane(j["referencePlane"]);
            density = j["density"].get<double>();
            return;
        }
        json toJson() const noexcept override {
            json j{};
            j["boundaryType"] = to_string(boundary_type);
            j["referencePlane"] = to_string(reference_plane);
            j["density"] = density;
            return j;
        }
};

class PeriodicBoundaryCondition : public BoundaryCondition {
    using json = nlohmann::json;
public:
    PeriodicBoundaryCondition(ReferencePlane const reference_plane = ReferencePlane::MinX)
        : BoundaryCondition{BoundaryType::Periodic, reference_plane} {
        return;
    }
    PeriodicBoundaryCondition(json const& j)
        : BoundaryCondition{j} {
        return;
    }
};

class InitialConditions {
    using json = nlohmann::json;
    protected:
        Vector<double> velocity;
    public:
        InitialConditions(Vector<double> const& velocity = Vector<double>{})
          : velocity{velocity} {
            return;
        }
        InitialConditions(json const& j) {
            velocity = Vector<double>{j["velocity"]};
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["velocity"] = velocity.toJson();
            return j;
        }
};

class BoundingBox {
    using json = nlohmann::json;
    protected:
        Vector<double> start;
        Vector<double> end;
    public:
        BoundingBox(Vector<double> const& start = Vector<double>{}, 
                    Vector<double> const& end = Vector<double>{})
          : start{start}, end{end} {
            return;
        }
        BoundingBox(json const& j) {
            start = Vector<double>{j["start"]};
            end = Vector<double>{j["end"]};
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["start"] = start.toJson();
            j["end"] = end.toJson();
            return j;
        }
};

class Model {
    using json = nlohmann::json;
    protected:
        std::string filename;
        std::optional<double> reduction_rate;
    public:
        Model(std::string const filename = "", std::optional<double> reduction_rate = std::nullopt) noexcept
          : filename{filename}, reduction_rate{reduction_rate} {
            return;
        }
        Model(json const& j) {
            filename = j["filename"].get<std::string>();
            if (j.count("reductionRate") > 0) {
                reduction_rate = j["reductionRate"].get<double>();
            }
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["filename"] = filename;
            if (reduction_rate.has_value()) {
                j["reductionRate"] = reduction_rate.value();
            }
            return j;
        }
};

class Geometry {
    using json = nlohmann::json;
    protected:
        BoundingBox bounding_box;
        std::vector<Model> models;
        std::optional<double> z_rotation;
    public:
        Geometry(BoundingBox const& bounding_box = BoundingBox{}, std::vector<Model> const& models = {Model{}},
                 std::optional<double> z_rotation = std::nullopt)
          : bounding_box{bounding_box}, models{models}, z_rotation{z_rotation} {
            return;
        }
        Geometry(json const& j) {
            bounding_box = BoundingBox{j["boundingBox"]};
            json const& ja = j["models"];
            for (auto it = ja.begin(); it != ja.end(); ++it) {
                models.emplace_back(*it);
            }
            if (j.count("zRotation") > 0) {
                z_rotation = j["zRotation"].get<double>();
            }
            return;
        }
        json toJson() const noexcept {
            json j {};
            j["boundingBox"] = bounding_box.toJson();
            json ja = json::array();
            for (auto const& m: models) {
                ja.push_back(m.toJson());
            }
            j["models"] = ja;
            if (z_rotation.has_value()) {
                j["zRotation"] = z_rotation.value();
            }
            return j;
        }
};

class Target {
    using json = nlohmann::json;
    protected:
        std::string type;
    public:
        Target(std::string const& type = "") noexcept 
          : type{type} {
            return;
        }
        virtual json toJson() const noexcept = 0;
};

class CpuTarget: public Target {
    using json = nlohmann::json;
    protected:
        int number_of_threads;
    public:
        CpuTarget(int const number_of_threads = 1)
          : Target{"cpu"}, number_of_threads{number_of_threads} {
            return;
        }
        CpuTarget(json const& j) {
            type = j["type"];
            number_of_threads = j["numberOfThreads"];
            return;
        }

        json toJson() const noexcept final {
            json j {};
            j["type"] = type;
            j["numberOfThreads"] = number_of_threads;
            return j;
        }
};

enum class DataFormat {Unknown, Vtk};

std::string to_string(DataFormat const data_format) noexcept {
    std::string type{ "unknown" };
    switch (data_format) {
        case DataFormat::Vtk:
            type = "vtk";
            break;
        default:
            break;
    }
    return type;
}

DataFormat to_data_format(std::string const& name) noexcept  {
    DataFormat data_format {DataFormat::Unknown};
    if (name == "vtk") {
        data_format = DataFormat::Vtk;
    } else {
        data_format = DataFormat::Unknown;
    }
    return data_format;
}

class Output {
    using json = nlohmann::json;
    protected:
        double first_output;
        double last_output;
        double interval;
        std::string output_folder;
        DataFormat data_format;
    public:
        Output(double const first_output = 0.0, double const last_output = 0.0, double const interval = 0.0,
               std::string const& output_folder = "", DataFormat const data_format = DataFormat::Vtk)
          : first_output{first_output}, last_output{last_output}, interval{interval},
            output_folder{output_folder}, data_format{data_format} {
            return;
        }
        Output(json const& j) {
            first_output = j["firstOutput"].get<double>();
            last_output = j["lastOutput"].get<double>();
            interval = j["interval"].get<double>();
            output_folder = j["outputFolder"].get<std::string>();
            data_format = to_data_format(j["dataFormat"]);

            return;
        }
        json toJson() const noexcept {
            json j {};
            j["firstOutput"] = first_output;
            j["lastOutput"] = last_output;
            j["interval"] = interval;
            j["outputFolder"] = output_folder;
            j["dataFormat"] = to_string(data_format);
            return j;
        }
};

class Settings {
  using json = nlohmann::json;
    protected:
        LbmSettings settings;
        Discretisation discretisation;
        Physics physics;
        std::vector<std::shared_ptr<BoundaryCondition>> boundary_conditions;
        InitialConditions initial_conditions;
        Geometry geometry;
        std::shared_ptr<Target> target;
        Output output;
        std::string start_from;
        double duration;

    public:
        Settings(LbmSettings const& settings, Discretisation const& discretisation, Physics const& physics,
                 std::vector<std::shared_ptr<BoundaryCondition>> boundary_conditions,
                 InitialConditions const& initial_conditions, Geometry const& geometry, 
                 std::shared_ptr<Target> target, Output const& output, std::string const& start_from,
                 double const duration) noexcept
          : settings{settings}, discretisation{discretisation}, physics{physics}, boundary_conditions{boundary_conditions},
            initial_conditions{ initial_conditions }, geometry{geometry}, target{target}, output{output}, 
            start_from{start_from}, duration{duration} {
            return;
        }
        Settings(json const& j) {
            settings = LbmSettings{j["lbmSettings"]};
            discretisation = Discretisation{j["discretisation"]};
            physics = Physics{j["physics"]};
            json const& ja = j["boundaryConditions"];
            for (auto it = ja.begin(); it != ja.end(); ++it) {
                if ((*it)["type"] == "Slip") {
                    boundary_conditions.push_back(std::make_shared<SlipBoundaryCondition>(*it));
                }
                else if ((*it)["type"] == "noSlip") {
                    boundary_conditions.push_back(std::make_shared<NoSlipBoundaryCondition>(*it));
                } else if ((*it)["type"] == "velocity") {
                    boundary_conditions.push_back(std::make_shared<VelocityBoundaryCondition>(*it));
                } else if ((*it)["type"] == "pressure") {
                    boundary_conditions.push_back(std::make_shared<PressureBoundaryCondition>(*it));
                } else if ((*it)["type"] == "periodic") {
                    boundary_conditions.push_back(std::make_shared<PeriodicBoundaryCondition>(*it));
                }
            }
            initial_conditions = InitialConditions{j["initialConditions"]};
            geometry = Geometry{j["geometry"]};
            if (j["target"]["type"] == "cpu") {
                target = std::make_shared<CpuTarget>(j["target"]);
            }
            output = Output{j["output"]};
            start_from = j["startFrom"].get<std::string>();
            duration = j["duration"].get<double>();
            return;
        }

        json toJson() const noexcept {
            json j {};
            j["lbmSettings"] = settings.toJson();
            j["discretisation"] = discretisation.toJson();
            j["physics"] = physics.toJson();
            json ja = json::array();
            for (auto const& b: boundary_conditions) {
                ja.push_back(b->toJson());
            }
            j["boundaryConditions"] = ja;
            j["initialConditions"] = initial_conditions.toJson();
            j["geometry"] = geometry.toJson();
            if (target != nullptr) {
                j["target"] = target->toJson();
            }
            j["output"] = output.toJson();
            j["startFrom"] = start_from;
            j["duration"] = duration;
            return j;
        }
};

#endif LBT_SETTINGS_PARSING 