#pcx
Persistent Collectives X- A collective communication library for high performance, low cost persistent collectives over RDMA devices.

This library is aimed at providing a very high performance collectives, while minimizing CPU/GPU utilization to near-zero.
This is done by taking advantage of the persistent attributes of the communication, while utilizing several different technologies- 
Including: RDMA, Core-Direct, HCA calculation capabilities, peer-direct and host-channel-adapter memory, 
in combination with complete sofware bypass on data-path.

PCX provides a (relatively) easy, c++ based mid-level interface for implementation of high performance collectives using simple commands.

PCX is currently in a PoC stage, targeted at accelerating Deep learning distributed training.

Requirements:
Using PCX currently requires a unique firmware modification to work.
