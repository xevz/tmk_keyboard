/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "keycode.h"
#include "action.h"
#include "action_util.h"
#include "action_code.h"
#include "action_macro.h"
#include "action_layer.h"
#include "bootloader.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"


/* ErgoDox keymap definition macro */
#define KEYMAP(                                                                                                 \
                                                                                                                \
    /* left hand, spatial positions */      /* right hand, spatial positions */                                 \
    k00,k01,k02,k03,k04,k05,                        k06,k07,k08,k09,k0A,k0B,                                    \
    k10,k11,k12,k13,k14,k15,                        k16,k17,k18,k19,k1A,k1B,                                    \
    k20,k21,k22,k23,k24,k25,                        k26,k27,k28,k29,k2A,k2B,                                    \
    k30,k31,k32,k33,k34,k35,                        k36,k37,k38,k39,k3A,k3B,                                    \
    k40,k41,k42,k43,k44,                                k48,k47,k4A,k49,k4B,                                    \
                            k52,k62,        k59,k68,                                                            \
                                k63,        k58,                                                                \
                        k53,k50,k61,        k56,k67,k66 )                                                       \
                                                                                                                \
                                                                                                                \
   /* matrix positions, k40 & k4B unused for now, k45 & k46 are untraced on plate */                            \
   /* pins */                                                                                                   \
   /* 0: D0    1: D1    2: D2    3: D3    4: D4      5: F0    6: F1    7: F4    8: F5    9: F6      A: F7    B: D5      C: C6    D: C7       */            \
   {                                                                                                                                                       \
    { KC_##k00,KC_##k10,KC_##k20,KC_##k30,KC_NO   ,  KC_##k06,KC_##k16,KC_##k26,KC_##k36,KC_##k48,  KC_##k50,KC_NO   ,  KC_##k56,KC_##k66},  /* 0: B0 */   \
    { KC_##k01,KC_##k11,KC_##k21,KC_##k31,KC_##k41,  KC_##k08,KC_##k18,KC_##k28,KC_##k38,KC_##k47,  KC_NO   ,KC_##k61,  KC_NO   ,KC_##k67},  /* 1: B1 */   \
    { KC_##k02,KC_##k12,KC_##k22,KC_##k32,KC_##k42,  KC_##k07,KC_##k17,KC_##k27,KC_##k37,KC_NO   ,  KC_##k52,KC_##k62,  KC_##k58,KC_##k68},  /* 2: B2 */   \
    { KC_##k03,KC_##k13,KC_##k23,KC_##k33,KC_##k43,  KC_##k0B,KC_##k1B,KC_##k2B,KC_##k3B,KC_NO   ,  KC_##k53,KC_##k63,  KC_##k59,KC_NO   },  /* 3: B3 */   \
    { KC_##k04,KC_##k14,KC_##k24,KC_##k34,KC_NO   ,  KC_##k09,KC_##k19,KC_##k29,KC_##k39,KC_##k4A,  KC_NO   ,KC_NO   ,  KC_NO   ,KC_NO   },  /* 4: B4 */   \
    { KC_##k05,KC_##k15,KC_##k25,KC_##k35,KC_##k44,  KC_##k0A,KC_##k1A,KC_##k2A,KC_##k3A,KC_##k49,  KC_NO   ,KC_NO   ,  KC_NO   ,KC_NO   },  /* 5: B5 */   \
   }



static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(  // layer 0 : default
        GRV ,1   ,2   ,3   ,4   ,5   ,               FN23,7   ,8   ,9   ,0   ,EQL ,
        FN2 ,Q   ,W   ,E   ,R   ,FN1 ,               FN24,U   ,I   ,O   ,P   ,LBRC,
        FN11,FN28,FN29,FN30,FN31,FN3 ,               FN25,J   ,K   ,L   ,SCLN,FN15,
        FN12,Z   ,X   ,C   ,FN27,FN4 ,               FN26,M   ,COMM,DOT ,SLSH,FN16,
        FN21,FN20,CAPS,FN13,FN14,                         LEFT,UP  ,DOWN,RGHT,FN22,
                                 FN17,FN19,     PGUP,LGUI,
                                      NO  ,     PGDN,
                            FN5 ,FN6 ,FN7 ,     FN8 ,FN9 ,FN10
    ),

    KEYMAP(  // Layer1: Workman layout, leftled:all
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,Q   ,D   ,R   ,W   ,B   ,               J   ,F   ,U   ,P   ,4   ,TRNS,
        TRNS,A   ,S   ,H   ,T   ,G   ,               Y   ,N   ,E   ,O   ,I   ,TRNS,
        TRNS,Z   ,X   ,M   ,C   ,V   ,               K   ,L   ,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,FN17,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    KEYMAP(  // Layer2: numpad, leftled:mid/blue
        // left hand
        TRNS,NO  ,NO  ,NO  ,NO  ,PAUS,               NLCK,PSLS,PAST,PAST,PMNS,BSPC,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,P7  ,P8  ,P9  ,PMNS,PGUP,
        TRNS,NO  ,NO  ,NO  ,TRNS,NO  ,               NO  ,P4  ,P5  ,P6  ,PPLS,PGDN,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,P1  ,P2  ,P3  ,PPLS,PENT,
        TRNS,TRNS,FN17,TRNS,TRNS,                         P0  ,PDOT,SLSH,PENT,PENT,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    KEYMAP(  // Layer3: F-keys + PgUp/PgDn on right hand, leftled:bot/green
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,NO  ,NO  ,NO  ,NO  ,TRNS,               TRNS,F1  ,F2  ,F3  ,F4  ,PGUP,
        TRNS,NO  ,TRNS,NO  ,NO  ,TRNS,               TRNS,F5  ,F6  ,F7  ,F8  ,PGDN,
        TRNS,NO  ,NO  ,NO  ,NO  ,TRNS,               TRNS,F9  ,F10 ,F11 ,F12 ,APP ,
        TRNS,TRNS,TRNS,LALT,LGUI,                         RGUI,RALT,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            LCTL,LSFT,TRNS,     TRNS,RSFT,RCTL
    ),

    KEYMAP(  // Layer4: unconvenient keys on right hand, leftled:top/white
                                                    /* in Workman right hand will be:
                                                            { } ( ) +
                                                          ^ ! ?     =
                                                          ' ! $ " ; \
                                                          # [ < > ] \         */
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,4   ,5   ,9   ,0   ,PPLS,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               MINS,2   ,FN5 ,9   ,0   ,EQL ,
        TRNS,TRNS,NO  ,NO  ,NO  ,NO  ,               BSLS,2   ,P   ,FN1 ,1   ,FN2 ,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               3   ,6   ,FN3 ,FN4 ,7   ,FN2 ,
        TRNS,TRNS,TRNS,LALT,LGUI,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    KEYMAP(  // Layer5: F-keys instead of numbers, leftled:top/white
        TRNS,F1  ,F2  ,F3  ,F4  ,F5  ,               F6  ,F7  ,F8  ,F9  ,F10 ,TRNS,
        TRNS,P1  ,P2  ,P3  ,P4  ,TRNS,               P6  ,P7  ,P8  ,P9  ,P0  ,TRNS,
        TRNS,TRNS,TRNS,E   ,TRNS,TRNS,               TRNS,U   ,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    KEYMAP(  // Layer6: F-keys + utils(Teensy, Workman-layer switch), leftled:top/white+onboard
        FN1 ,F1  ,F2  ,F3  ,F4  ,F5  ,               F6  ,F7  ,F8  ,F9  ,F10 ,FN0 ,
        TRNS,P1  ,P2  ,P3  ,P4  ,P5  ,               TRNS,P7  ,P8  ,P9  ,P0  ,TRNS,
        TRNS,TRNS,TRNS,E   ,TRNS,TRNS,               TRNS,U   ,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      SLEP,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    //
    // rarely used
    //

    KEYMAP(  // Layer7: F-keys only, leftled:top/white
        FN1 ,NO  ,NO  ,NO  ,NO  ,NO  ,               TRNS,NO  ,NO  ,NO  ,NO  ,FN0 ,
        FN2 ,F13 ,F14 ,F15 ,F16 ,TRNS,               TRNS,F1  ,F2  ,F3  ,F4  ,TRNS,
        TRNS,F17 ,F18 ,F19 ,F20 ,TRNS,               TRNS,F5  ,F6  ,F7  ,F8  ,TRNS,
        TRNS,F21 ,F22 ,F23 ,F24 ,TRNS,               TRNS,F9  ,F10 ,F11 ,F12 ,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     SLEP,TRNS,TRNS
    ),

    KEYMAP(  // Layer8: mouse and navigation, leftled:mid/blue+bot/green
        // left hand
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               MPLY,MPRV,MNXT,VOLD,VOLU,MUTE,
        TRNS,NO  ,NO  ,NO  ,ACL0,NO  ,               BTN2,WH_L,WH_U,WH_D,WH_R,PGUP,
        TRNS,NO  ,NO  ,TRNS,ACL1,NO  ,               BTN1,MS_L,MS_U,MS_D,MS_R,PGDN,
        TRNS,NO  ,NO  ,NO  ,ACL2,NO  ,               BTN3,HOME,END ,DEL ,INS ,NO  ,
        TRNS,TRNS,TRNS,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),

    KEYMAP(  // Layer9: application-specific shortcuts (mostly browser), leftled:top/white+bot/green
        // left hand
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,              NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,              NO  ,FN12,FN13,FN14,FN15,FN10,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,              FN1 ,FN2 ,FN3 ,FN4 ,FN5 ,FN11,
        TRNS,NO  ,NO  ,NO  ,TRNS,NO  ,              TRNS,FN6 ,FN7 ,FN8 ,FN9 ,FN0 ,
        TRNS,TRNS,TRNS,TRNS,TRNS,                        TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,    TRNS,TRNS,
                                      TRNS,    TRNS,
                            TRNS,TRNS,TRNS,    TRNS,TRNS,TRNS
    ),

/*
    // templates to copy from

    KEYMAP(  // LayerN: transparent on edges + hard-defined thumb keys, all others are empty
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,               NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,
        TRNS,TRNS,TRNS,LALT,LGUI,                         RGUI,RALT,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            LCTL,LSFT,TRNS,     TRNS,RSFT,RCTL
    ),
    KEYMAP(  // LayerN: fully transparent
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,               TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,                         TRNS,TRNS,TRNS,TRNS,TRNS,
                                 TRNS,TRNS,     TRNS,TRNS,
                                      TRNS,     TRNS,
                            TRNS,TRNS,TRNS,     TRNS,TRNS,TRNS
    ),
*/

};

/* id for user defined functions & macros */
enum function_id {
    TEENSY_KEY,
    CUSTOM_KEY,
    L_CTRL_ALT_ENT,
    R_CTRL_ALT_ENT,
    L_CTRL_ALT_T,
    R_CTRL_ALT_Y,
};

enum macro_id {
    XMONAD_RESET,
    PASSWORD1,
    PASSWORD2,
    PASSWORD3,
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
    [0]  =  ACTION_FUNCTION(TEENSY_KEY),                    // FN0  - Teensy key

    // unused: [1]  =  ACTION_MODS_KEY(MOD_LSFT, KC_BSLS),  // FN1  = Shifted BackSlash // " in Workman
    [2]  =  ACTION_MODS_KEY(MOD_LSFT, KC_MINS),             // FN2  = Shifted Minus     // \ in Workman
    // unused: [3]  =  ACTION_MODS_KEY(MOD_LSFT, KC_COMM),  // FN3  = Shifted comma     // < in Workman
    // unused: [4]  =  ACTION_MODS_KEY(MOD_LSFT, KC_DOT),   // FN4  = Shifted dot       // > in Workman

    [5]  =  ACTION_MODS_TAP_KEY(MOD_LCTL, KC_BSPC),         // FN5  = LShift with tap BackSpace
    [6]  =  ACTION_MODS_TAP_KEY(MOD_LSFT, KC_DEL),          // FN6  = LCtrl  with tap Delete
    [7]  =  ACTION_MODS_TAP_KEY(MOD_LALT, KC_ESC),          // FN7  = LAlt   with tap Escape
    [8]  =  ACTION_MODS_TAP_KEY(MOD_RGUI, KC_INS),          // FN8  = RGui   with tap Ins
    [9]  =  ACTION_MODS_TAP_KEY(MOD_RSFT, KC_ENT),          // FN9  = RShift with tap Enter
    [10] =  ACTION_MODS_TAP_KEY(MOD_RCTL, KC_SPC),          // FN10 = RCtrl  with tap Space

    [11] =  ACTION_MODS_TAP_KEY(MOD_LSFT, KC_TAB),          // FN11 = LShift with tap Tab
    [12] =  ACTION_MODS_TAP_KEY(MOD_LCTL, KC_GRV),          // FN12 = LCtrl  with tap Tilda
    [13] =  ACTION_MODS_TAP_KEY(MOD_LALT, KC_SPC),          // FN13 = LAlt   with tap Space
    [14] =  ACTION_MODS_TAP_KEY(MOD_LGUI, KC_ESC),          // FN14 = LGui   with tap Escape
    [15] =  ACTION_MODS_TAP_KEY(MOD_RSFT, KC_QUOT),         // FN15 = RShift with tap quotes
    [16] =  ACTION_MODS_TAP_KEY(MOD_RCTL, KC_RBRC),         // FN16 = RCtrl  with tap ]

    [17] =  ACTION_LAYER_SET(0, ON_BOTH),                   // FN17 - set Layer0
    [18] =  ACTION_LAYER_SET(1, ON_BOTH),                   // FN18 - set Layer1, to use Workman layout at firmware level
    [19] =  ACTION_LAYER_SET(2, ON_BOTH),                   // FN19 - set Layer2, to use with Numpad keys

    // i'd like to remove this - will try to get used to live without this and convert them to usual keys
    [20] =  ACTION_LAYER_MOMENTARY(2),                      // FN20 - momentary Layer2, to use with Numpad keys
// or
//  [20] =  ACTION_FUNCTION_TAP(CUSTOM_KEY),                // FN20 - use custom key, with tapping support

    [21] =  ACTION_FUNCTION_TAP(L_CTRL_ALT_ENT),            // FN21 - momentary Layer5+CTRL+ALT on Enter, to use with F* keys on top row
    [22] =  ACTION_FUNCTION_TAP(R_CTRL_ALT_ENT),            // FN22 - momentary Layer6+CTRL+ALT on Enter, to use with F* keys on top row + utils

    [23] =  ACTION_LAYER_TAP_KEY(7, KC_6),                  // FN23 - momentary Layer7 on '6' , to use with F* keys (F1-F24)

    // unused:[24] =  ACTION_LAYER_TAP_KEY(4, KC_Z),                  // FN24 = momentary Layer4 on Z key, to use with unconvenient keys
    // unused:[25] =  ACTION_LAYER_TAP_KEY(3, KC_X),                  // FN25 = momentary Layer3 on X key, to use with F* keys
    // unused:[26] =  ACTION_LAYER_TAP_KEY(8, KC_C),                  // FN26 = momentary Layer8 on C key, to use with mouse and navigation keys
    [27] =  ACTION_LAYER_TAP_KEY(9, KC_V),                  // FN27 = momentary Layer9 on V key, to use with application-specific shortcuts

    [28] =  ACTION_LAYER_TAP_KEY(4, KC_A),                  // FN28 = momentary Layer4 on A key, to use with unconvenient keys
    [29] =  ACTION_LAYER_TAP_KEY(3, KC_S),                  // FN29 = momentary Layer3 on S key, to use with F* keys
    [30] =  ACTION_LAYER_TAP_KEY(8, KC_D),                  // FN30 = momentary Layer8 on D key, to use with mouse and navigation keys
    [31] =  ACTION_LAYER_TAP_KEY(2, KC_F),                  // FN31 = momentary Layer2 on F key, to use with Numpad keys

    // new, on kinesis only
    // unused are: 1, 3, 4, 24, 25, 26
    [1]  =  ACTION_FUNCTION_TAP(L_CTRL_ALT_T),              // FN1  = momentary Layer5+CTRL+ALT on T, to use with F* keys on top row
    [3]  =  ACTION_MODS_TAP_KEY(MOD_LGUI|MOD_LALT, KC_G),   // FN3  = LGui+LAlt with tap G
    [4]  =  ACTION_MODS_TAP_KEY(MOD_LGUI,          KC_B),   // FN4  = LGui      with tap B

    [24] =  ACTION_FUNCTION_TAP(R_CTRL_ALT_Y),              // FN24 = momentary Layer6+CTRL+ALT on Y, to use with F* keys on top row + utils
    [25] =  ACTION_MODS_TAP_KEY(MOD_LGUI|MOD_LALT, KC_H),   // FN25 = RGui+LAlt with tap H
    [26] =  ACTION_MODS_TAP_KEY(MOD_LGUI,          KC_N),   // FN26 = RGui      with tap N
};

static const uint16_t PROGMEM fn_actions_4[] = {
    [1]  =  ACTION_MODS_KEY(MOD_LSFT, KC_BSLS),             // FN1  = Shifted BackSlash // " in Workman
    [2]  =  ACTION_MODS_KEY(MOD_LSFT, KC_MINS),             // FN2  = Shifted Minus     // \ in Workman
    [3]  =  ACTION_MODS_KEY(MOD_LSFT, KC_COMM),             // FN3  = Shifted comma     // < in Workman
    [4]  =  ACTION_MODS_KEY(MOD_LSFT, KC_DOT),              // FN4  = Shifted dot       // > in Workman
    [5]  =  ACTION_MODS_KEY(MOD_LSFT, KC_SLSH),             // FN5  = Shifted slash     // ? in Workman
};

static const uint16_t PROGMEM fn_actions_6[] = {
    [0]  =  ACTION_FUNCTION(TEENSY_KEY),                    // FN0  = Teensy key
    [1]  =  ACTION_LAYER_SET(1, ON_BOTH),                   // FN1  = set Layer1
};

static const uint16_t PROGMEM fn_actions_7[] = {
    [0]  =  ACTION_FUNCTION(TEENSY_KEY),                    // FN0  = Teensy key
    [1]  =  ACTION_MACRO(XMONAD_RESET),                     // FN1  = xmonad-reanimator
    [2]  =  ACTION_MACRO(PASSWORD1),                        // FN2  = default password
    [3]  =  ACTION_MACRO(PASSWORD1),                        // FN3  = other password
    [4]  =  ACTION_MACRO(PASSWORD1),                        // FN4  = mega password
};

static const uint16_t PROGMEM fn_actions_9[] = {
    [0]  =  ACTION_MODS_KEY(MOD_LCTL,          KC_P0),      // FN0  = Ctrl+0
    [1]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P1),      // FN1  = Alt+1
    [2]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P2),      // FN2  = Alt+2
    [3]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P3),      // FN3  = Alt+3
    [4]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P4),      // FN4  = Alt+4
    [5]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P5),      // FN5  = Alt+5
    [6]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P6),      // FN6  = Alt+6
    [7]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P7),      // FN7  = Alt+7
    [8]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P8),      // FN8  = Alt+8
    [9]  =  ACTION_MODS_KEY(MOD_LALT,          KC_P9),      // FN9  = Alt+9
    [10] =  ACTION_MODS_KEY(MOD_LCTL|MOD_LSFT, KC_TAB),     // FN10 = Ctrl+Shift+Tab
    [11] =  ACTION_MODS_KEY(MOD_LCTL,          KC_TAB),     // FN11 = Ctrl+Tab
    [12] =  ACTION_MODS_KEY(MOD_LCTL|MOD_LSFT, KC_PGUP),    // FN12 = Ctrl+Shift+PgUp
    [13] =  ACTION_MODS_KEY(MOD_LCTL|MOD_LSFT, KC_PGDN),    // FN13 = Ctrl+Shift+PgDn
    [14] =  ACTION_MODS_KEY(MOD_LCTL,          KC_PMNS),    // FN14 = Ctrl+Pad Minus
    [15] =  ACTION_MODS_KEY(MOD_LCTL,          KC_PPLS),    // FN15 = Ctrl+Pad Plus
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // print("action_function called\n");
    // print("id  = "); phex(id); print("\n");
    // print("opt = "); phex(opt); print("\n");

    if (id == TEENSY_KEY) {
        clear_keyboard();
        print("\n\nJump to bootloader... ");
        _delay_ms(50);
        bootloader_jump(); // should not return
        print("not supported.\n");
    }

    if (   id == L_CTRL_ALT_ENT
        || id == R_CTRL_ALT_ENT
        || id == L_CTRL_ALT_T
        || id == R_CTRL_ALT_Y
    ) {
        // debug
        /*
        if (record->event.pressed) dprint("P"); else dprint("R");
        dprintf("%d", record->tap.count);
        if (record->tap.interrupted) dprint("i");
        dprint("\n");
        */

        if (record->tap.count == 0 || record->tap.interrupted) {
            uint8_t weak_mods = 0;
            uint8_t layer = 0;

            if (id == L_CTRL_ALT_ENT || id == L_CTRL_ALT_T) {
                weak_mods = MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT);
                layer     = 5;
            } else if (id == R_CTRL_ALT_ENT || id == R_CTRL_ALT_Y) {
                weak_mods = MOD_BIT(KC_RCTL) | MOD_BIT(KC_RALT);
                layer     = 6;
            } else {
                // do nothing - this should never happen
            }

            if (record->event.pressed) {
                layer_on(layer);
                add_weak_mods(weak_mods);
            } else {
                del_weak_mods(weak_mods);
                layer_off(layer);
            }
        } else {
            if (id == L_CTRL_ALT_ENT || id == R_CTRL_ALT_ENT) {
                if (record->event.pressed) {
                    add_key(KC_ENT);
                } else {
                    del_key(KC_ENT);
                }
            } else if (id == L_CTRL_ALT_T) {
                if (record->event.pressed) {
                    add_key(KC_T);
                } else {
                    del_key(KC_T);
                }
            } else if (id == R_CTRL_ALT_Y) {
                if (record->event.pressed) {
                    add_key(KC_Y);
                } else {
                    del_key(KC_Y);
                }
            } else {
                // do nothing - this should never happen
            }

            send_keyboard_report();
        }
    }
}

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    } else {
        // fall back to layer 0
        return pgm_read_byte(&keymaps[0][(key.row)][(key.col)]);
    }
}


#include "keymap_passwords.h"
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    if (record->event.pressed) {
        switch (id) {
            case XMONAD_RESET:  return MACRO_XMONAD_RESET;
            case PASSWORD1:     return MACRO_PASSWORD1;
        }
    }
    return MACRO_NONE;
}

#define FN_ACTIONS_SIZE     (sizeof(fn_actions)   / sizeof(fn_actions[0]))
#define FN_ACTIONS_4_SIZE   (sizeof(fn_actions_4) / sizeof(fn_actions_4[0]))
#define FN_ACTIONS_6_SIZE   (sizeof(fn_actions_6) / sizeof(fn_actions_6[0]))
#define FN_ACTIONS_7_SIZE   (sizeof(fn_actions_7) / sizeof(fn_actions_7[0]))
#define FN_ACTIONS_9_SIZE   (sizeof(fn_actions_9) / sizeof(fn_actions_9[0]))

/*
 * translates Fn keycode to action
 * for some layers, use different translation table
 */
action_t keymap_fn_to_action(uint8_t keycode)
{
    uint8_t layer = biton32(layer_state);

    action_t action;
    action.code = ACTION_NO;

    if (layer == 4 && FN_INDEX(keycode) < FN_ACTIONS_4_SIZE) {
        action.code = pgm_read_word(&fn_actions_4[FN_INDEX(keycode)]);
    }

    if (layer == 6 && FN_INDEX(keycode) < FN_ACTIONS_6_SIZE) {
        action.code = pgm_read_word(&fn_actions_6[FN_INDEX(keycode)]);
    }

    if (layer == 7 && FN_INDEX(keycode) < FN_ACTIONS_7_SIZE) {
        action.code = pgm_read_word(&fn_actions_7[FN_INDEX(keycode)]);
    }

    if (layer == 9 && FN_INDEX(keycode) < FN_ACTIONS_9_SIZE) {
        action.code = pgm_read_word(&fn_actions_9[FN_INDEX(keycode)]);
    }

    // by default, use fn_actions from default layer 0
    // this is needed to get mapping for same key, that was used switch to some layer,
    // to have possibility to switch layers back
    if (action.code == ACTION_NO && FN_INDEX(keycode) < FN_ACTIONS_SIZE) {
        action.code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
    }

    return action;
}

