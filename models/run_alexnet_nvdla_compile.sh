#!/bin/bash

NVDLA_WORKLOAD_DIR=$PWD/..
OUTPUT_DIR=$NVDLA_WORKLOAD_DIR/models/alexnet
COMPILER_PATH=$NVDLA_WORKLOAD_DIR/compiler

export LD_LIBRARY_PATH=$NVDLA_WORKLOAD_DIR/compiler:$LD_LIBRARY_PATH

$COMPILER_PATH/nvdla_compiler \
    --prototxt $NVDLA_WORKLOAD_DIR/models/alexnet/source/alexnet.prototxt \
    --caffemodel $NVDLA_WORKLOAD_DIR/models/alexnet/source/alexnet.caffemodel \
    --configtarget nv_small \
    --informat nchw \
    --cprecision fp16 \
    -o $OUTPUT_DIR