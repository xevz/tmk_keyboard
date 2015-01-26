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
    k40,k41,k42,k43,k44,                                k47,k48,k49,k4A,k4B,                                    \
                            k50,k52,        k58,k59,                                                            \
                                k53,        k56,                                                                \
                        k61,k62,k63,        k66,k67,k68 )                                                       \
                                                                                                                \
                                                                                                                \
   /* matrix positions, k40 & k4B unused for now, k45 & k46 are untraced on plate */                            \
   /* pins */                                                                                                   \
   /* 0: D0    1: D1    2: D2    3: D3    4: D4      5: F0    6: F1    7: F4    8: F5    9: F6      A: F7    B: D5      C: D6    D: D7       */            \
   {                                                                                                                                                       \
    { KC_##k00,KC_##k10,KC_##k20,KC_##k30,KC_##k40,  KC_##k06,KC_##k16,KC_##k26,KC_##k36,KC_##k48,  KC_##k50,KC_NO   ,  KC_##k56,KC_##k66},  /* 0: B0 */   \
    { KC_##k01,KC_##k11,KC_##k21,KC_##k31,KC_##k41,  KC_##k08,KC_##k18,KC_##k28,KC_##k38,KC_##k47,  KC_NO   ,KC_##k61,  KC_NO   ,KC_##k67},  /* 1: B1 */   \
    { KC_##k02,KC_##k12,KC_##k22,KC_##k32,KC_##k42,  KC_##k07,KC_##k17,KC_##k27,KC_##k37,KC_NO   ,  KC_##k52,KC_##k62,  KC_##k58,KC_##k68},  /* 2: B2 */   \
    { KC_##k03,KC_##k13,KC_##k23,KC_##k33,KC_##k43,  KC_##k0B,KC_##k1B,KC_##k2B,KC_##k3B,KC_##k4B,  KC_##k53,KC_##k63,  KC_##k59,KC_NO   },  /* 3: B3 */   \
    { KC_##k04,KC_##k14,KC_##k24,KC_##k34,KC_NO   ,  KC_##k09,KC_##k19,KC_##k29,KC_##k39,KC_##k4A,  KC_NO   ,KC_NO   ,  KC_NO   ,KC_NO   },  /* 4: B4 */   \
    { KC_##k05,KC_##k15,KC_##k25,KC_##k35,KC_##k44,  KC_##k0A,KC_##k1A,KC_##k2A,KC_##k3A,KC_##k49,  KC_NO   ,KC_NO   ,  KC_NO   ,KC_NO   },  /* 5: B5 */   \
   }


static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(  // layer 0 : default
        // left hand                             // right hand
        EQL, 1,   2,   3,   4,   5,                        6,   7,   8,   9,   0,   MINS,
        BSLS,Q,   W,   E,   R,   T,                        Y,   U,   I,   O,   P,   TAB,
        TAB, A,   S,   D,   F,   G,                        H,   J,   K,   L,   SCLN,QUOT,
        LSFT,Z,   X,   C,   V,   B,                        N,   M,   COMM,DOT, SLSH,RSFT,
        LGUI,GRV, BSLS,LEFT,RGHT,                               DOWN,UP,  RBRC,LBRC,RGUI,
                                   LCTL,LALT,    RALT,RCTL,
                                        HOME,    PGUP,
                              BSPC,DEL, END,     PGDN,ENT, SPC
    ),

};

/* id for user defined functions */
enum function_id {
    TEENSY_KEY,
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
    ACTION_FUNCTION(TEENSY_KEY),                            // FN0 - Teensy key
};

static const uint16_t PROGMEM fn_actions_4[] = {
    [1]  =  ACTION_MODS_KEY(MOD_LSFT, KC_BSLS),             // FN1  = Shifted BackSlash // " in Workman
    [2]  =  ACTION_MODS_KEY(MOD_LSFT, KC_MINS),             // FN2  = Shifted Minus     // \ in Workman
    [3]  =  ACTION_MODS_KEY(MOD_LSFT, KC_COMM),             // FN3  = Shifted comma     // < in Workman
    [4]  =  ACTION_MODS_KEY(MOD_LSFT, KC_DOT),              // FN4  = Shifted dot       // > in Workman
    [5]  =  ACTION_MODS_KEY(MOD_LSFT, KC_SLSH),             // FN5  = Shifted slash     // ? in Workman
};

static const uint16_t PROGMEM fn_actions_7[] = {
    ACTION_FUNCTION(TEENSY_KEY),                            // FN0 - Teensy key
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


void action_function(keyrecord_t *event, uint8_t id, uint8_t opt)
{
    if (id == TEENSY_KEY) {
        clear_keyboard();
        print("\n\nJump to bootloader... ");
        _delay_ms(250);
        bootloader_jump(); // should not return
        print("not supported.\n");
    }
}

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
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

#define FN_ACTIONS_SIZE     (sizeof(fn_actions)   / sizeof(fn_actions[0]))
#define FN_ACTIONS_4_SIZE   (sizeof(fn_actions_4) / sizeof(fn_actions_4[0]))
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

