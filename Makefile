#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

export TOOLS 	:= $(CURDIR)/tools
export LIB 		:= $(CURDIR)/lib


.PHONY: all clean

all: AsyncRSP.rel ftp.rel NetLog.rel

AsyncRSP.rel:
	$(MAKE) -C AsyncRSP
	@cp AsyncRSP/$@ $@

ftp.rel:
	$(MAKE) -s -C ftp
	@cp ftp/$@ $@

NetLog.rel:
	$(MAKE) -s -C NetLog
	@cp NetLog/$@ $@

clean:
	@rm ./*.rel
	$(MAKE) -s -C AsyncRSP clean
	$(MAKE) -s -C ftp clean
	$(MAKE) -s -C NetLog clean