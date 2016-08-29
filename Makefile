CONTIKI = $(DROPBOX_HOME)/neighbor-discovery/nddev-contiki

ifndef TARGET
TARGET=sky
endif

DEFINES=NETSTACK_MAC=nullmac_driver,NETSTACK_RDC=nullrdc_noframer_driver,CC2420_CONF_AUTOACK=0,RF_CHANNEL=11

all: jammer

include $(CONTIKI)/Makefile.include
