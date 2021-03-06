NAME = hunters
CXX := g++

all: $(NAME)

MODULES := src/display src/client includes src/net
LIBDIRS := 
#CXXFLAGS := -Iincludes -Wall -Werror -Wextra -g
CXXFLAGS := -I./includes -g -fsanitize=address
LDFLAGS := -lncurses -lform -lpthread -lsqlite3 -lmenu
MODNAME := module.mk
DEBUGDEFINES := -D_DEBUG
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))
DEP :=	$(patsubst %.cpp,%.d,$(filter %.cpp,$(SRC)))

-include $(DEP)

debug: CXXFLAGS += -D_DEBUG 
debug: re

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.d : %.c
	@./depend.sh $*.o $(CXXFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CXXFLAGS) -c -o $*.o $<" >> $@
	@echo $@ >> all.log

clean:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean_nolib
	rm -f $(NAME)
	

re: fclean all

.PHONY: all clean fclean re
