
## SIO simple example with pointer relocation

### Target

Shows how to read and write a simple data structure in block/record with SIO. 
Shows how to write references to pointer members.

### Run the examples

In the top level directory, run:

```shell
$ ./bin/examples/relocation_write example.sio
```

to produce a sio file with a linked list stored in a block/record.

The linked list written in this file can be dumped using the `relocation_read` binary:

```shell
$ ./bin/examples/relocation_read example.sio
```

More generally, any file produced with the sio library can be inspected with the sio binary `sio-dump` or `sio-dump-detailed`:

```shell
$ ./bin/sio-dump example.sio
$ ./bin/sio-dump-detailed example.sio
```
