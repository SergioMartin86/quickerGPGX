Bizhawk GPGX Core
------------------

[![Build and Run Tests](https://github.com/TASEmulators/Genesis-Plus-GX/actions/workflows/make.yml/badge.svg)](https://github.com/TASEmulators/Genesis-Plus-GX/actions/workflows/make.yml)

This is a candidate replacement repository for the Bizhawk GPGX port. The goals are:

- Update the Bizhawk GPGX core with the advancements in the upstream GPGX development
- Preserve the current features added by the Bizhawk GPGX core while adding a few more (e.g., additional sound chip support)
- Create a testbed to make sure none of the changes causes desyncs with respect to the upstream GPGX code

Building and Running Testbed
-----------------------------

[Optional] Step 0: populate testbed/tests/roms with the commercial roms and BIOS files that cannot be uploaded to github

mkdir build
cd build
meson .. -DonlyOpenSource=true # (false, if you have done step 0)
ninja
meson test

Credits
---------

- The changes to the GPGX core are adapted from the current (or rather former) [Bizhawk GPGX core](https://github.com/TASEmulators/BizHawk/tree/572d989a94603f4e5145a36042402d045e29dcf8/waterbox/gpgx/core) 
- This repository core and the testbed are maintained by eien86
- The upstream GPGX core is developed by ekeeke and others under open source free licenses. For more information see: https://github.com/ekeeke/Genesis-Plus-GX

All base code for this project was found under open source licenses, which we preserved in their corresponding files/folders. Any non-credited work is unintentional and shall be immediately rectfied.

