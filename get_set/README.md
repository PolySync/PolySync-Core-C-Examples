### get_set

This example shows you how to use the get/set API to modify node parameters.  It uses the `ps_parameters_msg`
to get all parameters from all nodes on the bus.  It also receives a response `ps_parameters_msg` in a callback and
prints the resulting parameters.

### Dependencies

Packages: libglib2.0-dev

To install on Ubuntu: 

```bash
sudo apt-get install libglib2.0-dev
```

### Building and running the node

```bash
$ cd get_set
$ make
$ ./bin/polysync-get-set-c 
```

For more API examples, visit the "Tutorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/).
