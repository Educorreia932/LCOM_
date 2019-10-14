#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include "i8042.h"
#include "utils.h"

extern uint8_t scancode, is_scancode_complete, kbc_error;
extern int global_kbd_bit_no;

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();

int kbc_send_cmd(uint8_t port, uint8_t cmd, uint8_t *status);
int kbc_read_outbf(uint8_t port, uint8_t *content, uint8_t *status);

int kbc_get_scancode(uint8_t isPoll);
int kbc_print_scancode_wrapper();

int kbc_reenable_default_int();
