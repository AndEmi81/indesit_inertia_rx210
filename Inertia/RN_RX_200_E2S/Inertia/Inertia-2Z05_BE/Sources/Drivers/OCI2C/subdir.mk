################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Workspace/FW_Platform2_5/trunk/Drivers/OCI2C/OCI2C_drv.c 

OBJS += \
./Sources/Drivers/OCI2C/OCI2C_drv.o 

C_DEPS += \
./Sources/Drivers/OCI2C/OCI2C_drv.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Drivers/OCI2C/OCI2C_drv.o Sources/Drivers/OCI2C/OCI2C_drv.d: C:/Workspace/FW_Platform2_5/trunk/Drivers/OCI2C/OCI2C_drv.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	rx-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT "$(@:%.o=%.d)"  -Wcomment -Wimplicit -Wparentheses -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wsign-compare -Waggregate-return -Wbad-function-cast -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -ansi  -I"C:/Workspace/FW_Platform2_5/trunk/Inertia/RN_RX_200_E2S/Inertia" -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS\tools" -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\EmbOS" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\McuModule" -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS\Services\ActivityList" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\HAL_General" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\IEC60335_B_libs\inc" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\IEC60335_B_libs\src\Common" -I"C:\Workspace\FW_Platform2_5\trunk\OS\EmbOS\RX200_GNU\V388\Inc" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Analog" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_BackUp" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_DigitalIO" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Error" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Feedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_GridVoltage" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Generic" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_IOManager" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_notify" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Reset" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_SecurityCheck" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Setting" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Trace" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_ZcLoads" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_ZeroCrossing" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_AnalogFeedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_BusCOMM" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_Consistency" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_DigitalOutputMulti" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_EnergyMeterSw" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_FreqFeedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_Library" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_ProfileLoadsPartialization" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_SWalarms" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_SwTimersCounters" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCTPM" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCDIO" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCCLK" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCRST" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCADC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCFLS" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCIRQ" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCI2C" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCLVD" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\SWCRC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCCRC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCUART" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCWDT" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\SWDMA" -I"C:\Workspace\FW_Platform2_5\trunk\CFG_DishWasher\CFG_Inertia" -I"C:\Workspace\FW_Platform2_5\trunk\Application_Platform" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\Control" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\Modules" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\General" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\UserInterface" -I"C:\Renesas\e2studio\GNURXV~1.03-\rx-elf/rx-elf/optlibinc" -DDEBUG=-2 -DOS_EmbOS -DRN_RX200 -DRN_RX200_R5F52Z0 -DRN_RX200_R5F52Z0_5 -DNO_BOOT_LOADER=1 -DEDGE_MODULE_LP1_0258_01 -DPLATFORM2_SUPPORT -DWILD_CARD -DDW_DISHWASHER -D_MQ_CONVERT_BITFIELD_ -D_ENABLE_USE_EMBOS_RESOURCES_ -D__RX_BIG_ENDIAN__=1 -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -g2 -g -mbig-endian-data -mcpu=rx200 -nofpu -mrelax -x c "$<"
	rx-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" -Wcomment -Wimplicit -Wparentheses -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wsign-compare -Waggregate-return -Wbad-function-cast -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -ansi -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS\tools" -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\EmbOS" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\McuModule" -I"C:\Workspace\FW_Platform2_5\trunk\Main\EmbOS\Services\ActivityList" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\HAL_General" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\IEC60335_B_libs\inc" -I"C:\Workspace\FW_Platform2_5\trunk\HAL\RN_RX200\IEC60335_B_libs\src\Common" -I"C:\Workspace\FW_Platform2_5\trunk\OS\EmbOS\RX200_GNU\V388\Inc" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Analog" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_BackUp" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_DigitalIO" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Error" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Feedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_GridVoltage" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Generic" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_IOManager" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_notify" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Reset" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_SecurityCheck" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Setting" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_Trace" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_ZcLoads" -I"C:\Workspace\FW_Platform2_5\trunk\PlatformLibrary\LIB_ZeroCrossing" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_AnalogFeedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_BusCOMM" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_Consistency" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_DigitalOutputMulti" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_EnergyMeterSw" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_FreqFeedbacks" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_Library" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_ProfileLoadsPartialization" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_SWalarms" -I"C:\Workspace\FW_Platform2_5\trunk\ProductModules\PM_SwTimersCounters" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCTPM" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCDIO" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCCLK" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCRST" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCADC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCFLS" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCIRQ" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCI2C" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCLVD" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\SWCRC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCCRC" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCUART" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\OCWDT" -I"C:\Workspace\FW_Platform2_5\trunk\Drivers\SWDMA" -I"C:\Workspace\FW_Platform2_5\trunk\CFG_DishWasher\CFG_Inertia" -I"C:\Workspace\FW_Platform2_5\trunk\Application_Platform" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\Control" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\Modules" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\General" -I"C:\Workspace\FW_Platform2_5\trunk\Inertia\UserInterface" -I"C:\Renesas\e2studio\GNURXV~1.03-\rx-elf/rx-elf/optlibinc" -DDEBUG=-2 -DOS_EmbOS -DRN_RX200 -DRN_RX200_R5F52Z0 -DRN_RX200_R5F52Z0_5 -DNO_BOOT_LOADER=1 -DEDGE_MODULE_LP1_0258_01 -DPLATFORM2_SUPPORT -DWILD_CARD -DDW_DISHWASHER -D_MQ_CONVERT_BITFIELD_ -D_ENABLE_USE_EMBOS_RESOURCES_ -D__RX_BIG_ENDIAN__=1 -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -g2 -g -mbig-endian-data -mcpu=rx200 -nofpu -mrelax -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

