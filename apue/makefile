####################################################
# 万能Makefile
# for compiling and linking C++ projects on Linux 
####################################################

CC:=g++
CFLAGS := -g -Wall
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD
RM-F := rm -f

# # You shouldn't need to change anything below this point.
#
SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))

.PHONY : all deps objs clean  info

all: $(EXECUTABLE)

deps : $(DEPS)

objs : $(OBJS)

clean :
	$(RM-F) *.o
	$(RM-F) *.d
run :
	./$(EXECUTABLE)

$(EXECUTABLE) : $(OBJS)
	$(CC) -o  $(EXECUTABLE) $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS)) -lpthread

info:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(MISSING_DEPS)
	@echo $(MISSING_DEPS_SOURCES)
