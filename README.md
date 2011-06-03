# db-oracle: Oracle database bindings for Node.js #

For detailed information about this and other Node.js
database bindings visit the [Node.js db-mysql homepage] [homepage].

## INSTALL ##

Before proceeding with installation, you need to have the 
[OCI instant client] [oci] [libraries] [oci-lib] and [include files] [oci-inc]. 
In order for the installation script to locate them properly, you'll 
need to set the OCI_INCLUDE_DIR and OCI_LIB_DIR environment variables. 
For example:

    $ export OCI_INCLUDE_DIR=/opt/instantclient/sdk/include/
    $ export OCI_LIB_DIR=/opt/instantclient

Once the environment variables are set, install with npm:

    $ npm install db-oracle

## QUICK START ##

    var oracle = require('db-oracle');
    new oracle.Database({
        hostname: 'localhost',
        user: 'root',
        password: 'password',
        database: 'node'
    }).connect(function(error) {
        if (error) {
            return console.log("CONNECTION ERROR: " + error);
        }

        this.query().select('*').from('users').execute(function(error, rows) {
            if (error) {
                return console.log('ERROR: ' + error);
            }
            console.log(rows.length + ' ROWS');
        });
    });

## LICENSE ##

This module is released under the [MIT License] [license].

[homepage]: http://nodejsdb.org/db-mysql
[license]: http://www.opensource.org/licenses/mit-license.php
[oci]: http://www.oracle.com/technetwork/database/features/oci/index.html
[oci-lib]: http://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html
[oci-inc]: http://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html
