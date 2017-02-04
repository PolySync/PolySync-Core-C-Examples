### Get set
This example shows you how to use the get/set API to modify node parameters.  It uses the `ps_parameters_msg`
to get all parameters from all nodes on the bus.  It also receives a response `ps_parameters_msg` in a callback and
prints the resulting parameters.

### Hardware requirements

### Dependencies

### Building and Running the Node
```bash
$ cd get_set
$ make
$ ./bin/polysync-get-set-c 
```

For more API examples visit the "Turorials" and "Development" sections in the PolySync Help Center [here](https://help.polysync.io/articles/)