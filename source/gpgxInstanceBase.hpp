#pragma once

#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>

#include "controller.hpp"

namespace gpgx
{

class EmuInstanceBase
{
  public:

  EmuInstanceBase() = default;
  virtual ~EmuInstanceBase() = default;

  inline void advanceState(const std::string &move)
  {
    bool isInputValid = _controller.parseInputString(move);
    if (isInputValid == false) JAFFAR_THROW_LOGIC("Move provided cannot be parsed: '%s'\n", move.c_str());

    // Parsing power
    if (_controller.getPowerButtonState() == true) JAFFAR_THROW_RUNTIME("Power button pressed, but not supported: '%s'\n", move.c_str());

    // Parsing reset
    if (_controller.getResetButtonState() == true) doSoftReset();

    // Parsing Controllers
    const auto controller1 = _controller.getController1Code();
    const auto controller2 = _controller.getController2Code();

    advanceStateImpl(controller1, controller2);
  }

  inline void setSystemType(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "Sega Genesis") { _controller.setSystemType(Controller::system_t::genesis); isTypeRecognized = true; }
    if (type == "Sega Game Gear")  { _controller.setSystemType(Controller::system_t::gamegear);  isTypeRecognized = true; }
    if (type == "Sega Master System") { _controller.setSystemType(Controller::system_t::sms); isTypeRecognized = true; }
    if (type == "Sega CD") { _controller.setSystemType(Controller::system_t::segacd); isTypeRecognized = true; }
    if (type == "Sega SG-1000") { _controller.setSystemType(Controller::system_t::sg1000); isTypeRecognized = true; }

    if (isTypeRecognized == false) JAFFAR_THROW_LOGIC("Input type not recognized: '%s'\n", type.c_str());
  }

  inline void setController1Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController1Type(Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "GameGear2B") { _controller.setController1Type(Controller::controller_t::gamegear2b);  isTypeRecognized = true; }
    if (type == "Gamepad2B")  { _controller.setController1Type(Controller::controller_t::gamepad2b); isTypeRecognized = true; }
    if (type == "Gamepad3B")  { _controller.setController1Type(Controller::controller_t::gamepad3b); isTypeRecognized = true; }
    if (type == "Gamepad6B")  { _controller.setController1Type(Controller::controller_t::gamepad6b); isTypeRecognized = true; }

    if (isTypeRecognized == false) JAFFAR_THROW_LOGIC("Input type not recognized: '%s'\n", type.c_str());
  }

  inline void setController2Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController2Type(Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "GameGear2B") { _controller.setController2Type(Controller::controller_t::gamegear2b);  isTypeRecognized = true; }
    if (type == "Gamepad2B")  { _controller.setController2Type(Controller::controller_t::gamepad2b); isTypeRecognized = true; }
    if (type == "Gamepad3B")  { _controller.setController2Type(Controller::controller_t::gamepad3b); isTypeRecognized = true; }
    if (type == "Gamepad6B")  { _controller.setController2Type(Controller::controller_t::gamepad6b); isTypeRecognized = true; }
    
    if (isTypeRecognized == false) JAFFAR_THROW_LOGIC("Input type not recognized: '%s'\n", type.c_str());
  }

  inline jaffarCommon::hash::hash_t getStateHash() const
  {
    MetroHash128 hash;
    
    auto workRam = getWorkRamPointer();

    //  Getting RAM pointer and size
    hash.Update(workRam, 0x10000);

    jaffarCommon::hash::hash_t result;
    hash.Finalize(reinterpret_cast<uint8_t *>(&result));
    return result;
  }

  virtual void initialize() = 0;
  virtual void initializeVideoOutput() = 0;
  virtual void finalizeVideoOutput() = 0;
  virtual void enableRendering() = 0;
  virtual void disableRendering() = 0;

  inline void loadROM(const std::string &romFilePath)
  {
    // Actually loading rom file
    auto status = loadROMImpl(romFilePath);
    if (status == false) JAFFAR_THROW_RUNTIME("Could not process ROM file");

    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  void enableStateBlock(const std::string& block) 
  {
    enableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  void disableStateBlock(const std::string& block)
  {
     disableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  inline size_t getStateSize() const 
  {
    return _stateSize;
  }

  inline size_t getDifferentialStateSize() const
  {
    return _differentialStateSize;
  }

  // Virtual functions

  virtual void updateRenderer() = 0;
  virtual void serializeState(jaffarCommon::serializer::Base& s) const = 0;
  virtual void deserializeState(jaffarCommon::deserializer::Base& d) = 0;

  virtual void doSoftReset() = 0;
  virtual void doHardReset() = 0;
  virtual std::string getCoreName() const = 0;

  protected:

  virtual uint8_t* getWorkRamPointer() const = 0;
  virtual bool loadROMImpl(const std::string &romData) = 0;
  virtual void advanceStateImpl(const Controller::port_t controller1, const Controller::port_t controller2) = 0;

  virtual void enableStateBlockImpl(const std::string& block) {};
  virtual void disableStateBlockImpl(const std::string& block) {};

  virtual size_t getStateSizeImpl() const = 0;
  virtual size_t getDifferentialStateSizeImpl() const = 0;
  // State size
  size_t _stateSize;

  private:

  // Controller class for input parsing
  Controller _controller;

  // Differential state size
  size_t _differentialStateSize;
};

} // namespace gpgx