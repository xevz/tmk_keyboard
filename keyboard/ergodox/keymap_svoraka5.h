#define KC_ARING   KC_LBRACKET // Å
#define KC_AUMLAUT KC_QUOTE    // Ä
#define KC_OUMLAUT KC_SCOLON   // Ö

#define KC_AR KC_ARING
#define KC_AU KC_AUMLAUT
#define KC_OU KC_OUMLAUT

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(  // Layer0: default, leftled:none
        // left hand
        EQL,    1,  2,  3,    4,     5, NO,
        TAB,   AR, AU, OU,    P,     Y, NO,
        ESC,    A,  O,  E,    U,     I,
        LSFT, DOT,  Q,  J,    K,     X, FN29,
        LGUI,      NO, NO, LEFT,  RGHT,
                                        LCTL,  LALT,
                                               HOME,
                                     BSPC, DEL, END,
        // right hand,
             NO,   6,   7,     8,    9,    0,   MINS,
             NO,   F,   G,     C,    R,    L,   BSLS,
                   D,   H,     T,    N,    S,   SLSH,
             FN29, B,   M,     W,    V,    Z,   RSFT,
                        UP, DOWN, COMM, RBRC,  RGUI,
        FN1, RCTL,
        PGUP,
        PGDN, ENT, SPC
    ),

    KEYMAP(  // Layer1: Function keys
        // left hand
        TRNS,  F1, F2, F3, F4, F5, F6,
        TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, FN29,
        TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS,
        TRNS,  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS,
        TRNS,          TRNS, TRNS, TRNS, TRNS,
                                       TRNS,  TRNS,
                                            TRNS,
                                    TRNS, TRNS, TRNS,
        // right hand
             F7,   F8, F9, F10, F11, F12, TRNS,
             FN29, TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS,
                   TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS,
             TRNS,   TRNS, TRNS,  TRNS,  TRNS,  TRNS, TRNS,
                       TRNS,  TRNS,  TRNS,  TRNS, TRNS,
        TRNS,  TRNS,
        TRNS,
        TRNS, TRNS, TRNS
    ),

    KEYMAP(  // Layer2: AltGr
        // left hand
        NO,   NO,    NO,      NO,   NO,   NO,   NO,
        NO,  FN2,   FN3,     FN4,  FN5,  FN6,   NO,
        NO,  FN12, FN13,    FN14, FN15, FN16,
        NO,  FN22, FN23,    FN24, FN25, FN26,   NO,
        NO,          NO,  NO,  NO,  NO,
                                        NO,  NO,
                                             NO,
                                     NO, NO, NO,
        // right hand
             NO,    NO,    NO,   NO,   NO,   NO,   NO,
             NO,   FN7,   FN8,  FN9, FN10, FN11,   NO,
                  FN17,  FN18, FN19, FN20, FN21,   NO,
             NO,  FN27,    NO,   NO,   NO,   NO,   NO,
                    NO,    NO,   NO, FN28,   NO,
        FN1,  NO,
        NO,
        NO, NO, NO
    ),


};

/* id for user defined functions */
enum function_id {
    TEENSY_KEY,
};

enum macro_id {
    NODEAD_TILDE,
    NODEAD_CARET,
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
    [0]  = ACTION_FUNCTION(TEENSY_KEY),                       // FN0  - Teensy key

    // AltGr toogle
    [1]  = ACTION_LAYER_MOMENTARY(2),                         // FN1 = Temporarily switch to layer 1

    // AltGr, top row
    [2]  = ACTION_MODS_KEY(MOD_RALT, KC_7),                   // FN2  = {
    [3]  = ACTION_MODS_KEY(MOD_RALT, KC_0),                   // FN3  = }
    [4]  = ACTION_MODS_KEY(MOD_RALT, KC_8),                   // FN4  = [
    [5]  = ACTION_MODS_KEY(MOD_RALT, KC_9),                   // FN5  = ]
    [6]  = ACTION_MODS_KEY(MOD_RALT, KC_4),                   // FN6  = $
    [7]  = ACTION_MODS_KEY(MOD_RSFT, KC_2),                   // FN7  = "
    [8]  = ACTION_MODS_KEY(MOD_RSFT, KC_MINUS),               // FN8  = ?
    [9]  = ACTION_MODS_KEY(MOD_RSFT, KC_6),                   // FN9  = &
    [10] = ACTION_KEY(KC_NONUS_BSLASH),                       // FN10 = <, not necessary, just easier to keep track
    [11] = ACTION_MODS_KEY(MOD_RSFT, KC_NONUS_BSLASH),        // FN11 = >

    // Middle row
    [12] = ACTION_MODS_KEY(MOD_RSFT, KC_COMMA),               // FN12 = ;
    [13] = ACTION_MODS_KEY(MOD_RSFT, KC_7),                   // FN13 = /
    [14] = ACTION_MODS_KEY(MOD_RSFT, KC_8),                   // FN14 = (
    [15] = ACTION_MODS_KEY(MOD_RSFT, KC_9),                   // FN15 = )
    [16] = ACTION_MODS_KEY(MOD_RALT, KC_NONUS_BSLASH),        // FN16 = |
    [17] = ACTION_MODS_KEY(MOD_RSFT, KC_3),                   // FN17 = #
    [18] = ACTION_MACRO(NODEAD_CARET),                        // FN18 = ^
    [19] = ACTION_MODS_KEY(MOD_RSFT, KC_3),                   // FN19 = #
    [20] = ACTION_MODS_KEY(MOD_RSFT, KC_2),                   // FN20 = "
    [21] = ACTION_MACRO(NODEAD_TILDE),                        // FN21 = ~

    // Last row
    [22] = ACTION_MODS_KEY(MOD_RSFT, KC_DOT),                 // FN22 = :
    [23] = ACTION_MODS_KEY(MOD_RSFT, KC_0),                   // FN23 = =
    [24] = ACTION_MODS_KEY(MOD_RALT, KC_2),                   // FN24 = @
    [25] = ACTION_MODS_KEY(MOD_RSFT, KC_1),                   // FN25 = !
    [26] = ACTION_MODS_KEY(MOD_RALT, KC_MINUS),               // FN26 = backslash
    [27] = ACTION_MODS_KEY(MOD_RSFT, KC_5),                   // FN27 = %
    [28] = ACTION_MODS_KEY(MOD_RALT, KC_RBRACKET),

    // Function key layer toggle
    [29]  = ACTION_LAYER_MOMENTARY(1),                        // FN29 = Temporarily switch to layer 2
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
    if (record->event.pressed) {
        switch (id) {
            case NODEAD_TILDE:
                return MACRO(D(RALT), T(RBRACKET), U(RALT), T(SPACE), END);
                break;
            case NODEAD_CARET:
                return MACRO(D(RSHIFT), T(RBRACKET), U(RSHIFT), T(SPACE), END);
                break;
        }
    }
    return MACRO_NONE;
}


void action_function(keyrecord_t *event, uint8_t id, uint8_t opt)
{
    print("action_function called\n");
    print("id  = "); phex(id); print("\n");
    print("opt = "); phex(opt); print("\n");
    if (id == TEENSY_KEY) {
        clear_keyboard();
        print("\n\nJump to bootloader... ");
        _delay_ms(250);
        bootloader_jump(); // should not return
        print("not supported.\n");
    }
}

