#pragma once

#include <cstdint>
#include <unordered_map>
#include "user_session.h"
#include "common/logger/log.h"

template <class Key, class UserSession>
class session_map
{
public:
    static_assert(std::is_base_of<user_session_base, UserSession>::value, "UserSession parent class is not user_session_base");
	
    using user_session_ptr = std::shared_ptr<UserSession>;
public:
    bool add_map(const Key& key, const user_session_ptr user_session)
    {
        auto iter = map_.find(key);
        LOG_PROCESS_ERROR_RET(iter == map_.end(), false);

        return map_.insert(std::make_pair(key, user_session)).second;
    }

    bool remove_map(const Key& key)
    {
        auto iter = map_.find(key);
        LOG_PROCESS_ERROR_RET(iter != map_.end(), false);

        map_.erase(iter);
        return true;
    }

    user_session_ptr find(const Key& key)
    {
        auto iter = map_.find(key);
        return iter != map_.end() ? iter->second : 0;
    }
    
private:
    std::unordered_map<Key, user_session_ptr> map_;
};