#pcx
Persistent Collectives X- A collective communication library for high performance, low cost persistent collectives over RDMA devices.

This library is aimed at providing a minimal latency persistent collectives, while minimizing CPU/GPU utilization to near-zero.
This is done by taking advantage of the persistent attributes of the communication, while taking advantage of several different technologies- 
Including: RDMA, Core-Direct, NIC calculation capabilities, peer-direct and network device memory (when available),
in combination with (almost) complete sofware bypass on data-path.

PCX provides a (relatively) easy, c++ based mid-level interface for implementation of high performance collectives using simple commands.

PCX is currently in a PoC stage, targeted at accelerating Deep learning distributed training.


Requirements:
Using PCX currently requires a unique firmware modification to work.


Examples:
Examples for implementation of collectives over PCX can be seen here:
[Recursive Doubling](https://github.com/yanivbl6/gloo/blob/yaniv_branch/gloo/pcx_allreduce_king.h)
[Ring](https://github.com/yanivbl6/gloo/blob/yaniv_branch/gloo/pcx_allreduce_ring.h)


