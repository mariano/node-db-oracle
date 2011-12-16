// Copyright 2011 Mariano Iglesias <mgiglesias@gmail.com>
#include "./oracle.h"

v8::Persistent<v8::FunctionTemplate> node_db_oracle::Oracle::constructorTemplate;

node_db_oracle::Oracle::Oracle(): node_db::Binding() {
    this->connection = new node_db_oracle::Connection();
    assert(this->connection);
}

node_db_oracle::Oracle::~Oracle() {
    if (this->connection != NULL) {
        delete this->connection;
    }
}

void node_db_oracle::Oracle::Init(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);

    constructorTemplate = v8::Persistent<v8::FunctionTemplate>::New(t);
    constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    node_db::Binding::Init(target, constructorTemplate);

    target->Set(v8::String::NewSymbol("Oracle"), constructorTemplate->GetFunction());
}

v8::Handle<v8::Value> node_db_oracle::Oracle::New(const v8::Arguments& args) {
    v8::HandleScope scope;

    node_db_oracle::Oracle* binding = new node_db_oracle::Oracle();
    if (binding == NULL) {
        THROW_EXCEPTION("Can't create client object")
    }

    if (args.Length() > 0) {
        ARG_CHECK_OBJECT(0, options);

        v8::Handle<v8::Value> set = binding->set(args[0]->ToObject());
        if (!set.IsEmpty()) {
            return scope.Close(set);
        }
    }

    binding->Wrap(args.This());

    return scope.Close(args.This());
}

v8::Handle<v8::Value> node_db_oracle::Oracle::set(const v8::Local<v8::Object> options) {
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, hostname);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, user);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, password);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, tnsname);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, sid);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_STRING(options, database);
    ARG_CHECK_OBJECT_ATTR_OPTIONAL_UINT32(options, port);

    node_db_oracle::Connection* connection = static_cast<node_db_oracle::Connection*>(this->connection);

    v8::String::Utf8Value hostname(options->Get(hostname_key)->ToString());
    v8::String::Utf8Value user(options->Get(user_key)->ToString());
    v8::String::Utf8Value password(options->Get(password_key)->ToString());
    v8::String::Utf8Value database(options->Get(database_key)->ToString());
    
    v8::String::Utf8Value sid(options->Get(sid_key)->ToString());
    v8::String::Utf8Value tnsname(options->Get(tnsname_key)->ToString());


    if (options->Has(hostname_key)) {
        connection->setHostname(*hostname);
    }

    if (options->Has(user_key)) {
        connection->setUser(*user);
    }

    if (options->Has(password_key)) {
        connection->setPassword(*password);
    }

    if (options->Has(tnsname_key)) {
        connection->setTNSName(*tnsname);
    }
    
    if (options->Has(sid_key)) {
        connection->setSid(*sid);
    }

    if (options->Has(database_key)) {
        connection->setDatabase(*database);
    }

    if (options->Has(port_key)) {
        connection->setPort(options->Get(port_key)->ToInt32()->Value());
    }

    return v8::Handle<v8::Value>();
}

v8::Persistent<v8::Object> node_db_oracle::Oracle::createQuery() const {
    v8::Persistent<v8::Object> query(
        node_db_oracle::Query::constructorTemplate->GetFunction()->NewInstance());
    return query;
}
