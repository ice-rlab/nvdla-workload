#!/bin/bash

SCRIPTS=(
    "~/riscv-performance-characterization/sims/firesim-workloads/nvdla-workload/models/run_alexnet_nvdla_compile.sh"
    "~/riscv-performance-characterization/sims/firesim-workloads/nvdla-workload/models/run_googlenet_nvdla_compile.sh"
    "~/riscv-performance-characterization/sims/firesim-workloads/nvdla-workload/models/run_lenet_nvdla_compile.sh"
    "~/riscv-performance-characterization/sims/firesim-workloads/nvdla-workload/models/run_resnet50_nvdla_compile.sh"
)

# loop running through the scripts
for script in "${SCRIPTS[@]}"; do
    echo "Running $script..."
    if [ -f "$script" ]; then
        bash "$script"
        if [ $? -eq 0 ]; then
            echo "$script completed successfully"
        else
            echo "Error: $script failed"
            exit 1
        fi
    else
        echo "Error: $script not found"
        exit 1
    fi
done

echo "All scripts completed"