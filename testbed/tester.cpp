#include "argparse/argparse.hpp"
#include <jaffarCommon/json.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/serializers/differential.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>
#include <jaffarCommon/deserializers/differential.hpp>
#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/string.hpp>
#include <jaffarCommon/logger.hpp>
#include <jaffarCommon/file.hpp>
#include "gpgxInstance.hpp"
#include <chrono>
#include <sstream>
#include <vector>
#include <string>


int main(int argc, char *argv[])
{
  // Parsing command line arguments
  argparse::ArgumentParser program("tester", "1.0");

  program.add_argument("scriptFile")
    .help("Path to the test script file to run.")
    .required();

  program.add_argument("sequenceFile")
    .help("Path to the input sequence file (.sol) to reproduce.")
    .required();

  program.add_argument("--hashOutputFile")
    .help("Path to write the hash output to.")
    .default_value(std::string(""));

  // Try to parse arguments
  try { program.parse_args(argc, argv); } catch (const std::runtime_error &err) { JAFFAR_THROW_LOGIC("%s\n%s", err.what(), program.help().str().c_str()); }

  // Getting test script file path
  const auto scriptFilePath = program.get<std::string>("scriptFile");

  // Getting path where to save the hash output (if any)
  const auto hashOutputFile = program.get<std::string>("--hashOutputFile");

  // Loading script file
  std::string configJsRaw;
  if (jaffarCommon::file::loadStringFromFile(configJsRaw, scriptFilePath) == false) JAFFAR_THROW_LOGIC("Could not find/read script file: %s\n", scriptFilePath.c_str());

  // Parsing script
  const auto configJs = nlohmann::json::parse(configJsRaw);

  // Getting rom file path
  const auto romFilePath = jaffarCommon::json::getString(configJs, "Rom File");

  // Getting sequence file path
  std::string sequenceFilePath = program.get<std::string>("sequenceFile");

  // Getting expected ROM SHA1 hash
  const auto expectedROMSHA1 = jaffarCommon::json::getString(configJs, "Expected ROM SHA1");

  // Parsing disabled blocks in lite state serialization
  const auto stateDisabledBlocks = jaffarCommon::json::getArray<std::string>(configJs, "Disable State Blocks");
  std::string stateDisabledBlocksOutput;
  for (const auto& entry : stateDisabledBlocks) stateDisabledBlocksOutput += entry + std::string(" ");
  
  // Getting System Type
  const auto systemType = jaffarCommon::json::getString(configJs, "System Type");

  // Getting Controller 1 type
  const auto controller1Type = jaffarCommon::json::getString(configJs, "Controller 1 Type");

  // Getting Controller 2 type
  const auto controller2Type = jaffarCommon::json::getString(configJs, "Controller 2 Type");

  // Creating emulator instance
  auto e = gpgx::EmuInstance();

  // Initializing emulator instance
  e.initialize();

  // Setting controller types
  e.setController1Type(controller1Type);
  e.setController2Type(controller2Type);

  // Setting system type
  e.setSystemType(systemType);

  // Loading ROM File
  std::string romFileData;
  if (jaffarCommon::file::loadStringFromFile(romFileData, romFilePath) == false) JAFFAR_THROW_LOGIC("Could not rom file: %s\n", romFilePath.c_str());
  e.loadROM(romFilePath);

  // Calculating ROM SHA1
  auto romSHA1 = jaffarCommon::hash::getSHA1String(romFileData);

  // Disabling requested blocks from state serialization
  for (const auto& block : stateDisabledBlocks) e.disableStateBlock(block);

  // Disable rendering
  e.disableRendering();

  // Checking with the expected SHA1 hash
  if (romSHA1 != expectedROMSHA1) JAFFAR_THROW_LOGIC("Wrong ROM SHA1. Found: '%s', Expected: '%s'\n", romSHA1.c_str(), expectedROMSHA1.c_str());

  // Loading sequence file
  std::string sequenceRaw;
  if (jaffarCommon::file::loadStringFromFile(sequenceRaw, sequenceFilePath) == false) JAFFAR_THROW_LOGIC("[ERROR] Could not find or read from input sequence file: %s\n", sequenceFilePath.c_str());

  // Building sequence information
  const auto sequence = jaffarCommon::string::split(sequenceRaw, ' ');

  // Getting sequence lenght
  const auto sequenceLength = sequence.size();

  // Getting emulation core name
  std::string emulationCoreName = e.getCoreName();

  // Printing test information
  printf("[] -----------------------------------------\n");
  printf("[] Running Script:                         '%s'\n", scriptFilePath.c_str());
  printf("[] Emulation Core:                         '%s'\n", emulationCoreName.c_str());
  printf("[] ROM File:                               '%s'\n", romFilePath.c_str());
  printf("[] Controller Types:                       '%s' / '%s'\n", controller1Type.c_str(), controller2Type.c_str());
  printf("[] ROM Hash:                               'SHA1: %s'\n", romSHA1.c_str());
  printf("[] Sequence File:                          '%s'\n", sequenceFilePath.c_str());
  printf("[] Sequence Length:                        %lu\n", sequenceLength);
  printf("[] ********** Running Test **********\n");

  fflush(stdout);

  // Actually running the sequence
  auto t0 = std::chrono::high_resolution_clock::now();
  for (const std::string &input : sequence) e.advanceState(input);
  auto tf = std::chrono::high_resolution_clock::now();

  // Calculating running time
  auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(tf - t0).count();
  double elapsedTimeSeconds = (double)dt * 1.0e-9;

  // Calculating final state hash
  auto result = e.getStateHash();

  // Creating hash string
  char hashStringBuffer[256];
  sprintf(hashStringBuffer, "0x%lX%lX", result.first, result.second);

  // Printing time information
  printf("[] Elapsed time:                           %3.3fs\n", (double)dt * 1.0e-9);
  printf("[] Performance:                            %.3f inputs / s\n", (double)sequenceLength / elapsedTimeSeconds);
  printf("[] Final State Hash:                       %s\n", hashStringBuffer);
  
  // If saving hash, do it now
  if (hashOutputFile != "") jaffarCommon::file::saveStringToFile(std::string(hashStringBuffer), hashOutputFile.c_str());

  // If reached this point, everything ran ok
  return 0;
}
