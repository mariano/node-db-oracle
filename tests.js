/* Escape & Query building tests */

var settings = JSON.parse(require('fs').readFileSync('./tests-settings.json','utf8'));

var client = require("./db-oracle");
var tests = require("./lib/node-db/tests.js").get(function() {
    new client.Database(settings).connect(function(err) {
        if (err) {
            throw new Error('Could not connect to test DB');
        }
        callback(this);
    });
}, '"');

for(var test in tests) {
    exports[test] = tests[test];
}

delete exports["Client"]["escape()"];
delete exports["Query"]["select markers"];
delete exports["Query"]["insert markers"];
delete exports["Query"]["select values"];
