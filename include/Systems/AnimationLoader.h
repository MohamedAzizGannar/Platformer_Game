#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <Systems/InputSystem.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
class AnimationLoader {
public:
  static EntityAnimConfig loadFromFile(const std::string &filePath) {
    EntityAnimConfig config;
    try {
      YAML::Node root = YAML::LoadFile(filePath);
      if (root["texture"]) {
        config.texturePath = root["texture"].as<std::string>();
      } else {
        std::cerr << "No texture found in " << filePath << std::endl;
      }

      if (root["scale"]) {
        config.scale = root["scale"].as<float>();
      } else {
        std::cerr << "No scale found in " << filePath << std::endl;
      }
      if (root["animations"]) {
        YAML::Node animations = root["animations"];
        for (auto it = animations.begin(); it != animations.end(); ++it) {
          std::string animName = it->first.as<std::string>();
          YAML::Node animNode = it->second; 

          AnimationData animData;

          animData.frameCount = animNode["frameCount"].as<int>();
          animData.frameTime = animNode["frameTime"].as<float>();
          animData.startX = animNode["startX"].as<int>();
          animData.startY = animNode["startY"].as<int>();
          animData.frameWidth = animNode["frameWidth"].as<int>();
          animData.frameHeight = animNode["frameHeight"].as<int>();
          animData.isLockable = animNode["isLockable"].as<bool>();

          config.animations[animName] = animData;
        }
      }
    } catch (YAML::Exception &e) {
      std::cerr << "YAML LOADING ERROR : on file :  " << filePath
                << " Msg: " << e.what() << std::endl;
    } catch (std::exception &e) {
      std::cerr << "STD LOADING ERROR : on file :  " << filePath
                << "Msg: " << e.what() << std::endl;
    }
    return config;
  }
  std::unordered_map<std::string, EntityAnimConfig>
  static loadALlAnimsFromFolder(const std::string &directoryPath) {
    std::unordered_map<std::string, EntityAnimConfig> allConfigs;
    try {
      if (!std::filesystem::exists(directoryPath)) {
        std::cerr << "Directory : " << directoryPath << " Doesnt exist "
                  << std::endl;
        return allConfigs;
      }
      for (const auto &entry :
           std::filesystem::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".yaml" ||
            entry.path().extension() == ".yml") {
          std::string entryName = entry.path().stem().string();
          allConfigs[entryName] = loadFromFile(entry.path().string());
        }
      }

    } catch (std::filesystem::filesystem_error &e) {
      std::cerr << "File System Error : " << e.what() << std::endl;
    }
    return allConfigs;
  }
};
