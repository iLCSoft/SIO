
## SIO simple example

### Target

Shows how to read and write a simple data structure in block/record with SIO.

### Run the examples

In the top level directory, run:

```shell
$ ./bin/examples/simple_write example.sio
```

to produce a sio file with a particle stored in a block/record.

The particle written in this file can be dumped using the `simple_read` binary:

```shell
$ ./bin/examples/simple_read example.sio
```

More generally, any file produced with the sio library can be inspected with the sio binary `sio-dump` or `sio-dump-detailed`:

```shell
$ ./bin/sio-dump example.sio
$ ./bin/sio-dump-detailed example.sio
```
