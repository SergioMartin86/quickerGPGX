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
rm -f ${newHashFile}

set -x
# Running script on base GPGX
${baseExecutable} ${script} --hashOutputFile ${baseHashFile} ${testerArgs}

# Running script on new GPGX (Simple)
${newExecutable} ${script} --hashOutputFile ${newHashFile} ${testerArgs}
set +x

# Comparing hashes
baseHash=`cat ${baseHashFile}`
newHash=`cat ${newHashFile}`

# Removing temporary files
rm -f ${baseHashFile} ${newHashFile} 

# Compare hashes
if [ "${baseHash}" = "${newHash}" ]; then
 echo "[] Test Passed"
 exit 0
else
 echo "[] Test Failed"
 exit -1
fi
