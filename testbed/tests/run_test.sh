#!/bin/bash

# Stop if anything fails
set -e

# Getting executable paths
baseExecutable=${1}
newExecutable=${2}

# Getting script name
script=${3}

# Getting additional arguments
testerArgs=${@:4}

# Getting current folder (game name)
folder=`basename $PWD`

# Getting pid (for uniqueness)
pid=$$

# Hash files
baseHashFile="/tmp/baseGPGX.${folder}.${script}.${pid}.hash"
newHashFile="/tmp/newGPGX.${folder}.${script}.${pid}.hash"

# Removing them if already present
rm -f ${baseHashFile}
rm -f ${newHashFile}.simple
rm -f ${newHashFile}.rerecord

set -x
# Running script on base GPGX
${baseExecutable} ${script} --hashOutputFile ${baseHashFile}.simple ${testerArgs} --cycleType Simple

# Running script on new GPGX (Simple)
${newExecutable} ${script} --hashOutputFile ${newHashFile}.simple ${testerArgs} --cycleType Simple
set +x

# Running script on new GPGX (Rerecord)
${newExecutable} ${script} --hashOutputFile ${newHashFile}.rerecord ${testerArgs} --cycleType Rerecord
set +x

# Comparing hashes
baseHash=`cat ${baseHashFile}.simple`
newHashSimple=`cat ${newHashFile}.simple`
newHashRerecord=`cat ${newHashFile}.rerecord`

# Removing temporary files
rm -f ${baseHashFile}.simple ${newHashFile}.simple ${newHashFile}.rerecord

# Compare hashes
if [ "${baseHash}" = "${newHashSimple}" -a "${baseHash}" = "${newHashRerecord}" ]; then
 echo "[] Test Passed"
 exit 0
else
 echo "[] Test Failed"
 exit -1
fi
