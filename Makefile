#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

all: AsyncRSP.rel ftp.rel NetLog.rel

AsyncRSP.rel:
	@cd AsyncRSP && make

ftp.rel:
	@cd ftp && make

NetLog.rel:
	@cd NetLog && make

clean:
	@cd AsyncRSP && make clean
	@cd ftp && make clean
	@cd NetLog && make clean