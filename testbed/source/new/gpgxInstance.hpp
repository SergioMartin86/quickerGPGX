#pragma once

#include "../gpgxInstanceBase.hpp"
#include <string>
#include <vector>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>

extern "C"
{ 
//  int state_load(unsigned char *state);
//  int state_save(unsigned char *state);
 void initialize();
 void initializeVideoOutput();
 void finalizeVideoOutput();
 void loadROM(const char* filePath);
 void renderFrame();
 void advanceFrame(const uint16_t controller1, const uint16_t controller2);
 size_t saveState(uint8_t* buffer);
 void loadState(const uint8_t* buffer);
 int state_save(unsigned char *state);
 void state_load(const uint8_t* buffer);
 uint8_t* getWorkRamPtr();
 void printMemoryMapChecksum();
}

namespace gpgx
{

class EmuInstance : public EmuInstanceBase
{
 public:

 uint8_t* _baseMem;
 uint8_t* _apuMem;

 EmuInstance() : EmuInstanceBase()
 {
 }

  virtual void initialize() override
  {
    ::initialize();
  }

  virtual bool loadROMImpl(const std::string &romFilePath) override
  {
    ::loadROM(romFilePath.c_str());

    #define DUMMY_SIZE 2*1024*1024
    void* buf = malloc(DUMMY_SIZE);
    _partialSize = ::state_save((uint8_t*)buf);
    free(buf);

    return true;
  }

  void initializeVideoOutput() override
  {
    ::initializeVideoOutput();
  }

  void finalizeVideoOutput() override
  {
    ::finalizeVideoOutput();
  }

  void enableRendering() override
  {
  }

  void disableRendering() override
  {
  }

  int _partialSize = 0;

  void serializeState(jaffarCommon::serializer::Base& s) const override
  {
    ::state_save(s.getOutputDataBuffer());
    ::saveState(&(s.getOutputDataBuffer())[_partialSize]);
  }

  void deserializeState(jaffarCommon::deserializer::Base& d) override
  {
    ::state_load((unsigned char*)d.getInputDataBuffer());
    ::loadState(&((unsigned char*)d.getInputDataBuffer())[_partialSize]);
  }

  size_t getStateSizeImpl() const override
  {
    auto size = ::saveState(nullptr);

    return _partialSize + size;
  }

  void updateRenderer() override
  {
    ::renderFrame();
  }

  inline size_t getDifferentialStateSizeImpl() const override { return getStateSizeImpl(); }

  void enableLiteStateBlockImpl(const std::string& block)
  {
    // Nothing to do here
  }

  void disableLiteStateBlockImpl(const std::string& block)
  {
    // Nothing to do here
  }

  void doSoftReset() override
  {
  }
  
  void doHardReset() override
  {
  }

  std::string getCoreName() const override { return "GPGX Base"; }


  virtual void advanceStateImpl(const Controller::port_t controller1, const Controller::port_t controller2)
  {
     ::advanceFrame(controller1, controller2);
  }

  inline uint8_t* getWorkRamPointer() const override
  {
    return getWorkRamPtr();
  }

};

} // namespace gpgx