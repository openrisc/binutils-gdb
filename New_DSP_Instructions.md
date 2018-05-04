# New DSP instructions

## compare instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 9|8|7|6 - 4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|OP|A|U|F|S|
 
- U : sign/unsign
0. signed except eq/ne
1. signed except ne/ne

- F : set F flag
0. store result into rD
1. set F flag

- A : anding (it has valid iff F is 1)
0. oring each sub results
1. anding each sub results

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

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3 - 1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|0|1|0|1|OP|S|

- OP : operation codes
0. average signed : make average of A and B for each operation size
1. min signed : select min between A and B for each operation size
2. average unsigned : make average of A and B for each operation size
3. min unsigned : select min between A and B for each operation size
4. mux if ge : select A if ge for each operation size
5. max signed : select max between A and B for each operation size
6. mux if ov : select A if ge for each operation size
7. max unsigned : select max between A and B for each operation size

- S : operation size
0. byte
1. half word

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

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|0|1|U|X|Y|

- X : selection of A
0. use higher half word of A
1. use lower half word of A

- Y : selection of B
0. use higher half word of B
1. use lower half word of B

- U : signed/unsigned
0. signed
1. unsigned

## word and half word multiplication instructions

|31 - 26|25 - 21|20 - 16|15 - 11|10 - 8|7|6|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0x3F|D|A|B|res.|1|0|0|1|0|U|H|Y|

- Y : selection of B
0. use higher half word of B
1. use lower half word of B

- H : output mux
0. get saturated lower 32bit of result
1. get higher 32bit of result

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
|0x3F|D|A|B|res.|1|1|0|0|1|0|B|W|

- W : operation unit
0. each half word
1. word

- B : bit/byte
0. bit
1. byte
