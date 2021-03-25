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

#include <unistd.h> // sleep
#include <list>
#include <random>
#include <limits>
#include <vector>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>
#include "nvmm/memory_manager.h"
#include "test_common/test.h"

using namespace nvmm;

TEST(EpochZoneHeapResize, MultiProcessResize)
{
#ifdef LFSWORKAROUND
    int const process_count = 4;
#else
    int const process_count = 32;
#endif
    pid_t pid[process_count];

    size_t size = 1024*1024LLU; // 1 MB

    PoolId pool_id = 1;

    for (int i=0; i< process_count; i++)
    {

        pid[i] = fork();
        if (pid[i]==0) {
           ErrorCode ret;
           MemoryManager *mm = MemoryManager::GetInstance();
           Heap *heap = NULL;
           if(i==0)
           {
              // create a heap
              EXPECT_EQ(ID_NOT_FOUND, mm->FindHeap(pool_id, &heap));
              EXPECT_EQ(NO_ERROR, mm->CreateHeap(pool_id, size));
              EXPECT_EQ(ID_FOUND, mm->CreateHeap(pool_id, size));
              std::cout<<"Initial heap size : "<<size<<std::endl;
           }

           // Loop until process 0 creates heap.
           do {
                 ret = mm->FindHeap(pool_id, &heap);
           } while(ret == ID_NOT_FOUND || heap == NULL);

           ret = heap->Open();
           if(ret != NO_ERROR) {
              exit(1);
           }
           do {
              size_t new_size;
              new_size = heap->Size() + size * (process_count%4 + 1);

              if (heap->Size() > 4 * process_count * size)
                 exit(0);

              ret = heap->Resize(new_size);
              if(ret == HEAP_BUSY) {
                 //continue
              }
              else if(ret == NO_ERROR) {
                 std::cout<<"**For process "<<i<<" ret : "<<ret <<", Size = "<<heap->Size()<<std::endl;
                 sleep(2);
              }
              else {
                 std::cout<<"%% Resize Error : "<<ret<<std::endl;
                 exit(1);
              }
           } while(heap->Size() < (2 * process_count * size));
           if(ret != NO_ERROR && ret != HEAP_BUSY) {
               heap->Close();
               exit(1);
           }
           heap->Close();
           delete heap;
           exit(0);
        }
        else {
             // parent process...
             // continue
        }
    }
    for (int i=0; i< process_count; i++)
    {
        int status;
        waitpid(pid[i], &status, 0);
        EXPECT_EQ(status, 0);
    }
    MemoryManager *mm = MemoryManager::GetInstance();
    EXPECT_EQ(NO_ERROR, mm->DestroyHeap(pool_id));
    EXPECT_EQ(ID_NOT_FOUND, mm->DestroyHeap(pool_id));
}

int main(int argc, char **argv) {
    InitTest(nvmm::trace, false);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

