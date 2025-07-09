# Enable logging, set the log level and set trace color.
export ENABLE_TRACE ?= 1
export TRACE_LEVEL ?= 4
export ENABLE_TRACE_COLOR ?= 1

ifeq ($(ENABLE_TRACE),1)
CFLAGS += -DENABLE_TRACE
endif

# To ensure that the value of TRACE_LEVEL is passed into the code, you need to add TRACE_LEVEL=$(TRACE_LEVEL).
CFLAGS += -DTRACE_LEVEL=$(TRACE_LEVEL)

ifeq ($(ENABLE_TRACE_COLOR),1)
CFLAGS += -DENABLE_TRACE_COLOR
endif


# BT
export BT_ENABLE_SNOOP ?= 1
ifeq ($(BT_ENABLE_SNOOP),1)
CFLAGS += -DBT_ENABLE_SNOOP
endif

