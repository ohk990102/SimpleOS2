#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <arch/i386/interrupt.h>
#include <arch/i386/idt.h>
#include <arch/i386/utility.h>
#include <kernel/queue.h>
#include <kernel/keyboard.h>




static uint8_t get_scancode() {
    while(!is_keyboard_outbuf_full());
    return inb(0x60);
}
static struct KeyboardManagerStruct keyboardManager = { 0, };
static struct Queue keyQueue;
static struct KeyDataStruct keyQueueBuffer[ KEY_MAXQUEUECOUNT ];

static struct KeyMappingEntryStruct keyMappingTable[ KEY_MAPPINGTABLEMAXCOUNT ] =
{
    /*  0   */  {   KEY_NONE        ,   KEY_NONE        },
    /*  1   */  {   KEY_ESC         ,   KEY_ESC         },
    /*  2   */  {   '1'             ,   '!'             },
    /*  3   */  {   '2'             ,   '@'             },
    /*  4   */  {   '3'             ,   '#'             },
    /*  5   */  {   '4'             ,   '$'             },
    /*  6   */  {   '5'             ,   '%'             },
    /*  7   */  {   '6'             ,   '^'             },
    /*  8   */  {   '7'             ,   '&'             },
    /*  9   */  {   '8'             ,   '*'             },
    /*  10  */  {   '9'             ,   '('             },
    /*  11  */  {   '0'             ,   ')'             },
    /*  12  */  {   '-'             ,   '_'             },
    /*  13  */  {   '='             ,   '+'             },
    /*  14  */  {   KEY_BACKSPACE   ,   KEY_BACKSPACE   },
    /*  15  */  {   KEY_TAB         ,   KEY_TAB         },
    /*  16  */  {   'q'             ,   'Q'             },
    /*  17  */  {   'w'             ,   'W'             },
    /*  18  */  {   'e'             ,   'E'             },
    /*  19  */  {   'r'             ,   'R'             },
    /*  20  */  {   't'             ,   'T'             },
    /*  21  */  {   'y'             ,   'Y'             },
    /*  22  */  {   'u'             ,   'U'             },
    /*  23  */  {   'i'             ,   'I'             },
    /*  24  */  {   'o'             ,   'O'             },
    /*  25  */  {   'p'             ,   'P'             },
    /*  26  */  {   '['             ,   '{'             },
    /*  27  */  {   ']'             ,   '}'             },
    /*  28  */  {   '\n'            ,   '\n'            },
    /*  29  */  {   KEY_CTRL        ,   KEY_CTRL        },
    /*  30  */  {   'a'             ,   'A'             },
    /*  31  */  {   's'             ,   'S'             },
    /*  32  */  {   'd'             ,   'D'             },
    /*  33  */  {   'f'             ,   'F'             },
    /*  34  */  {   'g'             ,   'G'             },
    /*  35  */  {   'h'             ,   'H'             },
    /*  36  */  {   'j'             ,   'J'             },
    /*  37  */  {   'k'             ,   'K'             },
    /*  38  */  {   'l'             ,   'L'             },
    /*  39  */  {   ';'             ,   ':'             },
    /*  40  */  {   '\''            ,   '\"'            },
    /*  41  */  {   '`'             ,   '~'             },
    /*  42  */  {   KEY_LSHIFT      ,   KEY_LSHIFT      },
    /*  43  */  {   '\\'            ,   '|'             },
    /*  44  */  {   'z'             ,   'Z'             },
    /*  45  */  {   'x'             ,   'X'             },
    /*  46  */  {   'c'             ,   'C'             },
    /*  47  */  {   'v'             ,   'V'             },
    /*  48  */  {   'b'             ,   'B'             },
    /*  49  */  {   'n'             ,   'N'             },
    /*  50  */  {   'm'             ,   'M'             },
    /*  51  */  {   ','             ,   '<'             },
    /*  52  */  {   '.'             ,   '>'             },
    /*  53  */  {   '/'             ,   '?'             },
    /*  54  */  {   KEY_RSHIFT      ,   KEY_RSHIFT      },
    /*  55  */  {   '*'             ,   '*'             },
    /*  56  */  {   KEY_LALT        ,   KEY_LALT        },
    /*  57  */  {   ' '             ,   ' '             },
    /*  58  */  {   KEY_CAPSLOCK    ,   KEY_CAPSLOCK    },
    /*  59  */  {   KEY_F1          ,   KEY_F1          },
    /*  60  */  {   KEY_F2          ,   KEY_F2          },
    /*  61  */  {   KEY_F3          ,   KEY_F3          },
    /*  62  */  {   KEY_F4          ,   KEY_F4          },
    /*  63  */  {   KEY_F5          ,   KEY_F5          },
    /*  64  */  {   KEY_F6          ,   KEY_F6          },
    /*  65  */  {   KEY_F7          ,   KEY_F7          },
    /*  66  */  {   KEY_F8          ,   KEY_F8          },
    /*  67  */  {   KEY_F9          ,   KEY_F9          },
    /*  68  */  {   KEY_F10         ,   KEY_F10         },
    /*  69  */  {   KEY_NUMLOCK     ,   KEY_NUMLOCK     },
    /*  70  */  {   KEY_SCROLLLOCK  ,   KEY_SCROLLLOCK  },

    /*  71  */  {   KEY_HOME        ,   '7'             },
    /*  72  */  {   KEY_UP          ,   '8'             },
    /*  73  */  {   KEY_PAGEUP      ,   '9'             },
    /*  74  */  {   '-'             ,   '-'             },
    /*  75  */  {   KEY_LEFT        ,   '4'             },
    /*  76  */  {   KEY_CENTER      ,   '5'             },
    /*  77  */  {   KEY_RIGHT       ,   '6'             },
    /*  78  */  {   '+'             ,   '+'             },
    /*  79  */  {   KEY_END         ,   '1'             },
    /*  80  */  {   KEY_DOWN        ,   '2'             },
    /*  81  */  {   KEY_PAGEDOWN    ,   '3'             },
    /*  82  */  {   KEY_INS         ,   '0'             },
    /*  83  */  {   KEY_DEL         ,   '.'             },
    /*  84  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  85  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  86  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  87  */  {   KEY_F11         ,   KEY_F11         },
    /*  88  */  {   KEY_F12         ,   KEY_F12         }
};
static bool is_alphabet_scancode(uint8_t scanCode) {
    if(('a' <= keyMappingTable[scanCode].normalCode) && 
    (keyMappingTable[scanCode].normalCode) <= 'b') 
        return true;
    return false;
}
static bool is_num_or_sym_scancode(uint8_t scanCode) {
    if((2 <= scanCode) && (scanCode <= 53) && !is_alphabet_scancode(scanCode))
        return true;
    return false;
}
static bool is_numpad_scancode(uint8_t scanCode) {
    if((71 <= scanCode) && (scanCode <= 83))
        return true;
    return false;
}
static bool is_use_combined_code(uint8_t scanCode) {
    uint8_t downScanCode = scanCode & 0x7F;
    bool useCombinedKey = false;

    if(is_alphabet_scancode(downScanCode)) {
        if(keyboardManager.shiftDown ^ keyboardManager.capsLockOn)
            useCombinedKey = true;
        else
            useCombinedKey = false;
    }
    else if(is_num_or_sym_scancode(downScanCode)) {
        if(keyboardManager.shiftDown)
            useCombinedKey = true;
        else
            useCombinedKey = false;
    }
    else if(is_numpad_scancode(downScanCode) && !keyboardManager.extendedCodeIn) {
        if(keyboardManager.numLockOn)
            useCombinedKey = true;
        else
            useCombinedKey = false;
    }
    return useCombinedKey;
}

static bool changeKeyboardLED(bool capsLockOn, bool numLockOn, bool scrollLockOn) {
    bool previousInterrupt = set_interrupt_flag(false);
    bool result;
    uint8_t data;

    for(int i = 0; i < 0xFFFF; i++) {
        if(!is_keyboard_inbuf_full()) 
            break;
    }
    outb(0x60, 0xED);
    for(int i = 0; i < 0xFFFF; i++) {
        if(!is_keyboard_inbuf_full()) 
            break;
    }
    result = wait_for_ACK();
    if(!result) {
        set_interrupt_flag(previousInterrupt);
        return false;
    }
    outb(0x60, (capsLockOn << 2) | (numLockOn << 1) | (scrollLockOn));
    for(int i = 0; i < 0xFFFF; i++) {
        if(!is_keyboard_inbuf_full()) 
            break;
    }
    result = wait_for_ACK();
    set_interrupt_flag(previousInterrupt);
    return result;
}
static void updateKeyboardStatus(uint8_t scanCode) {
    bool down;
    uint8_t downScanCode;
    bool LEDStatusChanged = false;

    if(scanCode & 0x80) {
        down = false;
        downScanCode = scanCode & 0x8F;
    }
    else {
        down = true;
        downScanCode = scanCode;
    }

    if((downScanCode == 42) || (downScanCode == 54))
        keyboardManager.shiftDown = down;
    else if((downScanCode == 58) && down) {
        keyboardManager.capsLockOn ^= true;
        LEDStatusChanged = true;
    }
    else if((downScanCode == 69) && down) {
        keyboardManager.numLockOn ^= true;
        LEDStatusChanged = true;
    }
    else if((downScanCode == 70) && down) {
        keyboardManager.scrollLockOn ^= true;
        LEDStatusChanged = true;
    }

    if(LEDStatusChanged == true)
        changeKeyboardLED(keyboardManager.capsLockOn, keyboardManager.numLockOn, keyboardManager.scrollLockOn);
}

static bool convert_to_ascii(uint8_t scanCode, uint8_t * asciiCode, bool * flags) {
    bool useCombinedKey;

    if(keyboardManager.skipCountForPause > 0) {
        keyboardManager.skipCountForPause--;
        return false;
    }

    if(scanCode == 0xE1) {
        *asciiCode = KEY_PAUSE;
        *flags = KEY_FLAGS_DOWN;
        keyboardManager.skipCountForPause = KEY_SKIPCOUNTFORPAUSE;
        return true;
    }
    else if(scanCode == 0xE0) {
        keyboardManager.extendedCodeIn = true;
        return false;
    }

    useCombinedKey = is_use_combined_code(scanCode);

    if(useCombinedKey)
        *asciiCode = keyMappingTable[scanCode & 0x7F].combinedCode;
    else
        *asciiCode = keyMappingTable[scanCode & 0x7F].normalCode;
    
    if(keyboardManager.extendedCodeIn) {
        *flags = KEY_FLAGS_EXTENDEDKEY;
        keyboardManager.extendedCodeIn = false;
    }
    else
        *flags = 0;
    
    if((scanCode & 0x80) == 0)
        *flags |= KEY_FLAGS_DOWN;
    
    updateKeyboardStatus(scanCode);

    return true;
}
uint8_t keyDataToQueue(uint8_t scanCode) {
    struct KeyDataStruct data;
    bool result = false;
    bool previousInterrupt;
    data.scanCode = scanCode;
    if(convert_to_ascii(scanCode, &(data.asciiCode), &(data.flags))) {
        previousInterrupt = set_interrupt_flag(false);
        result = putQueue(&keyQueue, &data);
        set_interrupt_flag(previousInterrupt);
    }
    return result;
}
static uint8_t wait_for_ACK() {
    bool previousInterrupt = set_interrupt_flag(false);
    bool result = false;

    outb(0x64, 0xAE);
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 0xFFFF; j++) {
            if(is_keyboard_outbuf_full()) {
                break;
            }
        }
        uint8_t data = inb(0x60);
        if(data == 0xFA) {
            result = true;
            break;
        }
        else
            keyDataToQueue(data);
    }
    return result;
}
bool keyDataFromQueue(struct KeyDataStruct * data) {
    bool result;
    bool previousInterrupt;

    if(isQueueEmpty(&keyQueue))
        return false;
    previousInterrupt = set_interrupt_flag(false);
    result = getQueue(&keyQueue, data);
    set_interrupt_flag(previousInterrupt);

    return result;
}
bool activate_keyboard() {
    bool previousInterrupt = set_interrupt_flag(false);
    bool result = false;

    outb(0x64, 0xAE);

    for(int j = 0; j < 0xFFFF; j++) {
        if(is_keyboard_outbuf_full()) {
            break;
        }
    }
    outb(0x60, 0xF4);
    result = wait_for_ACK();
    set_interrupt_flag(previousInterrupt);

    return result;
}
void keyboard_handler(struct registers_t * r) {
    uint8_t tmp;
    if(is_keyboard_outbuf_full()) {
        tmp = get_scancode();
        keyDataToQueue(tmp);
    }
    send_EOI_to_PIC(r->int_no);
}

