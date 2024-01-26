#pragma once

#include "sha1/sha1.hpp"
#include <utils.hpp>
#include "controller.hpp"


class EmuInstanceBase
{
  public:

  EmuInstanceBase() = default;
  virtual ~EmuInstanceBase() = default;

  inline void advanceState(const std::string &move)
  {
  }

  inline void setController1Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController1Type(Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "Gamepad") { _controller.setController1Type(Controller::controller_t::gamepad); isTypeRecognized = true; }

    if (isTypeRecognized == false) EXIT_WITH_ERROR("Input type not recognized: '%s'\n", type.c_str());
  }

  inline void setController2Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController2Type(Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "Gamepad") { _controller.setController2Type(Controller::controller_t::gamepad); isTypeRecognized = true; }
    
    if (isTypeRecognized == false) EXIT_WITH_ERROR("Input type not recognized: '%s'\n", type.c_str());
  }

  inline std::string getRomSHA1() const { return _romSHA1String; }

  inline hash_t getStateHash() const
  {
    MetroHash128 hash;
    
    hash_t result;
    hash.Finalize(reinterpret_cast<uint8_t *>(&result));
    return result;
  }

  inline void enableRendering()
   {
    
   };

  inline void disableRendering()
   {
    
   };

  inline void loadStateFile(const std::string &stateFilePath)
  {
    std::string stateData;
    bool status = loadStringFromFile(stateData, stateFilePath);
    if (status == false) EXIT_WITH_ERROR("Could not find/read state file: %s\n", stateFilePath.c_str());
    deserializeFullState((uint8_t *)stateData.data());
  }

  inline void saveStateFile(const std::string &stateFilePath) const
  {
    std::string stateData;
    stateData.resize(_fullStateSize);
    serializeFullState((uint8_t *)stateData.data());
    saveStringToFile(stateData, stateFilePath.c_str());
  }

  inline void loadROMFile(const std::string &romFilePath)
  {
    // Loading ROM data
    bool status = loadStringFromFile(_romData, romFilePath);
    if (status == false) EXIT_WITH_ERROR("Could not find/read ROM file: %s\n", romFilePath.c_str());

    // Calculating ROM hash value
    _romSHA1String = SHA1::GetHash((uint8_t *)_romData.data(), _romData.size());

    // Actually loading rom file
    status = loadROMFileImpl(_romData);
    if (status == false) EXIT_WITH_ERROR("Could not process ROM file: %s\n", romFilePath.c_str());

    // Detecting full state size
    _fullStateSize = getFullStateSize();

    // Detecting lite state size
    _liteStateSize = getLiteStateSize();
  }

  // Virtual functions

  virtual bool loadROMFileImpl(const std::string &romData) = 0;
  virtual void advanceStateImpl(const Controller::port_t controller1, const Controller::port_t controller2) = 0;
  virtual void serializeFullState(uint8_t *state) const = 0;
  virtual void deserializeFullState(const uint8_t *state) = 0;
  virtual void serializeLiteState(uint8_t *state) const = 0;
  virtual void deserializeLiteState(const uint8_t *state) = 0;
  virtual size_t getFullStateSize() const = 0;
  virtual size_t getLiteStateSize() const = 0;
  virtual void enableLiteStateBlock(const std::string& block) = 0;
  virtual void disableLiteStateBlock(const std::string& block) = 0;

  virtual void doSoftReset() = 0;
  virtual void doHardReset() = 0;
  virtual std::string getCoreName() const = 0;

  protected:

  // Storage for the light state size
  size_t _liteStateSize;

  // Storage for the full state size
  size_t _fullStateSize;

  // Flag to determine whether to enable/disable rendering
  bool _doRendering = true;

  private:

  // Storage for the ROM data
  std::string _romData;

  // SHA1 rom hash
  std::string _romSHA1String;

  // Controller class for input parsing
  Controller _controller;
};
