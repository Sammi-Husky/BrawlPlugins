#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

export TOOLS 	:= $(CURDIR)/tools
export LIB 		:= $(CURDIR)/lib


.PHONY: all AsyncRSP ftp NetLog

all: AsyncRSP ftp NetLog

AsyncRSP:
	$(MAKE) -C AsyncRSP
	@cp AsyncRSP/$@.rel $@.rel

ftp:
	$(MAKE) -s -C ftp
	@cp ftp/$@.rel $@.rel

NetLog:
	$(MAKE) -s -C NetLog
	@cp NetLog/$@.rel $@.rel

clean:
	@rm ./*.rel
	$(MAKE) -s -C AsyncRSP clean
	$(MAKE) -s -C ftp clean
	$(MAKE) -s -C NetLog clean