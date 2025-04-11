#ifndef KEY_H
#define KEY_H
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

#define KEY_UP    1001
#define KEY_DOWN  1002
#define KEY_LEFT  1003
#define KEY_RIGHT 1004
#define KEY_Q     113
 
class Keyboard_ctrl {
  struct termios initial_settings, new_settings;
  int peek_character = -1;
 
 public:
  Keyboard_ctrl() { init_keyboard(); };
  ~Keyboard_ctrl() { close_keyboard(); };
 
  int get_keyboard_press_key() {
    kbhit();
    return readch();
    // printf("%02x \n", readch());
  };
 
 private:
  void init_keyboard() {
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VEOL] = 1;
    new_settings.c_cc[VEOF] = 2;
    tcsetattr(0, TCSANOW, &new_settings);
  }
 
  void close_keyboard() { tcsetattr(0, TCSANOW, &initial_settings); }
 
  int kbhit() {
    unsigned char ch;
    int nread;
 
    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    if (nread == 1) {
      peek_character = ch;
      return 1;
    }
    return 0;
  }
    
  int readch() {
    char ch;
    read(0, &ch, 1);

    if (ch == 0x1B) {  // 检测 ESC 开头的序列
      char seq[2];
      if (read(0, &seq[0], 1) == 0) return 0x1B;
      if (read(0, &seq[1], 1) == 0) return 0x1B;

      if (seq[0] == '[') {
        switch (seq[1]) {
          case 'A': return KEY_UP;      // Up
          case 'B': return KEY_DOWN;    // Down
          case 'C': return KEY_RIGHT;   // Right
          case 'D': return KEY_LEFT;    // Left
        }
      }
    }
    return ch;
  }
};
 
#endif  // KEY_H
 
