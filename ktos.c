#include <string.h>
#include <stdio.h>
#include "termkey.h"

#define KC_DEFAULT        0
#define KC_C0_SYMBOL      1
#define KC_C0_SYMBOL_F    2
#define KC_SPECIAL_SYMBOL 3
#define KC_ARROW_SYMBOL   4

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

static struct {
    char *seq;
    int   category;
} termkey_sym_book[] = {
    [TERMKEY_SYM_BACKSPACE] = { "\x08", KC_C0_SYMBOL },
    [TERMKEY_SYM_TAB]       = { "\x09", KC_C0_SYMBOL },
    [TERMKEY_SYM_ENTER]     = { "\x0d", KC_C0_SYMBOL },
    [TERMKEY_SYM_ESCAPE]    = { "\x1b", KC_C0_SYMBOL_F },
    [TERMKEY_SYM_SPACE]     = { "\x20", KC_C0_SYMBOL },
    [TERMKEY_SYM_DEL]       = { "\x7f", KC_C0_SYMBOL },
    [TERMKEY_SYM_UP]        = { "\e[A", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_DOWN]      = { "\e[B", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_LEFT]      = { "\e[D", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_RIGHT]     = { "\e[C", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_HOME]      = { "\e[H", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_END]       = { "\e[F", KC_ARROW_SYMBOL },
    [TERMKEY_SYM_INSERT]    = { "\e[2~", KC_SPECIAL_SYMBOL },
    [TERMKEY_SYM_DELETE]    = { "\e[3~", KC_SPECIAL_SYMBOL },
    [TERMKEY_SYM_PAGEUP]    = { "\e[5~", KC_SPECIAL_SYMBOL },
    [TERMKEY_SYM_PAGEDOWN]  = { "\e[6~", KC_SPECIAL_SYMBOL },
};

static char *termkey_func_seq[] = {
    [1]        = "\e[11~",
    [2]        = "\e[12~",
    [3]        = "\e[13~",
    [4]        = "\e[14~",
    [5]        = "\e[15~",
    [6]        = "\e[17~",
    [7]        = "\e[18~",
    [8]        = "\e[19~",
    [9]        = "\e[20~",
    [10]       = "\e[21~",
    [11]       = "\e[23~",
    [12]       = "\e[24~",
};

static int unicode_ctrl_raw[256] = {
    ['H'] = 1,
    ['I'] = 1,
    ['M'] = 1,
    ['['] = 1
};

int
termkey_ktos(TermKey *tk, char *buf, size_t len, TermKeyKey *key) {
    switch (key->type) {
    case TERMKEY_TYPE_UNICODE:
        if (key->modifiers == 0) {
            
            int l = strlen(key->utf8);
            if (len > l)
                strcpy(buf, key->utf8);
            return l;
            
        }

        if (key->modifiers == TERMKEY_KEYMOD_CTRL) {
            int cp = key->code.codepoint;
            if (cp >= 'a' && cp <= 'z') cp = cp + 'A' - 'a';

            if (cp >= 0x40 && cp <= 0x5F &&
                unicode_ctrl_raw[cp] == 0) {
                
                if (len >= 2) {
                    buf[0] = cp - 0x40;
                    buf[1] = 0;
                }
                
                return 1;
            }            
        }

        if (key->modifiers == TERMKEY_KEYMOD_ALT) {
            
            int l = strlen(key->utf8);
            if (len > l + 1) {
                buf[0] = '\e';
                strcpy(buf + 1, key->utf8);
            }
            return l + 1;
            
        } else {
            return snprintf(buf, len,
                            "\e[%ld;%du",
                            key->code.codepoint, 1 + key->modifiers);
        }

    case TERMKEY_TYPE_KEYSYM:
        
        if (key->code.sym < ARRAY_LENGTH(termkey_sym_book) &&
            termkey_sym_book[key->code.sym].seq) {
            
            int   category = termkey_sym_book[key->code.sym].category;
            char *keyseq   = termkey_sym_book[key->code.sym].seq;
            int   l        = strlen(keyseq);

            if (category == KC_C0_SYMBOL_F) {
                return snprintf(buf, len, "\e[%d;%du",
                                keyseq[0], 1 + key->modifiers);
            }
            else if (key->modifiers) {
                switch (category) {
                case KC_C0_SYMBOL:
                    if (key->modifiers == TERMKEY_KEYMOD_ALT) {
                        
                        return snprintf(buf, len, "\e%s", keyseq);
                        
                    } else {

                        /* fallback to unicode for other
                         * modifiers. Assume the keyseq is always one
                         * char long. */
                        return snprintf(buf, len, "\e[%d;%du",
                                        keyseq[0], 1 + key->modifiers);
                    }
                    
                case KC_SPECIAL_SYMBOL: { 
                    int ll = snprintf(buf, len, "%s%d~",
                                      keyseq,
                                      1 + key->modifiers);
                    if (ll >= 0 && ll < len)
                        /* change '~' to ';' */
                        buf[l - 1] = ';';
                    return ll;
                }

                case KC_ARROW_SYMBOL:
                    return snprintf(buf, len, "\e[1;%d%s",
                                    1 + key->modifiers,
                                    keyseq + 2); /* Ignore CSI */
                }
            } else {
                if (len > l)
                    strcpy(buf, keyseq);
                return l;
            }
        } else {
            return 0;
        }

    case TERMKEY_TYPE_FUNCTION:
        if (key->code.number < 1 ||
            key->code.number > 12) return 0;
        if (key->code.number < ARRAY_LENGTH(termkey_func_seq) &&
            termkey_func_seq[key->code.number]) {
            
            char *keyseq = termkey_func_seq[key->code.number];
            int   l      = strlen(keyseq);
            if (key->modifiers) {
                int ll = snprintf(buf, len, "%s%d~",
                                  keyseq,
                                  1 + key->modifiers);
                if (ll >= 0 && ll < len)
                    /* change '~' to ';' */
                    buf[l - 1] = ';';
                return ll;
            } else {
                if (len > l)
                    strcpy(buf, keyseq);
                return l;
            }
        } else {
            return 0;
        }
        
    default:
        return 0;
    }
}
