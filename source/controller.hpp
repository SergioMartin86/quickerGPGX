#pragma once

// Base controller class
// by eien86

#include <cstdint>
#include <string>
#include <sstream>
#include <gpgx/core/core/input_hw/input.h>

class Controller
{
public:

  enum controller_t { none, gamepad3b };

  typedef uint16_t port_t;

  struct input_t
  {
    bool power = false;
    bool reset = false;
    port_t port1 = 0;
    port_t port2 = 0;
  };

  inline bool parseInputString(const std::string& input)
  {
    // Parse valid flag
    bool isValid = true;

    // Converting input into a stream for parsing
    std::istringstream ss(input);

    // Start separator
    if (ss.get() != '|') isValid = false;

    // Parsing console inputs
    isValid &= parseConsoleInputs(_input.reset, _input.power, ss);
    
    // Parsing controller 1 inputs
    isValid &= parseControllerInputs(_controller1Type, _input.port1, ss);

    // Parsing controller 2 inputs
    isValid &= parseControllerInputs(_controller2Type, _input.port2, ss);

    // End separator
    if (ss.get() != '|') isValid = false;

    // If its not the end of the stream, then extra values remain and its invalid
    ss.get();
    if (ss.eof() == false) isValid = false;
    
    // Returning valid flag 
    return isValid;
  };

  inline void setController1Type(const controller_t type) { _controller1Type = type; }
  inline void setController2Type(const controller_t type) { _controller2Type = type; }

  inline bool getPowerButtonState() { return _input.power; }
  inline bool getResetButtonState() { return _input.reset; }
  inline port_t getController1Code() { return _input.port1; }
  inline port_t getController2Code() { return _input.port2; }

  private:

  static bool parseGamePad6BInput(uint16_t& code, std::istringstream& ss)
  {
    // Currently read character
    char c;

    // Cleaning code
    code = 0;

    c = ss.get();
    if (c != '.' && c != 'U') return false;
    if (c == 'U') code |= INPUT_UP;

    c = ss.get();
    if (c != '.' && c != 'D') return false;
    if (c == 'D') code |= INPUT_DOWN;

    c = ss.get();
    if (c != '.' && c != 'L') return false;
    if (c == 'L') code |= INPUT_LEFT;

    c = ss.get();
    if (c != '.' && c != 'R') return false;
    if (c == 'R') code |= INPUT_RIGHT;

    c = ss.get();
    if (c != '.' && c != 'M') return false;
    if (c == 's') code |= INPUT_MODE;

    c = ss.get();
    if (c != '.' && c != 'S') return false;
    if (c == 'S') code |= INPUT_START;

    c = ss.get();
    if (c != '.' && c != 'A') return false;
    if (c == 'Y') code |= INPUT_A;

    c = ss.get();
    if (c != '.' && c != 'B') return false;
    if (c == 'B') code |= INPUT_B;

    c = ss.get();
    if (c != '.' && c != 'C') return false;
    if (c == 'X') code |= INPUT_C;

    c = ss.get();
    if (c != '.' && c != 'X') return false;
    if (c == 'A') code |= INPUT_X;

    c = ss.get();
    if (c != '.' && c != 'Y') return false;
    if (c == 'l') code |= INPUT_Y;

    c = ss.get();
    if (c != '.' && c != 'Z') return false;
    if (c == 'r') code |= INPUT_Z;

    return true;
  }

  static bool parseGamePad3BInput(uint16_t& code, std::istringstream& ss)
  {
    // Currently read character
    char c;

    // Cleaning code
    code = 0;

    c = ss.get();
    if (c != '.' && c != 'U') return false;
    if (c == 'U') code |= INPUT_UP;

    c = ss.get();
    if (c != '.' && c != 'D') return false;
    if (c == 'D') code |= INPUT_DOWN;

    c = ss.get();
    if (c != '.' && c != 'L') return false;
    if (c == 'L') code |= INPUT_LEFT;

    c = ss.get();
    if (c != '.' && c != 'R') return false;
    if (c == 'R') code |= INPUT_RIGHT;

    c = ss.get();
    if (c != '.' && c != 'A') return false;
    if (c == 'Y') code |= INPUT_A;

    c = ss.get();
    if (c != '.' && c != 'B') return false;
    if (c == 'B') code |= INPUT_B;

    c = ss.get();
    if (c != '.' && c != 'C') return false;
    if (c == 'X') code |= INPUT_C;

    c = ss.get();
    if (c != '.' && c != 'S') return false;
    if (c == 'S') code |= INPUT_START;


    return true;
  }

  static bool parseControllerInputs(const controller_t type, port_t& port, std::istringstream& ss)
  {
    // Parse valid flag
    bool isValid = true; 
 
    // If no controller assigned then, its port is all zeroes.
    if (type == controller_t::none) { port = 0; return true; }

    // Controller separator
    if (ss.get() != '|') isValid = false;

    // If normal gamepad, parse its code now
    if (type == controller_t::gamepad3b) 
    {
      // Storage for gamepad's code
      uint16_t code = 0;

      // Parsing gamepad code
      isValid &= parseGamePad3BInput(code, ss);

      // Pushing input code into the port
      port = code;
    }

    // Return valid flag
    return isValid;
  }

  static bool parseConsoleInputs(bool& reset, bool& power, std::istringstream& ss)
  {
    // Parse valid flag
    bool isValid = true; 

    // Currently read character
    char c;

    // Power trigger
    c = ss.get();
    if (c != '.' && c != 'P') isValid = false;
    if (c == 'P') power = true;
    if (c == '.') power = false;

    // Reset trigger
    c = ss.get();
    if (c != '.' && c != 'r') isValid = false;
    if (c == 'r') reset = true;
    if (c == '.') reset = false;

    // Return valid flag
    return isValid;
  }

  input_t _input;
  controller_t _controller1Type;
  controller_t _controller2Type;
};
