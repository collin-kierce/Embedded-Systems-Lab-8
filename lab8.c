/*
 * Arty A7-100 Robot Lab -- Student Starter
 * Vivado / Vitis 2025.2, MicroBlaze, standalone BSP
 *
 * Hardware:
 *   JA: Pmod MAXSONAR (AXI UART Lite, 9600 baud)
 *   JC: Pmod DHB1 (AXI GPIO, 8 bits)
 *   JD: PmodLS1 (AXI GPIO, 4 inputs; S1 and S4 used)
 *   USB: AXI UART Lite (115200 baud)
 *
 * STUDENT TASKS:
 *   1. Confirm hardware addresses in the Vivado Address Editor.
 *   2. Implement read_light_sensors().
 *   3. Implement poll_sonar() to parse MAXSONAR messages.
 *   4. Implement motor_set() and motor_stop().
 *   5. Implement line_follow_step() and obstacle detection.
 *
 * SAFETY: Test with the wheels raised. The starter never enables motors.
 */

#include "xil_types.h"
#include "xil_io.h"
#include "xuartlite_l.h"
#include "sleep.h"

/* Replace EACH example address with its assigned address in Vivado.
 * MOTOR_GPIO_BASE was previously 0x40000000 in the lab prototype.
 * Check all values after rebuilding/exporting your XSA.
 */
#define USB_UART_BASE    ((UINTPTR)0x40600000U) /* VERIFY */
#define SONAR_UART_BASE  ((UINTPTR)0x40610000U) /* VERIFY */
#define LS1_GPIO_BASE    ((UINTPTR)0x41200000U) /* VERIFY */
#define MOTOR_GPIO_BASE  ((UINTPTR)0x40000000U) /* VERIFY */

/* AXI GPIO channel 1 register offsets */
#define GPIO_DATA_OFFSET 0x00U
#define GPIO_TRI_OFFSET  0x04U

/* PmodLS1 on JD: S1 -> GPIO[0], S4 -> GPIO[3] */
#define LS1_S1_BIT 0U //Front left wheel
#define LS1_S4_BIT 3U //Front right wheel

/* Pmod DHB1 on JC -- check these against your XDC */
#define MOTOR1_EN_BIT  0U
#define MOTOR1_DIR_BIT 1U
#define MOTOR2_EN_BIT  4U
#define MOTOR2_DIR_BIT 5U
#define MOTOR_GPIO_INPUT_MASK 0xCCU /* Encoder signals are inputs */

typedef struct {
    u8 s1;
    u8 s4;
} LightSensors;

typedef struct {
    u32 distance_inches;
    u8 valid;
} SonarReading;

/* Provided: simple UART console output, no printf configuration needed. */
static void serial_char(char c)
{
    if (c == '\n') {
        XUartLite_SendByte(USB_UART_BASE, (u8)'\r');
    }
    XUartLite_SendByte(USB_UART_BASE, (u8)c);
}

static void serial_text(const char *message)
{
    while (*message != '\0') {
        serial_char(*message++);
    }
}

/* TODO 1: Read the PmodLS1 AXI GPIO DATA register.
 * Extract the bits for S1 and S4, and return them in the structure.
 * Use Xil_In32() and the LS1 definitions above.
 */
static LightSensors read_light_sensors(void)
{
    LightSensors result = {0U, 0U};

    /* TODO: Read real sensor values here. */

    return result;
}

/* TODO 2: Nonblocking MAXSONAR receiver.
 * The sonar streams messages of the form 'R' + 3 digits + '\r'.
 * Parse bytes from the sonar UART receiver and return a new reading
 * with valid=1 only after a complete, well-formed message arrives.
 * Returning valid=0 means no complete new reading yet.
 */
static SonarReading poll_sonar(void)
{
    SonarReading result = {0U, 0U};

    /* TODO: Read incoming UART bytes and parse a complete record.
     * You will likely need static parser state across calls.
     */

    return result;
}

/* TODO 3: Implement motor control with AXI GPIO.
 * Inputs: direction and enable for each motor (each 0 or 1).
 * The DHB1 EN and DIR controls are on MOTOR1_EN_BIT etc.
 * Stop motors BEFORE changing directions. Do not write encoder bits.
 */
static void motor_set(u8 motor1_enable, u8 motor1_direction,
                      u8 motor2_enable, u8 motor2_direction)
{
    (void)motor1_enable;
    (void)motor1_direction;
    (void)motor2_enable;
    (void)motor2_direction;

    /* TODO: Compose the output word and write GPIO DATA. */
}

/* TODO 4: Implement a proper motor stop. */
static void motor_stop(void)
{
    /* TODO: Disable both motors through the MOTOR GPIO DATA register. */
}

/* TODO 5: Implement the decision-making logic.
 * - Determine which light-sensor state means 'on the line'.
 * - Decide how to steer based on S1 and S4.
 * - If a valid sonar distance indicates an obstacle, stop.
 * - Use motor_set() and motor_stop() when implemented.
 */
static void line_follow_step(LightSensors light, SonarReading sonar)
{
    (void)light;
    (void)sonar;

    /* TODO: Your line-following and obstacle-avoidance algorithm. */
}

int main(void)
{
    SonarReading latest_sonar = {0U, 0U};
    LightSensors light;

    /* Motor safety: write disabled outputs before changing direction.
     * Verify MOTOR_GPIO_BASE and physical wiring before running.
     */
    Xil_Out32(MOTOR_GPIO_BASE + GPIO_DATA_OFFSET, 0U);
    Xil_Out32(MOTOR_GPIO_BASE + GPIO_TRI_OFFSET,
              MOTOR_GPIO_INPUT_MASK);

    serial_text("\n=== Arty Robot Lab Starter ===\n");
    serial_text("Complete the TODO functions to enable robot behavior.\n");
    

    while (1) {
        SonarReading new_sonar = poll_sonar();
        if (new_sonar.valid != 0U) {
            latest_sonar = new_sonar;
        }

        light = read_light_sensors();
        line_follow_step(light, latest_sonar);

        /* Starting control-loop interval; students may tune this. */
        usleep(10000U);
    }
}
