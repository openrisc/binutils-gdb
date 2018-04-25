# New DSP instructions

## compare instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5 - 4|3 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|0|GRP|OP|S|

- GRP : operation groups
1. compare A and B and the results are stored in D
2. compare A and B, if all true, set F flag
3. compare A and B, if any true, set F flag

- OP : operation codes
0. eq : true if equal
1. ne : true if not equal
2. reserved
3. reserved
4. ge : true if greater or equal
5. gt : true if greater
6. le : true if less or equal
7. lt : true if less

- S : operation size
0. byte
1. half word

## add/sub instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|0|0|T|U|A|S|

- T : saturaion bit
0. normal
1. saturated

- U : signed/unsigned
0. signed
1. unsigned

- A : add/sub
0. add
1. sub

- S : operation size
0. byte
1. half word

## select instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|0|1|OP|U|OP|S|

- OP : operation codes
0. average : make average of A and B for each operation size
1. min : select min between A and B for each operation size
2. merge : make merge using A and B, {A,B} or {A,B,A,B}
3. max : select max between A and B for each operation size

- S : operation size
0. byte
1. half word

- U : sign
0. signed
1. unsigned

## pack/unpack instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|1|0|0|OP|S|

- OP : operation codes
0. pack signed
1. pack signed saturated
2. pack unsigned saturated
3. unpack signed

- S : operation size
0. byte
1. half word

## shift instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|1|0|1|OP|S|

- OP : operation codes
0. rotate left
1. shift left logical
2. shift right arithmetic
3. shift right logical

- S : operation size
0. byte
1. half word

## logical instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|1|1|0|OP|0|

- OP : operation codes
0. NAND
1. NOR
2. reserved
3. reserved


## permutation instruction

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7 - 0|
|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0x7E|

## basic multiplication instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1 - 0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|0|0|U|OP|

- OP : operation codes
0. multiply and add saturated
1. multiply and subtract saturated
2. multiply saturated
3. multiply and get higher half word

- U : signed/unsigned
0. signed
1. unsigned

## half word multiplication instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1 - 0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|0|1|U|OP|

- OP : operation codes
0. multiply lower half word of A and lower half word of B
1. multiply lower half word of A and higher half word of B
2. multiply higher half word of A and lower half word of B
3. multiply higher half word of A and higher half word of B

- U : signed/unsigned
0. signed
1. unsigned

## word and half word multiplication instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1 - 0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|1|0|U|OP|

- OP : operation codes
0. multiply A and lower half word of B and get lower word saturated
1. multiply A and higher half word of B and get lower word saturated
2. multiply A and lower half word of B and get higher word
3. multiply A and higher half word of B and get higher word

- U : signed/unsigned
0. signed
1. unsigned

## move partial instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4 - 3|2 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|1|SRC|DST|S|

move b[i] -> d[i], and other data from a
half word transfer should be aligned

- S : tranfer size
0. byte
1. half word

- SRC : source byte offset

- DST : destination byte offset


## extension instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|1|0|U|T|S|

- U : signed/unsigned
0. signed
1. unsigned

- S : source size
0. from byte
1. from half word

- T : target size
0. to half word
1. to word

## ordering instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|1|0|0|1|0|W|B|

- W : operation unit
0. each half word
1. word

- B : bit/byte
0. bit
1. byte
