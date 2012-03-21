# db-oracle: Oracle database bindings for Node.js #

For detailed information about this and other Node.js
database bindings visit the [Node.js db-mysql homepage] [homepage].

## INSTALL ##

### OCI libraries ###

Before proceeding with installation, you need to have the 
[OCI instant client] [oci] [libraries] [oci-lib] and [include files] [oci-inc].
For example, you download the `instantclient-basic-linux.x64-11.2.0.3.0.zip` 
library file, and the `instantclient-sdk-linux.x64-11.2.0.3.0.zip` SDK file, 
and save them in your `$HOME/Downloads` directory. You would then uncompress 
both files, and move the generated directory to your `/opt` path:

```bash
$ cd $HOME/Downloads
$ unzip instantclient-basic-linux.x64-11.2.0.3.0.zip
$ unzip instantclient-sdk-linux.x64-11.2.0.3.0.zip 
$ sudo mv instantclient_11_2/ /opt/instantclient
```

After uncompressing you will probably need to create symbolink links:

```bash
$ cd /opt/instantclient
$ sudo ln -s libocci.so.11.1 libocci.so
$ sudo ln -s libclntsh.so.11.1 libclntsh.so
```

You will also need `libaio`. In **Arch Linux** this can easily be installed with:

```bash
$ sudo pacman -S libaio
```

On **Debian** based distros:

```bash
$ sudo apt-get install libaio
```

### Configuring OCI ###

Once you have the library and include files installed, and in order for the 
installation script to locate them properly, you'll need to set the 
`OCI_INCLUDE_DIR` and `OCI_LIB_DIR` environment variables. For example:

```bash
$ export OCI_INCLUDE_DIR=/opt/instantclient/sdk/include/
$ export OCI_LIB_DIR=/opt/instantclient
```

### Install ###

Once the environment variables are set, install with npm:

```bash
$ npm install db-oracle
```

## QUICK START ##

```javascript
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
```

## LICENSE ##

This module is released under the [MIT License] [license].

[homepage]: http://nodejsdb.org/db-mysql
[license]: http://www.opensource.org/licenses/mit-license.php
[oci]: http://www.oracle.com/technetwork/database/features/oci/index.html
[oci-lib]: http://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html
[oci-inc]: http://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html
