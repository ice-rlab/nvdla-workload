#!/bin/bash

loadable=$1
image=$2

./nvdla_perf_harness --start $loadable $image