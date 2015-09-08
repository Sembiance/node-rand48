"use strict";
var Rand48 = require("./index").Rand48;

var r = new Rand48();   		// Creates a new Rand48 object, with no seed
var r2 = new Rand48(43912, 23555, 12660);     // Creates a new Rand48 object with a seed of 43912, 23555, 12660
var r3 = new Rand48();

console.log(r.rand(1, 10));     // Requests a number between 1 and 10 inclusive
console.log(r2.rand(1, 10));     // Produces the number 10, random based on seed
//console.log(r3.rand(0, -1));	// Should NOT cause a floating point exception, should throw an exception
