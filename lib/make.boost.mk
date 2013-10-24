#  Makefile for Libraries

Machine = kashiwa

SRCDIR	= .
INCD00	= $(shell pwd)/../inc
LIBD00	= $(shell pwd)
ARFLAG   = crsv
LFILE    = $(LIBD00)/libwrm1.a
DEST	= $(shell pwd)
CP       = /bin/mv
LIBS	= 

ifeq ($(Machine),kashiwa)
CC=icpc
BOOSTPATH = /opt/nano/alps/boost_1_52_0
CFLAGS	= -Wall -O3 -I$(INCD00) -I$(BOOSTPATH) -Xg -L$(LIBD00)
endif
ifeq ($(Machine),fx10)
CC = FCCpx
BOOSTPATH = /global/nano/alps/boost_1_52_0-r2491
CFLAGS	= -Xg -Kfast -O3 -Kdalign -Klib -Keval -Kprefetch_conditional -Kilfunc -Kparallel -Ksimd=2 -I$(INCD00) -I$(BOOSTPATH) -L$(LIBD00)
endif
ifeq ($(Machine),psi)
CC=icpc
RNDPATH = /opt/nano/alps/boost_1_52_0
CFLAGS	= -Wall -O3 -I$(INCD00) -I$(BOOSTPATH) -Xg -L$(LIBD00)
endif
ifeq ($(Machine),maki)
CC = FCCpx
BOOSTPATH = /global/nano/alps/boost_1_52_0-r2491
CFLAGS	= -Xg -Kfast -O3 -Kdalign -Klib -Keval -Kprefetch_conditional -Kilfunc -Kparallel -Ksimd=2 -I$(INCD00) -I$(BOOSTPATH) -L$(LIBD00)
endif
ifeq ($(Machine),reims)
CC=icpc
CFLAGS	= -Wall -O3 -I$(INCD00) -I$(BOOSTPATH) -Xg -L$(LIBD00)
endif


AR       = ar


vpath %.h $(INCD00) 
vpath %.cpp $(INCD00) 


SRCS =\
	$(LIBD00)/My_rdm.boost.cpp \
	$(LIBD00)/stdma.cpp \
#	$(LIBD00)/configuration.cpp \

OBJS =\
	$(LIBD00)/My_rdm.boost.o \
	$(LIBD00)/stdma.o \
#	$(LIBD00)/configuration.o \

all:	$(OBJS) $(LFILE)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

$(LFILE): $(OBJS) $(SRCS)
	$(AR) $(ARFLAG) $(LFILE) $(OBJS)
	@rm -f $(OBJS)
	@echo "libMy_rdm.a make success"

install: $(LFILE)
	@echo Installing $(LFILE) in $(DEST)
	@$(CP) $(LFILE) $(DEST)

clean:; @rm -f $(OBJS) 
	@rm -f $(LFILE)
