#include "json_tree.h"
#include "json_value.h"

#include <iostream>
#include <regex>

bool
is_os (std::string os)
{
    if (os == "\"linux\"" || os == "\"darwin\"" || os == "\"freebsd\"") {
        return true;
    }
    return false;
}

bool
is_architecture (std::string arch)
{
    if (arch == "\"386\"" || arch == "\"amd64\"" || arch == "\"arm\"") {
        return true;
    }
    return false;
}

bool
is_hexadecimal_256bits (std::string s)
{
    std::regex hexastring_64 ("^\"[0-9a-fA-F]{64}\"$");

    if (std::regex_match (s, hexastring_64)) {
        return true;
    }

    return false;
}

bool
is_iso8601_datetime (std::string s)
{
    std::regex date ("^$");

    // XXX
    return true;

    if (std::regex_match (s, date)) {
        return true;
    }
    return false;
}

bool
is_user_group (std::string s)
{
  std::regex uid ("^\"[0-9]+\"$");
  std::regex user ("^\"[a-zA-Z_][a-zA-Z0-9_-]*\"$");
  std::regex user_group ("^\"[a-zA-Z_][a-zA-Z0-9_-]*:[a-zA-Z_][a-zA-Z0-9_-]*\"$");
  std::regex uid_guid ("^\"[0-9]+:[0-9]+\"$");
  std::regex uid_group ("^\"[0-9]+:[a-zA-Z_][a-zA-Z0-9_-]*\"$");
  std::regex user_gid ("^\"[a-zA-Z_][a-zA-Z0-9_-]*:[0-9]+\"$");

  if (std::regex_match (s, uid)) {
      return true;
  }

  if (std::regex_match (s, user)) {
      return true;
  }

  if (std::regex_match (s, user_group)) {
      return true;
  }

  if (std::regex_match (s, uid_guid)) {
      return true;
  }

  if (std::regex_match (s, uid_group)) {
      return true;
  }

  if (std::regex_match (s, user_gid)) {
      return true;
  }

  return false;
}

bool
is_port_spec (std::string s)
{
    std::regex port ("^\"([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])\"$");
    std::regex udp ("^\"([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])/udp\"$");
    std::regex tcp ("^\"([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])/tcp\"$");

    if (std::regex_match (s, port)) {
        return true;
    }

    if (std::regex_match (s, udp)) {
        return true;
    }

    if (std::regex_match (s, tcp)) {
        return true;
    }

    return false;
}

bool
is_env_spec (std::string s)
{
    std::regex env ("\"^[A-Z]+=[^=]*$\"");

    if (std::regex_match (s, env)) {
        return true;
    }

    return false;
}

/*
// XXX ignore extra parameters or not?

//is_hexadecimal_256bits
is_iso8601_datetime
//is_architecture
//is_os
is_integer // XXX probably not necessary; change syntax analysis?
// XXX config can be null
//is_user_group
//is_port_spec
//is_env_spec

*/

bool
is_valid_env (std::shared_ptr< json_value > val_)
{
    return true;
}

bool
is_valid_exposedports (std::shared_ptr< json_value > val_)
{
    return true;
}


bool
is_valid_config (std::shared_ptr< json_value > val_)
{
    bool ret = true;

    /*
    * User -> J_STRING, is_user_group()
    * Memory -> J_INT
    * MemorySwap -> J_INT
    * CpuShares -> J_INT
    * ExposedPorts -> J_OBJECT, is_valid_exposedports()
    * Env -> J_ARRAY, is_valid_env()
    * Entrypoint, J_ARRAY
    * Cmd, J_ARRAY
    * Volumes, J_OBJECT
    * WorkingDir, J_OBJECT
    */

    auto object = std::static_pointer_cast< json_object >(val_);

    // validate Env
    auto Env = object->find("\"Env\"");
    if (Env != nullptr) {
        if (Env->jtype() != json_type::J_ARRAY) {
            std::cerr << "Env isn't array" << std::endl;
            ret = false;
        } else {
            if (!is_valid_env(Env)) {
                std::cerr << "Env isn't valid env array" << std::endl;
                ret = false;
            }
        }
    }

    // validate ExposedPorts
    auto ExposedPorts = object->find("\"ExposedPorts\"");
    if (ExposedPorts != nullptr) {
        if (ExposedPorts->jtype() != json_type::J_OBJECT) {
            std::cerr << "ExposedPorts isn't object" << std::endl;
            ret = false;
        } else {
            if (!is_valid_exposedports(ExposedPorts)) {
                std::cerr << "ExposedPorts isn't valid exposedports object" << std::endl;
                ret = false;
            }
        }
    }

    // validate WorkingDir
    auto WorkingDir = object->find("\"WorkingDir\"");
    if (WorkingDir != nullptr) {
        if (WorkingDir->jtype() != json_type::J_STRING) {
            std::cerr << "WorkingDir isn't object" << std::endl;
            ret = false;
        }
    }

    // validate Volumes
    auto Volumes = object->find("\"Volumes\"");
    if (Volumes != nullptr) {
        if (Volumes->jtype() != json_type::J_OBJECT) {
            std::cerr << "Volumes isn't object" << std::endl;
            ret = false;
        }
    }

    // validate Cmd
    auto Cmd = object->find("\"Cmd\"");
    if (Cmd != nullptr) {
        if (Cmd->jtype() != json_type::J_ARRAY) {
            std::cerr << "Cmd isn't array" << std::endl;
            ret = false;
        }
    }

    // validate Entrypoint
    auto Entrypoint = object->find("\"Entrypoint\"");
    if (Entrypoint != nullptr) {
        if (Entrypoint->jtype() != json_type::J_ARRAY) {
            std::cerr << "Entrypoint isn't array" << std::endl;
            ret = false;
        }
    }

    // validate CpuShares
    auto CpuShares = object->find("\"CpuShares\"");
    if (CpuShares != nullptr) {
        if (CpuShares->jtype() != json_type::J_INT) {
            std::cerr << "CpuShares isn't int" << std::endl;
            ret = false;
        }
    }

    // validate MemorySwap
    auto MemorySwap = object->find("\"MemorySwap\"");
    if (MemorySwap != nullptr) {
        if (MemorySwap->jtype() != json_type::J_INT) {
            std::cerr << "MemorySwap isn't int" << std::endl;
            ret = false;
        }
    }

    // validate Memory
    auto Memory = object->find("\"Memory\"");
    if (Memory != nullptr) {
        if (Memory->jtype() != json_type::J_INT) {
            std::cerr << "Memory isn't int" << std::endl;
            ret = false;
        }
    }

    // validate User
    auto User = object->find("\"User\"");
    if (User != nullptr) {
        if (User->jtype() != json_type::J_STRING) {
            std::cerr << "User isn't string" << std::endl;
            ret = false;
        } else {
            if (!is_user_group(User->to_string())) {
                std::cerr << "User isn't valid user_group string" << std::endl;
                ret = false;
            }
        }
    }

    return ret;
}


bool
do_semantic_analysis (std::shared_ptr<json_value> val_)
{
    bool ret = true;

    /*
     * id -> J_STRING, is_hexadecimal_256bits()
     * created -> J_STRING, is_iso8601_datetime()
     * os -> J_STRING, is_os()
     * parent -> J_STRING, is_hexadecimal_256bits()
     * Size -> J_INT
     * architecture -> J_STRING, is_architecture()
     * author -> J_STRING
     * checksum -> J_STRING
     * config -> J_OBJECT / J_NULL, is_valid_config()
     */

    auto object = std::static_pointer_cast< json_object >(val_); // type std::shared_ptr< json_object >

    // validate id
    auto id = object->find("\"id\""); // id is of type std::shared_ptr< json_value >
    if (id != nullptr) {
        if (id->jtype() != json_type::J_STRING) {
            std::cerr << "id isn't string" << std::endl;
            ret = false;
        } else {
            // is string, check it's value
            if (!is_hexadecimal_256bits(id->to_string())) {
                std::cerr << "id isn't valid hexadecimal string" << std::endl;
                ret = false;
            }
        }
    } else {
        std::cerr << "id doesn't exist" << std::endl;
        ret = false;
    }

    // validate created
    auto created = object->find("\"created\"");
    if (created != nullptr) {
        if (created->jtype() != json_type::J_STRING) {
            std::cerr << "created isn't string" << std::endl;
            ret = false;
        } else {
            // is string, check it's value
            if (!is_iso8601_datetime(created->to_string())) {
                std::cerr << "created isn't valid datetime string" << std::endl;
                ret = false;
            }
        }
    } else {
        std::cerr << "created doesn't exist" << std::endl;
        ret = false;
    }

    // validate os
    auto os = object->find("\"os\"");
    if (os != nullptr) {
        if (os->jtype() != json_type::J_STRING) {
            std::cerr << "os isn't string" << std::endl;
            ret = false;
        } else {
            // is string, check it's value
            if (!is_os(os->to_string())) {
                std::cerr << "os isn't valid os string" << std::endl;
                ret = false;
            }
        }
    } else {
        std::cerr << "os doesn't exist" << std::endl;
        ret = false;
    }

    // validate parent
    auto parent = object->find("\"parent\"");
    if (parent != nullptr) {
        if (parent->jtype() != json_type::J_STRING) {
            std::cerr << "parent isn't string" << std::endl;
            ret = false;
        } else {
            // is string, check it's value
            if (!is_hexadecimal_256bits(parent->to_string())) {
                std::cerr << "parent isn't valid hexadecimal string" << std::endl;
                ret = false;
            }
        }
    } else {
        std::cerr << "parent doesn't exist" << std::endl;
        ret = false;
    }

    // validate Size
    auto size = object->find("\"Size\"");
    if (size != nullptr) {
        if (size->jtype() != json_type::J_INT) {
            std::cerr << "size isn't int" << std::endl;
            ret = false;
        }
    } else {
        std::cerr << "size doesn't exist" << std::endl;
        ret = false;
    }

    // validate architecture
    auto architecture = object->find("\"architecture\"");
    if (architecture != nullptr) {
        if (architecture->jtype() != json_type::J_STRING) {
            std::cerr << "architecture isn't string" << std::endl;
            ret = false;
        } else {
            // is string, check it's value
            if (!is_architecture(architecture->to_string())) {
                std::cerr << "architecture isn't valid architecture string" << std::endl;
                ret = false;
            }
        }
    } else {
        std::cerr << "architecture doesn't exist" << std::endl;
        ret = false;
    }

    // validate author
    auto author = object->find("\"author\"");
    if (author != nullptr) {
        if (author->jtype() != json_type::J_STRING) {
            std::cerr << "author isn't string" << std::endl;
            ret = false;
        }
    } else {
        std::cerr << "author doesn't exist" << std::endl;
        ret = false;
    }

    // validate checksum
    auto checksum = object->find("\"checksum\"");
    if (checksum != nullptr) {
        if (checksum->jtype() != json_type::J_STRING) {
            std::cerr << "checksum isn't string" << std::endl;
            ret = false;
        }
    } else {
        std::cerr << "checksum doesn't exist" << std::endl;
        ret = false;
    }

    // validate config
    auto config = object->find("\"config\"");
    if (config != nullptr) {
        if (config->jtype() == json_type::J_NULL) {
            ; // can be null, is valid
        } else if (config->jtype() == json_type::J_OBJECT) {
            if (!is_valid_config(config)) {
                std::cerr << "config isn't valid" << std::endl;
                ret = false;
            }
        } else {
            std::cerr << "config isn't valid type" << std::endl;
            ret = false;
        }
    } else {
        std::cerr << "config doesn't exist" << std::endl;
        ret = false;
    }

    return ret;
}
