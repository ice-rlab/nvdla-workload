#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <sys/stat.h>

// From nvdla_ioctl.h
#define NVDLA_IOCTL_REG_READ _IOWR('N', 0x05, struct nvdla_reg)
#define NVDLA_IOCTL_REG_WRITE _IOW('N', 0x06, struct nvdla_reg)

struct nvdla_reg {
    uint32_t addr;
    uint32_t data;
};

#define DEVICE "/dev/nvdla0"

struct reg_info {
    const char *name;
    uint32_t addr;
    uint32_t initial;
    uint32_t final;
};

// List of registers (from nvdla docs - might need adjustment from memory setup in sim)
struct reg_info regs[] = {
    {"CDMA_D_PERF_ENABLE", 0x5000 + 0xd4, 0, 0},
    {"CDMA_D_PERF_DAT_READ_STALL", 0x5000 + 0xd8, 0, 0},
    {"CDMA_D_PERF_WT_READ_STALL", 0x5000 + 0xdc, 0, 0},
    {"CDMA_D_PERF_DAT_READ_LATENCY", 0x5000 + 0xe0, 0, 0},
    {"CDMA_D_PERF_WT_READ_LATENCY", 0x5000 + 0xe4, 0, 0},
    {"CDMA_D_INF_INPUT_DATA_NUM", 0x5000 + 0xe8, 0, 0},
    {"CDMA_D_INF_INPUT_WEIGHT_NUM", 0x5000 + 0xec, 0, 0},
    {"CDMA_D_NAN_INPUT_DATA_NUM", 0x5000 + 0xf0, 0, 0},
    {"CDMA_D_NAN_INPUT_WEIGHT_NUM", 0x5000 + 0xf4, 0, 0},
    
    {"SDP_D_PERF_ENABLE", 0x8000 + 0xd4, 0, 0},
    {"SDP_D_PERF_MRDMA_READ_STALL", 0x8000 + 0xd8, 0, 0},
    {"SDP_D_PERF_BRDMA_READ_STALL", 0x8000 + 0xdc, 0, 0},
    {"SDP_D_PERF_NRDMA_READ_STALL", 0x8000 + 0xe0, 0, 0},
    {"SDP_D_PERF_ERDMA_READ_STALL", 0x8000 + 0xe4, 0, 0},
    {"SDP_D_PERF_WDMA_WRITE_STALL", 0x8000 + 0xe8, 0, 0},
    {"SDP_D_PERF_OUT_SATURATION", 0x8000 + 0xec, 0, 0},
    {"SDP_D_PERF_LUT_UFLOW", 0x8000 + 0xf0, 0, 0},
    {"SDP_D_PERF_LUT_OFLOW", 0x8000 + 0xf4, 0, 0},
    {"SDP_D_PERF_LUT_HYBRID", 0x8000 + 0xf8, 0, 0},
    {"SDP_D_PERF_LUT_LE_HIT", 0x8000 + 0xfc, 0, 0},
    {"SDP_D_PERF_LUT_LO_HIT", 0x8000 + 0x100, 0, 0},
    
    {"SDP_RDMA_D_STATUS_NAN_INPUT_NUM", 0x7000 + 0xb0, 0, 0},
    {"SDP_RDMA_D_STATUS_INF_INPUT_NUM", 0x7000 + 0xb4, 0, 0},
    {"SDP_D_STATUS_NAN_INPUT_NUM", 0x8000 + 0xb0, 0, 0},
    {"SDP_D_STATUS_INF_INPUT_NUM", 0x8000 + 0xb4, 0, 0},
    {"SDP_D_STATUS_NAN_OUTPUT_NUM", 0x8000 + 0xb8, 0, 0},
    
    {"CDP_D_PERF_ENABLE", 0xc000 + 0xd4, 0, 0},
    {"CDP_D_PERF_READ_STALL", 0xc000 + 0xd8, 0, 0},
    {"CDP_D_PERF_WRITE_STALL", 0xc000 + 0xdc, 0, 0},
    {"CDP_D_OUT_SATURATION", 0xc000 + 0xe0, 0, 0},
    {"CDP_D_PERF_LUT_UFLOW", 0xc000 + 0xe4, 0, 0},
    {"CDP_D_PERF_LUT_OFLOW", 0xc000 + 0xe8, 0, 0},
    {"CDP_D_PERF_LUT_HYBRID", 0xc000 + 0xec, 0, 0},
    {"CDP_D_PERF_LUT_LE_HIT", 0xc000 + 0xf0, 0, 0},
    {"CDP_D_PERF_LUT_LO_HIT", 0xc000 + 0xf4, 0, 0},
    {"CDP_D_INF_INPUT_NUM", 0xc000 + 0xf8, 0, 0},
    {"CDP_D_NAN_INPUT_NUM", 0xc000 + 0xfc, 0, 0},
    {"CDP_D_NAN_OUTPUT_NUM", 0xc000 + 0x100, 0, 0},
    
    {"PDP_D_PERF_ENABLE", 0xa000 + 0xd4, 0, 0},
    {"PDP_D_PERF_READ_STALL", 0xa000 + 0xd8, 0, 0},
    {"PDP_D_PERF_WRITE_STALL", 0xa000 + 0xdc, 0, 0},
    {"PDP_D_INF_INPUT_NUM", 0xa000 + 0xe0, 0, 0},
    {"PDP_D_NAN_INPUT_NUM", 0xa000 + 0xe4, 0, 0},
    {"PDP_D_NAN_OUTPUT_NUM", 0xa000 + 0xe8, 0, 0},
    
    {"RUBIK_D_PERF_ENABLE", 0x3000 + 0xd4, 0, 0},
    {"RUBIK_D_PERF_READ_STALL", 0x3000 + 0xd8, 0, 0},
    {"RUBIK_D_PERF_WRITE_STALL", 0x3000 + 0xdc, 0, 0},
    
    {"BDMA_CFG_STATUS_PERF_STALL_COUNT_EN", 0x4000 + 0x8, 0, 0},
    {"BDMA_STATUS_GRP0_READ_STALL", 0x4000 + 0x30, 0, 0},
    {"BDMA_STATUS_GRP0_WRITE_STALL", 0x4000 + 0x34, 0, 0},
    {"BDMA_STATUS_GRP1_READ_STALL", 0x4000 + 0x38, 0, 0},
    {"BDMA_STATUS_GRP1_WRITE_STALL", 0x4000 + 0x3c, 0, 0},
    
    {"CACC_D_OUT_SATURATION", 0xd000 + 0x14, 0, 0},
    {"D_CLIP_CFG", 0xd000 + 0x10, 0, 0},
    
    {NULL, 0, 0, 0} // once up to null - end
};

int main(int argc, char *argv[]) {
    if (argc < 4 || strcmp(argv[1], "--start") != 0) {
        fprintf(stderr, "Usage: %s --start <loadable> <image1> [image2 ...]\n", argv[0]);
        return 1;
    }

    char *loadable = argv[2];

    printf("DEBUG: Starting harness with loadable=%s\n", loadable);

    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("DEBUG: Failed to open device");
        return 1;
    }
    printf("DEBUG: Device opened successfully\n");

    // Enable all PERF_ENABLE registers and similar registers to take performance measurements
    struct nvdla_reg reg;
    printf("DEBUG: Enabling performance counters...\n");
    int enable_count = 0;
    for (int i = 0; regs[i].name != NULL; i++) {
        if (strstr(regs[i].name, "PERF_ENABLE") || strstr(regs[i].name, "PERF_STALL_COUNT_EN") || strstr(regs[i].name, "PERF_NAN_INF_COUNT_EN")) {
            reg.addr = regs[i].addr;
            reg.data = 1;
            if (ioctl(fd, NVDLA_IOCTL_REG_WRITE, &reg) < 0) {
                perror("DEBUG: Failed to enable perf counter");
                printf("DEBUG: Failed for register %s at addr 0x%x\n", regs[i].name, regs[i].addr);
            } else {
                printf("DEBUG: Enabled %s at addr 0x%x\n", regs[i].name, regs[i].addr);
                enable_count++;
            }
        }
    }
    printf("DEBUG: Enabled %d performance counters\n", enable_count);

    // Read initial values
    printf("DEBUG: Reading initial register values...\n");
    int initial_read_count = 0;
    for (int i = 0; regs[i].name != NULL; i++) {
        reg.addr = regs[i].addr;
        if (ioctl(fd, NVDLA_IOCTL_REG_READ, &reg) < 0) {
            perror("DEBUG: Failed to read initial value");
            printf("DEBUG: Failed for register %s at addr 0x%x\n", regs[i].name, regs[i].addr);
        } else {
            regs[i].initial = reg.data;
            printf("DEBUG: Initial value for %s (0x%x): 0x%x\n", regs[i].name, regs[i].addr, regs[i].initial);
            initial_read_count++;
        }
    }
    printf("DEBUG: Read %d initial values successfully\n", initial_read_count);

    // Run the benchmark for each image
    printf("DEBUG: Starting benchmark runs...\n");
    for (int img_idx = 3; img_idx < argc; img_idx++) {
        char *image = argv[img_idx];
        printf("Running '%s' through '%s'\n", image, loadable);
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "/root/nvdla_runtime --loadable %s --image %s", loadable, image);
        int ret = system(cmd);
        if (ret != 0) {
            fprintf(stderr, "DEBUG: Benchmark failed for image '%s' with return code: %d\n", image, ret);
            continue; // Continue with next images if one fails
        }
        printf("DEBUG: Benchmark completed for image '%s'\n", image);

        // Check and rename output (this is adapted from nvdla-run.sh script - hopefully it works)
        struct stat buffer;
        if (stat("output.dimg", &buffer) == 0) {
            char newname[256];
            snprintf(newname, sizeof(newname), "%s.dimg", image);
            if (rename("output.dimg", newname) == 0) {
                printf("DEBUG: Renamed output.dimg to %s\n", newname);
            } else {
                perror("DEBUG: Failed to rename output.dimg");
            }
        } else {
            printf("No output from '%s' detected!\n", image);
        }
    }
    printf("DEBUG: All benchmark runs completed\n");

    // Read final values
    printf("DEBUG: Reading final register values...\n");
    int final_read_count = 0;
    for (int i = 0; regs[i].name != NULL; i++) {
        reg.addr = regs[i].addr;
        if (ioctl(fd, NVDLA_IOCTL_REG_READ, &reg) < 0) {
            perror("DEBUG: Failed to read final value");
            printf("DEBUG: Failed for register %s at addr 0x%x\n", regs[i].name, regs[i].addr);
        } else {
            regs[i].final = reg.data;
            printf("DEBUG: Final value for %s (0x%x): 0x%x\n", regs[i].name, regs[i].addr, regs[i].final);
            final_read_count++;
        }
    }
    printf("DEBUG: Read %d final values successfully\n", final_read_count);

    // Output deltas
    printf("DEBUG: Computing and outputting deltas...\n");
    FILE *out = fopen("/root/perf_results.txt", "w");
    if (out == NULL) {
        perror("DEBUG: Failed to open output file");
    } else {
        for (int i = 0; regs[i].name != NULL; i++) {
            uint32_t delta = regs[i].final - regs[i].initial;
            fprintf(out, "%s: %u\n", regs[i].name, delta);
            printf("DEBUG: Delta for %s: %u (initial=0x%x, final=0x%x)\n", regs[i].name, delta, regs[i].initial, regs[i].final);
        }
        fclose(out);
        printf("DEBUG: Deltas written to perf_results.txt\n");
    }

    close(fd);
    printf("DEBUG: Harness completed\n");

    // Power off at the end, like the regular run script
    system("poweroff");

    return 0;
}