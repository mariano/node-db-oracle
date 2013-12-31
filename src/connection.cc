// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./connection.h"

node_db_oracle::Connection::Connection()
    : environment(NULL),
      connection(NULL) {
    this->port = 1521;
    this->quoteName = '"';
}

node_db_oracle::Connection::~Connection() {
    this->close();
    if (this->environment) {
        oracle::occi::Environment::terminateEnvironment(this->environment);
    }
}

void node_db_oracle::Connection::setTns(const std::string& iTns) throw() {
    this->tns = iTns;
}

void node_db_oracle::Connection::setCharset(const std::string& charset) throw() {
    this->charset = charset;
}

void node_db_oracle::Connection::setNCharset(const std::string& ncharset) throw() {
    this->ncharset = ncharset;
}

bool node_db_oracle::Connection::isAlive(bool ping) throw() {
    if (ping && this->alive) {
    }
    return this->alive;
}

void node_db_oracle::Connection::open() throw(node_db::Exception&) {
    this->close();

    if (!this->environment) {
        if (!this->charset.empty() && !this->ncharset.empty()) {
            this->environment = oracle::occi::Environment::createEnvironment(this->charset.c_str(), this->ncharset.c_str(), oracle::occi::Environment::THREADED_MUTEXED);
        } else {
            this->environment = oracle::occi::Environment::createEnvironment(oracle::occi::Environment::THREADED_MUTEXED);
        }
        if (this->environment == NULL) {
            throw node_db::Exception("Cannot create environment");
        }
    }

    std::ostringstream connection;
    if(this->tns != "")
    {
        connection << this->tns;
    }
    else
    {
        connection << "//" << this->hostname << ":" << this->port << "/" << this->database;
    }
    try {
        this->connection = this->environment->createConnection(this->user, this->password, connection.str());
        this->alive = true;
    } catch(oracle::occi::SQLException& exception) {
        throw node_db::Exception(exception.getMessage());
    }
}

void node_db_oracle::Connection::close() {
    if (this->alive && this->environment) {
        this->environment->terminateConnection(this->connection);
    }
    this->alive = false;
}

std::string node_db_oracle::Connection::escape(const std::string& string) const throw(node_db::Exception&) {
    throw node_db::Exception("This binding does not implement escape()");
}

std::string node_db_oracle::Connection::version() const {
    return this->connection->getServerVersion();
}

node_db::Result* node_db_oracle::Connection::query(const std::string& query) const throw(node_db::Exception&) {
    return new node_db_oracle::Result(this->connection->createStatement(query));
}
