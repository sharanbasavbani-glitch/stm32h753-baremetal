# Experiment 02: Button-Controlled LED Using STM32H753 GPIO

## 1. Objective

To develop a bare-metal embedded C program that reads a push button using GPIO input and controls an LED using GPIO output on the STM32H753 microcontroller.

The experiment demonstrates direct register-level programming without using STM32 HAL or LL libraries.

The firmware is compiled using ARM GNU GCC and tested using Renode 1.17.0.

## 2. Development Environment

| Component | Configuration |
|---|---|
| Microcontroller | STM32H753 |
| Simulation board | NUCLEO-H753ZI |
| IDE | Visual Studio Code |
| Compiler | ARM GNU GCC |
| Build system | CMake + Ninja |
| Simulator | Renode 1.17.0 |
| Programming approach | Bare-metal C |
| Peripheral | GPIO |

**Testing status:** Successfully tested in Renode simulation. Physical hardware testing has not yet been performed.

## 3. GPIO Pin Configuration

| Pin | Function | Configuration |
|---|---|---|
| PB0 | Green LED | GPIO output |
| PB3 | Virtual push button | GPIO input with internal pull-up |

### Button operation

The button uses an active-LOW configuration.

| Button condition | PB3 logic level |
|---|---|
| Released | HIGH (1) |
| Pressed | LOW (0) |

The internal pull-up resistor keeps PB3 HIGH when the button is released. Pressing the button connects the input to ground, producing a LOW level.

## 4. Registers Used

### RCC_AHB4ENR — GPIO Clock Enable

Address: `0x580244E0`

Bit 1 enables the GPIOB peripheral clock.

```c
RCC_AHB4ENR |= (1U << 1);
```

A read-back ensures that the clock-enable write has completed before accessing GPIOB.

### GPIOB_MODER — GPIO Mode Register

Address: `0x58020400`

- PB0: Output mode (`01`)
- PB3: Input mode (`00`)

Each GPIO pin uses two bits in MODER.

### GPIOB_PUPDR — Pull-up/Pull-down Register

Address: `0x5802040C`

PB3 is configured with an internal pull-up resistor (`01`).

### GPIOB_IDR — Input Data Register

Address: `0x58020410`

Used to read the logic level of PB3.

```c
GPIOB_IDR & (1U << 3)
```

### GPIOB_BSRR — Bit Set/Reset Register

Address: `0x58020418`

Used to control the LED on PB0.

| Operation | Register value |
|---|---|
| LED ON | `1U << 0` |
| LED OFF | `1U << 16` |

BSRR allows setting or resetting an output pin without a read-modify-write operation.

## 5. Program Logic

1. Enable the GPIOB peripheral clock.
2. Configure PB0 as output.
3. Configure PB3 as input.
4. Enable the internal pull-up resistor on PB3.
5. Initially switch OFF the LED.
6. Continuously read PB3.
7. If PB3 is HIGH, keep the LED OFF.
8. If PB3 is LOW, wait and read PB3 again.
9. If PB3 remains LOW, switch ON the LED.
10. If PB3 becomes HIGH, keep the LED OFF.

## 6. Renode Simulation

The NUCLEO-H753ZI platform was loaded in Renode.

A virtual button was configured on PB3 using `pb3_button.repl`.

### Virtual button configuration

```text
PB3Button: Miscellaneous.Button @ gpioPortB
    invert: true
    -> gpioPortB@3
```

The `invert: true` setting ensures that pressing the virtual button produces the LOW input level required by the active-LOW firmware logic.

### Simulation commands

Load the board:

```text
mach create
machine LoadPlatformDescription @D:/RENODE/renode_1.17.0+20261102git249dd50e9-portable/platforms/boards/nucleo_h753zi.repl
```

Load the virtual button:

```text
machine LoadPlatformDescription @C:/Users/Sharanbasav/06_stm32/stm32h753/pb3_button.repl
```

Load the firmware:

```text
sysbus LoadELF @C:/Users/Sharanbasav/06_stm32/stm32h753/build/firmware.elf
```

Start execution:

```text
start
```

### Button control commands

Press:

```text
sysbus.gpioPortB.PB3Button Press
```

Release:

```text
sysbus.gpioPortB.PB3Button Release
```

## 7. Simulation Results

The following register values were observed during testing.

| Test condition | GPIOB_IDR | GPIOB_ODR | LED |
|---|---:|---:|---|
| Button released | `0x00000008` | `0x00000000` | OFF |
| Button pressed | `0x00000000` | `0x00000001` | ON |
| Button released again | `0x00000008` | `0x00000000` | OFF |

### Register interpretation

**Button released:**

`GPIOB_IDR = 0x00000008`

Bit 3 is HIGH, indicating that the button is released.

**Button pressed:**

`GPIOB_IDR = 0x00000000`

Bit 3 is LOW, indicating that the button is pressed.

**LED ON:**

`GPIOB_ODR = 0x00000001`

Bit 0 is HIGH.

**LED OFF:**

`GPIOB_ODR = 0x00000000`

Bit 0 is LOW.

The simulation confirmed that the firmware correctly detected button presses and releases and controlled the LED accordingly.

## 8. Current Limitations and Future Improvements

The current implementation includes a basic delay and a second input check to reduce false activation during a press.

However, it does not implement complete switch debouncing:

- The release transition is not independently debounced.
- A software delay is not a calibrated time measurement.
- A blocking delay prevents the processor from performing other work during that period.
- Mechanical switch bounce has not been tested using physical hardware.

Future improvements:

1. Implement proper press and release debouncing.
2. Introduce timer-based non-blocking debounce.
3. Explore GPIO external interrupts.
4. Verify the implementation using physical STM32H753 hardware.

## 9. Learning Outcomes

Through this experiment, the following concepts were implemented and verified:

- Bare-metal GPIO programming.
- RCC peripheral clock configuration.
- GPIO input and output modes.
- Internal pull-up configuration.
- Active-LOW input logic.
- Reading GPIO input data registers.
- Controlling outputs using BSRR.
- Software delays and basic switch debouncing.
- Register-level debugging in Renode.

## 10. Conclusion

A bare-metal button-controlled LED application was successfully developed for the STM32H753 and verified through Renode simulation.

The experiment provided practical experience in configuring GPIO registers, reading digital inputs, controlling digital outputs, and debugging embedded firmware through peripheral register inspection.