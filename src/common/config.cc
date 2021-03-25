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

#include <cstddef> // size_t
#include <map>
#include <iostream>
#include <fstream>
#include <string>

#include "shelf_mgmt/shelf_manager.h"
#include "config.h"
#include "yaml-cpp/yaml.h"

namespace nvmm {

Config config;

void Config::PrintConfigFile(std::string path) {
    YAML::Node file = YAML::LoadFile(path);
    if(file.IsNull()) {
        std::cout << "Cannot find the NVMM config file at " << path << std::endl;
        return;
    }

    YAML::Node const &nvmm = file["nvmm"];
    if(nvmm.IsNull()) {
        std::cout << "Invalid NVMM config format" << std::endl;
        return;
    }

    for (YAML::const_iterator it=nvmm.begin(); it!=nvmm.end(); it++) {
        std::string key = it->first.as<std::string>();
        YAML::Node const &config = nvmm[key];
        switch(config.Type()) {
        case YAML::NodeType::Scalar: {
            std::string value = it->second.as<std::string>();
            std::cout << key << ": " << value << std::endl;
            break;
        }
        case YAML::NodeType::Sequence: {
            std::cout << "Sequence..." << std::endl;
            break;
        }
        case YAML::NodeType::Map: {
            std::cout << key << ": " << std::endl;
            for (YAML::const_iterator j=config.begin(); j!=config.end(); j++) {
                std::cout << " " << j->first.as<std::string>() << ": " << j->second.as<std::string>() << std::endl;
            }
            break;
        }
        case YAML::NodeType::Null:
        case YAML::NodeType::Undefined:
        default:
            std::cout << "Default..." << std::endl;
            break;
        }
    }
}

int Config::LoadConfigFile(std::string path) {
    if(path.empty()) return 0;

    int ret=0;

    YAML::Node const &file = YAML::LoadFile(path);
    if(file.IsNull()) {
        std::cout << "Cannot find the NVMM config file at " << path << std::endl;
        return -1;
    }

    YAML::Node const &nvmm = file["nvmm"];
    if(nvmm.IsNull()) {
        std::cout << "Invalid NVMM config format" << std::endl;
        return -1;
    }

    if(nvmm["shelf_base"]) {
        ShelfBase=nvmm["shelf_base"].as<std::string>();
    }
    if(nvmm["shelf_user"]) {
        ShelfUser=nvmm["shelf_user"].as<std::string>();
    }

    Setup();
    return ret;
}

void Config::Print() {
    std::cout << "NVMM Config " << std::endl;
    std::cout << "- shelf_base: " << ShelfBase << std::endl;
    std::cout << "- shelf_user: " << ShelfUser << std::endl;
}


void Config::Setup() {
    RootShelfPath = ShelfBase+"/"+ShelfUser+"_NVMM_ROOT";
    EpochShelfPath = ShelfBase+"/"+ShelfUser+"_NVMM_EPOCH";

    // TODO: another global variable...
    ShelfManager::Reset();
}

} // namespace nvmm
