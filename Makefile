#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

export TOOLS 	:= $(CURDIR)/tools
export LIB 		:= $(CURDIR)/lib


.PHONY: all Physics #AsyncRSP CodeMenu ftp NetLog

all: Physics #AsyncRSP CodeMenu ftp NetLog

AsyncRSP:
	$(MAKE) -C AsyncRSP
	@cp AsyncRSP/$@.rel $@.rel

CodeMenu:
	$(MAKE) -s -C CodeMenu
	@cp CodeMenu/$@.rel $@.rel

ftp:
	$(MAKE) -s -C ftp
	@cp ftp/$@.rel $@.rel

NetLog:
	$(MAKE) -s -C NetLog
	@cp NetLog/$@.rel $@.rel

Physics:
	$(MAKE) -s -C Physics
	@cp Physics/$@.rel $@.rel

clean:
	@rm ./*.rel
	$(MAKE) -s -C AsyncRSP clean
	$(MAKE) -s -C CodeMenu clean
	$(MAKE) -s -C ftp clean
	$(MAKE) -s -C NetLog clean
	$(MAKE) -s -C Physics clean