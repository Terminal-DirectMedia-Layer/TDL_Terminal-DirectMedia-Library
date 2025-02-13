
#ifndef INPUTEVENTCODE_HPP
    #define INPUTEVENTCODE_HPP

    #include <map>
	#include <linux/input-event-codes.h>
	#include <linux/input.h>

    #define TDL_KEY_RESERVED "RESERVED"
    #define TDL_KEY_ESC "Escape"
    #define TDL_KEY_AMPSAND "ampersand"
    #define TDL_KEY_EACUTE "eacute"
    #define TDL_KEY_QUOTEDOUBLE "quotedbl"
    #define TDL_KEY_QUOTE "apostrophe"
    #define TDL_KEY_LEFTPAREN "parenleft"
    #define TDL_KEY_MINUS "minus"
    #define TDL_KEY_EGRAVE "egrave"
    #define TDL_KEY_UNDERSCORE "underscore"
    #define TDL_KEY_CCEDILLA "ccedilla"
    #define TDL_KEY_AGRAVE "agrave"
    #define TDL_KEY_RIGHTPAREN "parenright"
    #define TDL_KEY_EQUAL "equal"
    #define TDL_KEY_DELETE "Delete"
    #define TDL_KEY_BACKSLASH "Meta_backslash"
    #define TDL_KEY_TAB "Tab"
    #define TDL_KEY_A "a"
    #define TDL_KEY_Z "z"
    #define TDL_KEY_E "e"
    #define TDL_KEY_R "r"
    #define TDL_KEY_T "t"
    #define TDL_KEY_Y "y"
    #define TDL_KEY_U "u"
    #define TDL_KEY_I "i"
    #define TDL_KEY_O "o"
    #define TDL_KEY_P "p"
    #define TDL_KEY_CIRCUMFLEX "dead_circumflex"
    #define TDL_KEY_DOLLAR "dollar"
    #define TDL_KEY_RETURN "Return"
    #define TDL_KEY_CONTROL "Control"
    #define TDL_KEY_Q "q"
    #define TDL_KEY_S "s"
    #define TDL_KEY_D "d"
    #define TDL_KEY_F "f"
    #define TDL_KEY_G "g"
    #define TDL_KEY_H "h"
    #define TDL_KEY_J "j"
    #define TDL_KEY_K "k"
    #define TDL_KEY_L "l"
    #define TDL_KEY_M "m"
    #define TDL_KEY_UGRAVE "ugrave"
    #define TDL_KEY_TWOSUPERIOR "twosuperior"
    #define TDL_KEY_CAPSSHIFT "CapsShift"
    #define TDL_KEY_SHIFT "Shift"
    #define TDL_KEY_ASTERISK "asterisk"
    #define TDL_KEY_W "w"
    #define TDL_KEY_X "x"
    #define TDL_KEY_C "c"
    #define TDL_KEY_V "v"
    #define TDL_KEY_B "b"
    #define TDL_KEY_N "n"
    #define TDL_KEY_COMMA "comma"
    #define TDL_KEY_SEMICOLON "semicolon"
    #define TDL_KEY_COLON "colon"
    #define TDL_KEY_EXCLAM "exclam"
    #define TDL_KEY_ASTERSIK "asterisk"
    #define TDL_KEY_ALT "Alt"
    #define TDL_KEY_SPACE "space"
    #define TDL_KEY_F1 "F1"
    #define TDL_KEY_F2 "F2"
    #define TDL_KEY_F3 "F3"
    #define TDL_KEY_F4 "F4"
    #define TDL_KEY_F5 "F5"
    #define TDL_KEY_F6 "F6"
    #define TDL_KEY_F7 "F7"
    #define TDL_KEY_F8 "F8"
    #define TDL_KEY_F9 "F9"
    #define TDL_KEY_F10 "F10"
    #define TDL_KEY_NUMLOCK "Num_Lock"
    #define TDL_KEY_SCROLLLOCK "Scroll_Lock"
    #define TDL_KEY_SEVEN "seven"
    #define TDL_KEY_EIGHT "eight"
    #define TDL_KEY_NINE "nine"
    #define TDL_KEY_MINUS "minus"
    #define TDL_KEY_FOUR "four"
    #define TDL_KEY_FIVE "five"
    #define TDL_KEY_SIX "six"
    #define TDL_KEY_PLUS "plus"
    #define TDL_KEY_ONE "one"
    #define TDL_KEY_TWO "two"
    #define TDL_KEY_THREE "three"
    #define TDL_KEY_ZERO "zero"
    #define TDL_KEY_PERIOD "period"
    #define TDL_KEY_SAK "SAK"
    #define TDL_KEY_F11 "F11"
    #define TDL_KEY_F12 "F12"
    #define TDL_KEY_CONTROL "Control"
    #define TDL_KEY_SLASH "slash"
    #define TDL_KEY_ALTGR "AltGr"
    #define TDL_KEY_HOME "Home"
    #define TDL_KEY_UP "Up"
    #define TDL_KEY_PAGEUP "PageUp"
    #define TDL_KEY_LEFT "Left"
    #define TDL_KEY_RIGHT "Right"
    #define TDL_KEY_END "End"
    #define TDL_KEY_DOWN "Down"
    #define TDL_KEY_PAGEDOWN "PageDown"
    #define TDL_KEY_INSERT "Insert"
    #define TDL_KEY_F13 "F13"
    #define TDL_KEY_F14 "F14"

// here is the event type reference
	#define TDL_NONE -1
	#define TDL_RESERVED 0
	#define TDL_KEYPRESSED 1
	#define TDL_KEYRELEASED 2
	#define TDL_KEYREPEAT 3
	#define TDL_MOUSEMOVED 4
	#define TDL_MOUSEPRESSED 5
	#define TDL_MOUSERELEASED 6
	#define TDL_MOUSEWHEEL 7
	#define TDL_WINDOWRESIZED 8
	#define TDL_CUSTOM 9


	#define TDL_MOUSELEFT 0
	#define TDL_MOUSEMIDDLE 1
	#define TDL_MOUSERIGHT 2
	#define TDL_WHEELUP 3
	#define TDL_WHEELDOWN 4


    const std::map<__u16, const char *> keys = {
        {KEY_RESERVED, NULL},       {KEY_ESC, NULL},
        {KEY_1, "1"},               {KEY_2, "2"},
        {KEY_3, "3"},               {KEY_4, "4"},
        {KEY_5, "5"},               {KEY_6, "6"},
        {KEY_7, "7"},               {KEY_8, "8"},
        {KEY_9, "9"},               {KEY_0, "0"},
        {KEY_MINUS, "-"},           {KEY_EQUAL, "="},
        {KEY_BACKSPACE, NULL},      {KEY_TAB, "  "},
        {KEY_Q, "q"},               {KEY_W, "w"},
        {KEY_E, "e"},               {KEY_R, "r"},
        {KEY_T, "t"},               {KEY_Y, "y"},
        {KEY_U, "u"},               {KEY_I, "i"},
        {KEY_O, "o"},               {KEY_P, "p"},
        {KEY_LEFTBRACE, "["},       {KEY_RIGHTBRACE, "]"},
        {KEY_ENTER, "\n"},          {KEY_LEFTCTRL, NULL},
        {KEY_A, "a"},               {KEY_S, "s"},
        {KEY_D, "d"},               {KEY_F, "f"},
        {KEY_G, "g"},               {KEY_H, "h"},
        {KEY_J, "j"},               {KEY_K, "k"},
        {KEY_L, "l"},               {KEY_SEMICOLON, ";"},
        {KEY_APOSTROPHE, "'"},      {KEY_GRAVE, "`"},
        {KEY_LEFTSHIFT, NULL},      {KEY_BACKSLASH, "\\"},
        {KEY_Z, "z"},               {KEY_X, "x"},
        {KEY_C, "c"},               {KEY_V, "v"},
        {KEY_B, "b"},               {KEY_N, "n"},
        {KEY_M, "m"},               {KEY_COMMA, ","},
        {KEY_DOT, "."},             {KEY_SLASH, "/"},
        {KEY_RIGHTSHIFT, NULL},     {KEY_KPASTERISK, "*"},
        {KEY_LEFTALT, NULL},        {KEY_SPACE, " "},
        {KEY_CAPSLOCK, NULL},       {KEY_F1, NULL},
        {KEY_F2, NULL},             {KEY_F3, NULL},
        {KEY_F4, NULL},             {KEY_F5, NULL},
        {KEY_F6, NULL},             {KEY_F7, NULL},
        {KEY_F8, NULL},             {KEY_F9, NULL},
        {KEY_F10, NULL},            {KEY_NUMLOCK, NULL},
        {KEY_SCROLLLOCK, NULL},     {KEY_KP7, "7"},
        {KEY_KP8, "8"},             {KEY_KP9, "9"},
        {KEY_KPMINUS, "-"},         {KEY_KP4, "4"},
        {KEY_KP5, "5"},             {KEY_KP6, "6"},
        {KEY_KPPLUS, "+"},          {KEY_KP1, "1"},
        {KEY_KP2, "2"},             {KEY_KP3, "3"},
        {KEY_KP0, "0"},             {KEY_KPDOT, "."},
        {KEY_ZENKAKUHANKAKU, NULL}, {KEY_102ND, NULL},
        {KEY_F11, NULL},            {KEY_F12, NULL},
        {KEY_RO, NULL},             {KEY_KATAKANA, NULL},
        {KEY_HIRAGANA, NULL},       {KEY_HENKAN, NULL},
        {KEY_KATAKANAHIRAGANA, NULL},{KEY_MUHENKAN, NULL},
        {KEY_KPJPCOMMA, NULL},      {KEY_KPENTER, NULL},
        {KEY_RIGHTCTRL, NULL},      {KEY_KPSLASH, "/"},
        {KEY_SYSRQ, NULL},          {KEY_RIGHTALT, NULL},
        {KEY_LINEFEED, NULL},       {KEY_HOME, NULL},
        {KEY_UP, NULL},             {KEY_PAGEUP, NULL},
        {KEY_LEFT, NULL},           {KEY_RIGHT, NULL},
        {KEY_END, NULL},            {KEY_DOWN, NULL},
        {KEY_PAGEDOWN, NULL},       {KEY_INSERT, NULL},
        {KEY_DELETE, NULL},         {KEY_MACRO, NULL},
        {KEY_MUTE, NULL},           {KEY_VOLUMEDOWN, NULL},
    };

#endif //INPUTEVENTCODE_HPP
