#pragma once

#include "i8042.h"
#include "utils.h"

/** @defgroup mouse Mouse
 * @{
 */

extern bool mouse_ih_error, is_mouse_packet_complete;
extern struct packet mouse_parsed_packet;

/** 
 * @brief Susbcribes mouse interrupts 
 * @returns 0 on success, non-zero otherwise
 */
uint8_t (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubcribes mouse interrupts
 * @returns 0 on success, non-zero otherwise
 */
uint8_t (mouse_unsubscribe_int)();

/** 
 * @brief Enables mouse interrupts 
 * @returns 0 on success, 1 otherwise
*/
uint8_t mouse_enable_int();

/** 
 * @brief Disables mouse interrupts 
 * @returns 0 on success, 1 otherwise
*/
uint8_t mouse_disable_int();

/** 
 * @brief Handles mouse interrutps.
 * Reads the status register and the output buffer (OB).
 * @note If there was some error, the byte read from the OB should be discarded.
 */
void (mouse_ih)();

/** 
 * @returns 0 on success, 1 otherwise
 */ 
uint8_t (mouse_data_handler)();

uint8_t mouse_data_reporting_enable();
uint8_t mouse_data_reporting_disable();
uint8_t mouse_set_stream_mode();
uint8_t mouse_set_remote_mode();
uint8_t mouse_read_data();
uint8_t gesture_sm(uint8_t x_len, uint8_t tolerance);

/** @} end of Mouse */
