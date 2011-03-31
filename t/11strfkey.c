#include "../termkey.h"
#include "taplib.h"

int main(int argc, char *argv[])
{
  TermKey   *tk;
  TermKeyKey key;
  char       buffer[16];
  size_t     len;

  plan_tests(24);

  tk = termkey_new(0, TERMKEY_FLAG_NOTERMIOS);

  key.type = TERMKEY_TYPE_UNICODE;
  key.code.number = 'A';
  key.modifiers = 0;
  key.utf8[0] = 0;

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, 0);
  is_int(len, 1, "length for unicode/A/0");
  is_str(buffer, "A", "buffer for unicode/A/0");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_WRAPBRACKET);
  is_int(len, 1, "length for unicode/A/0 wrapbracket");
  is_str(buffer, "A", "buffer for unicode/A/0 wrapbracket");

  key.type = TERMKEY_TYPE_UNICODE;
  key.code.number = 'b';
  key.modifiers = TERMKEY_KEYMOD_CTRL;
  key.utf8[0] = 0;

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, 0);
  is_int(len, 3, "length for unicode/b/CTRL");
  is_str(buffer, "C-b", "buffer for unicode/b/CTRL");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_LONGMOD);
  is_int(len, 6, "length for unicode/b/CTRL longmod");
  is_str(buffer, "Ctrl-b", "buffer for unicode/b/CTRL longmod");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_CARETCTRL);
  is_int(len, 2, "length for unicode/b/CTRL caretctrl");
  is_str(buffer, "^B", "buffer for unicode/b/CTRL caretctrl");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_WRAPBRACKET);
  is_int(len, 5, "length for unicode/b/CTRL wrapbracket");
  is_str(buffer, "<C-b>", "buffer for unicode/b/CTRL wrapbracket");

  key.type = TERMKEY_TYPE_UNICODE;
  key.code.number = 'c';
  key.modifiers = TERMKEY_KEYMOD_ALT;
  key.utf8[0] = 0;

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, 0);
  is_int(len, 3, "length for unicode/c/ALT");
  is_str(buffer, "A-c", "buffer for unicode/c/ALT");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_LONGMOD);
  is_int(len, 5, "length for unicode/c/ALT longmod");
  is_str(buffer, "Alt-c", "buffer for unicode/c/ALT longmod");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_ALTISMETA);
  is_int(len, 3, "length for unicode/c/ALT altismeta");
  is_str(buffer, "M-c", "buffer for unicode/c/ALT altismeta");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_LONGMOD|TERMKEY_FORMAT_ALTISMETA);
  is_int(len, 6, "length for unicode/c/ALT longmod|altismeta");
  is_str(buffer, "Meta-c", "buffer for unicode/c/ALT longmod|altismeta");

  key.type = TERMKEY_TYPE_KEYSYM;
  key.code.sym = TERMKEY_SYM_UP;
  key.modifiers = 0;

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, 0);
  is_int(len, 2, "length for sym/Up/0");
  is_str(buffer, "Up", "buffer for sym/Up/0");

  len = termkey_strfkey(tk, buffer, sizeof buffer, &key, TERMKEY_FORMAT_WRAPBRACKET);
  is_int(len, 4, "length for sym/Up/0 wrapbracket");
  is_str(buffer, "<Up>", "buffer for sym/Up/0 wrapbracket");

  termkey_destroy(tk);

  return exit_status();
}
