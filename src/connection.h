// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#ifndef SRC_CONNECTION_H_
#define SRC_CONNECTION_H_

#include <occi.h>
#include <string>
#include <sstream>
#include "./node-db/connection.h"
#include "./result.h"

namespace node_db_oracle {
class Connection : public node_db::Connection {
    public:
        Connection();
        ~Connection();
        void setTns(const std::string& iTns) throw();
        void setCharset(const std::string& charset) throw();
        void setNCharset(const std::string& charset) throw();
        bool isAlive(bool ping) throw();
        void open() throw(node_db::Exception&);
        void close();
        std::string escape(const std::string& string) const throw(node_db::Exception&);
        std::string version() const;
        node_db::Result* query(const std::string& query) const throw(node_db::Exception&);

    protected:
        std::string tns;
        std::string charset;
        std::string ncharset;

    private:
        oracle::occi::Environment* environment;
        oracle::occi::Connection* connection;
};
}

#endif  // SRC_CONNECTION_H_
