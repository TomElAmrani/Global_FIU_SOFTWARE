################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project" --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project/device" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-187638225: ../driverlib_cc_f28004x.sysconfig
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1270/ccs/utils/sysconfig_1.20.0/sysconfig_cli.bat" --script "C:/F28004x/Labs/Lab2/project/global_lear_project/driverlib_cc_f28004x.sysconfig" -o "syscfg" -s "C:/ti/c2000/C2000Ware_5_02_00_00/.metadata/sdk.json" -b "/boards/TMDSCNCD280049C" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-187638225 ../driverlib_cc_f28004x.sysconfig
syscfg/board.h: build-187638225
syscfg/board.cmd.genlibs: build-187638225
syscfg/board.opt: build-187638225
syscfg/board.json: build-187638225
syscfg/pinmux.csv: build-187638225
syscfg/adc.dot: build-187638225
syscfg/c2000ware_libraries.cmd.genlibs: build-187638225
syscfg/c2000ware_libraries.opt: build-187638225
syscfg/c2000ware_libraries.c: build-187638225
syscfg/c2000ware_libraries.h: build-187638225
syscfg/clocktree.h: build-187638225
syscfg: build-187638225

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project" --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project/device" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/F28004x/Labs/Lab2/project/global_lear_project/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


