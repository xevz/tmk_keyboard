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
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"


#define KEYMAP( \
  K00,K01, K02,K03,K04,K05,        K40,K41,K42,K43, K44,K45, \
  K10,K11, K12,K13,K14,K15,        K50,K51,K52,K53, K54,K55, \
  K20,K21, K22,K23,K24,K25,        K60,K61,K62,K63, K64,K65, \
    T30,T31,T32,T33,T34,              T70,T71,T72,T73,T74   \
) { \
  {  KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05 },\
  {  KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15 },\
  {  KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25 },\
  {  KC_0,     KC_##T30, KC_##T31, KC_##T32, KC_##T33, KC_##T34 },\
\
  {  KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45 },\
  {  KC_##K50, KC_##K51, KC_##K52, KC_##K53, KC_##K54, KC_##K55 },\
  {  KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65 },\
  {  KC_##T70, KC_##T71, KC_##T72, KC_##T73, KC_##T74, KC_0     } \
}

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/** Layer 0: (lowercase char)
 *
 *   ` q w e r t    y u i o p =
 *   \ a s d f g    h j k l ; '
 *   [ z x c v b    n m , . / ]
 */
 KEYMAP( \
    GRV,   Q,   W,   E,   R,   T,      Y,   U,   I,   O,   P, EQL, \
   SLSH,   A,   S,   D,   F,   G,      H,   J,   K,   L,SCLN,QUOT, \
   LBRC,   Z,   X,   C,   V,   B,      N,   M,COMM, DOT,BSLS,RBRC, \
      FN0, FN1, FN2, FN3, FN4,           FN9, FN8, FN7, FN6, FN5   ),

/** Layer 1: (symbols and navigation)
 *
 *     1 2 3 4 5    6 7 8 9 0 -
 *       ↰ ↑ ↳ ⤒    ← ↓ ↑ →  
 *       ← ↓ → ⤓    
 */
  KEYMAP( \
     NO,   1,   2,   3,   4,   5,      6,   7,   8,   9,   0,MINS, \
     NO,  NO,PGUP,  UP,PGDN,HOME,   LEFT,DOWN,  UP,RGHT,SCLN,QUOT, \
     NO,  NO,LEFT,DOWN,RGHT, END,     NO,  NO,  NO,  NO,  NO,  NO, \
      FN0, FN1, FN2, FN3, FN4,         FN9, FN8, FN7, FN6, FN5   )
};

/*
 * Fn action definition
 */
// TODO: use [1] = KEYMAP(...) to prevent from changing index of element?
static const uint16_t PROGMEM fn_actions[] = {
    ACTION_LAYER_TAP_KEY(1, KC_SPC),                 // FN0 = L1 symbols 
    ACTION_MODS_TAP_KEY(MOD_LSFT, KC_SPC),           // FN1 = Shift with tap Space
    ACTION_MODS_TAP_KEY(MOD_LCTL, KC_ESC),           // FN2 = Ctrl with tap Escape
    ACTION_MODS_TAP_KEY(MOD_LALT, KC_TAB),           // FN3 = Alt with tap Tab
    ACTION_MODS_TAP_KEY(MOD_LGUI, KC_DEL),           // FN4 = Meta with tap Del

    ACTION_LAYER_TAP_KEY(1, KC_ENT),                 // FN5 = L1 symbols 
    ACTION_MODS_TAP_KEY(MOD_RSFT, KC_ENT),           // FN6 = Shift with tap Enter
    ACTION_MODS_TAP_KEY(MOD_RCTL, KC_BSPC),          // FN7 = Ctrl with tap Backspace
    ACTION_MODS_TAP_KEY(MOD_RALT, KC_RALT),          // FN8 = Ctrl with tap Backspace
    ACTION_MODS_TAP_KEY(MOD_RGUI, KC_TAB)            // FN9 = Meta with tap Tab
};


#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, key_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    } else {
        // XXX: this may cuaes bootlaoder_jump inconsistent fail.
        //debug("key_to_keycode: base "); debug_dec(layer); debug(" is invalid.\n");
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
