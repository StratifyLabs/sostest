#ifndef QSPITEST_HPP
#define QSPITEST_HPP

#include <sapi/hal.hpp>
#include <sapi/test.hpp>
#include "sos/dev/qspi.h"


class QSPITest : public Test {
public:
    /* Configuration Register for mx25l512 start */
    #define QSPI_DUMMY_CYCLES_READ_QUAD      3
    #define QSPI_DUMMY_CYCLES_READ           8
    #define QSPI_DUMMY_CYCLES_READ_QUAD_IO   10
    #define QSPI_DUMMY_CYCLES_READ_DTR       6
    #define QSPI_DUMMY_CYCLES_READ_QUAD_DTR  8

    #define QSPI_CR_ODS                      (0x07)    /*!< Output driver strength */
    #define QSPI_CR_ODS_30                   (0x07)    /*!< Output driver strength 30 ohms (default)*/
    #define QSPI_CR_ODS_15                   (0x06)    /*!< Output driver strength 15 ohms */
    #define QSPI_CR_ODS_20                   (0x05)    /*!< Output driver strength 20 ohms */
    #define QSPI_CR_ODS_45                   (0x03)    /*!< Output driver strength 45 ohms */
    #define QSPI_CR_ODS_60                   (0x02)    /*!< Output driver strength 60 ohms */
    #define QSPI_CR_ODS_90                   (0x01)    /*!< Output driver strength 90 ohms */
    #define QSPI_CR_TB                       (0x08)    /*!< Top/Bottom bit used to configure the block protect area */
    #define QSPI_CR_PBE                      (0x10)    /*!< Preamble Bit Enable */
    #define QSPI_CR_4BYTE                    (0x20)    /*!< 3-bytes or 4-bytes addressing */
    #define QSPI_CR_NB_DUMMY                 (0xC0)    /*!< Number of dummy clock cycles */

    #define QUAD_OUT_FAST_READ_CMD               0x6B
    /* Quad Operations */
    #define ENTER_QUAD_CMD                       0x35
    #define EXIT_QUAD_CMD                        0xF5
    /* Write Operations */
    #define WRITE_ENABLE_CMD                     0x06
    #define WRITE_DISABLE_CMD                    0x04
    #define QUAD_IN_FAST_PROG_CMD                0x38
    #define EXT_QUAD_IN_FAST_PROG_CMD            0x38
    #define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x3E

    /* Status Register */
    #define QSPI_SR_WIP                      ((uint8_t)0x01)    /*!< Write in progress */
    #define QSPI_SR_WREN                     ((uint8_t)0x02)    /*!< Write enable latch */
    #define QSPI_SR_BLOCKPR                  ((uint8_t)0x5C)    /*!< Block protected against program and erase operations */
    #define QSPI_SR_PRBOTTOM                 ((uint8_t)0x20)    /*!< Protected memory area defined by BLOCKPR starts from top or bottom */
    #define QSPI_SR_QUADEN                   ((uint8_t)0x40)    /*!< Quad IO mode enabled if =1 */
    #define QSPI_SR_SRWREN                   ((uint8_t)0x80)    /*!< Status register write enable/disable */
    /* Register Operations */
    #define READ_STATUS_REG_CMD                  0x05
    #define READ_CFG_REG_CMD                     0x15
    #define WRITE_STATUS_CFG_REG_CMD             0x01

    /* 4-byte Address Mode Operations */
    #define ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
    #define EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

    #define QSPI_PAGE_SIZE                       256
    #define TIMER_SAFE_VALUE                     10000

    #define QUAD_INOUT_FAST_READ_CMD             0xEB
    #define QUAD_INOUT_FAST_READ_DTR_CMD         0xED
    #define QPI_READ_4_BYTE_ADDR_CMD             0xEC
    /* Erase Operations */
    #define SUBSECTOR_ERASE_CMD                  0x20
    #define SUBSECTOR_ERASE_4_BYTE_ADDR_CMD      0x21
    /* Program Operations */
    #define PAGE_PROG_CMD                        0x02
    #define QPI_PAGE_PROG_4_BYTE_ADDR_CMD        0x12
    /* Preference*/
    #define FLASH_SIZE  0x4000000
    #define QSPI_SECTOR_SIZE                     4096

    QSPITest();
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();

private:
    u32 qspi_flash_count;
};

#endif // QSPITEST_HPP
