#!/bin/env make
################################################################################
# uStr Makefile
################################################################################

# Compilers
CC ?= clang
AR ?= ar
AS ?= as

# Project config
TYPE        ?= Debug
ARCH        := $(shell uname -m)
PROJECT_NAME:= ulibc
PROJECT_ROOT:= .
SRC_DIR     := ${PROJECT_ROOT}/src
INC_DIR     := ${PROJECT_ROOT}/inc
LIB_DIR     := ${PROJECT_ROOT}/lib
OUTPUT_DIR  := ${PROJECT_ROOT}/bin/${TYPE}/${ARCH}
TEST_DIR    := ${PROJECT_ROOT}/test

## Platform-concerning
OUT_PREFIX := lib
OUT_SUFFIX := .so

SRC = $(wildcard ${SRC_DIR}/*.c)
INC = $(wildcard ${SRC_DIR}/*.h)
OBJ = $(patsubst %.c, %.o, ${SRC})
OUT = ${OUTPUT_DIR}/${OUT_PREFIX}${PROJECT_NAME}${OUT_SUFFIX}
TEST_SRC = $(wildcard ${TEST_DIR}/*.c)
TEST_OUT = $(basename ${TEST_SRC})

# Flags
ifeq (${TYPE}, Debug)
	CFLAGS += -D_FORTIFY_SOURCE=0
	DEBUG  := -g3
	OPT    := -O0
else ifeq (${TYPE}, Release)
	DEBUG  :=
	OPT    := -O2
endif
WARN    = -Wall -Wextra
CFLAGS  = -std=c2x -fPIC ${WARN} ${OPT} ${DEBUG} -I ${INC_DIR} -L ${OUTPUT_DIR} -D LITTLE_ENDIAN
LDFLAGS = -shared ${WARN} ${OPT} ${DEBUG} 


# Rules
# Default
.PHONY: all
all: clean mkbuild ${OUT} ${TEST_OUT}

# Clean
.PHONY: clean
clean:
	-${RM} ${OUT}
	-${RM} ${OBJ}
	-${RM} $(addprefix ${OUTPUT_DIR}/,$(notdir ${TEST_OUT}))

# compile
%.o: %.c
	${CC} -c ${CFLAGS} $^ -o $@

%: %.c
	${CC} ${CFLAGS} $^ -o ${OUTPUT_DIR}/$(notdir $@) -l ${PROJECT_NAME} -Wl,-rpath='$$ORIGIN' -l cunit

.PHONY: test
test: ${TEST_OUT}

ifeq ($(wildcard ${OUTPUT_DIR}),)
.PHONY: mkbuild
mkbuild:
	-@mkdir -p ${OUTPUT_DIR}
else
.PHONY: mkbuild
mkbuild:
	-@:
endif

${OUT}: ${OBJ}
	${CC} ${DEBUG} ${LDFLAGS} $^ -o $@

run: ${TEST_OUT}
