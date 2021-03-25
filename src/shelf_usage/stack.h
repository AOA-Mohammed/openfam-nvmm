/*
 *  (c) Copyright 2016-2021 Hewlett Packard Enterprise Development Company LP.
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

/*
  modified from the stack implementation from Mark
 */

#ifndef _NVMM_STACK_H_
#define _NVMM_STACK_H_

#include "nvmm/global_ptr.h"
#include "shelf_usage/smart_shelf.h"


namespace nvmm {
    
/**
 ** A very simple lock-free stack of blocks
 ** 
 ** Must be allocated in FAM.
 ** 
 ** The blocks pushed on the stack must be cache line aligned, at
 ** least a cache line long, and not accessed by anyone else while on
 ** the stack.  They must also all belong to the same Shelf, which must
 ** be passed to pop and push.
 **/
struct Stack {
    // we access the following two fields atomically via 128-bit CAS:
    alignas(16) Offset head;
    uint64_t  aba_counter;  // incremented each time head is written

    // returns 0 if stack is empty
    Offset pop (SmartShelf_& shelf);
    void     push(SmartShelf_& shelf, Offset block);

    // returns 0 if stack is empty
    Offset pop (void *addr);
    void     push(void *addr, Offset block);
    
private:
    Stack(const Stack&);              // disable copying
    Stack& operator=(const Stack&);   // disable assignment
};


}

#endif
