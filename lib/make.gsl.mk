#  Makefile for Libraries

#CC       = icpc
CC = FCCpx
AR       = ar
SRCDIR	= .
INCD00	= $(shell pwd)/../inc
LIBD00	= $(shell pwd)
ARFLAG   = crsv
LFILE    = $(LIBD00)/libwrm1.a
DEST	= $(shell pwd)
CP       = /bin/mv
LIBS	= 
GSLPATH = /k/home/users/zs1/usr
CFLAGS	= -Kfast -Ksimd=2 -lgsl -lgslcblas -I$(INCD00) -I$(GSLPATH)/include -L$(GSLPATH)/lib -L$(LIBD00)

vpath %.h $(INCD00) 
vpath %.cpp $(INCD00) 


SRCS =\
	$(LIBD00)/My_rdm.cpp \
	$(LIBD00)/stdma.cpp \
#	$(LIBD00)/configuration.cpp \

OBJS =\
	$(LIBD00)/My_rdm.o \
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
