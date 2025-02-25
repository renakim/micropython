/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "string.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "sam.h"
#include "pin_af.h"
#include "pins.h"
#include "samd_soc.h"

extern const mp_obj_type_t samd_flash_type;

STATIC mp_obj_t samd_pininfo(mp_obj_t pin_obj) {
    mp_hal_pin_obj_t pin = mp_hal_get_pin_obj(pin_obj);
    const pin_af_t *pin_af = get_pin_af_info(pin);
    const char *name = ((machine_pin_obj_t *)MP_OBJ_TO_PTR(pin_obj))->name;
    if (pin_af) {
        #if defined(MCU_SAMD21)
        mp_obj_t tuple[7] = {
            tuple[0] = mp_obj_new_str(name, strlen(name)),
            tuple[1] = mp_obj_new_int(pin_af->eic),
            tuple[2] = mp_obj_new_int(pin_af->adc0),
            tuple[3] = mp_obj_new_int(pin_af->sercom1),
            tuple[4] = mp_obj_new_int(pin_af->sercom2),
            tuple[5] = mp_obj_new_int(pin_af->tcc1),
            tuple[6] = mp_obj_new_int(pin_af->tcc2),
        };
        return mp_obj_new_tuple(7, tuple);
        #elif defined(MCU_SAMD51)
        mp_obj_t tuple[9] = {
            tuple[0] = mp_obj_new_str(name, strlen(name)),
            tuple[1] = mp_obj_new_int(pin_af->eic),
            tuple[2] = mp_obj_new_int(pin_af->adc0),
            tuple[3] = mp_obj_new_int(pin_af->adc1),
            tuple[4] = mp_obj_new_int(pin_af->sercom1),
            tuple[5] = mp_obj_new_int(pin_af->sercom2),
            tuple[6] = mp_obj_new_int(pin_af->tc),
            tuple[7] = mp_obj_new_int(pin_af->tcc1),
            tuple[8] = mp_obj_new_int(pin_af->tcc2),
        };
        return mp_obj_new_tuple(9, tuple);
        #endif
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(samd_pininfo_obj, samd_pininfo);

STATIC const mp_rom_map_elem_t samd_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_samd) },
    { MP_ROM_QSTR(MP_QSTR_Flash),    MP_ROM_PTR(&samd_flash_type) },
    { MP_ROM_QSTR(MP_QSTR_pininfo),  MP_ROM_PTR(&samd_pininfo_obj) },
};
STATIC MP_DEFINE_CONST_DICT(samd_module_globals, samd_module_globals_table);

const mp_obj_module_t mp_module_samd = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&samd_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_samd, mp_module_samd);
