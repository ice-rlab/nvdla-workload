#!/bin/bash

# Model definitions (folder prototxt_url caffemodel_url model_name)
declare -A MODELS=(
  ["alexnet"]="alexnet https://raw.githubusercontent.com/BVLC/caffe/master/models/bvlc_alexnet/deploy.prototxt http://dl.caffe.berkeleyvision.org/bvlc_alexnet.caffemodel alexnet"
  ["googlenet"]="googlenet https://raw.githubusercontent.com/BVLC/caffe/master/models/bvlc_googlenet/deploy.prototxt http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel googlenet"
  ["lenet_mnist"]="lenet_mnist https://www.esp.cs.columbia.edu/docs/thirdparty_acc/lenet_mnist.prototxt https://www.esp.cs.columbia.edu/docs/thirdparty_acc/lenet_mnist.caffemodel lenet_mnist"
  ["resnet50"]="imagenet https://raw.githubusercontent.com/KaimingHe/deep-residual-networks/master/prototxt/ResNet-50-deploy.prototxt https://www.deepdetect.com/downloads/platform/pretrained/caffe/resnet_50/ResNet-50-model.caffemodel resnet50"
)

# Check dependencies
if ! command -v wget &> /dev/null; then
  echo "Error: wget is required. Install it (e.g., sudo apt-get install wget)."
  exit 1
fi

# Function to process a model
process_model() {
  local key=$1
  IFS=' ' read -r folder prototxt_url caffemodel_url model_name <<< "${MODELS[$key]}"

  echo "Processing $model_name (folder: $folder)..."

  # Create source directory if needed
  mkdir -p "$folder/source"

  # Download and rename prototxt
  local prototxt_file="$folder/source/${model_name}.prototxt"
  if [ ! -f "$prototxt_file" ]; then
    echo "Downloading prototxt for $model_name..."
    wget -q "$prototxt_url" -O "$prototxt_file" || { echo "Failed to download prototxt"; return 1; }
  fi

  # Download and rename caffemodel
  local caffemodel_file="$folder/source/${model_name}.caffemodel"
  if [ ! -f "$caffemodel_file" ]; then
    echo "Downloading caffemodel for $model_name..."
    wget -q "$caffemodel_url" -O "$caffemodel_file" || { echo "Failed to download caffemodel"; return 1; }
  fi

}

# Main logic
if [ $# -eq 1 ]; then
  # Process specific model
  local key="$1"
  if [[ -n "${MODELS[$key]}" ]]; then
    process_model "$key"
  else
    echo "Error: Model $key not supported. Choose from: ${!MODELS[@]}"
    exit 1
  fi
else
  # Process all models
  for key in "${!MODELS[@]}"; do
    process_model "$key"
  done
fi

echo "Processing complete."