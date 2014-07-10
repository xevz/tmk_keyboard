/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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
#include "bootloader.h"
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"


#define KEYMAP(                                                                         \
    /* spatial positions */                                                             \
    k06,k07,k08,k09,k0A,k0B,                                k05,k04,k03,k02,k01,k00,    \
    k16,k17,k18,k19,k1A,k1B,                                k15,k14,k13,k12,k11,k10,    \
    k26,k27,k28,k29,k2A,k2B,                                k25,k24,k23,k22,k21,k20,    \
    k36,k37,k38,k39,k3A,k3B,                                k35,k34,k33,k32,k31,k30,    \
    k46,k47,k48,k49,k4A,k4B, k5B,k69,k56,   k64,k63,k61,    k45,k44,k43,k42,k41,k40,    \
        k57,k58,k59,k5A,     k66,k67,k6A,   k65,k62,k50,        k54,k53,k52,k51,        \
                                     k68,   k55                                         \
    )                                                                                   \
    /* matrix positions */                                                              \
    {                                                                                   \
     { KC_##k00,KC_##k10,KC_##k20,KC_##k30,KC_##k40,KC_##k50,KC_NO   },                 \
     { KC_##k01,KC_##k11,KC_##k21,KC_##k31,KC_##k41,KC_##k51,KC_##k61},                 \
     { KC_##k02,KC_##k12,KC_##k22,KC_##k32,KC_##k42,KC_##k52,KC_##k62},                 \
     { KC_##k03,KC_##k13,KC_##k23,KC_##k33,KC_##k43,KC_##k53,KC_##k63},                 \
     { KC_##k04,KC_##k14,KC_##k24,KC_##k34,KC_##k44,KC_##k54,KC_##k64},                 \
     { KC_##k05,KC_##k15,KC_##k25,KC_##k35,KC_##k45,KC_##k55,KC_##k65},                 \
     { KC_##k06,KC_##k16,KC_##k26,KC_##k36,KC_##k46,KC_##k56,KC_##k66},                 \
     { KC_##k07,KC_##k17,KC_##k27,KC_##k37,KC_##k47,KC_##k57,KC_##k67},                 \
     { KC_##k08,KC_##k18,KC_##k28,KC_##k38,KC_##k48,KC_##k58,KC_##k68},                 \
     { KC_##k09,KC_##k19,KC_##k29,KC_##k39,KC_##k49,KC_##k59,KC_##k69},                 \
     { KC_##k0A,KC_##k1A,KC_##k2A,KC_##k3A,KC_##k4A,KC_##k5A,KC_##k6A},                 \
     { KC_##k0B,KC_##k1B,KC_##k2B,KC_##k3B,KC_##k4B,KC_##k5B,KC_NO   },                 \
    }

//    /* matrix positions */                                                               \
//    {                                                                                    \
//     { KC_##k00,KC_##k01,KC_##k02,KC_##k03,KC_##k04,KC_##k05,KC_##k06,KC_##k07,KC_##k08,KC_##k09,KC_##k0A,KC_##k0B},     \
//     { KC_##k10,KC_##k11,KC_##k12,KC_##k13,KC_##k14,KC_##k15,KC_##k16,KC_##k17,KC_##k18,KC_##k19,KC_##k1A,KC_##k1B},     \
//     { KC_##k20,KC_##k21,KC_##k22,KC_##k23,KC_##k24,KC_##k25,KC_##k26,KC_##k27,KC_##k28,KC_##k29,KC_##k2A,KC_##k2B},     \
//     { KC_##k30,KC_##k31,KC_##k32,KC_##k33,KC_##k34,KC_##k35,KC_##k36,KC_##k37,KC_##k38,KC_##k39,KC_##k3A,KC_##k3B},     \
//     { KC_##k40,KC_##k41,KC_##k42,KC_##k43,KC_##k44,KC_##k45,KC_##k46,KC_##k47,KC_##k48,KC_##k49,KC_##k4A,KC_##k4B},     \
//     { KC_##k50,KC_##k51,KC_##k52,KC_##k53,KC_##k54,KC_##k55,KC_##k56,KC_##k57,KC_##k58,KC_##k59,KC_##k5A,KC_##k5B},     \
//     { KC_NO,   KC_##k61,KC_##k62,KC_##k63,KC_##k64,KC_##k65,KC_##k66,KC_##k67,KC_##k68,KC_##k69,KC_##k6A,KC_NO   },     \
//    }


static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Layer0
KEYMAP(
    NO  , NO  , NO  , NO  , NO  , NO  ,                                               NO  , NO  , NO  , NO  , NO  , NO  ,
    ESC , FN11, FN12, FN13, FN14, FN15,                                               FN16, FN17, FN18, FN19, FN10, PAUS,
    BSLS, EQL , D   , R   , W   , B   ,                                               J   , F   , U   , P   , LBRC, RBRC,
    Q   , A   , S   , H   , T   , G   ,                                               Y   , N   , E   , O   , I   , QUOT,
    FN7 , Z   , X   , M   , C   , V   , INS , F7  , 2   ,       RALT, HOME, END ,     K   , L   , COMM, DOT , SLSH, MINS,
        LALT, LCTL, TAB , LGUI,         FN2 , FN21, GRV ,       RGUI, FN22, FN20,         LEFT, UP  , DOWN, RGHT,
                                                    DEL ,       RCTL
),

// Layer1
KEYMAP(
    NO  , NO  , NO  , NO  , NO  , NO  ,                                               NO  , NO  , NO  , NO  , NO  , NO  ,
    F12 , F1  , F2  , F3  , F4  , F5  ,                                               F6  , F7  , F8  , F9  , F10 , F11 ,
    TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,                                               TRNS, FN3 , 8   , 9   , TRNS, TRNS,
    FN4 , F10 , F2  , F3  , F4  , TRNS,                                               2   , 1   , 0   , 6   , 7   , TRNS,
    FN5 , F9  , F8  , F6  , F5  , TRNS, PSCR, MINS, 1   ,       FN0 , PGUP, PGDN,     3   , 4   , 5   , TRNS, TRNS, TRNS,
        TRNS, TRNS, SPC , TRNS,         TRNS, TRNS, TRNS,       TRNS, TRNS, TRNS,         HOME, PGUP, PGDN, END ,
                                                    TRNS,       TRNS
),

};

#define MACRO_JIRA_NOFORMAT                                 \
        MACRO(                                              \
            I(15),                                          \
            D(LSFT), T(LBRC), U(LSFT),                      \
            T(N), T(O), T(F), T(O), T(R), T(M), T(A), T(T), \
            D(LSFT), T(RBRC), U(LSFT),                      \
            T(ENT),                                         \
        END)

#define MACRO_JIRA_QUOTE                                    \
        MACRO(                                              \
            I(15),                                          \
            D(LSFT), T(LBRC), U(LSFT),                      \
            T(Q), T(U), T(O), T(T), T(E),                   \
            D(LSFT), T(RBRC), U(LSFT),                      \
            T(ENT),                                         \
        END)


/* id for user defined functions and macros */
enum function_id {
    TEENSY_KEY,
};

enum macro_id {
    JIRA_NOFORMAT,
    JIRA_QUOTE,
};

/* user defined functions implementation */
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

/* user defined macros implementation */
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    if (record->event.pressed) {
        switch (id) {
            case JIRA_NOFORMAT:  return MACRO_JIRA_NOFORMAT;
            case JIRA_QUOTE:     return MACRO_JIRA_QUOTE;
        }
    }
    return MACRO_NONE;
}

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
    [0]  =  ACTION_FUNCTION(TEENSY_KEY),                    // FN0  - Teensy key - reboot to programmator mode
    [1]  =  ACTION_LAYER_MOMENTARY(1),
    [2]  =  ACTION_MODS_TAP_KEY(MOD_LSFT, KC_BSPC),

    [3]  =  ACTION_MACRO(JIRA_NOFORMAT),
    [4]  =  ACTION_MACRO(JIRA_QUOTE),

    [5]  =  ACTION_MODS_KEY(MOD_LGUI | MOD_LSFT, KC_0),
//  [6]  =  ACTION_MODS_KEY(MOD_LGUI | MOD_LSFT, KC_Z),
    [7]  =  ACTION_MODS_KEY(MOD_LGUI, KC_Z),
//  [8]  =  ACTION_MODS_KEY(MOD_LSFT, KC_LBRC),
//  [9]  =  ACTION_MODS_KEY(MOD_LSFT, KC_RBRC),

    [10] =   ACTION_MODS_KEY(MOD_LSFT, KC_0),
    [11] =   ACTION_MODS_KEY(MOD_LSFT, KC_1),
    [12] =   ACTION_MODS_KEY(MOD_LSFT, KC_2),
    [13] =   ACTION_MODS_KEY(MOD_LSFT, KC_3),
    [14] =   ACTION_MODS_KEY(MOD_LSFT, KC_4),
    [15] =   ACTION_MODS_KEY(MOD_LSFT, KC_5),
    [16] =   ACTION_MODS_KEY(MOD_LSFT, KC_6),
    [17] =   ACTION_MODS_KEY(MOD_LSFT, KC_7),
    [18] =   ACTION_MODS_KEY(MOD_LSFT, KC_8),
    [19] =   ACTION_MODS_KEY(MOD_LSFT, KC_9),

    [20] =   ACTION_LAYER_TAP_KEY(1, KC_SPC),
    [21] =   ACTION_LAYER_TAP_KEY(1, KC_ENT),
    [22] =   ACTION_LAYER_TAP_KEY(1, KC_SCLN),
};

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, key_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    } else {
        // fall back to layer 0
        return pgm_read_byte(&keymaps[0][(key.row)][(key.col)]);
    }
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    action_t action;
    if (FN_INDEX(keycode) < FN_ACTIONS_SIZE) {
        action.code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
    } else {
        action.code = ACTION_NO;
    }
    return action;
}

