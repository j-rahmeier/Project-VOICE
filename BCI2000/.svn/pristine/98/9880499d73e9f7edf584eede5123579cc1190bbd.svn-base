typedef enum
{
    SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    SCANCODE_A = 4,
    SCANCODE_B = 5,
    SCANCODE_C = 6,
    SCANCODE_D = 7,
    SCANCODE_E = 8,
    SCANCODE_F = 9,
    SCANCODE_G = 10,
    SCANCODE_H = 11,
    SCANCODE_I = 12,
    SCANCODE_J = 13,
    SCANCODE_K = 14,
    SCANCODE_L = 15,
    SCANCODE_M = 16,
    SCANCODE_N = 17,
    SCANCODE_O = 18,
    SCANCODE_P = 19,
    SCANCODE_Q = 20,
    SCANCODE_R = 21,
    SCANCODE_S = 22,
    SCANCODE_T = 23,
    SCANCODE_U = 24,
    SCANCODE_V = 25,
    SCANCODE_W = 26,
    SCANCODE_X = 27,
    SCANCODE_Y = 28,
    SCANCODE_Z = 29,

    SCANCODE_1 = 30,
    SCANCODE_2 = 31,
    SCANCODE_3 = 32,
    SCANCODE_4 = 33,
    SCANCODE_5 = 34,
    SCANCODE_6 = 35,
    SCANCODE_7 = 36,
    SCANCODE_8 = 37,
    SCANCODE_9 = 38,
    SCANCODE_0 = 39,

    SCANCODE_RETURN = 40,
    SCANCODE_ESCAPE = 41,
    SCANCODE_BACKSPACE = 42,
    SCANCODE_TAB = 43,
    SCANCODE_SPACE = 44,

    SCANCODE_MINUS = 45,
    SCANCODE_EQUALS = 46,
    SCANCODE_LEFTBRACKET = 47,
    SCANCODE_RIGHTBRACKET = 48,
    SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    SCANCODE_SEMICOLON = 51,
    SCANCODE_APOSTROPHE = 52,
    SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    SCANCODE_COMMA = 54,
    SCANCODE_PERIOD = 55,
    SCANCODE_SLASH = 56,

    SCANCODE_CAPSLOCK = 57,

    SCANCODE_F1 = 58,
    SCANCODE_F2 = 59,
    SCANCODE_F3 = 60,
    SCANCODE_F4 = 61,
    SCANCODE_F5 = 62,
    SCANCODE_F6 = 63,
    SCANCODE_F7 = 64,
    SCANCODE_F8 = 65,
    SCANCODE_F9 = 66,
    SCANCODE_F10 = 67,
    SCANCODE_F11 = 68,
    SCANCODE_F12 = 69,

    SCANCODE_PRINTSCREEN = 70,
    SCANCODE_SCROLLLOCK = 71,
    SCANCODE_PAUSE = 72,
    SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    SCANCODE_HOME = 74,
    SCANCODE_PAGEUP = 75,
    SCANCODE_DELETE = 76,
    SCANCODE_END = 77,
    SCANCODE_PAGEDOWN = 78,
    SCANCODE_RIGHT = 79,
    SCANCODE_LEFT = 80,
    SCANCODE_DOWN = 81,
    SCANCODE_UP = 82,

    SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    SCANCODE_KP_DIVIDE = 84,
    SCANCODE_KP_MULTIPLY = 85,
    SCANCODE_KP_MINUS = 86,
    SCANCODE_KP_PLUS = 87,
    SCANCODE_KP_ENTER = 88,
    SCANCODE_KP_1 = 89,
    SCANCODE_KP_2 = 90,
    SCANCODE_KP_3 = 91,
    SCANCODE_KP_4 = 92,
    SCANCODE_KP_5 = 93,
    SCANCODE_KP_6 = 94,
    SCANCODE_KP_7 = 95,
    SCANCODE_KP_8 = 96,
    SCANCODE_KP_9 = 97,
    SCANCODE_KP_0 = 98,
    SCANCODE_KP_PERIOD = 99,

    SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    SCANCODE_KP_EQUALS = 103,
    SCANCODE_F13 = 104,
    SCANCODE_F14 = 105,
    SCANCODE_F15 = 106,
    SCANCODE_F16 = 107,
    SCANCODE_F17 = 108,
    SCANCODE_F18 = 109,
    SCANCODE_F19 = 110,
    SCANCODE_F20 = 111,
    SCANCODE_F21 = 112,
    SCANCODE_F22 = 113,
    SCANCODE_F23 = 114,
    SCANCODE_F24 = 115,
    SCANCODE_EXECUTE = 116,
    SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
    SCANCODE_MENU = 118,    /**< Menu (show menu) */
    SCANCODE_SELECT = 119,
    SCANCODE_STOP = 120,    /**< AC Stop */
    SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
    SCANCODE_UNDO = 122,    /**< AC Undo */
    SCANCODE_CUT = 123,     /**< AC Cut */
    SCANCODE_COPY = 124,    /**< AC Copy */
    SCANCODE_PASTE = 125,   /**< AC Paste */
    SCANCODE_FIND = 126,    /**< AC Find */
    SCANCODE_MUTE = 127,
    SCANCODE_VOLUMEUP = 128,
    SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     SCANCODE_LOCKINGNUMLOCK = 131, */
/*     SCANCODE_LOCKINGSCROLLLOCK = 132, */
    SCANCODE_KP_COMMA = 133,
    SCANCODE_KP_EQUALSAS400 = 134,

    SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SCANCODE_INTERNATIONAL2 = 136,
    SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SCANCODE_INTERNATIONAL4 = 138,
    SCANCODE_INTERNATIONAL5 = 139,
    SCANCODE_INTERNATIONAL6 = 140,
    SCANCODE_INTERNATIONAL7 = 141,
    SCANCODE_INTERNATIONAL8 = 142,
    SCANCODE_INTERNATIONAL9 = 143,
    SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    SCANCODE_LANG2 = 145, /**< Hanja conversion */
    SCANCODE_LANG3 = 146, /**< Katakana */
    SCANCODE_LANG4 = 147, /**< Hiragana */
    SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    SCANCODE_LANG6 = 149, /**< reserved */
    SCANCODE_LANG7 = 150, /**< reserved */
    SCANCODE_LANG8 = 151, /**< reserved */
    SCANCODE_LANG9 = 152, /**< reserved */

    SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
    SCANCODE_SYSREQ = 154,
    SCANCODE_CANCEL = 155,      /**< AC Cancel */
    SCANCODE_CLEAR = 156,
    SCANCODE_PRIOR = 157,
    SCANCODE_RETURN2 = 158,
    SCANCODE_SEPARATOR = 159,
    SCANCODE_OUT = 160,
    SCANCODE_OPER = 161,
    SCANCODE_CLEARAGAIN = 162,
    SCANCODE_CRSEL = 163,
    SCANCODE_EXSEL = 164,

    SCANCODE_KP_00 = 176,
    SCANCODE_KP_000 = 177,
    SCANCODE_THOUSANDSSEPARATOR = 178,
    SCANCODE_DECIMALSEPARATOR = 179,
    SCANCODE_CURRENCYUNIT = 180,
    SCANCODE_CURRENCYSUBUNIT = 181,
    SCANCODE_KP_LEFTPAREN = 182,
    SCANCODE_KP_RIGHTPAREN = 183,
    SCANCODE_KP_LEFTBRACE = 184,
    SCANCODE_KP_RIGHTBRACE = 185,
    SCANCODE_KP_TAB = 186,
    SCANCODE_KP_BACKSPACE = 187,
    SCANCODE_KP_A = 188,
    SCANCODE_KP_B = 189,
    SCANCODE_KP_C = 190,
    SCANCODE_KP_D = 191,
    SCANCODE_KP_E = 192,
    SCANCODE_KP_F = 193,
    SCANCODE_KP_XOR = 194,
    SCANCODE_KP_POWER = 195,
    SCANCODE_KP_PERCENT = 196,
    SCANCODE_KP_LESS = 197,
    SCANCODE_KP_GREATER = 198,
    SCANCODE_KP_AMPERSAND = 199,
    SCANCODE_KP_DBLAMPERSAND = 200,
    SCANCODE_KP_VERTICALBAR = 201,
    SCANCODE_KP_DBLVERTICALBAR = 202,
    SCANCODE_KP_COLON = 203,
    SCANCODE_KP_HASH = 204,
    SCANCODE_KP_SPACE = 205,
    SCANCODE_KP_AT = 206,
    SCANCODE_KP_EXCLAM = 207,
    SCANCODE_KP_MEMSTORE = 208,
    SCANCODE_KP_MEMRECALL = 209,
    SCANCODE_KP_MEMCLEAR = 210,
    SCANCODE_KP_MEMADD = 211,
    SCANCODE_KP_MEMSUBTRACT = 212,
    SCANCODE_KP_MEMMULTIPLY = 213,
    SCANCODE_KP_MEMDIVIDE = 214,
    SCANCODE_KP_PLUSMINUS = 215,
    SCANCODE_KP_CLEAR = 216,
    SCANCODE_KP_CLEARENTRY = 217,
    SCANCODE_KP_BINARY = 218,
    SCANCODE_KP_OCTAL = 219,
    SCANCODE_KP_DECIMAL = 220,
    SCANCODE_KP_HEXADECIMAL = 221,

    SCANCODE_LCTRL = 224,
    SCANCODE_LSHIFT = 225,
    SCANCODE_LALT = 226, /**< alt, option */
    SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    SCANCODE_RCTRL = 228,
    SCANCODE_RSHIFT = 229,
    SCANCODE_RALT = 230, /**< alt gr, option */
    SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    SCANCODE_AUDIONEXT = 258,
    SCANCODE_AUDIOPREV = 259,
    SCANCODE_AUDIOSTOP = 260,
    SCANCODE_AUDIOPLAY = 261,
    SCANCODE_AUDIOMUTE = 262,
    SCANCODE_MEDIASELECT = 263,
    SCANCODE_WWW = 264,             /**< AL Internet Browser */
    SCANCODE_MAIL = 265,
    SCANCODE_CALCULATOR = 266,      /**< AL Calculator */
    SCANCODE_COMPUTER = 267,
    SCANCODE_AC_SEARCH = 268,       /**< AC Search */
    SCANCODE_AC_HOME = 269,         /**< AC Home */
    SCANCODE_AC_BACK = 270,         /**< AC Back */
    SCANCODE_AC_FORWARD = 271,      /**< AC Forward */
    SCANCODE_AC_STOP = 272,         /**< AC Stop */
    SCANCODE_AC_REFRESH = 273,      /**< AC Refresh */
    SCANCODE_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    SCANCODE_BRIGHTNESSDOWN = 275,
    SCANCODE_BRIGHTNESSUP = 276,
    SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    SCANCODE_KBDILLUMTOGGLE = 278,
    SCANCODE_KBDILLUMDOWN = 279,
    SCANCODE_KBDILLUMUP = 280,
    SCANCODE_EJECT = 281,
    SCANCODE_SLEEP = 282,           /**< SC System Sleep */

    SCANCODE_APP1 = 283,
    SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    SCANCODE_AUDIOREWIND = 285,
    SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
    SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} Scancode;




#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */


/*
 * 0x07 : unassigned
 */

#define KeyBackSpace           0x08
#define KeyTab            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VK_CLEAR          0x0C
#define KeyReturn         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define KeyBreak          0x13
#define KeyCapsLock        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define KeyEscape         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define KeySpace          0x20
#define KeyPageUp         0x21
#define KeyPageDown       0x22
#define KeyEnd            0x23
#define KeyHome           0x24
#define KeyLeft           0x25
#define KeyUp             0x26
#define KeyRight          0x27
#define KeyDown           0x28
#define VK_SELECT         0x29
#define KeyPrint          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define KeyInsert         0x2D
#define KeyDelete         0x2E
#define VK_HELP           0x2F

#define Key0 '0'
#define Key1 '1'
#define Key2 '2'
#define Key3 '3'
#define Key4 '4'
#define Key5 '5'
#define Key6 '6'
#define Key7 '7'
#define Key8 '8'
#define Key9 '9'
/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define KeySuperL           0x5B
#define KeySuperR           0x5C
#define VK_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VK_SLEEP          0x5F

#define KeyKpInsert        0x60
#define KeyKpEnd        0x61
#define KeyKpDown        0x62
#define KeyKpPageDown        0x63
#define KeyKpLeft        0x64
#define KeyKpBegin        0x65
#define KeyKpRight        0x66
#define KeyKpHome        0x67
#define KeyKpUp        0x68
#define KeyKpPageUp        0x69
#define KeyKpMultiply     0x6A
#define KeyKpAdd          0x6B
#define KeyKpEnter      0x6C
#define KeyKpSubtract       0x6D
#define KeyKpDelete        0x6E
#define KeyKpDivide       0x6F
#define KeyF1             0x70
#define KeyF2             0x71
#define KeyF3             0x72
#define KeyF4             0x73
#define KeyF5             0x74
#define KeyF6             0x75
#define KeyF7             0x76
#define KeyF8             0x77
#define KeyF9             0x78
#define KeyF10            0x79
#define KeyF11            0x7A
#define KeyF12            0x7B
#define KeyF13            0x7C
#define KeyF14            0x7D
#define KeyF15            0x7E
#define KeyF16            0x7F
#define KeyF17            0x80
#define KeyF18            0x81
#define KeyF19            0x82
#define KeyF20            0x83
#define KeyF21            0x84
#define KeyF22            0x85
#define KeyF23            0x86
#define KeyF24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define KeyNumLock        0x90
#define KeyScrollLock         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define KeyKpEqual  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define KeyShiftL         0xA0
#define KeyShiftR         0xA1
#define KeyCtrlL       0xA2
#define KeyCtrlR       0xA3
#define KeyAltL          0xA4
#define KeyAltR          0xA5

/*
 * 0xB8 - 0xB9 : reserved
 */

#define KeySemicolon          0xBA   // ';:' for US
#define KeyEqual       0xBB   // '+' any country
#define KeyComma      0xBC   // ',' any country
#define KeyMinus      0xBD   // '-' any country
#define KeyPeriod     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define KeyGrave          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define KeyBracketLeft          0xDB  //  '[{' for US
#define KeyBackslash          0xDC  //  '\|' for US
#define KeyBracketRight          0xDD  //  ']}' for US
#define KeyApostrophe          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO

#define VK_ICO_CLEAR      0xE6



/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE

