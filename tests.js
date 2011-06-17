/* Escape & Query building tests */

var client = require("./db-oracle");
var tests = require("./lib/node-db/tests.js").get(function() {
    return new client.Database();
}, '"');

for(var test in tests) {
    exports[test] = tests[test];
}

delete exports["Client"]["escape()"];
delete exports["Query"]["select markers"];
delete exports["Query"]["insert markers"];
delete exports["Query"]["select values"];
