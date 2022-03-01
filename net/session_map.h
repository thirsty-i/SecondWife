#pragma once

#include <cstdint>
#include <unordered_map>
#include "user_session.h"
#include "logger/log.h"

template <class Key, class UserSession>
class session_map
{
public:
    static_assert(std::is_base_of<UserSession, user_session_base>::value, "UserSession parent class is not user_session_base");

    using pointer = std::shared_ptr<session_map<Key, UserSession>>;
public:
    bool add_map(const Key& key, const user_session_ptr_t user_session)
    {
        auto iter = map_.find(key);
        LOG_PROCESS_ERROE_RET(iter == map_.end(), false);

        map_.insert(std::make_pair(key, user_session));
    }

    bool remove_map(const Key& key)
    {
        auto iter = map_.find(key);
        LOG_PROCESS_ERROE_RET(iter != map_.end(), false);

        map_.erase(iter);
    }

    user_session_ptr_t find(const Key& key)
    {
        auto iter = map_.find(key);
        return user_session_ptr_t { iter != map_.end() ? iter->second : 0};
    }
    
private:
    using user_session_ptr_t = UserSession::pointer;
    std::unordered_map<Key, user_session_ptr_t> map_;
};