/*!
 * Copyright by Mariano Iglesias
 *
 * See license text in LICENSE file
 */

/**
 * Require bindings native binary
 *
 * @ignore
 */
var binding = require("./build/Release/oracle_bindings");
exports.Database = binding.Oracle;
exports.Query = binding.Query;
