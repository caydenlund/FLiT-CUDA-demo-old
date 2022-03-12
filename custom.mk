# This file is included by the generated Makefile.
# Here is where you can add your own customizations.

PACKAGES_DIR   := $(abspath ./packages)
MATX_SRC       := $(PACKAGES_DIR)/MatX

SOURCE         :=
SOURCE         += $(wildcard *.cpp)
SOURCE         += $(wildcard tests/*.cpp)

SOURCE         += $(wildcard $(MATX_SRC)/*.cpp)

CC_REQUIRED    += -I${MATX_SRC}/include
CC_REQUIRED    += -I${MATX_SRC}/include/kernels
CC_REQUIRED    += -I${MATX_SRC}/examples
