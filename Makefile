OBJECTS := \
	.build/obj/src/main.o \
	.build/obj/src/system.o

.build/out/c8e: $(OBJECTS)
	mkdir -p .build/out
	$(CXX) -std=c++11 -Wall -Werror -Wextra -g -O0 -o $@ $^

$(OBJECTS): .build/obj/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) -c -std=c++11 -Wall -Werror -Wextra -g -O0 -MMD -MP -o $@ $<

-include $(OBJECTS:.o=.d)
