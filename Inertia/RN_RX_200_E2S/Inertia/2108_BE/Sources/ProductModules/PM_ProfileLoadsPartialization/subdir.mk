################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Workspace/FW_Inertia/branch_indesit/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.c 

OBJS += \
./Sources/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.o 

C_DEPS += \
./Sources/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.o Sources/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.d: C:/Workspace/FW_Inertia/branch_indesit/ProductModules/PM_ProfileLoadsPartialization/PM_ProfileLoadsPartialization.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	rx-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT "$(@:%.o=%.d)"  -Wcomment -Wimplicit -Wparentheses -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wsign-compare -Waggregate-return -Wbad-function-cast -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -ansi -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS\tools" -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\EmbOS" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\McuModule" -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS\Services\ActivityList" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\HAL_General" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\IEC60335_B_libs\inc" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\IEC60335_B_libs\src\Common" -I"C:\Workspace\FW_Inertia\branch_indesit\OS\EmbOS\RX200_GNU\V388\Inc" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Analog" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_BackUp" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_DigitalIO" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Error" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Feedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_GridVoltage" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Generic" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_IOManager" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_notify" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Reset" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_SecurityCheck" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Setting" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Trace" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_ZcLoads" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_ZeroCrossing" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Fixed" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_AnalogFeedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_BusCOMM" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Consistency" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_DigitalOutputMulti" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_EnergyMeterSw" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_FreqFeedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Library" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_ProfileLoadsPartialization" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_SWalarms" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_SwTimersCounters" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_TurbiditySensor" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_PulseCounter" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Alternato" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_PowerProfile" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCTPM" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCDIO" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCCLK" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCRST" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCADC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCFLS" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCIRQ" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCI2C" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCLVD" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\SWCRC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCCRC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCUART" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCWDT" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\SWDMA" -I"C:\Workspace\FW_Inertia\branch_indesit\CFG_DishWasher\CFG_Inertia" -I"C:\Workspace\FW_Inertia\branch_indesit\Application_Platform" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\Control" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\Modules" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\General" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\UserInterface" -I"C:\Renesas\E2_STU~1\GNURXV~1.01-\rx-ELF/rx-elf/optlibinc" -DDEBUG=-2 -DDW_DISHWASHER -DOS_EmbOS -DRN_RX200 -DRN_RX200_R5F5210 -DRN_RX200_R5F5210_8 -DNO_BOOT_LOADER=1 -DEDGE_MODULE_LP1_0258_01 -DPLATFORM2_SUPPORT -DWILD_CARD -D_MQ_CONVERT_BITFIELD_ -D_ENABLE_USE_EMBOS_RESOURCES_ -D__RX_BIG_ENDIAN__=1 -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -g2 -g -mbig-endian-data -mcpu=rx200 -nofpu -x c "$<"
	rx-elf-gcc -Wa,-adlshn="$(basename $(notdir $<)).lst" -Wcomment -Wimplicit -Wparentheses -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wsign-compare -Waggregate-return -Wbad-function-cast -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -ansi -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS\tools" -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\EmbOS" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\McuModule" -I"C:\Workspace\FW_Inertia\branch_indesit\Main\EmbOS\Services\ActivityList" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\HAL_General" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\IEC60335_B_libs\inc" -I"C:\Workspace\FW_Inertia\branch_indesit\HAL\RN_RX200\IEC60335_B_libs\src\Common" -I"C:\Workspace\FW_Inertia\branch_indesit\OS\EmbOS\RX200_GNU\V388\Inc" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Analog" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_BackUp" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_DigitalIO" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Error" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Feedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_GridVoltage" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Generic" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_IOManager" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_notify" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Reset" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_SecurityCheck" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Setting" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Trace" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_ZcLoads" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_ZeroCrossing" -I"C:\Workspace\FW_Inertia\branch_indesit\PlatformLibrary\LIB_Fixed" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_AnalogFeedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_BusCOMM" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Consistency" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_DigitalOutputMulti" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_EnergyMeterSw" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_FreqFeedbacks" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Library" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_ProfileLoadsPartialization" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_SWalarms" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_SwTimersCounters" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_TurbiditySensor" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_PulseCounter" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_Alternato" -I"C:\Workspace\FW_Inertia\branch_indesit\ProductModules\PM_PowerProfile" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCTPM" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCDIO" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCCLK" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCRST" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCADC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCFLS" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCIRQ" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCI2C" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCLVD" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\SWCRC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCCRC" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCUART" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\OCWDT" -I"C:\Workspace\FW_Inertia\branch_indesit\Drivers\SWDMA" -I"C:\Workspace\FW_Inertia\branch_indesit\CFG_DishWasher\CFG_Inertia" -I"C:\Workspace\FW_Inertia\branch_indesit\Application_Platform" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\Control" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\Modules" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\General" -I"C:\Workspace\FW_Inertia\branch_indesit\Inertia\UserInterface" -I"C:\Renesas\E2_STU~1\GNURXV~1.01-\rx-ELF/rx-elf/optlibinc" -DDEBUG=-2 -DDW_DISHWASHER -DOS_EmbOS -DRN_RX200 -DRN_RX200_R5F5210 -DRN_RX200_R5F5210_8 -DNO_BOOT_LOADER=1 -DEDGE_MODULE_LP1_0258_01 -DPLATFORM2_SUPPORT -DWILD_CARD -D_MQ_CONVERT_BITFIELD_ -D_ENABLE_USE_EMBOS_RESOURCES_ -D__RX_BIG_ENDIAN__=1 -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -g2 -g -mbig-endian-data -mcpu=rx200 -nofpu -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

