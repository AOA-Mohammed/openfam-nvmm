/*
 *  (c) Copyright 2016-2020 Hewlett Packard Enterprise Development Company LP.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
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

#ifndef _NVMM_COMMON_H_
#define _NVMM_COMMON_H_

namespace nvmm {

/*
 * System-specific parameters
 */
#define CACHE_LINE_SIZE 64

static int const kCacheLineSize = CACHE_LINE_SIZE;
static int const kVirtualPageSize = 64*1024;

/*
 * Permission mask
 */
#define PERM_MASK (S_IRWXU|S_IRWXG|S_IRWXO) /* 0777 */

/*
 * Maximum supported heap size
 */
#define KB 1024UL
#define MB (KB * KB)
#define GB (MB * KB)

#define MAX_ZONE_SIZE (1024 * GB)

/*
 * Round non-negative x up to the nearest multiple of positive
 * multiple
 */
static inline size_t round_up(size_t x, size_t multiple) {
    return ((x+multiple-1)/multiple)*multiple;
}

/*
 * Round non-negative x down to the nearest multiple of positive
 * multiple
 */
static inline size_t round_down(size_t x, size_t multiple) {
    return x/multiple*multiple;
}

} // namespace nvmm

#endif
