#!/bin/bash
set -e

wget -O darknet-nvdla.tar.gz http://www.ittc.ku.edu/~farshchi/darknet-nvdla.tar.gz
tar xzf darknet-nvdla.tar.gz -C overlay/root
