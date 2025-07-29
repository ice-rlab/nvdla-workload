#!/bin/bash

NVDLA_WORKLOAD_DIR=$PWD/..
OUTPUT_DIR=$NVDLA_WORKLOAD_DIR/models/googlenet
COMPILER_PATH=$NVDLA_WORKLOAD_DIR/compiler

export LD_LIBRARY_PATH=$NVDLA_WORKLOAD_DIR/compiler:$LD_LIBRARY_PATH

$COMPILER_PATH/nvdla_compiler \
    --prototxt $NVDLA_WORKLOAD_DIR/models/googlenet/source/googlenet.prototxt \
    --caffemodel $NVDLA_WORKLOAD_DIR/models/googlenet/source/googlenet.caffemodel \
    --configtarget nv_small \
    --informat nchw \
    --cprecision fp16 \
    -o $OUTPUT_DIR

mv fast-math.nvdla $OUTPUT_DIR/googlenet_small.nvdla