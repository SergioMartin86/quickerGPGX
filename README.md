QuickerGPGX
--------------

[![Build & Tests](https://github.com/SergioMartin86/quickerGPGX/actions/workflows/make.yml/badge.svg)](https://github.com/SergioMartin86/quickerGPGX/actions/workflows/make.yml)


QuickerGPGX is an attempt at improving the performance and reduce the save state memory footprint of the [Genesis Plus GX (gpgx)](https://github.com/ekeeke/Genesis-Plus-GX) emulator. The main aim is to improve the performance of headless re-recording for TASing and botting (See: [JaffarPlus](https://github.com/SergioMartin86/jaffarPlus)) purposes. It will be also be used as testbed for proposing and update to the GPGX core in [BizHawk](https://github.com/TASEmulators/BizHawk).

Improvements
-------------

- Remove all warnings
- Decentralize include structure
- Made the core thread safe
- Revamp the state saving/loading procedures to be 100% deterministic
- Updating the code to modern C pratices
- Adding automated tests and coverage analysis
  
Credits
---------

- gpgx is developed by ekeeke and others under open source free licenses. For more information see: https://github.com/ekeeke/Genesis-Plus-GX

All base code for this project was found under open source licenses, which I preserved in their corresponding files/folders. Any non-credited work is unintentional and shall be immediately rectfied.

