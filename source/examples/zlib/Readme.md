
## SIO zlib example

### Target

Shows how to read and write a simple data structure in block/record with SIO using zlib for compression.

### Run the examples

In the top level directory, run:

```shell
$ ./bin/examples/zlib_write example.sio
```

to produce a compressed sio file with a particle stored in a block/record.

The particle written in this file can be dumped using the `zlib_read` binary:

```shell
$ ./bin/examples/zlib_read example.sio
```

More generally, any file produced with the sio library can be inspected with the sio binary `sio-dump` or `sio-dump-detailed`:

```shell
$ ./bin/sio-dump example.sio
$ ./bin/sio-dump-detailed example.sio
```
