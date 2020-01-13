#ifndef FL_CONST_H_
#define FL_CONST_H_

#define VER "1.0 (64bit)"

enum error_codes
{ TIMEOUT = -1, FILEERROR_O = -2, FILEERROR_W = -3, FILEERROR_R =
    -4, SEND_ERROR = -5,
  BAD_PACKET = -6, BAD_PARAMS = -7, PORT_ERROR = -8, WRONG_SIZE = -9
};

enum cchars
{ ACK = 0xAA, NAK = 0xF0, END = 0x0F, DATA = 0x55 };

#endif
