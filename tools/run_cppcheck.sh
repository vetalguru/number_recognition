#!/bin/bash

PROJECT_DIR=$(dirname "$(realpath "$0")")/..
BUILD_DIR="build"
THIRDPARTY_DIR="submodule"

if ! command -v cppcheck &> /dev/null
then
    echo "ERROR: cppcheck is not installed"
    exit 1
fi

CPPCHECK_OPTS=(
  --enable=warning,style,performance,portability,information,unusedFunction
  --inconclusive
  --std=c++17
  --language=c++
  --force
  --library=qt
  --suppress=missingIncludeSystem
  --suppress=missingInclude
  --suppress=unusedStructMember
  --inline-suppr
  --error-exitcode=1
  -i"$PROJECT_DIR/$BUILD_DIR"
  -i"$PROJECT_DIR/$THIRDPARTY_DIR"
  --include="$PROJECT_DIR/cmd/include"
  --include="$PROJECT_DIR/gui/include"
  --include="$PROJECT_DIR/lib/include"
  --checkers-report="$PROJECT_DIR/$BUILD_DIR/cppcheck_report.log"
)

SRC_FILES=$(find "$PROJECT_DIR" \
  -type d -name $BUILD_DIR -prune -o \
  -type d -name $THIRDPARTY_DIR -prune -o \
  -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -print)

for FILE in $SRC_FILES; do
  CPPCHECK_OPTS+=("$FILE")
done

echo "Running cppcheck with the following options: ${CPPCHECK_OPTS[@]}"
cppcheck "${CPPCHECK_OPTS[@]}"

exit 0
