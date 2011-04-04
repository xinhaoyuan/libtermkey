#include "../termkey.h"
#include "taplib.h"

int main(int argc, char *argv[])
{
  TermKey      *tk;
  TermKeyKey    key;

#define CLEAR_KEY do { key.type = -1; key.code.codepoint = -1; key.modifiers = -1; key.utf8[0] = 0; } while(0)

  plan_tests(38);

  tk = termkey_new(0, TERMKEY_FLAG_NOTERMIOS);

  CLEAR_KEY;
  termkey_strpkey(tk, "A", &key, 0);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/A/0");
  is_int(key.code.codepoint, 'A',               "key.code.codepoint for unicode/A/0");
  is_int(key.modifiers,   0,                    "key.modifiers for unicode/A/0");
  is_str(key.utf8,        "A",                  "key.utf8 for unicode/A/0");

  CLEAR_KEY;
  termkey_strpkey(tk, "C-b", &key, 0);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/b/CTRL");
  is_int(key.code.codepoint, 'b',               "key.code.codepoint for unicode/b/CTRL");
  is_int(key.modifiers,   TERMKEY_KEYMOD_CTRL,  "key.modifiers for unicode/b/CTRL");
  is_str(key.utf8,        "b",                  "key.utf8 for unicode/b/CTRL");

  CLEAR_KEY;
  termkey_strpkey(tk, "Ctrl-b", &key, TERMKEY_FORMAT_LONGMOD);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/b/CTRL longmod");
  is_int(key.code.codepoint, 'b',               "key.code.codepoint for unicode/b/CTRL longmod");
  is_int(key.modifiers,   TERMKEY_KEYMOD_CTRL,  "key.modifiers for unicode/b/CTRL longmod");
  is_str(key.utf8,        "b",                  "key.utf8 for unicode/b/CTRL longmod");

  CLEAR_KEY;
  termkey_strpkey(tk, "^B", &key, TERMKEY_FORMAT_CARETCTRL);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/b/CTRL caretctrl");
  is_int(key.code.codepoint, 'b',               "key.code.codepoint for unicode/b/CTRL caretctrl");
  is_int(key.modifiers,   TERMKEY_KEYMOD_CTRL,  "key.modifiers for unicode/b/CTRL caretctrl");
  is_str(key.utf8,        "b",                  "key.utf8 for unicode/b/CTRL caretctrl");

  CLEAR_KEY;
  termkey_strpkey(tk, "A-c", &key, 0);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/c/ALT");
  is_int(key.code.codepoint, 'c',               "key.code.codepoint for unicode/c/ALT");
  is_int(key.modifiers,   TERMKEY_KEYMOD_ALT,   "key.modifiers for unicode/c/ALT");
  is_str(key.utf8,        "c",                  "key.utf8 for unicode/c/ALT");

  CLEAR_KEY;
  termkey_strpkey(tk, "Alt-c", &key, TERMKEY_FORMAT_LONGMOD);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/c/ALT longmod");
  is_int(key.code.codepoint, 'c',               "key.code.codepoint for unicode/c/ALT longmod");
  is_int(key.modifiers,   TERMKEY_KEYMOD_ALT,   "key.modifiers for unicode/c/ALT longmod");
  is_str(key.utf8,        "c",                  "key.utf8 for unicode/c/ALT longmod");

  CLEAR_KEY;
  termkey_strpkey(tk, "M-c", &key, TERMKEY_FORMAT_ALTISMETA);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/c/ALT altismeta");
  is_int(key.code.codepoint, 'c',               "key.code.codepoint for unicode/c/ALT altismeta");
  is_int(key.modifiers,   TERMKEY_KEYMOD_ALT,   "key.modifiers for unicode/c/ALT altismeta");
  is_str(key.utf8,        "c",                  "key.utf8 for unicode/c/ALT altismeta");

  CLEAR_KEY;
  termkey_strpkey(tk, "Meta-c", &key, TERMKEY_FORMAT_ALTISMETA|TERMKEY_FORMAT_LONGMOD);
  is_int(key.type,        TERMKEY_TYPE_UNICODE, "key.type for unicode/c/ALT altismeta+longmod");
  is_int(key.code.codepoint, 'c',               "key.code.codepoint for unicode/c/ALT altismeta+longmod");
  is_int(key.modifiers,   TERMKEY_KEYMOD_ALT,   "key.modifiers for unicode/c/ALT altismeta+longmod");
  is_str(key.utf8,        "c",                  "key.utf8 for unicode/c/ALT altismeta+longmod");

  CLEAR_KEY;
  termkey_strpkey(tk, "Up", &key, 0);
  is_int(key.type,        TERMKEY_TYPE_KEYSYM, "key.type for sym/Up/0");
  is_int(key.code.sym,    TERMKEY_SYM_UP,      "key.code.codepoint for sym/Up/0");
  is_int(key.modifiers,   0,                   "key.modifiers for sym/Up/0");

  CLEAR_KEY;
  termkey_strpkey(tk, "F5", &key, 0);
  is_int(key.type,        TERMKEY_TYPE_FUNCTION, "key.type for func/5/0");
  is_int(key.code.number, 5,                     "key.code.number for func/5/0");
  is_int(key.modifiers,   0,                     "key.modifiers for func/5/0");

  termkey_destroy(tk);

  return exit_status();
}