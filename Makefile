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

SRC = $(wildcard ${SRC_DIR}/*.c)
INC = $(wildcard ${SRC_DIR}/*.h)
OBJ = $(patsubst %.c, %.o, ${SRC})
OUT = ${OUTPUT_DIR}/${PROJECT_NAME}.so
TEST_SRC = $(wildcard ${TEST_DIR}/*.c)
TEST_OUT = $(basename ${TEST_SRC})

# Flags
DEBUG   = -g
OPT     = -O0
WARN    = -Wall -Wextra
CFLAGS  = -std=c23 -fPIC ${WARN} ${OPT} ${DEBUG} -I ${INC_DIR} -L ${OUTPUT_DIR}
LDFLAGS = -shared ${WARN} ${OPT} ${DEBUG}

# Rules
# Default
.PHONY: all
all: clean mkbuild ${OUT} ${TEST_OUT}

# Clean
.PHONY: clean
clean:
	-@${RM} ${OUT}
	-@${RM} ${OBJ}

# compile
%.o: %.c
	${CC} -c ${CFLAGS} $^ -o $@

%: %.c
	${CC} ${CFLAGS} $^ -o ${OUTPUT_DIR}/$(notdir $@) -l ustr

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
