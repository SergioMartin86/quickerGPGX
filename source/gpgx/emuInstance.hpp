#pragma once

#include <emuInstanceBase.hpp>
#include <utils.hpp>
#include <string>
#include <vector>

extern "C"
{ 
 void gpgx_init();
 int audio_init(int samplerate, double framerate);
 int load_cart_rom(const uint8_t* buffer, size_t size);
 void system_init(void);
 void system_reset(void);
 void update_viewport(void);
 void gpgx_clear_sram(void);
}

class EmuInstance : public EmuInstanceBase
{
 public:

 uint8_t* _baseMem;
 uint8_t* _apuMem;

 EmuInstance() : EmuInstanceBase()
 {
  gpgx_init();
 }

  virtual bool loadROMFileImpl(const std::string &romData) override
  {
    load_cart_rom((uint8_t*)romData.data(), romData.size());

   	audio_init(44100, 0);
    system_init();
    system_reset();

    update_viewport();
    gpgx_clear_sram();

    return true;
  }

  void serializeFullState(uint8_t *state) const override
  {
    state_save(state);
  }

  void deserializeFullState(const uint8_t *state) override
  {
    state_load(state);
  }

  void serializeLiteState(uint8_t *state) const override
  {
     state_save(state);
  }

  void deserializeLiteState(const uint8_t *state) override
  {
    state_load(state);
  }

  size_t getFullStateSize() const override
  {
    return 0;
  }

  size_t getLiteStateSize() const override
  {
   return 0;
  }

  void enableLiteStateBlock(const std::string& block)
  {
    // Nothing to do here
  }

  void disableLiteStateBlock(const std::string& block)
  {
    // Nothing to do here
  }

  void doSoftReset() override
  {
    
  }
  
  void doHardReset() override
  {
    
  }

  std::string getCoreName() const override { return "gpgx"; }


  virtual void advanceStateImpl(const Controller::port_t controller1, const Controller::port_t controller2)
  {
  }

};
