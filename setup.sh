#!/bin/bash
#
# This script is used to setup the environment for the FLiT CUDA demo.
# It fetches and builds the MatX package.
#
# Note: MatX requires cmake 3.16 or higher.
#       Ubuntu's default version is 3.12, which is too old.

set -e # Exit on failure.
set -u # Exit if using an undefined variable.

export MAKEFLAGS=-j$(nproc)

PACKAGES_DIR="packages"
SUCCESS_LOG="${PACKAGES_DIR}/success.log"
FORCE=false

PACKAGE="MatX"
PACKAGE_URL="https://github.com/NVIDIA/MatX/archive/refs/tags/v0.2.2.tar.gz"
PACKAGE_FILENAME="matx-v0.2.2.tar.gz"
TAR_DIRNAME="MatX-0.2.2"

# Parse command-line arguments.
while [ $# -gt 0 ]; do
    case $1 in
    -h | --help)
        echo "
Usage:
    $0 [-h|--help]
    $0 [-f|--force]

Description:
    Performs initial setup of the dependent packages used in the demo
    of FLiT running on CUDA. It will download ${PACKAGE} and configure it.
"
        exit 0
        ;;

    -f | --force)
        FORCE=true
        echo "Forcing setup."
        echo
        rm -rf ${SUCCESS_LOG}
        ;;
    esac
done

if [ "${FORCE}" != "true" ] && [ -f "${SUCCESS_LOG}" ]; then
    echo "Setup already completed. Skipping."
    exit 0
fi

# Begin setup.

mkdir -p ${PACKAGES_DIR}
pushd ${PACKAGES_DIR}

# Fetch and extract the MatX package.
rm -rf ${PACKAGE}
wget -c ${PACKAGE_URL} -O ${PACKAGE_FILENAME}
tar xf ${PACKAGE_FILENAME}
mv ${TAR_DIRNAME} ${PACKAGE}

popd

echo
echo "Setup completed on $(date)." | tee ${SUCCESS_LOG}
echo
