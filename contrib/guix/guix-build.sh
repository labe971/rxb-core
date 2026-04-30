#!/usr/bin/env bash
export LC_ALL=C
set -e -o pipefail

# RXB Guix Build Script
# Copyright (c) 2025 The RXB developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Determine the maximum number of jobs to run simultaneously (overridable by
# environment)
MAX_JOBS="${MAX_JOBS:-$(nproc)}"

# Download the depends sources now as we won't have internet access in the build
# container
make -C "${PWD}/depends" -j"$MAX_JOBS" download ${V:+V=1} ${SOURCES_PATH:+SOURCES_PATH="$SOURCES_PATH"}

# Determine the reference time used for determinism (overridable by environment)
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(git log --format=%at -1)}"

# RXB specific Guix time machine configuration
time-machine() {
    guix time-machine --url=https://github.com/dongcarl/guix.git \
                      --commit=b3a7c72c8b2425f8ddb0fc6e3b1caeed40f86dee \
                      -- "$@"
}

# Deterministically build RXB for HOSTs (overriable by environment)
for host in ${HOSTS=x86_64-linux-gnu arm-linux-gnueabihf aarch64-linux-gnu riscv64-linux-gnu}; do

    # Display proper warning when the user interrupts the build
    trap 'echo "** INT received while building ${host}, you may want to clean up the relevant output and distsrc-* directories before rebuilding"' INT

    # RXB build environment configuration
    echo "=== Building RXB for ${host} ==="
    echo "Build timestamp: ${SOURCE_DATE_EPOCH}"
    echo "Max jobs: ${MAX_JOBS}"

    # Run the build script 'contrib/guix/libexec/build.sh' in the build
    # container specified by 'contrib/guix/manifest.scm'
    # shellcheck disable=SC2086
    time-machine environment --manifest="${PWD}/contrib/guix/manifest.scm" \
                             --container \
                             --pure \
                             --no-cwd \
                             --share="$PWD"=/rxb \
                             ${SOURCES_PATH:+--share="$SOURCES_PATH"} \
                             ${ADDITIONAL_GUIX_ENVIRONMENT_FLAGS} \
                             -- env HOST="$host" \
                                    MAX_JOBS="$MAX_JOBS" \
                                    SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:?unable to determine value}" \
                                    DISTNAME="rxb-0.3.0" \
                                    ${V:+V=1} \
                                    ${SOURCES_PATH:+SOURCES_PATH="$SOURCES_PATH"} \
                                  bash -c "cd /rxb && bash contrib/guix/libexec/build.sh"

    echo "=== RXB build for ${host} completed successfully ==="
    echo "Output files:"
    find "${PWD}/output" -name "rxb-*${host}*" -type f 2>/dev/null || true
    echo ""
done

# Generate SHA256SUMS after all builds are complete
if [ -d "${PWD}/output" ]; then
    echo "=== Generating SHA256 checksums for RXB ==="
    cd "${PWD}/output"
    sha256sum rxb-* > SHA256SUMS 2>/dev/null || true
    if [ -f "SHA256SUMS" ]; then
        echo "Checksums saved to ${PWD}/output/SHA256SUMS"
        echo ""
        echo "To verify:"
        echo "  cd output && sha256sum -c SHA256SUMS"
    fi
    cd ..
fi

echo "=== RXB Guix build completed ==="
