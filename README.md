comms_ariel
======================

Ariel's USV and Drone communication package.

Dependencies
======================

Install Protobuf 3.0:

    $ sudo apt install protobuf-compiler

Install
======================
    $ catkin_make

Usage
======================

To test if everything is ok, initialize the server:

    $ rosrun comms_ariel_server main_server 
    
Then initialize the client:

    $ devel/lib/comms_ariel_client/main_client 

The server listens to the port 6000.

If everything is fine you should see messages published on the ros topics `/usv_position`, `/usv_velocity` and `usv_orientation` every 100 ms.
