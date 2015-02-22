Description
===========

Provides a seedable uniformly distributed random number generator for [node.js](http://nodejs.org/) by binding to the POSIX nrand48_r function.

NOTE: Only tested under Linux

2.0 WARNING: Seed behavior changed when only sending one or two seed numbers. Numbers will differ from node-rand48 versions earlier than 2.0


Installation with NPM
=====================

	npm install rand48


Usage
=====
<pre>
var Rand48 = require("rand48").Rand48;

var r = new Rand48();	// Creates a new Rand48 object, with no seed
r.rand(1, 10);			// Requests a number between 1 and 10 inclusive

var r2 = new Rand48(43912);		// Creates a new Rand48 object with a seed of 43912
r2.rand(1, 10);					// Produces the number 10, random based on seed

var r2 = new Rand48(392, 129, 18984);	// Creates a new Rand48 object with a 3 seed numbers (max allowed)
r2.rand(1, 10);							// Produces the number 10, random based on seed
</pre>

Note that the constructor to Rand48 takes up to 3 unsigned shorts (0 to 65,535) as a single seed

Manual Installation
===================

    git clone "https://github.com/Sembiance/node-rand48.git"
    cd node-rand48
    node-waf configure build
	# Now can in JavaScript: require("./build/default/rand48") 
