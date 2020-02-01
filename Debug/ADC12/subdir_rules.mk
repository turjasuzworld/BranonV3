################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ADC12/%.obj: ../ADC12/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.3.LTS/bin/cl430" -vmspx --data_model=restricted --opt_for_speed=2 --use_hw_mpy=F5 --include_path="C:/ti/ccs901/ccs/ccs_base/msp430/include" --include_path="C:/Users/Turjasu/OneDrive - TURJASUZWORLD/Projects/OnGoing/BranonWrkSpc/BRANONFW_00_00_03_00" --include_path="C:/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.3.LTS/include" --advice:power="all" --define=__MSP430F5419A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ADC12/$(basename $(<F)).d_raw" --obj_directory="ADC12" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


