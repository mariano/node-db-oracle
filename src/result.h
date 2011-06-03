// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#ifndef SRC_RESULT_H_
#define SRC_RESULT_H_

#include <cstring>
#include <string>
#include <stdexcept>
#include <occi.h>
#include "./node-db/exception.h"
#include "./node-db/result.h"

namespace node_db_oracle {
class Result : public node_db::Result {
    public:
        class Column : public node_db::Result::Column {
            public:
                explicit Column(oracle::occi::MetaData& column);
                ~Column();
                bool isBinary() const;
                std::string getName() const;
                node_db::Result::Column::type_t getType() const;

            protected:
                std::string name;
                type_t type;
                bool binary;
        };

        explicit Result(oracle::occi::Statement* statement) throw(node_db::Exception&);
        ~Result();
        void release() throw();
        bool hasNext() const throw();
        char** next() throw(node_db::Exception&);
        unsigned long* columnLengths() throw(node_db::Exception&);
        uint64_t index() const throw(std::out_of_range&);
        Column* column(uint16_t i) const throw(std::out_of_range&);
        uint16_t columnCount() const throw();
        uint64_t affectedCount() const throw();
        bool isBuffered() const throw();
        bool isEmpty() const throw();

    protected:
        Column** columns;
        uint16_t totalColumns;
        uint64_t rowNumber;
        bool empty;

        char** row(unsigned long* rowColumnLengths) throw(node_db::Exception&);
        void free() throw();

    private:
        oracle::occi::Statement* statement;
        oracle::occi::ResultSet* resultSet;
        unsigned long* previousColumnLengths;
        char** previousRow;
        unsigned long* nextColumnLengths;
        char** nextRow;

        void freeRow(char** row) throw();
};
}

#endif  // SRC_RESULT_H_
