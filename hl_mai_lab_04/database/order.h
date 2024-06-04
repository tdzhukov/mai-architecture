#pragma once

#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database
{
    class Order{
        private:
            long _id;
            long _user_id;
            std::vector<long> _service_ids;
            std::string _address;

        public:

            static Order fromJSON(const std::string& str);

            long               get_id() const;
            long               get_user_id() const;
            std::vector<long>  get_service_ids() const;
            std::string        get_address() const;

            long&              id();
            long&              user_id();
            std::vector<long>& service_ids();
            std::string&       address();

            static std::optional<Order> read_by_id(long id);
            static std::vector<Order> read_by_user_id(long user_id);
            void   add();
            void   update();
            void   del();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}
