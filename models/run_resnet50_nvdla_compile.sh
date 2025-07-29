#!/bin/bash

NVDLA_WORKLOAD_DIR=$PWD/..
OUTPUT_DIR=$NVDLA_WORKLOAD_DIR/models/imagenet
COMPILER_PATH=$NVDLA_WORKLOAD_DIR/compiler

export LD_LIBRARY_PATH=$NVDLA_WORKLOAD_DIR/compiler:$LD_LIBRARY_PATH

$COMPILER_PATH/nvdla_compiler \
    --prototxt $NVDLA_WORKLOAD_DIR/models/imagenet/source/resnet50.prototxt \
    --caffemodel $NVDLA_WORKLOAD_DIR/models/imagenet/source/resnet50.caffemodel \
    --configtarget nv_small \
    --informat nchw \
    --cprecision int8 \
    --calibtable $NVDLA_WORKLOAD_DIR/models/imagenet/source/resnet50.json \
    -o $OUTPUT_DIR

mv fast-math.nvdla $OUTPUT_DIR/resnet50_small.nvdla