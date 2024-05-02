/*
 *  (c) Copyright 2024 Hewlett Packard Enterprise Development Company LP.
 *
 *  This software is available to you under a choice of one of two
 *  licenses. You may choose to be licensed under the terms of the
 *  GNU Lesser General Public License Version 3, or (at your option)
 *  later with exceptions included below, or under the terms of the
 *  MIT license (Expat) available in COPYING file in the source tree.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As an exception, the copyright holders of this Library grant you permission
 *  to (i) compile an Application with the Library, and (ii) distribute the
 *  Application containing code generated by the Library and added to the
 *  Application during this compilation process under terms of your choice,
 *  provided you also meet the terms and conditions of the Application license.
 *
 */

#ifndef _NVMM_FAM_ATOMIC_GENERIC_H_
#define _NVMM_FAM_ATOMIC_GENERIC_H_

#include <stdint.h>

inline int32_t fam_atomic_32_fetch_add(int32_t *address, int32_t increment) {
    return __atomic_fetch_add(address, increment, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_fetch_add(int64_t *address, int64_t increment) {
    return __atomic_fetch_add(address, increment, __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_swap(int32_t *address, int32_t value) {
    return __atomic_exchange_n(address, value, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_swap(int64_t *address, int64_t value) {
    return __atomic_exchange_n(address, value, __ATOMIC_SEQ_CST);
}

inline void fam_atomic_128_swap(int64_t *address, int64_t value[2],
                                int64_t result[2]) {
    __atomic_exchange((__int128_t *)address, (__int128_t *)value,
                      (__int128_t *)result, __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_compare_store(int32_t *address, int32_t compare,
                                           int32_t store) {
    __atomic_compare_exchange_n(address, &compare, store, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return compare;
}

inline int64_t fam_atomic_64_compare_store(int64_t *address, int64_t compare,
                                           int64_t store) {
    __atomic_compare_exchange_n(address, &compare, store, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return compare;
}

inline void fam_atomic_128_compare_store(int64_t *address, int64_t compare[2],
                                         int64_t store[2], int64_t result[2]) {
    /* While all other implementations should be equal to the inline asm in
     * fam_atomic_x86.c, this one here is likely different: clang will generate
     * a library call for generic code, and a proper cmpxchg16b if -mcx16 (or
     * coresponding -march=) is being used. GCC 7+ will always emit a function
     * call, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=94649. Another
     * difference is that compared to the native implementation we fully rely on
     * the atomicity on the full 128bit value, rather than using a loop.
     */
    __int128_t old = *(__int128_t *)compare;
    __atomic_compare_exchange((__int128_t *)address, &old, (__int128_t *)store,
                              false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    *(__int128_t *)result = old;
}

inline int32_t fam_atomic_32_read(int32_t *address) {
    int32_t ret;
    __atomic_load(address, &ret, __ATOMIC_SEQ_CST);
    return ret;
}

inline int64_t fam_atomic_64_read(int64_t *address) {
    int64_t ret;
    __atomic_load(address, &ret, __ATOMIC_SEQ_CST);
    return ret;
}

inline void fam_atomic_128_read(int64_t *address, int64_t result[2]) {
    __atomic_load((__int128_t *)address, (__int128_t *)result,
                  __ATOMIC_SEQ_CST);
}

inline void fam_atomic_32_write(int32_t *address, int32_t value) {
    __atomic_store_n(address, value, __ATOMIC_SEQ_CST);
}

inline void fam_atomic_64_write(int64_t *address, int64_t value) {
    __atomic_store_n(address, value, __ATOMIC_SEQ_CST);
}

inline void fam_atomic_128_write(int64_t *address, int64_t value[2]) {
    __atomic_store((__int128_t *)address, (__int128_t *)value,
                   __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_fetch_and(int32_t *address, int32_t arg) {
    return __atomic_fetch_and(address, arg, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_fetch_and(int64_t *address, int64_t arg) {
    return __atomic_fetch_and(address, arg, __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_fetch_or(int32_t *address, int32_t arg) {
    return __atomic_fetch_or(address, arg, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_fetch_or(int64_t *address, int64_t arg) {
    return __atomic_fetch_or(address, arg, __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_fetch_xor(int32_t *address, int32_t arg) {
    return __atomic_fetch_xor(address, arg, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_fetch_xor(int64_t *address, int64_t arg) {
    return __atomic_fetch_xor(address, arg, __ATOMIC_SEQ_CST);
}

inline uint32_t fam_atomic_u32_read(uint32_t *addr) {
    uint32_t ret;
    __atomic_load(addr, &ret, __ATOMIC_SEQ_CST);
    return ret;
}

inline uint64_t fam_atomic_u64_read(uint64_t *addr) {
    uint64_t ret;
    __atomic_load(addr, &ret, __ATOMIC_SEQ_CST);
    return ret;
}

inline void fam_atomic_u128_read(uint64_t *address, uint64_t result[2]) {
    __atomic_load((__uint128_t *)address, (__uint128_t *)result,
                  __ATOMIC_SEQ_CST);
}

inline void fam_atomic_u32_write(uint32_t *addr, uint32_t value) {
    __atomic_store_n(addr, value, __ATOMIC_SEQ_CST);
}

inline void fam_atomic_u64_write(uint64_t *addr, uint64_t value) {
    __atomic_store_n(addr, value, __ATOMIC_SEQ_CST);
}

inline void fam_atomic_u128_write(uint64_t *addr, uint64_t value[2]) {
    __atomic_store((__uint128_t *)addr, (__uint128_t *)value, __ATOMIC_SEQ_CST);
}

inline uint32_t fam_atomic_u32_fetch_and_add(uint32_t *addr,
                                             uint32_t increment) {
    return __atomic_fetch_add(addr, increment, __ATOMIC_SEQ_CST);
}

inline uint64_t fam_atomic_u64_fetch_and_add(uint64_t *addr,
                                             uint64_t increment) {
    return __atomic_fetch_add(addr, increment, __ATOMIC_SEQ_CST);
}

inline uint32_t fam_atomic_u32_compare_and_store(uint32_t *addr,
                                                 uint32_t oldval,
                                                 uint32_t newval) {
    __atomic_compare_exchange_n(addr, &oldval, newval, false, __ATOMIC_SEQ_CST,
                                __ATOMIC_SEQ_CST);
    return oldval;
}

inline uint64_t fam_atomic_u64_compare_and_store(uint64_t *addr,
                                                 uint64_t oldval,
                                                 uint64_t newval) {
    __atomic_compare_exchange_n(addr, &oldval, newval, false, __ATOMIC_SEQ_CST,
                                __ATOMIC_SEQ_CST);
    return oldval;
}

inline void fam_atomic_u128_compare_and_store(uint64_t *addr,
                                              uint64_t oldval[2],
                                              uint64_t newval[2],
                                              uint64_t result[2]) {
    /* see comment in fam_atomic_128_compare_store */
    __uint128_t old = *(__uint128_t *)oldval;
    __atomic_compare_exchange((__uint128_t *)addr, &old, (__uint128_t *)newval,
                              false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    *(__uint128_t *)result = old;
}

inline int32_t fam_atomic_32_fetch_and_add(int32_t *addr, int32_t increment) {
    return __atomic_fetch_add(addr, increment, __ATOMIC_SEQ_CST);
}

inline int64_t fam_atomic_64_fetch_and_add(int64_t *addr, int64_t increment) {
    return __atomic_fetch_add(addr, increment, __ATOMIC_SEQ_CST);
}

inline int32_t fam_atomic_32_compare_and_store(int32_t *address,
                                               int32_t compare, int32_t store) {
    __atomic_compare_exchange_n(address, &compare, store, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return compare;
}

inline int64_t fam_atomic_64_compare_and_store(int64_t *address,
                                               int64_t compare, int64_t store) {
    __atomic_compare_exchange_n(address, &compare, store, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return compare;
}

inline void fam_atomic_128_compare_and_store(int64_t *address,
                                             int64_t compare[2],
                                             int64_t store[2],
                                             int64_t result[2]) {
    /* see comment in fam_atomic_128_compare_store */
    __int128_t old = *(__int128_t *)compare;
    __atomic_compare_exchange((__int128_t *)address, &old, (__int128_t *)store,
                              false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    *(__int128_t *)result = old;
}

#endif
