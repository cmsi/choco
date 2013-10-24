# Makefile

Machine = reims

TARGET = DLA1

INCDIR = $(shell pwd)/inc
LIBDIR = $(shell pwd)/lib
OBJDIR = $(shell pwd)/obj


CFLAGS = -lwrm1

ifeq ($(Machine),kashiwa)
CC=icpc
RNDPATH = /opt/nano/alps/boost_1_52_0
CFLAGS += -lmpi -O3 #kashiwa
endif
ifeq ($(Machine),fx10)
CC = mpiFCCpx
RNDPATH = /global/nano/alps/boost_1_52_0-r2491
CFLAGS += -Kfast -Ksimd=2 -O3 -Kdalign -Klib -Keval -Kprefetch_conditional -Kilfunc -Kparallel -Xg -Nlst
endif
ifeq ($(Machine),psi)
CC=mpicxx
RNDPATH = /opt/nano/alps/boost_1_52_0
CFLAGS += -O3 -Wall -fno-exceptions #PSI
endif
ifeq ($(Machine),reims)
CC=mpic++
RNDPATH = $(HOME) #/opt/nano/alps/boost_1_52_0
CFLAGS += -O3 #kashiwa
endif


CFLAGS += -I${INCDIR}
CFLAGS += -I${RNDPATH}
CFLAGS += -I${MPIPATH}/include
CFLAGS += -L${RNDPATH}
CFLAGS += -L${LIBDIR}


OBJS =\
	$(OBJDIR)/DLA.o \
	$(OBJDIR)/lattice.o \
	$(OBJDIR)/probability.o \
	$(OBJDIR)/configuration.o \
	$(OBJDIR)/graphspace.o \
	$(OBJDIR)/quantities.o \



# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o

# プライマリターゲット
.PHONY: all
all: $(TARGET)

# プログラムの生成ルール
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) 
#	@rm -f $(OBJS)

# サフィックスルール
$(OBJDIR)/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) depend.inc

# ヘッダーファイルの依存関係
.PHONY: depend
depend: $(OBJS:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do\
		cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc;\
	done

-include depend.inc
