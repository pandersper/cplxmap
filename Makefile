GCC := g++
CFLAGS := -std=c++23 -fmodules
MFLAGS := -fmodule-only

src_dir := src
obj_dir := obj
cmis_dir := gcm.cache

prev_src := ../last

MDIRS := src/ $(addprefix src/, model view control)
CDIRS := $(addprefix src/, nonclass model/nonclass view/nonclass control/nonclass)

std.gcm std.compat.gcm:
	$(GCC) $(CFLAGS) $(MFLAGS) --compile-std-module -c
	mkdir -p $(obj_dir)

std-module: std.gcm std.compat.gcm

msrcs := types.cppm statics_model.cppm sets.cppm functions.cppm context.cppm model.cppm
csrcs := statics_control.cppm runtime.cppm computing.cppm to_string.cppm control.cppm 
srcs := $(msrcs) $(csrcs) cplxmap.cppm

mobjs := $(patsubst %.cppm, %.o, $(msrcs))
cobjs := $(patsubst %.cppm, %.o, $(csrcs))
objs := $(mobjs) $(cobjs) cplxmap.o

mcmis := $(patsubst %.cppm, mvc-%.gcm, $(msrcs))
ccmis := $(patsubst %.cppm, mvc-%.gcm, $(csrcs))	
cmis := $(mcmis) $(ccmis) mvc.gcm

mobjsp := $(addprefix $(obj_dir)/, $(mobjs))
cobjsp := $(addprefix $(obj_dir)/, $(cobjs))
objsp := $(mobjsp) $(cobjsp) $(obj_dir)/cplxmap.o

mcmisp := $(addprefix $(cmis_dir)/, $(mcmis))
ccmisp := $(addprefix $(cmis_dir)/, $(ccmis))	
cmisp := $(mcmis) $(ccmis) $(cmis_dir)/mvc.gcm

vars:
	@echo $(msrcs)
	@echo $(csrcs)
	@echo $(mobjs)
	@echo $(cobjs)
	@echo $(mcmis)
	@echo $(ccmis)
	@echo $(objs)
	@echo $(srcs)
	@echo $(cmis)

vpath %.gcm $(cmis_dir)	
vpath %.cppm $(MDIRS)
vpath %.cpp $(CDIRS)
vpath %.o $(obj_dir)/

%.o: %.cppm
	$(GCC) $(CFLAGS) -c $< -o $(obj_dir)/$@

model-partitions:	$(mobjs)
view-partitions: 	
control-partitions: 	$(cobjs)

p_% :: %.cppm ; $(GCC) $(CFLAGS) -c $^
	@mv *.o obj/

c_% :: %.cpp ; $(GCC) $(CFLAGS) -c $^
	@mv *.o obj/

.PHONY: model view control mvc all
model:		model-partitions 
view:		view-partitions 
control:	model-partitions control-partitions cplxmap.o
mvc:		model-partitions control-partitions view-partitions cplxmap.o

all:		std-module mvc

.PHONY: main.model main.control main.to_string sysinfo typecheck

main.model:  		$(srcs) src/main.model.cpp
	$(MAKE) model p_cplxmap
	$(GCC) $(CFLAGS) src/main.model.cpp -o main.model.o
	@echo Runnning main.model.o
	@./main.model.o

main.control:  		$(srcs) src/main.control.cpp
	$(MAKE) control
	$(GCC) $(CFLAGS) src/control/runtime.cppm src/main.control.cpp -o main.control.o
	@echo Runnning main.control.o
	@./main.control.o

main.to_string:  	$(srcs) src/main.to_string.cpp
	$(MAKE) all
	$(GCC) $(CFLAGS) src/main.to_string.cpp -o main.to_string.o
	@echo Runnning main.to_string.o
	@./main.to_string.o

sysinfo:		$(srcs) src/sysinfo.cpp 
	$(GCC) $(CFLAGS) $^ -o sysinfo.o
	@echo Runnning sysinfo.o
	@./sysinfo.o

backup_dir := /workspace/tgz_cplxmap
.PHONY: try-backup
try-backup:
	@find . -name '*~' 
	@find . -name '.#*' 
	@find . -name '*#' 
	@echo $(backup_dir)/project_$(shell date +'%Y%m%d%H%M').tgz

.PHONY: backup
backup:
	find . -name '*~' -delete
	find . -name '.#*' -delete
	find . -name '*#' -delete
	tar -zcvf $(backup_dir)/project_$(shell date +'%Y%m%d%H%M').tgz ../project ../last

.PHONY:	clean
clean:
	@rm -rf ./obj/*
	@rm -rf ./gcm.cache/mvc*
	@rm -rf ./*.o	

.PHONY:		clean-all
clean-all:
		@rm -r obj gcm.cache ./*.o
