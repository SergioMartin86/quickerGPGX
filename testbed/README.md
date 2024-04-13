Bizhawk GPGX Core
------------------

[![Build and Run Tests](https://github.com/TASEmulators/Genesis-Plus-GX/actions/workflows/make.yml/badge.svg)](https://github.com/TASEmulators/Genesis-Plus-GX/actions/workflows/make.yml)

This is a candidate replacement repository for the Bizhawk GPGX port. The goals are:

- Update the waterboxed Bizhawk GPGX core with the advancements in the upstream GPGX development
- Preserve the current features added by the Bizhawk GPGX core
- Add few more features (e.g., additional sound chip support, deep freeze of memory positions)
- Create a testbed to make sure none of the changes causes desyncs with respect to the upstream GPGX code

Building and Running Testbed
-----------------------------

(Optional) Populate testbed/tests/roms with the commercial roms and BIOS files that cannot be uploaded to github

```
mkdir build
cd build
meson .. -DonlyOpenSource=true # (false, if you have done step 0)
ninja
meson test
```

Credits
---------

- The changes to the GPGX core are adapted from the current (or rather former) [Bizhawk GPGX core](https://github.com/TASEmulators/BizHawk/tree/572d989a94603f4e5145a36042402d045e29dcf8/waterbox/gpgx/core) 
- This repository and its testbed are maintained by [eien86](https://github.com/SergioMartin86)
- The upstream GPGX core is developed by ekeeke and others under open source free licenses. For more information see: https://github.com/ekeeke/Genesis-Plus-GX



