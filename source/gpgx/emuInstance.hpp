#pragma once

#include <emuInstanceBase.hpp>
#include <utils.hpp>
#include <string>
#include <vector>
#include <state.h>

#define DUMMY_SIZE 1048576

extern "C"
{ 
 void gpgx_init();
 int audio_init(int samplerate, double framerate);
 int load_cart_rom(const uint8_t* buffer, size_t size);
 void system_init(void);
 void system_reset(void);
 void update_viewport(void);
 void gpgx_clear_sram(void);
 void gpgx_advance(void);
 void system_frame_gen(int);
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

        printf("\n");
     int size;
    uint8_t* ram;
    gpgx_get_memdom(0, (void**)&ram, &size);
    for (size_t i = 0; i < 100; i++) printf("%2X ", ram[i]);
  }

  void serializeLiteState(uint8_t *state) const override
  {
     serializeFullState(state);
  }

  void deserializeLiteState(const uint8_t *state) override
  {
    deserializeFullState(state);
  }

  size_t getFullStateSize() const override
  {
    auto buffer = (uint8_t*) malloc(DUMMY_SIZE);
    auto size = state_save(buffer);
    free (buffer);
    return size;
  }

  size_t getLiteStateSize() const override
  {
   return getFullStateSize();
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
     input.pad[0] = controller1;
     input.pad[1] = controller2;
     system_frame_gen(1);
  }

};
