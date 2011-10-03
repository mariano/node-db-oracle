// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./result.h"

node_db_oracle::Result::Column::Column(oracle::occi::MetaData& column) {
    this->binary = false;
    this->name = column.getString(oracle::occi::MetaData::ATTR_NAME);

    switch (column.getInt(oracle::occi::MetaData::ATTR_DATA_TYPE)) {
        case oracle::occi::OCCI_TYPECODE_SMALLINT:
            this->type = (column.getInt(oracle::occi::MetaData::ATTR_DATA_SIZE) == 1 ? BOOL : INT);
            break;
        case oracle::occi::OCCI_TYPECODE_OCTET:
        case oracle::occi::OCCI_TYPECODE_INTEGER:
            this->type = INT;
            break;
        case oracle::occi::OCCI_TYPECODE_REAL:
        case oracle::occi::OCCI_TYPECODE_DOUBLE:
        case oracle::occi::OCCI_TYPECODE_BDOUBLE:
        case oracle::occi::OCCI_TYPECODE_FLOAT:
        case oracle::occi::OCCI_TYPECODE_BFLOAT:
        case oracle::occi::OCCI_TYPECODE_NUMBER:
        case oracle::occi::OCCI_TYPECODE_DECIMAL:
            this->type = NUMBER;
            break;
        case oracle::occi::OCCI_TYPECODE_DATE:
            this->type = DATETIME;
            break;
        case oracle::occi::OCCI_TYPECODE_CLOB:
        case oracle::occi::OCCI_TYPECODE_BLOB:
            this->type = TEXT;
            this->binary = true;
            break;
        default:
            this->type = STRING;
            break;
    }
}

node_db_oracle::Result::Column::~Column() {
}

bool node_db_oracle::Result::Column::isBinary() const {
    return this->binary;
}

std::string node_db_oracle::Result::Column::getName() const {
    return this->name;
}

node_db::Result::Column::type_t node_db_oracle::Result::Column::getType() const {
    return this->type;
}

node_db_oracle::Result::Result(oracle::occi::Statement* statement) throw(node_db::Exception&)
    : columns(NULL),
    totalColumns(0),
    rowNumber(0),
    empty(true),
    statement(statement),
    resultSet(NULL),
    previousColumnLengths(NULL),
    previousRow(NULL),
    nextColumnLengths(NULL),
    nextRow(NULL) {

    try {
        try {
            oracle::occi::Statement::Status status = this->statement->execute();
            if (status == oracle::occi::Statement::RESULT_SET_AVAILABLE) {
                this->resultSet = this->statement->getResultSet();
            } else if (status != oracle::occi::Statement::UPDATE_COUNT_AVAILABLE) {
                throw node_db::Exception("Cannot handle statement statuses other than RESULT_SET_AVAILABLE or UPDATE_COUNT_AVAILABLE");
            }
        } catch(oracle::occi::SQLException& ex) {
            throw node_db::Exception(ex.getMessage());
        }

        if (this->resultSet != NULL) {
            std::vector<oracle::occi::MetaData> columns = this->resultSet->getColumnListMetaData();

            this->empty = false;
            this->totalColumns = columns.size();

            this->previousColumnLengths = new unsigned long[this->totalColumns];
            if (this->previousColumnLengths == NULL) {
                throw node_db::Exception("Could not create buffer for column lengths");
            }

            this->nextColumnLengths = new unsigned long[this->totalColumns];
            if (this->nextColumnLengths == NULL) {
                throw node_db::Exception("Could not create buffer for column lengths");
            }


            this->columns = new Column*[this->totalColumns];
            if (this->columns == NULL) {
                throw node_db::Exception("Could not allocate storage for columns");
            }

            for (uint16_t i = 0; i < this->totalColumns; i++) {
                this->columns[i] = new Column(columns[i]);
                if (this->columns[i] == NULL) {
                    this->totalColumns = i;
                    throw node_db::Exception("Could not allocate storage for column");
                }
            }

            this->nextRow = this->row(this->nextColumnLengths);
        }
    } catch(...) {
        this->free();
        throw;
    }
}

node_db_oracle::Result::~Result() {
    this->free();
}

void node_db_oracle::Result::free() throw() {
    this->release();

    this->freeRow(this->previousRow);
    this->freeRow(this->nextRow);

    if (this->previousColumnLengths != NULL) {
        delete [] this->previousColumnLengths;
    }

    if (this->nextColumnLengths != NULL) {
        delete [] this->nextColumnLengths;
    }

    if (this->columns != NULL) {
        for (uint16_t i = 0; i < this->totalColumns; i++) {
            delete this->columns[i];
        }
        delete [] this->columns;
    }
}

void node_db_oracle::Result::release() throw() {
    if (this->resultSet != NULL) {
        this->statement->closeResultSet(this->resultSet);
        this->resultSet = NULL;
    }

    if (this->statement != NULL) {
        this->statement->getConnection()->terminateStatement(this->statement);
        this->statement = NULL;
    }
}

void node_db_oracle::Result::freeRow(char** row) throw() {
    if (row != NULL) {
        for (uint16_t i = 0; i < this->totalColumns; i++) {
            delete [] row[i];
        }

        delete [] row;
    }
}

bool node_db_oracle::Result::hasNext() const throw() {
    return (this->nextRow != NULL);
}

char** node_db_oracle::Result::next() throw(node_db::Exception&) {
    this->freeRow(this->previousRow);

    if (this->nextRow == NULL) {
        return NULL;
    }

    this->rowNumber++;
    this->previousRow = this->nextRow;

    for(uint16_t i=0; i < this->totalColumns; i++) {
        this->previousColumnLengths[i] = this->nextColumnLengths[i];
    }
    this->nextRow = this->row(this->nextColumnLengths);

    return this->previousRow;
}

unsigned long* node_db_oracle::Result::columnLengths() throw(node_db::Exception&) {
    return this->previousColumnLengths;
}

char** node_db_oracle::Result::row(unsigned long* rowColumnLengths) throw(node_db::Exception&) {
    if (this->resultSet == NULL) {
        throw node_db::Exception("No ResultSet");
    } else if (!this->resultSet->next()) {
        return NULL;
    }

    uint16_t c=0;
    char** row;
    try {
        row = new char*[this->totalColumns];
        if (row == NULL) {
            throw node_db::Exception("Could not allocate buffer for row");
        }

        for (c=0; c < this->totalColumns; c++) {
            if (this->columns[c]->isBinary()) {
                oracle::occi::Blob blob = this->resultSet->getBlob(c + 1);
                rowColumnLengths[c] = blob.length();

                row[c] = new char[rowColumnLengths[c]];
                if (row[c] == NULL) {
                    throw node_db::Exception("Could not allocate buffer for row column");
                }

                blob.read(rowColumnLengths[c], (unsigned char*) row[c], rowColumnLengths[c]);
            } else {
                std::string string;
                if (this->columns[c]->getType() == Column::DATETIME) {
                    oracle::occi::Date date = this->resultSet->getDate(c + 1);
                    if (!date.isNull())
                        string = date.toText("YYYY-MM-DD HH:II:SS");
                    }
                } else {
                    string = this->resultSet->getString(c + 1);
                }

                rowColumnLengths[c] = string.length();
                if (rowColumnLengths[c] == 0) {
                    row[c] = NULL;
                    continue;
                }

                row[c] = new char[rowColumnLengths[c]];
                if (row[c] == NULL) {
                    throw node_db::Exception("Could not allocate buffer for row column");
                }

                memcpy(row[c], string.c_str(), rowColumnLengths[c]);
            }
        }
    } catch(...) {
        if (row != NULL) {
            for (uint16_t i=0; i < c; i++) {
                if (row[i] != NULL) {
                    delete [] row[i];
                }
            }
            delete [] row;
        }
        throw;
    }

    return row;
}

uint64_t node_db_oracle::Result::index() const throw(std::out_of_range&) {
    if (this->rowNumber == 0) {
        throw std::out_of_range("Not standing on a row");
    }
    return (this->rowNumber - 1);
}

node_db_oracle::Result::Column* node_db_oracle::Result::column(uint16_t i) const throw(std::out_of_range&) {
    if (i >= this->totalColumns) {
        throw std::out_of_range("Wrong column index");
    }
    return this->columns[i];
}

uint64_t node_db_oracle::Result::affectedCount() const throw() {
    return this->statement->getUpdateCount();
}

uint16_t node_db_oracle::Result::columnCount() const throw() {
    return this->totalColumns;
}

bool node_db_oracle::Result::isBuffered() const throw() {
    return false;
}

bool node_db_oracle::Result::isEmpty() const throw() {
    return this->empty;
}
