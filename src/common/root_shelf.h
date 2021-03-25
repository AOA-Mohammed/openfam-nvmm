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

#ifndef _NVMM_ROOT_SHELF_H_
#define _NVMM_ROOT_SHELF_H_

#include <string>

#include "nvmm/error_code.h"

namespace nvmm {

/*
  A shelf that is used to "bootstrap" the memory manager
  It must be created before any memory manager instance is created
*/
class RootShelf
{
public:
    RootShelf() = delete; // no default
    RootShelf(std::string pathname);
    ~RootShelf();

    ErrorCode Create();
    ErrorCode Destroy();
    ErrorCode Open();
    ErrorCode Close();
    bool Exist();
    bool IsOpen();
    void *Addr();

private:
    static uint64_t const kMagicNum = 766874353; // root shelf
    static size_t const kShelfSize = 128*1024*1024; // 128MB
    std::string path_;
    int fd_;
    void *addr_;
};

} // namespace nvmm
#endif
