# See LICENSE for license details.

ifndef _SIFIVE_MK_COMMON
_SIFIVE_MK_COMMON := # defined

.PHONY: all
all: $(TARGET)

include $(BSP_BASE)/libwrap/libwrap.mk

ENV_DIR = $(BSP_BASE)/env
PLATFORM_DIR = $(ENV_DIR)/$(BOARD)

ASM_SRCS += $(ENV_DIR)/start.S
ASM_SRCS += $(ENV_DIR)/entry.S
C_SRCS += $(PLATFORM_DIR)/init.c

LINKER_SCRIPT := $(PLATFORM_DIR)/$(LINK_TARGET).lds

INCLUDES += -I$(BSP_BASE)/include
INCLUDES += -I$(BSP_BASE)/drivers/
INCLUDES += -I$(ENV_DIR)
INCLUDES += -I$(PLATFORM_DIR)

TOOL_DIR = $(BSP_BASE)/../toolchain/bin

LDFLAGS += -T $(LINKER_SCRIPT) -nostartfiles
LDFLAGS += -L$(ENV_DIR) --specs=nano.specs

ASM_OBJS := $(ASM_SRCS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)

LINK_OBJS += $(ASM_OBJS) $(C_OBJS)
LINK_DEPS += $(LINKER_SCRIPT)

CLEAN_OBJS += $(TARGET) $(LINK_OBJS) $(TARGET).bin $(TARGET).hex $(TARGET).elf $(TARGET).lst

CFLAGS += -g
CFLAGS += -march=$(RISCV_ARCH)
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=medany

OBJCOPY := $(abspath $(RISCV_PATH)/bin/riscv64-unknown-elf-objcopy)
OBJDUMP := $(abspath $(RISCV_PATH)/bin/riscv64-unknown-elf-objdump)

elf := $(TARGET).elf
$(elf): $(LINK_OBJS) $(LINK_DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $(TARGET).elf $(LDFLAGS)

.PHONY: elf
elf: $(elf)

bin := $(TARGET).bin
$(bin): $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $@

.PHONY: bin
bin: $(bin)

lst := $(TARGET).lst
$(lst) :$(TARGET).elf
	$(OBJDUMP) --source --all-headers --demangle --line-numbers --wide $< > $@

hex := $(TARGET).hex
$(hex): $(bin) $(lst)
	od -t x4 -An -w4 -v $< > $@

.PHONY: hex
hex: $(hex)

$(TARGET): $(elf) $(bin) $(hex) $(lst)
	echo "$(TARGET) is $(TARGET).elf"


$(ASM_OBJS): %.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -include sys/cdefs.h -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(CLEAN_OBJS)

endif # _SIFIVE_MK_COMMON
