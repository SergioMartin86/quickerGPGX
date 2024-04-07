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

set -x
# Running script on base GPGX
${baseExecutable} ${script} --hashOutputFile ${baseHashFile}.simple ${testerArgs}

# Running script on new GPGX (Simple)
${newExecutable} ${script} --hashOutputFile ${newHashFile}.simple ${testerArgs}
set +x

# Comparing hashes
baseHash=`cat ${baseHashFile}.simple`
newHashSimple=`cat ${newHashFile}.simple`

# Removing temporary files
rm -f ${baseHashFile}.simple ${newHashFile}.simple 

# Compare hashes
if [ "${baseHash}" = "${newHashSimple}" ]; then
 echo "[] Test Passed"
 exit 0
else
 echo "[] Test Failed"
 exit -1
fi
