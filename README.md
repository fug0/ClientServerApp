# ClientServerApp

This project is the implementation of Client-Server communication, it is written using ACE (Adaptive Communication Enviroment) 7.0.0 framework.
Project has the multithreaded server with TCP and a thread-pool strategy and the client. 
The main idea of communication is in transferring encoded data of arbitrary length from client to server, so it's simplistically implement in the following way:
Client encodes length of data with TLV(Tag-Length-Value) rules for length and sends it to the server, then server decodes the length value and allocates a certain memory size (of this length) for data

To run client and server you have to do following:
- Obtain ACE 7.0.0 from https://download.dre.vanderbilt.edu/.
- Build/use shared lib version by specifying your paths to library in "CMakeLists.txt" of Server and Client.
- After successful building copy "ACEd.dll" from ../ACE_wrappers/lib to each executable file directory.
Now you can run executable files of Server and Client!

Building:
cmake .
make