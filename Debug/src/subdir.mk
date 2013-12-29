################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/filters.cc \
../src/statistics.cc \
../src/stockpile.cc 

OBJS += \
./src/filters.o \
./src/statistics.o \
./src/stockpile.o 

CC_DEPS += \
./src/filters.d \
./src/statistics.d \
./src/stockpile.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -I"/home/ardillo/workspace/impro-assignment" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


