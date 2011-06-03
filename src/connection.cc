// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./connection.h"

node_db_oracle::Connection::Connection()
    : environment(NULL),
      connection(NULL) {
    this->port = 1521;
    this->quoteName = '"';
    this->environment = oracle::occi::Environment::createEnvironment(oracle::occi::Environment::THREADED_MUTEXED);
    if (this->environment == NULL) {
        throw node_db::Exception("Cannot create environment");
    }
}

node_db_oracle::Connection::~Connection() {
    this->close();
    oracle::occi::Environment::terminateEnvironment(this->environment);
}

bool node_db_oracle::Connection::isAlive(bool ping) throw() {
    if (ping && this->alive) {
    }
    return this->alive;
}

void node_db_oracle::Connection::open() throw(node_db::Exception&) {
    this->close();

    std::ostringstream connection;
    connection << "//" << this->hostname << ":" << this->port << "/" << this->database;
    try {
        this->connection = this->environment->createConnection(this->user, this->password, connection.str());
        this->alive = true;
    } catch(oracle::occi::SQLException& exception) {
        throw node_db::Exception(exception.getMessage());
    }
}

void node_db_oracle::Connection::close() {
    if (this->alive) {
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
