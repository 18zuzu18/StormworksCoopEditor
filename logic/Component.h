//
// Created by jens on 14.06.20.
//

#ifndef STROMWORKSCOOPEDITOR_COMPONENT_H
#define STROMWORKSCOOPEDITOR_COMPONENT_H

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <rapidxml.hpp>
#include <filesystem>

static std::string gamepath = "D:/SteamLibrary/steamapps/common/Stormworks/rom/";

#define LOGIC_BOOL 0
#define LOGIC_NUM 1
#define LOGIC_ELE 4

struct LogicNode {
    std::string label;
    std::string description;

    unsigned char type;
    unsigned char mode;
    int flags;

    glm::vec3 position;
};

struct Surface {
    glm::vec3 position;
    unsigned char orientation;
    unsigned char rotation;
    unsigned char shape;
    unsigned char transType;
};

struct Voxel {
    glm::vec3 position;
    glm::mat3 physicsShapeRotation;
    unsigned char flags;
    unsigned char physicsShape;
    unsigned char buoyPipes;
};

struct Component {
private:
    rapidxml::xml_node<char> *definition;

    void readString(std::string name, std::string *out) {
        *out = definition->first_attribute(name.c_str()) ? definition->first_attribute(name.c_str())->value() : "";
    }

    void readInt(std::string name, int *out) {
        *out = definition->first_attribute(name.c_str()) ? atoi(definition->first_attribute(name.c_str())->value()) : 0;
    }

    void readFloat(std::string name, float *out) {
        *out = definition->first_attribute(name.c_str()) ? atof(definition->first_attribute(name.c_str())->value())
                                                         : 0.0f;
    }

    void readBool(std::string name, bool *out) {
        if (!definition->first_attribute(name.c_str())) {
            *out = false;
            return;
        }
        std::stringstream ss(std::string(definition->first_attribute(name.c_str())->value()));
        if (!(ss >> std::boolalpha >> *out)) {
            // Parsing error.
        }
    }

public:

    Mesh mesh;
    Mesh mesh0;
    Mesh mesh1;

    std::string filename;

    explicit Component(std::string path) {
        rapidxml::file<> xmlFile(path.c_str()); // Default template is char
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        definition = doc.first_node("definition");
        // Values
        readString("name", &name);
        readInt("category", &category);
        readInt("type", &type);
        readInt("mass", &mass);
        readInt("value", &value);
        readInt("flags", &flags);
        readString("tags", &tags);
        readInt("phys_collision_dampen", &physCollisionDampen);
        readString("audio_filename_start", &audioFilenameStart);
        readString("audio_filename_loop", &audioFilenameLoop);
        readString("audio_filename_end", &audioFilenameEnd);
        readString("audio_filename_start_b", &audioFilenameStartB);
        readString("audio_filename_loop_b", &audioFilenameLoopB);
        readString("audio_filename_end_b", &audioFilenameEndB);
        readFloat("audio_gain", &audioGain);
        readString("mesh_data_name", &meshDataName);
        readString("mesh_0_name", &mesh0Name);
        readString("mesh_1_name", &mesh1Name);
        readString("mesh_editor_only_name", &meshEditorOnlyName);
        readInt("block_type", &blockType);
        readString("child_name", &childName);
        readString("extender_name", &extenderName);
        readInt("constraint_type", &constraintType);
        readInt("constraint_axis", &constraintAxis);
        readInt("constraint_range_of_motion", &constraintRangeOfMotion);
        readInt("max_motor_force", &maxMotorForce);
        readInt("max_motor_speed", &maxMotorSpeed);
        readFloat("cable_radius", &cableRadius);
        readInt("cable_length", &cableLength);
        readInt("seat_pose", &seatPose);
        readInt("seat_health_per_sec", &seatHealthPerSec);
        readInt("buoy_radius", &buoyRadius);
        readInt("buoy_factor", &buoyFactor);
        readInt("buoy_force", &buoyForce);
        readInt("force_emitter_max_force", &forceEmitterMaxForce);
        readInt("force_emitter_max_vector", &forceEmitterMaxVector);
        readInt("force_emitter_default_pitch", &forceEmitterDefaultPitch);
        readInt("force_emitter_blade_height", &forceEmitterBladeHeight);
        readInt("force_emitter_rotation_speed", &forceEmitterRotationSpeed);
        readInt("force_emitter_blade_physics_length", &forceEmitterBladePhysicsLength);
        readInt("engine_max_force", &engineMaxForce);
        readInt("trans_conn_type", &transConnType);
        readFloat("wheel_radius", &wheelRadius);
        readInt("button_type", &buttonType);
        readInt("light_intensity", &lightIntensity);
        readInt("light_range", &lightRange);
        readString("light_ies_map", &lightIesMap);
        readFloat("light_fov", &lightFov);
        readInt("light_type", &lightType);
        readInt("door_lower_limit", &doorLowerLimit);
        readInt("door_upper_limit", &doorUpperLimit);
        readBool("door_flipped", &doorFlipped);
        readInt("custom_door_type", &customDoorType);
        readInt("door_side_dist", &doorSideDist);
        readInt("door_up_dist", &doorUpDist);
        readFloat("dynamic_min_rotation", &dynamicMinRotation);
        readFloat("dynamic_max_rotation", &dynamicMaxRotation);
        readInt("logic_gate_type", &logicGateType);
        readInt("indicator_type", &indicatorType);
        readInt("connector_type", &connectorType);
        readInt("magnet_force", &magnetForce);
        readInt("gyro_type", &gyroType);
        readInt("rudder_surface_area", &rudderSurfaceArea);
        readFloat("pump_pressure", &pumpPressure);
        readInt("water_component_type", &waterComponentType);
        readFloat("particle_speed", &particleSpeed);
        readInt("inventory_type", &inventoryType);
        readInt("inventory_default_outfit", &inventoryDefaultOutfit);
        readInt("electric_type", &electricType);
        readInt("electric_charge_capacity", &electricChargeCapacity);
        readFloat("electric_magnitude", &electricMagnitude);
        // Description
        longDescription = definition->first_node("tooltip_properties")->first_attribute("description")->value();
        shortDescription = definition->first_node("tooltip_properties")->first_attribute("short_description")->value();
        // Surfaces
        rapidxml::xml_node<> *xmlSurfaces = definition->first_node("surfaces");
        for (rapidxml::xml_node<> *child = xmlSurfaces->first_node(); child; child = child->next_sibling()) {
            Surface s;
            s.rotation = atoi(child->first_attribute("rotation")->value());
            s.orientation = atoi(child->first_attribute("orientation")->value());
            s.shape = atoi(child->first_attribute("shape")->value());
            s.transType = atoi(child->first_attribute("trans_type")->value());
            rapidxml::xml_node<> *pos = child->first_node();
            s.position.x = atof(pos->first_attribute("x")->value());
            s.position.y = atof(pos->first_attribute("y")->value());
            s.position.z = atof(pos->first_attribute("z")->value());
            surfaces.emplace_back(s);
        }
        // BuoySurfaces
        rapidxml::xml_node<> *xmlBuoySurfaces = definition->first_node("buoyancy_surfaces");
        for (rapidxml::xml_node<> *child = xmlBuoySurfaces->first_node(); child; child = child->next_sibling()) {
            Surface s;
            s.rotation = atoi(child->first_attribute("rotation")->value());
            s.orientation = atoi(child->first_attribute("orientation")->value());
            s.shape = atoi(child->first_attribute("shape")->value());
            s.transType = atoi(child->first_attribute("trans_type")->value());
            rapidxml::xml_node<> *pos = child->first_node();
            s.position.x = atof(pos->first_attribute("x")->value());
            s.position.y = atof(pos->first_attribute("y")->value());
            s.position.z = atof(pos->first_attribute("z")->value());
            buoyancySurfaces.emplace_back(s);
        }
        // Voxels
        int tmp = 0;
        rapidxml::xml_node<> *xmlvoxels = definition->first_node("voxels");
        for (rapidxml::xml_node<> *child = xmlvoxels->first_node(); child; child = child->next_sibling()) {
            Voxel v;
            v.buoyPipes = child->first_attribute("buoy_pipes") ? atoi(child->first_attribute("buoy_pipes")->value()) : 0;
            v.physicsShape = child->first_attribute("physics_shape") ? atoi(child->first_attribute("physics_shape")->value()) : 0;
            v.flags = child->first_attribute("flags") ? atoi(child->first_attribute("flags")->value()) : 0;
            rapidxml::xml_node<> *pos = child->first_node("position");
            v.position.x = atof(pos->first_attribute("x")->value());
            v.position.y = atof(pos->first_attribute("y")->value());
            v.position.z = atof(pos->first_attribute("z")->value());
            rapidxml::xml_node<> *phr = child->first_node("physics_shape_rotation");
            if (phr) {
                v.physicsShapeRotation = glm::mat3(atof(phr->first_attribute("00")->value()),
                                                   atof(phr->first_attribute("01")->value()),
                                                   atof(phr->first_attribute("02")->value()),
                                                   atof(phr->first_attribute("10")->value()),
                                                   atof(phr->first_attribute("11")->value()),
                                                   atof(phr->first_attribute("12")->value()),
                                                   atof(phr->first_attribute("20")->value()),
                                                   atof(phr->first_attribute("21")->value()),
                                                   atof(phr->first_attribute("22")->value()));
            }
            voxels.emplace_back(v);
            ++tmp;
        }
        // Logic Nodes
        rapidxml::xml_node<> *logicnodes = definition->first_node("logic_nodes");
        for (rapidxml::xml_node<> *child = logicnodes->first_node(); child; child = child->next_sibling()) {
            LogicNode ln;
            ln.label = child->first_attribute("label") ? child->first_attribute("label")->value() : "";
            ln.description = child->first_attribute("description") ? child->first_attribute("description")->value()
                                                                   : "";
            ln.mode = child->first_attribute("mode") ? atoi(child->first_attribute("mode")->value()) : 0;
            ln.type = child->first_attribute("type") ? atoi(child->first_attribute("type")->value()) : 0;
            ln.flags = child->first_attribute("flags") ? atoi(child->first_attribute("flags")->value()) : 0;
            rapidxml::xml_node<> *pos = child->first_node("position");
            ln.position.x = atof(pos->first_attribute("x")->value());
            ln.position.y = atof(pos->first_attribute("y")->value());
            ln.position.z = atof(pos->first_attribute("z")->value());
            logicNodes.emplace_back(ln);
        }
        // Mesh
        if (!meshDataName.empty()) {
            if (meshDataName == "meshes/component_robotic_pivot_b_no_trans.mesh")
                meshDataName = "meshes/assets_meshes_component_robotic_pivot_b_no_trans.mesh";
            if (std::filesystem::exists(gamepath + this->meshDataName)) {
                MeshDecoder::decodeMesh(
                        gamepath + this->meshDataName, &vertices_m,
                        &indices_m);
            }
        }
        if (!mesh0Name.empty()) {
            if (mesh0Name == "meshes/component_robotic_pivot_b_no_trans.mesh")
                mesh0Name = "meshes/assets_meshes_component_robotic_pivot_b_no_trans.mesh";
            if (std::filesystem::exists(gamepath + this->mesh0Name)) {
                MeshDecoder::decodeMesh(
                        gamepath + this->mesh0Name, &vertices_m0,
                        &indices_m0);
            }
        }
        if (!mesh1Name.empty()) {
            if (mesh1Name == "meshes/component_robotic_pivot_b_no_trans.mesh")
                mesh1Name = "meshes/assets_meshes_component_robotic_pivot_b_no_trans.mesh";
            if (std::filesystem::exists(gamepath + this->mesh1Name)) {
                MeshDecoder::decodeMesh(
                        gamepath + this->mesh1Name, &vertices_m1,
                        &indices_m1);
            }
        }
    }

    // This Function has to run on the Main thread, because of OpenGL
    void loadMeshes(){
        mesh.load(vertices_m, indices_m);
        mesh0.load(vertices_m0, indices_m0);
        mesh1.load(vertices_m1, indices_m1);
    }

    std::vector<Vertex> vertices_m;
    std::vector<unsigned int> indices_m;
    std::vector<Vertex> vertices_m0;
    std::vector<unsigned int> indices_m0;
    std::vector<Vertex> vertices_m1;
    std::vector<unsigned int> indices_m1;

    std::string name;
    std::string longDescription;
    std::string shortDescription;
    // MeshNames
    std::string meshDataName;
    std::string mesh0Name;
    std::string mesh1Name;
    std::string meshEditorOnlyName;
    // Types
    int category;
    int type;
    // Lists
    std::vector<LogicNode> logicNodes;
    std::vector<Voxel> voxels;
    std::vector<Surface> surfaces;
    std::vector<Surface> buoyancySurfaces;
    // Additional Properties
    int mass;
    int value;
    int flags;
    std::string tags;
    int physCollisionDampen;
    std::string audioFilenameStart;
    std::string audioFilenameLoop;
    std::string audioFilenameEnd;
    std::string audioFilenameStartB;
    std::string audioFilenameLoopB;
    std::string audioFilenameEndB;
    float audioGain;
    int blockType;
    std::string childName;
    std::string extenderName;
    int constraintType;
    int constraintAxis;
    int constraintRangeOfMotion;
    int maxMotorForce;
    int maxMotorSpeed;
    float cableRadius;
    int cableLength;
    int seatPose;
    int seatHealthPerSec;
    int buoyRadius;
    int buoyFactor;
    int buoyForce;
    int forceEmitterMaxForce;
    int forceEmitterMaxVector;
    int forceEmitterDefaultPitch;
    int forceEmitterBladeHeight;
    int forceEmitterRotationSpeed;
    int forceEmitterBladePhysicsLength;
    int engineMaxForce;
    int transConnType;
    float wheelRadius;
    int buttonType;
    int lightIntensity;
    int lightRange;
    std::string lightIesMap;
    float lightFov;
    int lightType;
    int doorLowerLimit;
    int doorUpperLimit;
    bool doorFlipped;
    int customDoorType;
    int doorSideDist;
    int doorUpDist;
    float dynamicMinRotation;
    float dynamicMaxRotation;
    int logicGateType;
    int indicatorType;
    int connectorType;
    int magnetForce;
    int gyroType;
    int rudderSurfaceArea;
    float pumpPressure;
    int waterComponentType;
    int torqueComponentType;
    float particleSpeed;
    int inventoryType;
    int inventoryDefaultOutfit;
    int electricType;
    int electricChargeCapacity;
    float electricMagnitude;
};

#endif //STROMWORKSCOOPEDITOR_COMPONENT_H
